#include "paciente.h"
#include "hash.h"
#include "lista.h"
#include <stdio.h>
#include <time.h>

struct paciente{
    char* nombre;
    size_t antiguedad;
};

int calcular_anio_actual(){
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    return timeinfo->tm_year + 1900;
}

paciente_t* paciente_crear(char* nombre, void* anio_inscripcion){
    paciente_t* paciente = malloc(sizeof(paciente_t));
    if(!paciente) return NULL;
    paciente->nombre = nombre;
    int anio_actual = calcular_anio_actual();
    paciente->antiguedad = (size_t) anio_actual - (size_t) anio_inscripcion;
    return paciente;
}

void* crear_paciente_desde_csv(char** campos, void* extra){
    return paciente_crear(campos[0], campos[1]);
}

char* paciente_nombre(paciente_t* paciente){
    return paciente->nombre;
}

size_t paciente_antiguedad(paciente_t* paciente){
    return paciente->antiguedad;
}

void paciente_destruir(paciente_t* paciente){
    free(paciente);
}

bool paciente_pertenece_a_hash(hash_t* hash_pacientes, char* nombre){
    return hash_pertenece(hash_pacientes, nombre);
}