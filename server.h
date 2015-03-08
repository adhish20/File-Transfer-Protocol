//  ADHISH SINGLA
//  201403004
//  Computer Networks Assignment
//  Server Functions(TCP and UDP)

int tcp_server(char *ListenPort)
{
	int listenfd = 0;
	int connfd = 0; 
	int portno = atoi(ListenPort);
	struct sockaddr_in serv_addr;

	char writeBuff[1024];
	char readBuff[1024];
	time_t ticks; 

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1){
		perror("Unable to create socket");
		exit(0);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(writeBuff, 0, sizeof(writeBuff));
	memset(readBuff, 0, sizeof(readBuff));

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno); 

	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		perror("Unable to bind");
		exit(0);
	}

	listen(listenfd, 10); 
	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

	int a , n , b , c;
	n = read(connfd,readBuff,sizeof(readBuff));


	for(;n > 0;)
	{
		size_t size = strlen(readBuff) + 1;
		char *request = malloc(size);
		strcpy(request,readBuff);

		strcpy(Hist[hist_count],readBuff);
		hist_count++;

		int type_request = parse_request(request);
	printf("\nReceived Message : %s",request);
	response[0] = '\0';
		writeBuff[0] = '\0';
	printf("Command: ");
	fflush(stdout);
		if(type_request == -1){
			error = 1;
			//sprintf(response,"ERROR: No request of this type.\n");
		}
		else if(type_request == 1)      //Indexget
		{
			IndexGet_handler(request);
		}
		else if(type_request == 2)      //FileHash
		{
			FileHash_handler(request);
		}
		else if(type_request == 3)      //FileDownload
		{
			FileDownload_handler(request);
		}

		if(error == 1)
		{
			strcat(writeBuff,response);
			strcat(writeBuff,"~@~");
			write(connfd , writeBuff , strlen(writeBuff));
			error = -1;
			memset(readBuff, 0, sizeof(readBuff));
			memset(writeBuff, 0, sizeof(writeBuff));
			for(;(n = read(connfd,readBuff,sizeof(readBuff)))<=0;);
			continue;
		}
		if(type_request == 1 && regex == 1){
			strcat(writeBuff,response);
			a = 0;
			while(a < i)
			{
				sprintf(response, "%-35s| %-10d| %-3c| %-20s" , pdata[a].fName , pdata[a].size , pdata[a].type , ctime(&pdata[a].mtime));
				strcat(writeBuff,response);
				a++;
			}
			strcat(writeBuff,"~@~");

			write(connfd , writeBuff , strlen(writeBuff));
		}
		else if(type_request == 1)
		{
			strcat(writeBuff,response);

			a = 0;
			while(a < i)
			{
				sprintf(response, "%-35s| %-10d| %-3c| %-20s" , pdata[a].fName , pdata[a].size , pdata[a].type , ctime(&pdata[a].mtime));
				strcat(writeBuff,response);
				a++;
			}
			strcat(writeBuff,"~@~");

			write(connfd , writeBuff , strlen(writeBuff));
		}
		else if(type_request == 2)
		{
			strcat(writeBuff,response);

			b = 0;
			while(b < i)
			{
				sprintf(response, "%-35s |   ",hdata[b].fName);
				strcat(writeBuff,response);
				for (c = 0 ; c < MD5_DIGEST_LENGTH ; c++)
				{
					sprintf(response, "%x",hdata[b].hash[c]);
					strcat(writeBuff,response);
				}
				sprintf(response, "\t %20s",ctime(&hdata[b].mtime));
				strcat(writeBuff,response);
				b++;
			}
			strcat(writeBuff,"~@~");

			write(connfd , writeBuff , strlen(writeBuff));
		}
		else if(type_request == 3)
		{
			FILE* fp;
			fp = fopen(FDownName,"rb");
			size_t bytes_read;
			for(;!feof(fp);)
			{
				bytes_read = fread(response, 1, 1024, fp);
				memcpy(writeBuff,response,bytes_read);
				write(connfd , writeBuff , bytes_read);
				memset(writeBuff, 0, sizeof(writeBuff));
				memset(response, 0, sizeof(response));
			}
			memcpy(writeBuff,"~@~",3);
			write(connfd , writeBuff , 3);
			memset(writeBuff, 0, sizeof(writeBuff));
			fclose(fp);
		}
		else if(type_request == 4)
		{
			printf("FileUpload Accepted\n");
			memcpy(writeBuff,"FileUpload Accept\n",18);
			write(connfd , writeBuff , 18);
			memset(writeBuff, 0,18);
			char copyrequest[1024];
			memset(copyrequest, 0,1024);
			memcpy(copyrequest,request,1024);
			char *size = strtok(copyrequest,"\n");
			size = strtok(NULL,"\n");
			long fsize = atol(size);
			char *request_data = NULL;
			const char delim[] = " \n";
			request_data = strtok(request,delim);
			request_data = strtok(NULL,delim);
			int f;
			int result;
			f = open(request_data, O_WRONLY | O_CREAT | O_EXCL, (mode_t)0600);
			if (f == -1) {
				perror("Error opening file for writing:");
				return 1;
			}
			result = lseek(f,fsize-1, SEEK_SET);
			result = write(f, "", 1);
			if (result < 0) {
				close(f);
				perror("Error opening file for writing:");
				return 1;
			}
			close(f);
			FILE *fp;
			fp = fopen(request_data,"wb");
			n = read(connfd, readBuff, sizeof(readBuff)-1);
			while(1)
			{
				readBuff[n] = 0;
				if(readBuff[n-1] == '~' && readBuff[n-3] == '~' && readBuff[n-2] == '@')
				{
					readBuff[n-3] = 0;
					fwrite(readBuff,1,n-3,fp);
					fclose(fp);
					memset(readBuff, 0,n-3);
					break;
				}
				else
				{
					fwrite(readBuff,1,n,fp);
					memset(readBuff, 0,n);
				}
				n = read(connfd, readBuff, sizeof(readBuff)-1);
				if(n < 0)
					break;
			}
			memset(writeBuff, 0,1024);

		}

		regex = 0;
		memset(writeBuff, 0, sizeof(writeBuff));
		memset(readBuff, 0, sizeof(readBuff)); 
		for(;(n = read(connfd,readBuff,sizeof(readBuff)))<=0;);
	}
	close(connfd);
	wait(NULL);
}

int udp_server(char *ListenPort)
{
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr; 
	int portno = atoi(ListenPort);

	char readBuff[1024];
	char writeBuff[1024];
	time_t ticks; 

	listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr)); 
	memset(writeBuff, 0, sizeof(writeBuff)); 
	memset(readBuff, 0, sizeof(readBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(portno); 

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 


	int a , n , b , c;
	n = read(listenfd,readBuff,sizeof(readBuff));


	while( n > 0)
	{
		size_t size = strlen(readBuff) + 1;
		char *request = malloc(size);
		strcpy(request,readBuff);
		printf("%s\n",request);
		fflush(stdout);

		strcpy(Hist[hist_count],readBuff);
		hist_count++;

		int type_request = parse_request(request);
		if(type_request == -1)      //Error
		{
			error = 1;
			//sprintf(response,"ERROR: No request of this type.\n");
		}
		else if(type_request == 1)      //Indexget
		{
			IndexGet_handler(request);
		}
		else if(type_request == 2)      //FileHash
		{
			FileHash_handler(request);
		}
		else if(type_request == 3)      //FileDownload
		{
			FileDownload_handler(request);
		}

		if(error == 1)
		{
			strcat(writeBuff,response);
			strcat(writeBuff,"~@~");
			write(listenfd , writeBuff , strlen(writeBuff));
			error = -1;
			memset(readBuff, 0, sizeof(readBuff));
			memset(writeBuff, 0, sizeof(writeBuff)); 
			for(;(n = read(listenfd,readBuff,sizeof(readBuff)))<=0;);
			continue;
		}
		if(type_request == 1 && regex == 1)
		{
			sprintf(response, "Response %d\n" , i);
			strcat(writeBuff,response);
			a = 0;
			while(a < i)
			{
				sprintf(response, "%s, %d, %c\n" , pdata[a].fName , pdata[a].size , pdata[a].type );
				strcat(writeBuff,response);
				a++;
			}
			strcat(writeBuff,"~@~");

			write(listenfd , writeBuff , strlen(writeBuff));
		}
		else if(type_request == 1)
		{
			sprintf(response, "Response %d\n" , i);
			strcat(writeBuff,response);

			a = 0;
			while(a < i)
			{
				sprintf(response, "%s, %d, %c, %s" , pdata[a].fName , pdata[a].size , pdata[a].type , ctime(&pdata[a].mtime));
				strcat(writeBuff,response);
				a++;
			}
			strcat(writeBuff,"~@~");

			write(listenfd , writeBuff , strlen(writeBuff));
		}
		else if(type_request == 2)
		{
			sprintf(response, "Response %d\n" , i);
			strcat(writeBuff,response);
			b = 0;
			while( b < i)
			{
				sprintf(response, "%s, ",hdata[b].fName);
				strcat(writeBuff,response);
				c = 0;
				while(c < MD5_DIGEST_LENGTH)
				{
					sprintf(response, "%02x",hdata[b].hash[c]);
					strcat(writeBuff,response);
					c++;
				}
				sprintf(response, ", %s",ctime(&hdata[b].mtime));
				strcat(writeBuff,response);
				b++;
			}
			strcat(writeBuff,"~@~");

			write(listenfd , writeBuff , strlen(writeBuff));
		}
		else if(type_request == 3)
		{
			FILE* fp;
			fp = fopen(FDownName,"rb");
			size_t bytes_read;
			for(;!feof(fp);)
			{
				bytes_read = fread(response, 1, 1024, fp);
				memcpy(writeBuff,response,bytes_read);
				write(listenfd , writeBuff , bytes_read);
				memset(response, 0, sizeof(response));
				memset(writeBuff, 0, sizeof(writeBuff));
			}
			memcpy(writeBuff,"~@~",3);
			write(listenfd , writeBuff , 3);
			memset(writeBuff, 0, sizeof(writeBuff));
			fclose(fp);
		}
		else if(type_request == 4)
		{
			printf("FileUpload Accepted\n");
			memcpy(writeBuff,"FileUpload Accept\n",18);
			write(listenfd , writeBuff , 18);
			memset(writeBuff, 0,18);
			char copyrequest[1024];
			memset(copyrequest, 0,1024);
			memcpy(copyrequest,request,1024);
			char *size = strtok(copyrequest,"\n");
			size = strtok(NULL,"\n");
			long fsize = atol(size);
			char *request_data = NULL;
			const char delim[] = " \n";
			request_data = strtok(request,delim);
			request_data = strtok(NULL,delim);
			int f;
			int result;
			f = open(request_data, O_WRONLY | O_CREAT | O_EXCL, (mode_t)0600);
			if (f == -1) {
				perror("Error opening file for writing:");
				return 1;
			}
			result = lseek(f,fsize-1, SEEK_SET);
			result = write(f, "", 1);
			if (result < 0) {
				close(f);
				perror("Error opening file for writing:");
				return 1;
			}
			close(f);
			FILE *fp;
			fp = fopen(request_data,"wb");
			n = read(listenfd, readBuff, sizeof(readBuff)-1);
			for(;;)
			{
				readBuff[n] = 0;
				if(readBuff[n-1] == '~' && readBuff[n-3] == '~' && readBuff[n-2] == '@')
				{
					readBuff[n-3] = 0;
					fwrite(readBuff,1,n-3,fp);
					fclose(fp);
					memset(readBuff, 0,n-3);
					break;
				}
				else
				{
					fwrite(readBuff,1,n,fp);
					memset(readBuff, 0,n);
				}
				n = read(listenfd, readBuff, sizeof(readBuff)-1);
				if(n < 0)
				{
					break;
				}
			}
			memset(writeBuff, 0,1024);

		}

		regex = 0;
		memset(writeBuff, 0, sizeof(writeBuff));
		memset(readBuff, 0, sizeof(readBuff));
		for(;(n = read(listenfd,readBuff,sizeof(readBuff)))<=0;);
	}
	close(listenfd);
	wait(NULL);
}
