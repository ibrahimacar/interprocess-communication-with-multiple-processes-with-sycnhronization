
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
int main(int argc, char* argv[])
{
	//Necessery initials
	HANDLE read, write;
	read = GetStdHandle(STD_INPUT_HANDLE);
	write = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE* handles;
	int *bytesWritten = 0;
	char messageFromParent[100];
	int time;
	int processNumber;
	int bytes;
	char *tokenizer;

	//Check arguments for error
	if (argc != 1)
	{
		printf("error in child process...now exiting %s\n", argv[0]);
		system("pause");
		exit(0);
	}

	//Read message from parent

	for (size_t i = 0; i < 5; i++)
	{
			while (1) {
				if (!ReadFile(read, messageFromParent, 100, &bytesWritten, NULL))

				{

					fprintf(stderr, "cant read\n");

				}
				else {
					break;
				}

			}
			//Divide and use message from parent with tokenizer
			tokenizer = strtok(messageFromParent, " ");
			time = atoi(tokenizer);
			tokenizer = strtok(NULL, " ");
			processNumber = atoi(tokenizer);

			//Burst time for child
			Sleep(time);


			//message to parent
			char sendingMessage[100];
			sprintf(sendingMessage, "P%d started\nP%d ended \n", processNumber, processNumber);
			int len = strlen(sendingMessage);
			len++;
			//send message to parent
			while (1) {
				if (!WriteFile(write, sendingMessage, len, &bytes, NULL)) {
					fprintf(stderr, "Write Error!\n");
				}
				else {
					break;
				}

			}
	}
	system("pause");
	return 1;
}
