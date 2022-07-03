#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "fragmenta.h"
#define COLOR_CYAN	"\x1b[36m"
#define COLOR_RESET	"\x1b[0m"

void fin(int sig);

int main(void)
{	
	int pid, i;
	char str[40];
	signal(SIGTERM, fin);
	
	while (1)
	{
		printf(COLOR_CYAN "minishell\\> " COLOR_RESET);
		scanf("%40[^\n]", str);
		fgetc(stdin);
		
		char **lista = fragmenta(str);
		
		if (strcmp(lista[0], "exit") == 0){
			printf("\nFin de la ejecución \n");
			
			exit(1);
		}
		pid = fork();
		if (pid == -1) {
			perror("Error al crear el proceso hijo\n");
			exit(2);
		}
		else if (pid == 0) {
			i = 0;
			while (lista[i] != NULL) {
				if (strcmp(lista[i], ">") == 0) {
					int fichero;
					fichero = open(lista[i+1], O_TRUNC|O_WRONLY|O_CREAT, 0600);
					if (fichero == -1) {
						perror("Error en la creación del fichero \n");
					}
					if (dup2(fichero, STDOUT_FILENO) == -1) {
						perror("Error al usar dup2\n");
						exit(2);
					}
					close(fichero);
					lista[i] = NULL;
				}
				else if (strcmp(lista[i], ">>") == 0) {
					int fichero;
					fichero = open(lista[i+1], O_APPEND|O_WRONLY|O_CREAT, 0600);
					if (fichero == -1) {
						perror("Error en la creación del fichero \n");
					}
					if (dup2(fichero, STDOUT_FILENO) == -1) {
						perror("Error al usar dup2\n");
					}
					close(fichero);
					lista[i] = NULL;
					
				}
				else if(strcmp(lista[i], "<") == 0) {
					int fichero;
					fichero = open(lista[i+1], O_RDONLY, 0666);
					if (fichero == -1) {
						perror("Error en la creación del fichero \n");
					}
					if (dup2(fichero, STDIN_FILENO) == -1) {
						perror("Error al usar dup2\n");
					}
					close(fichero);

					lista[i] = NULL;
				}
				else if (strcmp(lista[i], "|") == 0){
					int tuberia[2];
					if (pipe(tuberia) == -1){
						perror("Error en la creación de la tubería \n");
						exit(-1);
					}
					int pid2;
					pid2 = fork();
					if (pid2 == -1) {
						perror("Eror al crear el proceso hijo \n");
						exit(-1);
					}
					if (pid2 == 0) {
						close(tuberia[0]);
						if (dup2(tuberia[1], STDOUT_FILENO) == -1) {
							perror("Error al usar dup2\n");
							exit(-1);
						}
						close(tuberia[1]);
					}
					else {
						char **nueva_lista;
						nueva_lista = &lista[i+1];
						close(tuberia[1]);
						if (dup2(tuberia[0], STDIN_FILENO) == -1) {
							perror("Error al usar dup2 \n");
							exit(-1);
						}
						close(tuberia[0]);
						waitpid(pid2, 0, 0);
						execvp(nueva_lista[0], nueva_lista);
					}	
				}
				i++;
			}
			execvp(lista[0], lista);
		}
		else {
			waitpid(pid, 0, 0);
		}
		borrarg(lista);
		
		kill(getppid(), 15);
	}
	return 0;
}

void fin(int sig) {
	printf("\nFin de la ejecución\n");
	exit(1);
}

