#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fragmenta.h"


void borrarg(char **arg) {
	
	int i = 0;
	while (arg[i] != NULL)
	{
		free(arg[i]);
		i++;
	}
	free(arg);
}


char** fragmenta(char str[40]) {

	char** lista;
	
	char frase[40];
	
	strcpy(frase, str);
	
	const char s[2] = " ";
	char *token;
	token = strtok(str, s);

	int espacios = 0;
	
	while (token != NULL ) 
	{	
		token = strtok(NULL, s);
		espacios++;
	}

	lista = calloc((espacios + 1), sizeof(char*));

	token = strtok(frase, s);
	
	int pos = 0;
	while (token != NULL ) {
		lista[pos] = calloc((strlen(token) + 1), sizeof(char));
		strcpy(lista[pos], token);
		
		token = strtok(NULL, s);
		pos++;
	}
	
	return lista;
}

