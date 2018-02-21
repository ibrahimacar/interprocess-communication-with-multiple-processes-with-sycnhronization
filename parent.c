
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#define NO_OF_PROCESS 5

//This function sorts an array depending other one
void sort_arrays(int *names, int *times, int arr_length)
{
	for (unsigned int i = 0; i<arr_length - 1; i++) {
		unsigned int k = i;

		for (int j = i + 1; j<arr_length; j++) {
			if (times[j] < times[k]) {
				k = j;
			}
		}

		if (k != i) {
			int   time = times[k];
			int    *name = names[k];

			times[k] = times[i];
			times[i] = time;

			names[k] = names[i];
			names[i] = name;
		}
	}
}

int main(int argc, char* argv[])
{
	//initial order and burst times
	int order[] = { 4,3,2,5,1 };
	int burstTimes[]= {45,180,220,255,300};
	//Process creation and necessary handles
	
	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	SECURITY_ATTRIBUTES sa[NO_OF_PROCESS];
	HANDLE writePipe[NO_OF_PROCESS],readPipe[NO_OF_PROCESS];
	HANDLE writePipe2[NO_OF_PROCESS], readPipe2[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS];
	char* lpCommandLine[NO_OF_PROCESS] = { "child.exe", "child.exe", "child.exe","child.exe","child.exe"};	
	char *message[100];
	

	for (size_t i = 0; i < NO_OF_PROCESS; i++)
	{	
		SecureZeroMemory(&sa[i], sizeof(SECURITY_ATTRIBUTES));
		sa[i].bInheritHandle = TRUE;
		sa[i].lpSecurityDescriptor = NULL;
		sa[i].nLength = sizeof(SECURITY_ATTRIBUTES);
		

		
		//pipe that sends to child
		if (!CreatePipe(&readPipe[i], &writePipe[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("unable to create pipe\n");
			system("pause");
			exit(0);
		}
		//pipe that takes message from child
		if (!CreatePipe(&readPipe2[i], &writePipe2[i], &sa[i], 0)) //use the default buffer size.
		{
			printf("unable to create pipe\n");
			system("pause");
			exit(0);
		}
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
		SecureZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		si[i].hStdInput = readPipe[i];
		si[i].hStdOutput = writePipe2[i];
		si[i].hStdError = GetStdHandle(STD_ERROR_HANDLE);
		si[i].dwFlags = STARTF_USESTDHANDLES;		
		if (!CreateProcess(NULL,
			lpCommandLine[i],
			NULL,
			NULL,
			TRUE,
			CREATE_NO_WINDOW,
			NULL,
			NULL,
			&si[i],
			&pi[i]))
		{
			printf("unable to create process: %d\n", i);
			system("pause");
			ExitProcess(0);
		}
		else
		{
			processHandles[i] = pi[i].hProcess;
			
		}		
	}
	
	int bytes=0;
	char messageFromChild[100];
	int bytesWritten = 0;
	int actualLengths[5];
	//random time number 
	srand((unsigned)time(NULL));
	int oldTimes[5];
	//initialize the message
	sprintf(message, "%d %d", 0, 0);
	for (size_t j = 0; j < NO_OF_PROCESS; j++) {		
		for (size_t i = 0; i < NO_OF_PROCESS; i++)
		{
			
			actualLengths[i] = (rand() % 300) + 50;
			sprintf(message, "%d %d", actualLengths[i], order[i]);
		
			while (1) {
				if (!WriteFile(writePipe[order[i] - 1], message, 100, &bytesWritten, NULL))
				{
					printf("unable to write to pipe\n");
								
				}
				else {
					break;
				}
					
			}
			while (1) {
				if (!ReadFile(readPipe2[order[i] - 1], messageFromChild, sizeof(messageFromChild), &bytes, NULL)) {
					printf("Unable to read pipe!");
				}
				else
				{
					printf(messageFromChild);
					break;
				}
			}	
			
		}
		//update burst times and save old ones for print
		for (size_t i = 0; i < NO_OF_PROCESS; i++)
		{
			
			oldTimes[order[i]-1] = burstTimes[order[i] - 1];
			burstTimes[order[i] - 1] = (actualLengths[order[i] - 1] + burstTimes[order[i] - 1]) / 2;
		}

		
		//necessary prints
		printf("Process     Tn      tn      tn+1\n");
		for (size_t i = 0; i < 5; i++)
		{
			printf("P%d          %d     %d      %d\n", order[i],oldTimes[i], actualLengths[i],burstTimes[i]);
			

			}
		sort_arrays(&order, burstTimes, NO_OF_PROCESS);
	}
	//close handles
	for (size_t i = 0; i < NO_OF_PROCESS; i++)
	{
		
		CloseHandle(pi[i].hProcess);
	}
	
	
	system("pause");
	return 1;
}

