#include "paciente.h"
#define ANIO_ACTUAL 2020

struct paciente{
    char* nombre;
    size_t antiguedad;
};

paciente_t* paciente_crear(char* nombre, void* antiguedad){
    paciente_t* paciente = malloc(sizeof(paciente_t));
    if(!paciente) return NULL;
    paciente->nombre = nombre;
    paciente->antiguedad = ANIO_ACTUAL - (size_t) antiguedad;
    return paciente;
}

char* paciente_nombre(paciente_t* paciente){
    return paciente->nombre;
}

char* paciente_antiguedad(paciente_t* paciente){
    return paciente->antiguedad;
}

void paciente_destruir(paciente_t* paciente){
    free(paciente);
}
