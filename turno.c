#include "heap.h"
#include "cola.h"
#include "turno.h"
#include "paciente.h"
#include <string.h>

// Definicion de la estructura

struct turno{
    heap_t* regulares;
    cola_t* urgentes;
    size_t cant_espera_reg;
    size_t cant_espera_urg;
};

// Funciones auxiliares

int _cmp_antiguedad_paciente(paciente_t* a, paciente_t* b){
    
}


int cmp_antiguedad_paciente(const void* a, const void* b){
    return _cmp_antiguedad_paciente((paciente_t*) a, (paciente_t*) b);
}

// Primitivas

turno_t* turno_crear(){
    turno_t* turno = malloc(sizeof(turno_t));
    if(!turno) return NULL;
    turno->urgentes = cola_crear();
    if(!turno->urgentes){
        free(turno);
        return NULL;
    }
    turno->regulares = heap_crear(strcmp);
    if(!turno->regulares){
        free(turno);
        cola_destruir(turno->urgentes, NULL);
        return NULL;
    }
    turno->cant_espera_reg = 0;
    turno->cant_espera_urg = 0;
    return turno;
}



