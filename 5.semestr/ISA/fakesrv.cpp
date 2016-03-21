/**
* ISA Projekt - FTP/SSH Honeypot
* Vaclav Bayer, xbayer05
* fakesrv.cpp
*/
#include "fakesrv.h"

ssh_session session;
ssh_bind sshbind;

int *user_count = NULL;
int shm_user_count = 0;

sem_t *f_writer;
FILE *file = NULL;

/**
* print_msg
* vypis chyboveho hlaseni a pripadne napovedy
*/
void print_msg(bool help, string msg){

	cerr << msg << endl;

	if(help){
		printf("\nISA Projekt - SSH/FTP Honeypot, Vaclav Bayer - xbayer05 \n"
			"Napoveda: \n"
			"Povinne parametry:\n"
			"\t-m \t\trezim \"ssh\" nebo \"ftp\"\n"
			"\t-a \t\tIP adresa\n"
			"\t-p \t\tcislo portu\n"
			"\t-l \t\tlogovaci soubor\n"
			"Nepovinne parametry:\n"
			"\t-r \t\tRSA klic - povinny pouze v SSH rezimu\n"
			"\t-c \t\tmax. pocet klientu\n"
			"\t-t \t\tmax. pocet pokusu zadani hesla - voli se jen v SSH rezimu\n\n");
	}
}

/**
* alloc_mem
* alokace pameti, vztvoreni semaforu
*/
int alloc_mem(){
	if((shm_user_count = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | IPC_EXCL | 0666)) == -1){
		print_msg(false, "Chyba: nepodarilo se vytvorit sdileny citac");
		return EXIT_FAILURE; 
	}
	 
	if((user_count = (int *)shmat(shm_user_count, NULL, 0)) == NULL){ 
		print_msg(false, "Chyba: nepodarilo se vytvorit sdileny citac");
		return EXIT_FAILURE; 
	}

    f_writer = sem_open(SEM_NAME,O_CREAT,0644,1);
  	if(f_writer == SEM_FAILED){
      print_msg(false, "Chyba: nepodarilo se vytvorit semafor");
      sem_unlink(SEM_NAME);
      return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}

/**
* clear_mem
* odalokovani pameti a odpojeni semaforu
*/
int clear_mem(){
	sem_close(f_writer);
	sem_unlink(SEM_NAME);

	if(shmctl(shm_user_count, IPC_RMID, NULL) == -1){
		return EXIT_FAILURE;
	}

	fclose(file);
}

/**
* counter_op
* citac pripojenych klientu
*/
int counter_op(int operation, int *count) {
	int retVal = 0;
	
	retVal = *count;
	if(operation == INC_COUNTER){ (*count)++; };
	if(operation == DEC_COUNTER){ (*count)--; };

	return retVal;
}

/**
* file_open
* overeni logovaciho souboru
*/
int file_open(FILE **file, params *par){
    if ((*file = fopen(par->flag_l, "a+")) == NULL) {
		print_msg(true, "Chyba: logovaci soubor se nepodarilo otevrit");
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}

/**
* signal_handler
* zachyceni signalu a nasledna reakce dle typu signalu
*/
void signal_handler(int signal){
	switch(signal){
		case SIGCHLD:
			pid_t pid;
			while(pid = waitpid(-1, NULL, WNOHANG) > 0){
				if(DEBUG){ cout << "SIGCHLD prevence pred zombie, hodnota: " << pid << endl; }
			}
			// snizeni poctu klientu
			counter_op(DEC_COUNTER, user_count);
			break;
		case SIGINT:
			if(DEBUG){ cout << " SIGQUIT" << endl; }
		case SIGQUIT:
			if(DEBUG){ cout << " SIGINT" << endl; }
			clear_mem();
			ssh_disconnect(session);
			ssh_bind_free(sshbind);
			ssh_finalize();
			exit(0);
	}
}

/**
* get_time
* vygenerovani casu
*/
void get_time(user_record *usRec){
	time_t time_now;
	time_now = time(NULL);

	struct tm * timeinfo;
	timeinfo = localtime(&time_now);

	strftime(usRec->time_of_con, BUFFSIZE, "%Y-%m-%d %H:%M:%S", timeinfo);
}

/**
* ssh_get_user_info
* ziskani zadaneho heslo, vygenerovani casu a zapis do souboru
*/
int ssh_get_user_info(user_record *usRec) {

	usRec->name = ssh_message_auth_user(usRec->msg);
	usRec->password = ssh_message_auth_password(usRec->msg);
	
	get_time(usRec);

	if(DEBUG){ cout << "SSH " << usRec->time_of_con << " " << usRec->client_ip << " " << usRec->name << " " << usRec->password << endl; }

	// kriticka sekce, zapis do souboru
	sem_wait(f_writer);
		fprintf(file, "SSH %s %s %s %s\n", usRec->time_of_con, usRec->client_ip, usRec->name, usRec->password);
	sem_post(f_writer);

	return EXIT_SUCCESS;
}

/**
* ssh_client_connection
* obsluha komunikace s klientem, zjistovani typu zprav
*/
int ssh_client_connection(params *par) {

	// klientsky zaznam, prebira vlastni session
	struct user_record usRec;
	usRec.session = session;

	// pocitadlo pokusu
	int log_count = 0;

	// vymena klicu
	if (ssh_handle_key_exchange(usRec.session)) {
		print_msg(false, "Chyba: pri vymene klicu");
		return EXIT_FAILURE;
	}
	if(DEBUG){ cout << "Uspesna vymena klicu" << endl; }

	counter_op(INC_COUNTER, user_count);
	if(DEBUG){ cout << "S poradovym cislem: " << *user_count << endl; }

	if(*user_count > par->flag_c){
		if(DEBUG){ cout << "Prekrocen limit pripojenych klientu" << endl; }
		ssh_disconnect(session);
		return EXIT_SUCCESS;
	}
	get_time(&usRec);

	unsigned int length = BUFFSIZE;
	struct sockaddr_storage tmp;
	struct sockaddr_in *sock;

	getpeername(ssh_get_fd(usRec.session), (struct sockaddr*)&tmp, &length);

	sock = (struct sockaddr_in *)&tmp;
	inet_ntop(AF_INET, &sock->sin_addr, usRec.client_ip, length);

	usRec.name = "";
	usRec.password = "";
	string name = "";

	if(DEBUG){ cout << "Klientuv proces s adresou " << usRec.client_ip << ", portem " <<  ntohs(sock->sin_port) << endl; }

	// vymena zprav s klientem
	while (true) {
		if ((usRec.msg = ssh_message_get(usRec.session)) == NULL) {
			usRec.password = "";
			//usRec.name = "";
			// kriticka sekce, zapis do souboru
			if(log_count < par->flag_t){
				if(DEBUG){ cout << "SSH " << usRec.time_of_con << " " << usRec.client_ip << " " << name << " " << usRec.password << endl; }
				sem_wait(f_writer);
					fprintf(file, "SSH %s %s %s %s\n", usRec.time_of_con, usRec.client_ip, name.c_str(), usRec.password);
				sem_post(f_writer);
			}
			break;
		}

		if (ssh_message_subtype(usRec.msg) == SSH_AUTH_METHOD_NONE) {
			name = ssh_message_auth_user(usRec.msg);
			if(DEBUG){ cout << "Klientuv login: " << name << endl; }
		}
 
		// pokus o prihlaseni zadanim hesla
		if (ssh_message_subtype(usRec.msg) == SSH_AUTH_METHOD_PASSWORD) {
			if(++log_count <= par->flag_t){
				ssh_get_user_info(&usRec);
			}else{
				break;
			}
		}

		ssh_message_reply_default(usRec.msg);
		ssh_message_free(usRec.msg);
	}

	ssh_message_free(usRec.msg);
	ssh_disconnect(session);

	if(DEBUG){ cout << "Konec childu" << endl; }
	return EXIT_SUCCESS;
}

/**
* ssh_serv
* vytvoreni ssh serveru
*/
int ssh_serv(params *par){
 
	// vytvoreni session pro server z ktere pak bude cerpat klient
	session = ssh_new();
	sshbind = ssh_bind_new();

	if(file_open(&file, par) == NOK){
		return EXIT_FAILURE;
	}
	setbuf(file, NULL);

	alloc_mem();
	pid_t pid;

	// uvedeni signalu pro spravne ukoncovani
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGCHLD, signal_handler);

	// spousteci parametry ssh
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, par->flag_a);
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT, &par->flag_p);
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_HOSTKEY, "ssh-rsa");
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, par->flag_r);
 
	// poslouchani serveru na danem portu
	if (ssh_bind_listen(sshbind) == SSH_ERROR) {
		print_msg(true, "Chyba: nepodarilo se naslouchat na danem portu");
		return EXIT_FAILURE;
	}
	if(DEBUG){ cout << "Naslouchani..." << endl; }
 
	while (true) {
		if (ssh_bind_accept(sshbind, session) == SSH_ERROR) {
			print_msg(true, "Chyba: nepodarilo se prijmout pripojeni");
			return EXIT_FAILURE;
		}

		while(true){
			if((pid = fork()) < 0){
				print_msg(true, "Chyba: fork selhal");
				exit(EXIT_FAILURE);
			// child
			}else if(pid == 0){
				exit(ssh_client_connection(par));
			// rodic
			}else{
				if(DEBUG){ cout << "fork PID: " << pid << endl; }
				break;
			}
		}
	}
}

/**
* ftp_send_msg
* odeslani zpravy klientovi
*/
int ftp_send_msg(int connectfd, string msg_to){
	send(connectfd, msg_to.c_str(), msg_to.length()+1, 0);
	if(DEBUG){ cout << "Odeslano: " << msg_to << endl; }
}

/**
* ftp_recv_msg
* prijmani a parsovani zpravy od klienta
*/
int ftp_recv_msg(int connectfd, user_record *usRec){
	int code;
	char buf[FTP_MSG];
	string msg_type; // typ zpravy
	string msg_text; // text zpravy

	code = recv(connectfd, buf, FTP_MSG, 0);
	// nepovedeny prijem zpravy
	if(code == -1){
		fprintf(stderr,"Chyba: prijem dat selhal\n");
		return EXIT_FAILURE;
	}else if(code == 0){
		return EXIT_FAILURE;
	}
	buf[code] = '\0';

	if(DEBUG){ cout << "Prijem: " << buf << endl; }

	msg_type.assign(buf, 0, 4);
	msg_text.assign(buf, 5, code);

	msg_text.erase(msg_text.size() - 2);

	if(DEBUG){ cout << "Typ zpravy: " << msg_type << endl; }
	if(DEBUG){ cout << "Text zpravy: " << msg_text << endl; }

	// ziskani potrebnych dat
	if(!msg_type.compare("USER")){
		// USER
		usRec->name = strdup(msg_text.c_str());
		if(DEBUG){ cout << "USER " << "\"" << usRec->name << "\"" <<  endl; }
	}else if(!msg_type.compare("PASS")){ 
		// PASS
		usRec->password = strdup(msg_text.c_str());
		if(DEBUG){ cout << "PASS " << "\"" << usRec->password << "\"" <<  endl; }
	}else if(!msg_type.compare("SYST")){ 
		// SYST neco je spatne
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
* ftp_communication
* komunikace s klientem a vytvoreni zaznamu v souboru
*/
int ftp_communication(int connectfd, sockaddr_in *clisent_sa, params *par){
	user_record usRec;
	unsigned int length = BUFFSIZE;

	// overeni poctu max. pripojenych klientu
	if(*user_count > par->flag_c){
		if(DEBUG){ cout << "Prekrocen limit pripojenych klientu" << endl; }
		return EXIT_SUCCESS;
	}
	get_time(&usRec);

	inet_ntop(AF_INET, &clisent_sa->sin_addr, usRec.client_ip, length);
	if(DEBUG){ cout << "Klientuv proces s adresou " << usRec.client_ip << ", portem " <<  ntohs(clisent_sa->sin_port) << endl; }

	ftp_send_msg(connectfd, "220 Service ready\r\n");
	if(ftp_recv_msg(connectfd, &usRec) == NOK){
		usRec.name = "";
		usRec.password = "";
		if(DEBUG){ cout << "FTP " << usRec.time_of_con << " " << usRec.client_ip << " " << usRec.name << " " << usRec.password << endl; }
		sem_wait(f_writer);
			fprintf(file, "FTP %s %s %s %s\n", usRec.time_of_con, usRec.client_ip, usRec.name, usRec.password);
		sem_post(f_writer);
		return EXIT_SUCCESS;
	}

	ftp_send_msg(connectfd, "331 User name ok, need password\r\n");
	if(ftp_recv_msg(connectfd, &usRec) == NOK){
		get_time(&usRec);
		usRec.password = "";
		if(DEBUG){ cout << "FTP " << usRec.time_of_con << " " << usRec.client_ip << " " << usRec.name << " " << usRec.password << endl; }
		sem_wait(f_writer);
			fprintf(file, "FTP %s %s %s %s\n", usRec.time_of_con, usRec.client_ip, usRec.name, usRec.password);
		sem_post(f_writer);
		return EXIT_SUCCESS;
	}

	ftp_send_msg(connectfd, "530 Login Incorrect\r\n");

	get_time(&usRec);

	if(DEBUG){ cout << "FTP " << usRec.time_of_con << " " << usRec.client_ip << " " << usRec.name << " " << usRec.password << endl; }

	// kriticka sekce, zapis do souboru
	sem_wait(f_writer);
		fprintf(file, "FTP %s %s %s %s\n", usRec.time_of_con, usRec.client_ip, usRec.name, usRec.password);
	sem_post(f_writer);

	return EXIT_SUCCESS;
}

/**
* ftp_prepare_conn
* nastaveni parametru serveru ze vstupnich dat
*/
int ftp_prepare_conn(params *par, int *welcome_socket, struct sockaddr_in *sa){

	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGCHLD, signal_handler);

	// vytvoreni socketu
	if((*welcome_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		print_msg(false, "Chyba: socket se nepodarilo vytvorit");
		return EXIT_FAILURE;
	}

	sa->sin_port = htons(par->flag_p);
	sa->sin_family = par->AF_type;
	sa->sin_addr.s_addr = inet_addr(par->flag_a);

	if ((bind(*welcome_socket, (struct sockaddr *)sa, sizeof(*sa))) < 0){
		print_msg(false, "Chyba: svazani socketu s portem neprobehlo uspesne");
		return EXIT_FAILURE;
	}
	if(DEBUG){ cout << "Nasloucham..." << endl; }
}

/**
* ftp_serv
* vytvoreni ftp serveru
*/
int ftp_serv(params *par){
	int ch, from_listen, connectfd;
	pid_t pid;

	int code;
	string msg_from;
	char buff[BUFFSIZE];

	int welcome_socket;
	struct sockaddr_in sa;  // serveru
	struct sockaddr_in clisent_sa; // klienta
	char ip_string[INET_ADDRSTRLEN];

	// overeni logovaciho souboru
	if(file_open(&file, par) == NOK){
		return EXIT_FAILURE;
	}
	setbuf(file, NULL);

	alloc_mem(); // alokace potrebnych dat
	memset(&sa, 0, sizeof(sa));
	socklen_t client_sa_value = sizeof(clisent_sa);

	ftp_prepare_conn(par, &welcome_socket, &sa);

	// naslouchani
	if ((from_listen = listen(welcome_socket, 10)) < 0){
		print_msg(false,"Chyba: pri prijimani pozadavku na spojeni se vyskytl problem");
		return EXIT_FAILURE;
	}

	while(true){
		// pripojeni klienta
		if ((connectfd = accept(welcome_socket, (struct sockaddr*)&clisent_sa, &client_sa_value)) <= 0){
			print_msg(true, "Chyba: nepodarilo se prijmout pripojeni");
			return EXIT_FAILURE;
		}
		if(DEBUG){ cout << "Navazano spojeni" << endl; }

		if ((pid = fork()) < 0){
			print_msg(false, "Chyba: vytvoreni procesu se nezdarilo");
			return EXIT_FAILURE;
		}else if(pid == 0){ 
			// child
			int child = getpid();
			close(welcome_socket);

			// zvyseni poctu klientu
			counter_op(INC_COUNTER, user_count);
			if(DEBUG){ cout << "S poradovym cislem: " << *user_count << endl; }

			// prijem zpravy
			ftp_communication(connectfd, &clisent_sa, par);
						
			close(connectfd);
			exit(0);
		}
		close(connectfd);
	}
	if (close(connectfd) < 0){
		print_msg(false,"Chyba: nepodarilo se ukoncit spojeni");
		return EXIT_FAILURE;
	}

  return EXIT_SUCCESS;
}

/**
* check_params
* kontrola vstupnich parametru
*/
int check_params(params *par){

	int ret_val = -1;

	struct addrinfo hint, *res = NULL;
	int ret_addinf;

	memset(&hint, '\0', sizeof hint);

	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;

	// adresa localhost
	if(!strcmp(par->flag_a, "localhost")){
		par->flag_a = "127.0.0.1";
	}else{
		// informace o zadane IP adrese
		ret_addinf = getaddrinfo(par->flag_a, NULL, &hint, &res);

		// nevalidni format IP adresy
		if (ret_addinf) {
			freeaddrinfo(res);
			print_msg(true, "Chyba: adresu nelze zpracovat");
			return EXIT_FAILURE;
		}

		if(res->ai_family == AF_INET) {
			// IPv4
			if(DEBUG){ cout << par->flag_a << " je ipv4 adresa" << endl; }
		} else if (res->ai_family == AF_INET6) {
			// IPv6
			if(DEBUG){ cout << par->flag_a << " je ipv6 adresa" << endl; }
			par->AF_type = AF_INET6;
		} else {
			if(DEBUG){ cout << par->flag_a << "je neznamy format adresy " << res->ai_family << endl; }
		}
	}

	freeaddrinfo(res);

	// rozmezi portu 1-65535
	if (!(par->flag_p >= MIN_PORT && par->flag_p <= MAX_PORT)){
		print_msg(true, "Chyba: port neni v platnem rozsahu");
		return EXIT_FAILURE;
	}
	
	// limit klientu
	if(par->flag_c == -1){
		par->flag_c = 10; // defaultne 10
	}
	if(DEBUG){ cout << "C je nastaveno na: " << par->flag_c << endl; }

	// SSH mod
	if(!strcmp(par->flag_m, "ssh")){
		if(!par->arg_r) { // key file
			print_msg(true, "Chyba: nebyl zadan parametr -r");
			return EXIT_FAILURE;
		}
	
		if(DEBUG){printf("Mode SSH\n");}

		// limit pokusu o prihlaseni
		if(par->flag_t == -1){
			par->flag_t = 3; // defaultne 3
		}

		if(DEBUG){ cout << "T je nastaveno na: " << par->flag_t << endl; }
		ret_val = MODE_SSH;
	// FTP mod
	}else if(!strcmp(par->flag_m, "ftp")){

		if(par->flag_t != -1){
			print_msg(true, "Chyba: v rezimu FTP se nezadava parametr -t");
			return EXIT_FAILURE;
		}

		if(DEBUG){ printf("Mode FTP\n");}
		ret_val = MODE_FTP;
	}else{
		print_msg(true, "Chyba: nebyl zadan spravny parametr -m");
		return EXIT_FAILURE;
	}

	return ret_val;
}

int main (int argc, char *argv[])
{
	int char_option;

	params par;
	par.flag_p = -1;
	par.flag_c = -1;
	par.flag_t = -1;

	if(argc < 2){
		print_msg(true, "Chyba: chybi parametry");
		return EXIT_FAILURE;
	}

	while ((char_option = getopt(argc, argv, "m:a:p:l:r:c:t:")) != -1) {
	  switch (char_option) {
	  	// -m mod
		case 'm':
			par.arg_m = true;
			par.flag_m = optarg;
			if(DEBUG){ cout << "Rezim -m:\t" << par.flag_m << endl; }
			break;
		// -a adresa
		case 'a':
			par.arg_a = true;
			par.flag_a = optarg;
			if(DEBUG){ cout << "IP -a:\t\t" << par.flag_a << endl; }
			break;
		// -p port
		case 'p':
			par.arg_p = true;
			par.flag_p = atoi(optarg); // TODO osetrit int/string?
			if(DEBUG){ cout << "Port -p:\t" << par.flag_p << endl; }
			break;
		// -l logovaci soubor
		case 'l':
			par.arg_l = true;
			par.flag_l = optarg;
			if(DEBUG){ cout << "Logfile -l:\t" << par.flag_l << endl; }
			break;
		// -r soubour s klicem
		case 'r':
			par.arg_r = true;
			par.flag_r = optarg;
			if(DEBUG){ cout << "RSAkey -r:\t" << par.flag_r << endl; }
			break;
		// -c pocet povolenych klientu
		case 'c':
			par.arg_c = true;
			par.flag_c = atoi(optarg);
			if(DEBUG){ cout << "ClientLimit -c:\t" << par.flag_c << endl; }
			break;
		// -t max. pocet prihlaseni
		case 't':
			par.arg_t = true;
			par.flag_t = atoi(optarg);
			if(DEBUG){ cout << "LogLimit -t:\t" << par.flag_t << endl; }
			break;
		// nepodporovany parametr
		case '?':
			print_msg(true, "Chyba: nepodporovany parametr");
			return EXIT_FAILURE;
			break;
		default:
			print_msg(true, "Chyba: chyba pri nacitani parametru");
			return EXIT_FAILURE;
			break;
	  }
	}

	// povinne parametry
	if (!par.arg_m or !par.arg_a or !par.arg_p or !par.arg_l){
	  print_msg(true, "Chyba: nebyly zadany povinne parametry");
	  return EXIT_FAILURE;
	}

	int mode_num = check_params(&par);
	if (mode_num == NOK){
		return EXIT_FAILURE;
	}else if (mode_num == MODE_SSH){
		ssh_serv(&par);
	}else{
		ftp_serv(&par);
	}

	return EXIT_SUCCESS;
}
