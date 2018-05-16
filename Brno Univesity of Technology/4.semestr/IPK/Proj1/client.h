#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <getopt.h>
#include <regex.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#define BUFFSIZE 4096
using namespace std;

typedef struct Tparams
{
    string hostname;
    string login;
    string opt;
    string uid;
    int port;

    string last;
}params;