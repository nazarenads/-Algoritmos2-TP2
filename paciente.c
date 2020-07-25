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

char* paciente_nombre(paciente_t* paciente){
    return paciente->nombre;
}

size_t paciente_antiguedad(paciente_t* paciente){
    return paciente->antiguedad;
}

void paciente_destruir(paciente_t* paciente){
    free(paciente);
}

bool guardar_paciente_en_hash(hash_t* hash, paciente_t* paciente){
    char* nombre = paciente_nombre(paciente);
    size_t antiguedad = paciente_antiguedad(paciente);
    bool guardado = hash_guardar(hash, nombre, (void*)antiguedad);
    if (!guardado) return false;
    return true;
}


hash_t* paciente_hash_crear(lista_t* lista_pacientes){
    hash_t* hash_pacientes = hash_crear(NULL);
    if (!hash_pacientes) return NULL;
    lista_iter_t* iter = lista_iter_crear(lista_pacientes);
    while (!lista_iter_al_final(iter)){
        paciente_t* paciente = (paciente_t*)lista_iter_ver_actual(iter);
        bool guardado = guardar_paciente_en_hash(hash_pacientes, paciente);
        if (!guardado) return NULL;
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    lista_destruir(lista_pacientes, free);
    return hash_pacientes;
}

bool paciente_pertenece_a_hash(paciente_t* paciente, hash_t* hash_pacientes){
    return hash_pertenece(hash_pacientes, paciente->nombre);
}