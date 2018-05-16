/**
* ISA Projekt - FTP/SSH Honeypot
* Vaclav Bayer, xbayer05
* fakesrv.h
*/
#include <libssh/libssh.h>
#include <libssh/server.h>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <semaphore.h>
#include <fcntl.h>

using namespace std;

#define BUFFSIZE 255
#define MODE_SSH 2
#define MODE_FTP 3
#define DEBUG false
#define MIN_PORT 1
#define MAX_PORT 65535
#define NOK 1
#define OK 1
#define INC_COUNTER 1
#define DEC_COUNTER 2

#define FTP_MSG 1024
#define FTP_TYPE_MSG 4
#define FTP_TEXT_MSG 1020
#define SEM_NAME "server_sem"

typedef struct Tparams
{
	bool arg_m;
    bool arg_a;
    bool arg_p;
    bool arg_l;
    bool arg_r;
    bool arg_c;
    bool arg_t;

	const char* flag_m;
    const char* flag_a;
    const char* flag_l;
    const char* flag_r;
    int flag_p;
    int flag_c;
    int flag_t;

    int AF_type = AF_INET;
}params;

struct user_record {
    char client_ip[BUFFSIZE];
    const char* name;
    const char* password;
    char time_of_con[BUFFSIZE];

    ssh_session session;
    ssh_message msg;
};