//  ADHISH SINGLA
//  201403004
//  Computer Networks Assignment
//  Handler for FileDownload Function

void FileDownload_handler(char *request)
{
	char *FName = NULL;
	char Delimeter[] = " \n";
	FName = strtok(request,Delimeter);
	FName = strtok(NULL,Delimeter);
	if(FName == NULL){
		error = 1;
		sprintf(response,"ERROR: Please Enter Command Again with this Format:\nFileDownload <file_name>\n");
		return;
	}
	strcpy(FDownName,FName);
	FName = strtok(NULL,Delimeter);
	if(FName != NULL){
		error = 1;
		sprintf(response,"ERROR: Please Enter Command Again with this Format:\nFileDownload <file_name>\n");
		return;
	}
}