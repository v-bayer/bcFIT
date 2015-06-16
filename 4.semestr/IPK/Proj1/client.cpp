/**
* IPK Projekt c.1 - komunikace klient/server
* Vaclav Bayer, xbayer05
* client.cpp
*
* skript castecne vychazi z poskytnute kostry
* three-way handshake
*/

#include "client.h"
using namespace std;

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)){
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    return split(s, delim, elems);
}

string new_message(params *par){

  stringstream strstream;
  string word;

  if (par->last == "LOGIN")
    strstream.str(par->login);
  else if(par->last == "UID")
    strstream.str(par->uid);

  // sestaveni zpravy
  string message;

  message.append("REQUEST");
  message.append("\n");

  message.append("FLAGS");
  message.append("\n");
  message.append(par->opt);
  message.append("\n");

  message.append(par->last);
  message.append("\n");
  while(strstream >> word)
  {
    message.append(word);
    message.append("\n");
  }

  return message;
}

void prepare_conn(params *par, int *s, struct sockaddr_in *sa){
  if((*s = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Chyba: socket se nepodarilo vytvorit\n");
    exit(EXIT_FAILURE);
  }

  sa->sin_port = htons(par->port);
  sa->sin_addr.s_addr = INADDR_ANY;
  sa->sin_family = PF_INET;

  struct hostent *hptr;

  const char* hostname;
  hostname = par->hostname.c_str();

  if((hptr =  gethostbyname(hostname)) == NULL){
    fprintf(stderr, "Chyba: gethostname(): %s\n", hostname);
    exit(EXIT_FAILURE);
  }

  // do sa adresu z hptr
  memcpy(&sa->sin_addr, hptr->h_addr, hptr->h_length);
}

void join_and_send(string message_to, string *message_from, int s, sockaddr_in sa){
  if(connect (s, (struct sockaddr *)&sa, sizeof(sa) ) < 0 ){
    fprintf(stderr, "Chyba: nepodarilo se navazat spojeni\n");
    exit(EXIT_FAILURE);
  }

  // reprezentace zpravy v bitech
  ssize_t code;
  if ((code = send(s, message_to.c_str(), message_to.length()+1, 0)) == -1){
    fprintf(stderr, "Chyba: serveru se nepodarilo odeslat zparvu\n");
    exit(EXIT_FAILURE);
  }

  // uspani a cekani na odpoved od serveru
  sleep(1);

  // prijem zpravy, cteni z bufferu
  char buffer[BUFFSIZE];
    while(1){
      code = recv(s, buffer, BUFFSIZE, 0);
      if(code == -1){
        fprintf(stderr,"Chyba: nepodarilo se prijat zpravu\n");
        exit(EXIT_FAILURE);
      }

      message_from->append(buffer);

      if(code > 0)
        break;
    }

  if (close(s) < 0){
    fprintf(stderr,"Chyba: nepodarilo se ukoncit spojeni");
    exit(EXIT_FAILURE);
  }
}

inline bool isInteger(string s){
  if((!isdigit(s[0])) && (s[0] != '+')) return false ;

  char * p ;
  strtol(s.c_str(), &p, 10) ;

  return (*p == 0) ;
}


void message_print(string message_from, params *par)
{
    istringstream ss(message_from);
    string line;
    string message_type;

    int i = 1;
    int k = 0;

    while(getline(ss, line)){
        if(i == 1){
          i++;
          continue;
        }

        if(i == 2){
          if (line == "ERROR")
          {
            fprintf(stderr, "Zadane polozky nebyly nalezeny\n");
            exit(EXIT_FAILURE);
          }
        }

        if (i == 3)
        {
          stringstream ss2(line);
          string word;

          vector<string> items;
          if (par->last == "LOGIN")
            items = split(par->login, ' ');
          else if(par->last == "UID")
            items = split(par->uid, ' ');

          /*for (std::vector<string>::iterator j = items.begin(); j != items.end(); ++j){
                cout << *j << endl;
              }*/

          while(ss2 >> word){
            if(word == "0"){
              cerr << "Polozka \"" << items.at(k) << "\" nebyla nalezena!" << endl;
              k++;
            }else if(word == "1"){
              k++;
            }
          }// while
        }// if

        if(i > 3){
          cout << line << endl;
        }
      i++;
    }
    if(k == 0)
      exit(EXIT_FAILURE);

    exit(EXIT_SUCCESS);
}

int main (int argc, char *argv[] )
{
    int s, ch;
    struct sockaddr_in sa; 
    //struct hostent *hptr;

    params par;

    par.hostname = "";
    par.port = -1;
    par.login = "";
    par.uid = "";
    par.last = "";

    //             L  U  G  N  H  S
    int is_in[] = {0, 0, 0, 0, 0, 0};


    if(argc < 3){
      fprintf(stderr, "Chyba: chybi parametry\n");
      return EXIT_FAILURE;
    }

    // parsovani argumentu
    while ((ch = getopt(argc, argv, "h:p:l:u:LUGNHS")) != -1) {
      switch (ch) {
          // hostname
          case 'h':
            par.hostname.append(optarg);
            break;
          // port
          case 'p':
            par.port = atoi(optarg);
            break;
          // login
          case 'l':
            optind--;
            if (*argv[optind] == '-'){
              fprintf(stderr, "Chyba: chybi hodnota parametru -l\n");
              return EXIT_FAILURE;
            }
            for( ; optind < argc && *argv[optind] != '-'; optind++){
              par.login.append( argv[optind] );
              par.login.append(" ");
            }
            par.last = "LOGIN";
            break;
          // uid
          case 'u':
            optind--;
            if (*argv[optind] == '-'){
              fprintf(stderr, "Chyba: chybi hodnota parametru -u\n");
              return EXIT_FAILURE;
            }
            for( ; optind < argc && *argv[optind] != '-'; optind++){
              if(!isInteger(argv[optind])){
                fprintf(stderr, "Chyba: zadane UID neni cislo\n");
                return EXIT_FAILURE;
              }
              par.uid.append( argv[optind] );
              par.uid.append(" ");
            }
            par.last = "UID";
            break;
          // parametry tisku
          case 'L':
            if(is_in[0] == 0){
              par.opt.append("l");
              is_in[0]++;
            }
            break;
          case 'U':
            if(is_in[1] == 0){
              par.opt.append("u");
              is_in[1]++;
            }
            break;
          case 'G':
            if(is_in[2] == 0){
              par.opt.append("g");
              is_in[2]++;
            }
            break;
          case 'N':
            if(is_in[3] == 0){
              par.opt.append("n");
              is_in[3]++;
            }
            break;
          case 'H':
            if(is_in[4] == 0){
              par.opt.append("h");
              is_in[4]++;
            }
            break;
          case 'S':
            if(is_in[5] == 0){
            par.opt.append("s");
            is_in[5]++;
            }
            break;
          default:
            fprintf(stderr, "Chyba: nepodporovany parametr\n");
            return EXIT_FAILURE;
            break;
        }
    }

    if (par.hostname.length() == 0){
      fprintf(stderr, "Chyba: nekorektni hodnota hostname\n");
      return EXIT_FAILURE;
    }

    if (par.port == -1){
      fprintf(stderr, "Chyba: nebyl zadan port\n");
      return EXIT_FAILURE;
    }

    string message_to = new_message(&par);
    string message_from;

    memset(&sa, 0, sizeof(sa));

    prepare_conn(&par, &s, &sa);
    
    join_and_send(message_to, &message_from, s, sa);

    message_print(message_from, &par);

    return EXIT_SUCCESS;
}
