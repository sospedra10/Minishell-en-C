#~ ejecutar: minishell
#~ 	./minishell
all: fragmenta prueba
fragmenta: 	fragmenta.c
			gcc -Wall -c fragmenta.c
prueba: minishell.c fragmenta.o
		gcc -Wall -o minishell minishell.c fragmenta.o
