#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>

#define BUFFSIZE 4096

using namespace std;

typedef struct Tparams
{
    string hostname;
    int port;

    string flags, search_type;
    vector<string> search_value;
    vector<string> search_items;
}params;