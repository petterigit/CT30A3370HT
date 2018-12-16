/* Shell with basic functionality
   Petteri Mäkelä
   0506167
   16/12/18
   */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>



size_t get_command(size_t, size_t, char*);
size_t get_commandline(char *, int, size_t, size_t, char*);
int check_command(char *, char(*)[64]);
int run_command(char **, char(*)[64], int);
int my_cd(char **, char *);
int my_path(char **, char(*)[64]);
int check_flags(char **);

int main(int argc, char **argv) {

	/* Error message */
	char error_message[30] = "An error has occured\n";

	/* int i is used for index at different points */
	int i = 0;

	/* int j for batch mode indexing */
	int j = 1;

	/* Allocate buffer for commands */
	size_t command;
	size_t buffersize = 4096;
	char *buffer;
	if (!(buffer = (char*)malloc(buffersize * sizeof(char)))) {
		write(STDERR_FILENO, "Unable to malloc buffer\n", strlen("Unable to malloc buffer\n"));
		exit(-1);
	}
	/* Define strtok() variables */
	char *strtok_pointer;
	char *flags[64];

	/* Define commandpath */
	char commandpath[32][64]={"/bin/"};
	for (int i = 1; i < 32; i++) {
			memset(commandpath[i],0,64);
	}

	/* Define working directory */
	char path[256];
	if (getcwd(path, sizeof(path)) == NULL) {
		write(STDERR_FILENO, "Could not get CWD\n", strlen("Could not get CWD\n"));
		exit(-1);
	}

	system("clear");

	printf("Welcome to wish shell!\n");

	/* MAIN FUNCTION LOOP */
	/* INPUT MODE */
	while(1) {
			printf("Working directory: %s\n", path);
			i = 0;
			while(commandpath[i][0] != '\0') {
				printf("Commandpath %d: %s\n", i+1, commandpath[i]);
				i++;
			}


			/* GET COMMAND */
			if (argc == 1) {
				/* INPUT MODE */
				command = get_command(command, buffersize, buffer);
			} else {
				/* BATCH MODE */
				command = get_commandline(argv[1], j, command, buffersize, buffer);
				if (command == -1) {
					exit(0);
				}
				j++;
			}

			strtok(buffer, "\n");
			strtok_pointer = strtok(buffer, " ");
			flags[0] = strtok_pointer;
			i = 1;
			printf("Command given: ");
			printf("%s ", flags[0]);
			while ((strtok_pointer = strtok(NULL, " ")) != NULL) {
				printf("%s ", strtok_pointer);
				flags[i] = strtok_pointer;
				i++;
			}
			flags[i] = NULL;
			printf("\n");
			

			/* CHECK AND RUN COMMAND */
			switch(check_command(flags[0],commandpath)) {
				case 0:
					/* Exit */
					printf("Bye Bye\n");
					free(buffer);
					exit(0);
					break;
				case 1:
					/* Bash command */
					/* CHECK FLAGS (for redirection) */
					i=0;
					i = check_flags(flags);

					printf("Executing command: %s\n", flags[0]);
					printf("\n");

					if (i != 0) {
						run_command(flags, commandpath, i);
					} else {
						run_command(flags, commandpath, i);
					}
					break;
				case 2:
					/* My-cd */
					if (my_cd(flags, path) != 0) {
						write(STDERR_FILENO, error_message, strlen(error_message));
					}
					break;
				case 3:
					my_path(flags, commandpath);
					break;
				case -1:
					/* Error returned from check_command */
					write(STDERR_FILENO, "Command not found\n", strlen("Command not found\n"));
					break;
			}
			printf("\n");
			printf("...........................\n");
	}

	return(-1);
}

/* Get next command on batch mode */
size_t get_commandline(char *argv, int j, size_t command, size_t buffersize, char* buffer) {
	FILE *fp;
	fp = fopen(argv, "r");
	for (int i = 0; i<j; i++) {
		command = getline(&buffer, &buffersize, fp);
	}
	fclose(fp);
	return(command);
}


/* get_command function
   Asks for command to run
   Returns given command 		*/
size_t get_command(size_t command, size_t buffersize, char* buffer) {
	printf("Type a command!\n");
	command = getline(&buffer, &buffersize, stdin);
	return(command);
}

/* my path function
   Changes commandpath to flags given
   returns 0				*/
int my_path(char** flags, char (*commandpath)[64]) {

	for (int i = 0; i < 32; i++) {
		commandpath[i][0] = '\0';
	}

	int i = 0;
	while(1) {
		if(flags[i] == NULL){
			break;
		}
		strcpy(commandpath[i-1], flags[i]);
		i++;
	}
	return(0);
}


/* my_cd function
   returns 0 upon successful completion
   returns -1 otherwise 		*/
int my_cd(char** flags, char* path) {
	char* a = NULL;
	char newpath[64] = {};

	if (flags[1] == NULL) {
		write(STDERR_FILENO, "No flags\n", strlen("No flags\n"));
		return(-1);
	} else if (flags[2] != NULL) {
		write(STDERR_FILENO, "Too many flags\n", strlen("Too many flags\n"));;
		return(-1);
	} else {
		if (strcmp(flags[1], "..") == 0) {
			
			a = strrchr(path, '/');
			if (a) {
				(*a) = '\0';
				chdir(path);
				return(0);
			} else {
				return(-1);
			}
		} else if (flags[1][0] == '/') {
			if(!chdir(flags[1])) {
				strcpy(path, flags[1]);
				return(0);
			} else {
				return(-1);
			}
		} else {

			strcat(newpath, path);
			strcat(newpath, "/");
			strcat(newpath, flags[1]);
			if (!chdir(newpath)) {
				strcpy(path, newpath);
				return(0);
			} else {
				return(-1);
			}
		}
	}

}

/* check_command function
    Returns command type as follows:
    0 exit()
    1 command found in commandpath
    2 cd
    3 path
    -1 command not found 			*/
int check_command(char *command, char (*commandpath)[64]) {

	if (strcmp(command, "exit") == 0 ) {
		return(0);
	} else if (strcmp(command, "cd") == 0 ) {
		return(2);
	} else if (strcmp(command, "path") == 0 ) {
		return(3);
	}

	char file_location[4096];
	int i = 0;

	while(commandpath[i][0] != '\0') {
		strcpy(file_location, commandpath[i]);
		strcat(file_location, command);
		if (!access(file_location, X_OK)) {
			return(1);
		}
		i++;
	}
	return(-1);
}

/* Return index of '>' or 0 
   file = i + 1 		*/
int check_flags(char **flags) {
	int i = 0;
	while(flags[i]) {
		if (strcmp(flags[i], ">") == 0) {
			flags[i] = NULL;
			return(i);
		}
		i++;
	}
	return(0);
}

/* run_command function
   Exit with code 127 if command can not be run
   Otherwise returns 0 				*/
int run_command(char **flags, char (*commandpath)[64], int j) {
	char file_location[4096];
	int i = 0;
	while(flags[i]) {
		strcpy(file_location, commandpath[i]);
		strcat(file_location, flags[0]);
		if (!access(file_location, X_OK)) {
			break;
		}
		i++;
	}

	char command[4096];
	strcpy(command, commandpath[i]);
	strcat(command, flags[0]);
	pid_t pid=fork();


	if (pid==0) {
		int fd;
		if (j != 0) {
			fd = open(flags[j+1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
			dup2(fd, 1);
			//dup2(fd, 2);
			
		}
		execv(command, flags);
		close(fd);
		exit(127);
	} else {
		waitpid(pid, 0, 0);
	}

	return(0);
}
