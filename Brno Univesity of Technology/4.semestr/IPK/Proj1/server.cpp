/**
* IPK Projekt c.1 - komunikace klient/server
* Vaclav Bayer, xbayer05
* client.cpp
*
* skript castecne vychazi z poskytnute kostry
* three-way handshake
*/
#include "server.h"

using namespace std;


void prepare_conn(params *par, int *welcome_socket, struct sockaddr_in *sa){
  if((*welcome_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Chyba: socket se nepodarilo vytvorit\n");
    exit(EXIT_FAILURE);
  }

  sa->sin_port = htons(par->port);
  sa->sin_addr.s_addr = INADDR_ANY;
  sa->sin_family = PF_INET;

  if ((bind(*welcome_socket, (struct sockaddr *)sa, sizeof(*sa))) < 0)
  {
    fprintf(stderr, "Chyba: svazani socketu s portem neprobehlo uspesne\n");
    exit(EXIT_FAILURE);
  }
}

void msg_parser(string msg_from, params *par){
  istringstream strstream(msg_from);
  string word;
  int i = 0;

  while(getline(strstream, word))
  {
    i++;
    switch(i){
      case 3:
        par->flags = word;
        break;
      case 4:
        par->search_type = word;
        break;
    }
    if(i > 4){
      par->search_value.push_back(word);
    }
  }
}

int search_process(params *par, string *search_result){
  struct passwd *pw;

  stringstream ss;
  stringstream ss2;
  stringstream ss_to;
  int k = 0;
  int find = -1;


  for (std::vector<string>::iterator l = par->search_value.begin(); l != par->search_value.end(); ++l){
    par->search_items.push_back("0");
  }
  std::copy(par->search_value.begin(), par->search_value.end(),std::ostream_iterator<std::string>(ss,"\n"));

  ifstream etcPasswd("/etc/passwd");
    if(!etcPasswd){
      fprintf(stderr, "Chyba: nepodarilo se otevrit /etc/passwd\n");
      exit(EXIT_FAILURE);
    }else{
      setpwent();
      while ((pw = getpwent()) != NULL){

        if(par->search_type == "UID"){
          uint word;
          while(ss >> word){
            ss2 << word << " ";
            if (word == pw->pw_uid){
              par->search_items.at(k) = "1";
              find++;
              
              for (uint i = 0; i < par->flags.length(); ++i){
                switch(par->flags.at(i)){
                  // login
                  case 'l':
                    ss_to << pw->pw_name << " ";
                    break;
                  // uid
                  case 'u':
                    ss_to << pw->pw_uid << " ";
                    break;
                  // gid
                  case 'g':
                    ss_to << pw->pw_gid << " ";
                    break;
                  // name
                  case 'n':
                    ss_to << pw->pw_gecos << " ";
                    break;
                  // adresa
                  case 'h':
                    ss_to << pw->pw_dir << " ";
                    break;
                  // shell
                  case 's':
                    ss_to << pw->pw_shell << " ";
                    break;
                }// switch
              }// for
              ss_to << "\n";
            }// if
            k++;
          }// while
          k = 0;
          ss.clear();
          ss << ss2.rdbuf();
        }// if

        if(par->search_type == "LOGIN"){
          for (std::vector<string>::iterator j = par->search_value.begin(); j != par->search_value.end(); ++j){
            string login = *j;
            if (login == pw->pw_name){
              par->search_items.at(k) = "1";
              find++;
              for (uint i = 0; i < par->flags.length(); ++i){
                switch(par->flags.at(i)){
                  // login
                  case 'l':
                    ss_to << pw->pw_name << " ";
                    break;
                  // uid
                  case 'u':
                    ss_to << pw->pw_uid << " ";
                    break;
                  // gid
                  case 'g':
                    ss_to << pw->pw_gid << " ";
                    break;
                  // name
                  case 'n':
                    ss_to << pw->pw_gecos << " ";
                    break;
                  // adresa
                  case 'h':
                    ss_to << pw->pw_dir << " ";
                    break;
                  // shell
                  case 's':
                    ss_to << pw->pw_shell << " ";
                    break;
                }// switch
              }// for
              ss_to << "\n";
            }// if
            k++;
          }// for
          k = 0;
        }// if

      }// while
    endpwent();
    }
    if(find > -1){
      *search_result = ss_to.str();
      return find;
    }else{
      return find;
    }
}

int main (int argc, char *argv[])
{
    int ch, from_listen, connectfd;
    params par;
    pid_t pid;

    int code;
    string msg_from;
    char buff[BUFFSIZE];
    //struct sockaddr_in sin;
    //struct hostent * hp;

    par.hostname = "";
    par.port = -1;

    int welcome_socket;
    struct sockaddr_in sa;  // serveru
    struct sockaddr_in sa2; // klienta


    if(argc < 2){
      fprintf(stderr, "Chyba: chybi parametry\n");
      return EXIT_FAILURE;
    }

    while ((ch = getopt(argc, argv, "p:")) != -1) {
      switch (ch) {
        case 'p':
            par.port = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Chyba: nepodporovany parametr\n");
            return EXIT_FAILURE;
            break;
      }
    }

    if (par.port == -1){
      fprintf(stderr, "Chyba: nebyl zadan port\n");
      return EXIT_FAILURE;
    }

    memset(&sa, 0, sizeof(sa));
    socklen_t sa2_value = sizeof(sa2);

    prepare_conn(&par, &welcome_socket, &sa);

    if ((from_listen = listen(welcome_socket, 10)) < 0)
    {
      fprintf(stderr,"Chyba: pri prijimani pozadavku na spojeni se vyskytl problem");
      exit(EXIT_FAILURE);
    }

    while(1){
      if ((connectfd = accept(welcome_socket, (struct sockaddr*)&sa2, &sa2_value)) <= 0){
        continue;
      }

      if ((pid = fork()) < 0){
        fprintf(stderr, "Chyba: vytvoreni procesu se nezdarilo");
        return EXIT_FAILURE;
      }else if(pid == 0){ 
        // child
        close(welcome_socket);

          while(1){
            // prijem zpravy
            code = recv(connectfd, buff, BUFFSIZE, 0);
            if(code == -1){
              fprintf(stderr,"Chyba: prijem dat selhal\n");
              return EXIT_FAILURE;
            }
            msg_from.append(buff);
            if(code == 0)
              break;

            msg_parser(msg_from, &par);

            string search_result;
            string message_to;
            message_to.append("ANSWER");
            message_to.append("\n");

            if(search_process(&par, &search_result) != -1){
              message_to.append("OK");
              message_to.append("\n");
              for (std::vector<string>::iterator j = par.search_items.begin(); j != par.search_items.end(); ++j){
                message_to.append(*j + " ");
              }
              message_to.append("\n");
              message_to.append(search_result);
            }else{
              message_to.append("ERROR");
            }
            if (send(connectfd, message_to.c_str(), message_to.length()+1, 0) == -1){
              fprintf(stderr, "Chyba: nepodarilo se odeslat zpravu klientovi\n");
              return EXIT_FAILURE;
            }
          }// while

          close(connectfd);
          return EXIT_SUCCESS;

      }else{ 
        // parent
        if (close(connectfd) < 0){
          fprintf(stderr,"Chyba: nepodarilo se ukoncit spojeni\n");
          return EXIT_FAILURE;
        }
      }
    }

  return EXIT_SUCCESS;
}
