//  ADHISH SINGLA
//  201403004
//  Computer Networks Assignment
//  Globals

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <openssl/md5.h>
#include <openssl/hmac.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <regex.h>
#define Max_Packet_Length 10240

typedef struct FileInfo{
	char fName[100]; //fName
	off_t size; //size
	time_t mtime; //last modified
	char type; //filetype
}FileInfo;

typedef struct HashPrint{
	char *fName; //fName
	unsigned char hash[MD5_DIGEST_LENGTH]; //hash
	time_t mtime; //last modified
}HashPrint;

char protocol[5];
int i, hist_count = 0, error = -1;
FileInfo pdata[1024];
HashPrint hdata[1024];
char Hist[1024][1024] = {0};
char FDownName[1024];
char response[Max_Packet_Length];
int regex = 0;
int udpFlag = 0;