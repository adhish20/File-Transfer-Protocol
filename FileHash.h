//  ADHISH SINGLA
//  201403004
//  Computer Networks Assignment
//  Handler for FileHash Function

int checkall_handler()
{
	DIR *dp;
	int a;
	unsigned char c[MD5_DIGEST_LENGTH];
	i = 0;
	struct dirent *ep;
	dp = opendir ("./");
	struct stat fileStat;

	if (dp != NULL){
		for(;ep = readdir (dp);){
			if(stat(ep->d_name,&fileStat) < 0)
			{
				return 1;
			}
			else
			{
				char *fName=ep->d_name;
				hdata[i].mtime = fileStat.st_mtime;
				hdata[i].fName = ep->d_name;
				FILE *inFile = fopen (fName, "r");
				MD5_CTX mdContext;
				int bytes;
				unsigned char data[1024];

				if (inFile == NULL){
					error = 1;
					sprintf (response,"%s can't be opened.\n", fName);
					return 0;
				}

				MD5_Init (&mdContext);
				while ((bytes = fread (data, 1, 1024, inFile)) != 0)
					MD5_Update (&mdContext, data, bytes);
				MD5_Final (c,&mdContext);
				for(a = 0; a < MD5_DIGEST_LENGTH; a++)
					hdata[i].hash[a] = c[a];
				fclose (inFile);
				i++;
			}
		}
	}
	else{
		error = 1;
		sprintf(response,"Couldn't open the directory");
	}
}

int verify_handler(char *file)
{
	DIR *dp;
	unsigned char c[MD5_DIGEST_LENGTH];
	i = 0;
	int a;
	struct dirent *ep;
	dp = opendir ("./");
	struct stat fileStat;

	if (dp != NULL){
		for (;ep = readdir (dp);){
			if(stat(ep->d_name,&fileStat) < 0)
			{
				return 1;
			}
			else if(strcmp(file,ep->d_name) == 0){
				char *fName=ep->d_name;
				hdata[i].mtime = fileStat.st_mtime;
				hdata[i].fName = ep->d_name;
				FILE *inFile = fopen (fName, "r");
				MD5_CTX mdContext;
				int bytes;
				unsigned char data[1024];

				if (inFile == NULL){
					error = 1;
					sprintf(response,"%s can't be opened.\n", fName);
					return 0;
				}

				MD5_Init (&mdContext);
				while ((bytes = fread (data, 1, 1024, inFile)) != 0)
				{
					MD5_Update (&mdContext, data, bytes);
				}
				MD5_Final (c,&mdContext);
				for(a = 0; a < MD5_DIGEST_LENGTH; a++)
				{
					hdata[i].hash[a] = c[a];
				}
				fclose (inFile);
				i++;
			}
			else
			{
				continue;
			}
		}
	}
	else{
		error = 1;
		sprintf(response,"Couldn't open the directory");
	}
}

void FileHash_handler(char *request)
{
	char *request_data = NULL;
	char delim[] = " \n";
	request_data = strtok(request,delim);
	request_data = strtok(NULL,delim);
	if(request_data == NULL)
	{
		error = 1;
		sprintf(response,"ERROR: Please Enter Command Again with this Format:\nFileHash --checkall\nFileHash --verify <fName>\n");
	}
	while(request_data != NULL)
	{
		if(strcmp(request_data,"--checkall") == 0)
		{
			request_data = strtok(NULL,delim);

			if(request_data != NULL)
			{
				error = 1;
				sprintf(response,"ERROR: Please Enter Command Again with this Format:\nFileHash --checkall\n");
				return;
			}
			else
			{
				checkall_handler();
			}

		}
		else if(strcmp(request_data,"--verify") == 0)
		{
			request_data = strtok(NULL,delim);
			if(request_data == NULL)
			{
				error = 1;
				sprintf(response,"ERROR: Please Enter Command Again with this Format:\nFileHash --verify <fName>\n");
				return;
			}
			char *fName = request_data;
			request_data = strtok(NULL,delim);
			if(request_data != NULL)
			{
				error = 1;
				sprintf(response,"ERROR: Please Enter Command Again with this Format:\nFileHash --verify <fName>\n");
				return;
			}
			else
				verify_handler(fName);
		}
	}
}