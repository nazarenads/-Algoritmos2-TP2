#define  _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strutil.h"
#include "mensajes.h"
#include "comandos.h"
#include "clinica.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void procesar_comando(char* comando, char** parametros, clinica_t* clinica) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		pedir_turno(parametros, clinica);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {

	} else {

	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(clinica_t* clinica) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros, clinica);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}



int main(int argc, char** argv) {
	if (argc != 3) printf(ENOENT_CANT_PARAMS);
	clinica_t* clinica = clinica_crear(argv[1], argv[2]);
	if (!clinica) return 1;
	procesar_entrada(clinica);
	clinica_destruir(clinica);
	return 0;
}
