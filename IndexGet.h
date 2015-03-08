//  ADHISH SINGLA
//  201403004
//  Computer Networks Assignment
//  Handler for IndexGet Function

int shortlist_handle(time_t start_time,time_t end_time)
{
	DIR *dp;
	i = 0;
	struct dirent *ep;
	dp = opendir ("./");
	struct stat fileStat;

	if (dp == NULL)
	{
		error = 1;
		sprintf(response,"ERROR : Directory Can't be Opened.\n");
	}
	else
	{
		for (;ep = readdir (dp);)
		{
			if(stat(ep->d_name,&fileStat) < 0)
			{
				return 1;
			}
			else if(difftime(fileStat.st_mtime,start_time) > 0 && difftime(end_time,fileStat.st_mtime) > 0)
			{
				strcpy(pdata[i].fName , ep->d_name);
				pdata[i].mtime = fileStat.st_mtime;
				pdata[i].size = fileStat.st_size;
				pdata[i].type = (S_ISDIR(fileStat.st_mode)) ? 'd' : '-';
				i++;
			}
		}
		closedir (dp);
	}
}

int longlist_handle()
{
	DIR *dp;
	i = 0; 
	struct dirent *ep;
	dp = opendir ("./");
	struct stat fileStat;

	if (dp != NULL)
	{
		for (;ep = readdir (dp);)
		{
			if(stat(ep->d_name,&fileStat) < 0)
				return 1;
			else
			{
				strcpy(pdata[i].fName, ep->d_name);
				pdata[i].mtime = fileStat.st_mtime;
				pdata[i].size = fileStat.st_size;
				pdata[i].type = (S_ISDIR(fileStat.st_mode)) ? 'd' : '-';
				i++;
			}
		}
		closedir (dp);
	}
	else
	{
		printf("ERROR : Directory Can't be Opened.\n");
	}
}

int regex_handle(char *regexp)
{
	FILE *pipein_fp;
	char str[1024];
	char string[1024] = "ls ";
	memset(str, 0,sizeof(str));
	char line[1024];
	char readbuf[1024];
	i = 0;
	regex = 1;

	strncpy(str,regexp+1,strlen(regexp)-2);
	strcat(string,str);
	regex_t regex;
	int reti;
	reti = regcomp(&regex, regexp, 0);
	if(!reti)
		perror("Regular Expression invalid.\n");
	DIR *dp;
	int a;
	struct dirent *ep;
	dp = opendir ("./");
	struct stat fileStat;

	if (dp != NULL){
		for(;ep = readdir (dp);)
		{
			if(regexec(&regex, ep->d_name, 0, NULL, 0))
				continue;
			if(stat(ep->d_name,&fileStat) < 0)
			{
				return 1;
			}
			else
			{
				if (( pipein_fp = popen(string, "r")) == NULL)
				{
					perror("popen");
					exit(1);
				}
				for(;fgets(readbuf, 1024, pipein_fp);)
				{
					strncpy(line,readbuf,strlen(readbuf)-1);
					if(strcmp(line,ep->d_name) == 0)
					{
						strcpy(pdata[i].fName , ep->d_name);
						pdata[i].type = (S_ISDIR(fileStat.st_mode)) ? 'd' : '-';
						pdata[i].size = fileStat.st_size;
						i++;
						break;
					}
					memset(line, 0,sizeof(line));
				}
			}
		}

		pclose(pipein_fp);
	}
	else
	{
		error = 1;
		sprintf(response,"ERROR : Directory Can't be Opened.");
	}
}

void IndexGet_handler(char *request)
{
	char *request_data = NULL;
	char Delimeter[] = " \n";
	struct tm tm;
	time_t start_time , end_time;
	int enter = 1;
	char *regexp;

	request_data = strtok(request,Delimeter);
	request_data = strtok(NULL,Delimeter);
	if(request_data == NULL)
	{
		error = 1;
		sprintf(response,"ERROR: Please Enter Command Again with this Format:\nIndexGet<space>--longlist\nIndexGet<space>--shortlist<space>StartTimeStamp<space>EndTimeStamp\nIndexGet<space>--regex<space>Search");
		return;
	}
	else
	{
		if(strcmp(request_data,"--longlist") == 0)
		{
			request_data = strtok(NULL,Delimeter);

			if(request_data != NULL)
			{
				printf("Entered in error\n");
				error = 1;
				sprintf(response,"ERROR: Please Enter Command Again with this Format:\nIndexGet --longlist\n");
				return;
			}
			else
				longlist_handle();
		}
		else if(strcmp(request_data,"--shortlist") == 0)
		{
			request_data = strtok(NULL,Delimeter);
			if(request_data == NULL)
			{
				error = 1;
				sprintf(response,"ERROR: Please Enter Command Again with this Format:\nIndexGet --shortlist <timestamp1> <timestamp2>\n");
				return;
			}
			while(request_data != NULL)
			{
				if(enter >= 3)
				{
					error = 1;
					sprintf(response,"ERROR: Please Enter Command Again with this Format:\nIndexGet --shortlist <timestamp1> <timestamp2>\n");
					return;
				}
				if (strptime(request_data, "%d-%b-%Y-%H:%M:%S", &tm) == NULL)
				{
					error = 1;
					sprintf(response,"ERROR: Please Enter Command Again with this Format:\nDate-Month-Year-hrs:min:sec\n");
					return;
				}
				if(enter == 1)
					start_time = mktime(&tm);
				else
					end_time = mktime(&tm);
				enter++;
				request_data = strtok(NULL,Delimeter);
			}
			shortlist_handle(start_time,end_time);
		}
		else if(strcmp(request_data,"--regex") == 0)
		{
			request_data = strtok(NULL,Delimeter);
			if(request_data == NULL)
			{
				printf("ERROR: Please Enter Command Again with this Format:\nIndexGet --regex <regular expression>\n");
				_exit(1);
			}
			regexp = request_data;
			request_data = strtok(NULL,Delimeter);
			if(request_data != NULL)
			{
				printf("ERROR: Please Enter Command Again with this Format:\nIndexGet --regex <regular expression>\n");
				_exit(1);
			}
			regex_handle(regexp);
		}
		else
		{
			error = 1;
			sprintf(response,"ERROR: Wrong Format.\n");
			return;
		}
	}
}
