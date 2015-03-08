//  ADHISH SINGLA
//  201403004
//  Computer Networks Assignment
//  Command Parser

int parse_request(char *request)
{
	char copyrequest[100];
	strcpy(copyrequest,request);
	char *command = NULL;
	const char delim[] = " \n";
	command = strtok(copyrequest,delim);
	if(command != NULL)
	{
		if(strcmp(command,"IndexGet") == 0)
			return 1;
		else if(strcmp(command,"FileHash") == 0)
			return 2;
		else if(strcmp(command,"FileDownload") == 0)
			return 3;
		else if(strcmp(command,"FileUpload") == 0)
			return 4;
		else if(strcmp(command, "Quit") == 0)
			return 5;
		else
			return -1;
	}
}