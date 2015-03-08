//  ADHISH SINGLA
//  201403004
//  Computer Networks Assignment
//  Main File

#include "global.h"
#include "parse.h"
#include "IndexGet.h"
#include "FileHash.h"
#include "FileDownload.h"
#include "server.h"
#include "client.h"

int main(int argc,char *argv[])
{
	if(argc != 5)
	{
		printf("Please Run Again with this Format:\n");
		printf("\n%s <listen port no> <connect port no> <ip of server> <protocol(tcp/udp)>\n\n",argv[0]);
		return 1;
	} 
	char *ListenPort = argv[1];
	char *ConnectPort = argv[2];
	char *ip = argv[3];
	if(strcmp(argv[4],"udp")==0)
		udpFlag = 1;
	else if(strcmp(argv[4],"tcp")==0)
		udpFlag = 0;
	else
	{
		printf("Please Run Again using a Standard Protocol.\n");
		exit(0);
	}
	pid_t pid;
	pid = fork();
	strcpy(protocol,argv[4]);
	if(pid > 0){
		if(udpFlag==1)
		{
			udp_client(ip,ConnectPort);
		}
		else
		{
			tcp_client(ip,ConnectPort);
		}
	}
	else if(pid == 0){
		if(udpFlag==1)
		{
			udp_server(ListenPort);
		}
		else
		{
			tcp_server(ListenPort);
		}
	}
	return 0;
}
