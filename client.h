//  ADHISH SINGLA
//  201403004
//  Computer Networks Assignment
//  Client Functions(TCP and UDP)

int tcp_client(char *ip,char *ConnectPort)
{
	int sockfd = 0, n = 0;
	char writeBuff[1024];
	char readBuff[1024];
	struct sockaddr_in serv_addr;
	int portno = atoi(ConnectPort);
	char UploadName[1024];
	char DownloadName[1024];

	memset(writeBuff, 0,sizeof(writeBuff));
	memset(readBuff, 0,sizeof(readBuff));

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Error : Socket can't be Created.\n");
		return 1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); 

	serv_addr.sin_port = htons(portno);
	serv_addr.sin_family = AF_INET;

	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0){
		printf("\n inet_pton error occured\n");
		return 1;
	}

	for(;;)
	{
		if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			continue;
		}
		else
		{
		if(strcmp(protocol,"udp"))
			printf("Client is Connected now : \n");
			break;
		}
	}

	int a , count = 0 , filedownload = 0 , fileupload = 0;

	for(;;)
	{
		printf("Command: ");
		fileupload = 0;
		filedownload = 0;
		FILE *fp = NULL;
		int i;
		char *cresponse = malloc(Max_Packet_Length);
		fgets(writeBuff,sizeof(writeBuff),stdin);

		char *fName;
		char copy[1024];
		strcpy(copy,writeBuff);
		fName = malloc(1024);
		fName = strtok(copy," \n");
		if(strcmp(fName,"quit") == 0)
		{
			_exit(1);
		}
		if(strcmp(fName,"FileDownload") == 0){
			filedownload = 1;
			fName = strtok(NULL," \n");
			strcpy(DownloadName,fName);
			fp = fopen(DownloadName,"wb");
		}
		if(strcmp(fName,"FileUpload") == 0){
			fileupload = 1;
			fName = strtok(NULL," \n");
			strcpy(UploadName,fName);
			FILE *f = fopen(UploadName, "r");
			fseek(f, 0, SEEK_END);
			unsigned long len = (unsigned long)ftell(f);
			char size[1024];
			memset(size, 0, sizeof(size));
			sprintf(size,"%ld\n",len);
			strcat(writeBuff,size);
			fclose(f);
		}
		write(sockfd, writeBuff , strlen(writeBuff));

		n = read(sockfd, readBuff, sizeof(readBuff)-1);
		size_t bytes_read;
		if(strcmp(readBuff,"FileUpload Accept\n") == 0){
			int b,c;
			printf("Upload Accepted\n");
			verify_handler(UploadName);
			b=0;
			while(b < 1)
			{
				sprintf(cresponse, "%s, ",hdata[b].fName);
				strcat(writeBuff,cresponse);
				c = 0;
				while (c < MD5_DIGEST_LENGTH)
				{
					sprintf(cresponse, "%02x",hdata[b].hash[c]);
					strcat(writeBuff,cresponse);
					c++;
				}
				sprintf(cresponse, ", %s",ctime(&hdata[b].mtime));
				strcat(writeBuff,cresponse);
				b++;
			}
			write(sockfd , writeBuff , bytes_read);
			printf("%s\n",writeBuff);
			memset(writeBuff, 0, sizeof(writeBuff));
			fp = fopen(UploadName,"rb");
			for(;!feof(fp);)
			{
				bytes_read = fread(cresponse, 1, 1024, fp);
				cresponse[1024] = 0;
				memcpy(writeBuff,cresponse,bytes_read);
				write(sockfd , writeBuff , bytes_read);
				memset(writeBuff, 0, sizeof(writeBuff));
				memset(cresponse, 0, sizeof(cresponse));
			}
			memcpy(writeBuff,"~@~",3);
			write(sockfd , writeBuff , 3);
			memset(writeBuff, 0, sizeof(writeBuff));
			memset(readBuff, 0, strlen(readBuff));
			fclose(fp);
		}
		else if(strcmp(readBuff,"FileUpload Deny\n") == 0){
			printf("Upload Denied\n");
			memset(readBuff, 0,sizeof(readBuff));
			continue;
		}
		else{
			for(;;)
			{
				readBuff[n] = 0;
				if(readBuff[n-1] == '~' && readBuff[n-3] == '~' && readBuff[n-2] == '@'){
					readBuff[n-3] = 0;
					if(filedownload == 1){
						fwrite(readBuff,1,n-3,fp);
						fclose(fp);
					}
					else
					{
						strcat(cresponse,readBuff);
					}
					memset(readBuff, 0,strlen(readBuff));
					break;
				}
				else{
					if(filedownload == 1)
					{
						fwrite(readBuff,1,n,fp);
					}
					else
					{
						strcat(cresponse,readBuff);
					}
					memset(readBuff, 0,strlen(readBuff));
				}
				n = read(sockfd, readBuff, sizeof(readBuff)-1);
				if(n < 0)
					break;
			}
		}

		if(filedownload == 0)
		{
			printf("%s\n",cresponse);
		}
		else
		{
			printf("File Downloaded\n");
		}
		if(n < 0)
		{
			printf("\n Read error \n");
		}
		memset(writeBuff, 0,sizeof(writeBuff));
		memset(readBuff, 0,sizeof(readBuff));
	}
	return 0;
}



int udp_client(char *ip,char *ConnectPort)
{
	int sockfd = 0, n = 0;
	char writeBuff[1024];
	char readBuff[1024];
	int portno = atoi(ConnectPort);
	struct sockaddr_in serv_addr;
	char UploadName[1024];
	char DownloadName[1024];

	memset(writeBuff, 0,sizeof(writeBuff));
	memset(readBuff, 0,sizeof(readBuff));
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		printf("\n Error : Socket can't be Created.\n");
		return 1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr)); 

	serv_addr.sin_port = htons(portno);
	serv_addr.sin_family = AF_INET;

	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0){
		printf("\n inet_pton error occured\n");
		return 1;
	} 

	/*for(;;)
	{
		if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		{
			continue;
		}
		else
		{
			printf("Client is Connected now : \n");
			break;
		}
	}
*/
	int a , count = 0 , filedownload = 0 , fileupload = 0;

	for(;;){
		fileupload = 0;
		filedownload = 0;
		FILE *fp = NULL;
		int i;
		char *cresponse = malloc(Max_Packet_Length);
		fgets(writeBuff,sizeof(writeBuff),stdin);

		char copy[1024];
		char *fName;
		strcpy(copy,writeBuff);
		fName = malloc(1024);
		fName = strtok(copy," \n");
		if(strcmp(fName,"FileDownload") == 0){
			filedownload = 1;
			fName = strtok(NULL," \n");
			strcpy(DownloadName,fName);
			fp = fopen(DownloadName,"wb");
		}
		if(strcmp(fName,"FileUpload") == 0){
			fileupload = 1;
			fName = strtok(NULL," \n");
			strcpy(UploadName,fName);
			FILE *f = fopen(UploadName, "r");
			fseek(f, 0, SEEK_END);
			unsigned long len = (unsigned long)ftell(f);
			char size[1024];
			memset(size, 0, sizeof(size));
			sprintf(size,"%ld\n",len);
			strcat(writeBuff,size);
			fclose(f);
		}
		write(sockfd, writeBuff , strlen(writeBuff));

		n = read(sockfd, readBuff, sizeof(readBuff)-1);
		size_t bytes_read;
		if(strcmp(readBuff,"FileUpload Accept\n") == 0)
		{
			int b,c;
			printf("Response 1\nUpload Accepted\n");
			verify_handler(UploadName);
			b = 0;
			while (b < 1)
			{
				sprintf(cresponse, "%s, ",hdata[b].fName);
				strcat(writeBuff,cresponse);
				c = 0;
				while (c < MD5_DIGEST_LENGTH)
				{
					sprintf(cresponse, "%02x",hdata[b].hash[c]);
					strcat(writeBuff,cresponse);
					c++;
				}
				sprintf(cresponse, ", %s",ctime(&hdata[b].mtime));
				strcat(writeBuff,cresponse);
				b++;
			}
			write(sockfd , writeBuff , bytes_read);
			printf("%s\n",writeBuff);
			memset(writeBuff, 0, sizeof(writeBuff));
			fp = fopen(UploadName,"rb");
			for(;!feof(fp);){
				bytes_read = fread(cresponse, 1, 1024, fp);
				cresponse[1024] = 0;
				memcpy(writeBuff,cresponse,bytes_read);
				write(sockfd , writeBuff , bytes_read);
				memset(writeBuff, 0, sizeof(writeBuff));
				memset(cresponse, 0, sizeof(cresponse));
			}
			memcpy(writeBuff,"~@~",3);
			write(sockfd , writeBuff , 3);
			memset(readBuff, 0, strlen(readBuff));
			memset(writeBuff, 0, sizeof(writeBuff));
			fclose(fp);
		}
		else if(strcmp(readBuff,"FileUpload Deny\n") == 0){
			printf("Response 1\nUpload Denied\n");
			memset(readBuff, 0,sizeof(readBuff));
			continue;
		}
		else{
			for(;;)
			{
				readBuff[n] = 0;
				if(readBuff[n-1] == '~' && readBuff[n-3] == '~' && readBuff[n-2] == '@')
				{
					readBuff[n-3] = 0;
					if(filedownload == 1)
					{
						fwrite(readBuff,1,n-3,fp);
						fclose(fp);
					}
					else
					{
						strcat(cresponse,readBuff);
					}
					memset(readBuff, 0,strlen(readBuff));
					break;
				}
				else
				{
					if(filedownload == 1)
					{
						fwrite(readBuff,1,n,fp);
					}
					else
					{
						strcat(cresponse,readBuff);
					}
					memset(readBuff, 0,strlen(readBuff));
				}
				n = read(sockfd, readBuff, sizeof(readBuff)-1);
				if(n < 0)
					break;
			}
		}

		if(filedownload == 0)
		{
			printf("%s\n",cresponse);
		}
		else 
		{
			printf("Response 1\nFile Downloaded\n");
		}

		if(n < 0)
		{
			printf("\n Read error \n");
		}
		memset(writeBuff, 0,sizeof(writeBuff));
		memset(readBuff, 0,sizeof(readBuff));
	}
	return 0;
}