#include "heap.h"
#include "cola.h"
#include "turno.h"
#include "paciente.h"
#include <string.h>

// Definicion de la estructura

struct turno{
    heap_t* regular;
    cola_t* urgente;
    size_t cant_espera_urg;
};

// Funciones auxiliares

int _cmp_antiguedad_paciente(paciente_t* a, paciente_t* b){
    if(paciente_antiguedad(a) == paciente_antiguedad(b)) return 0;
    return paciente_antiguedad(a) > paciente_antiguedad(a) ? 1 : -1;
}

int cmp_antiguedad_paciente(const void* a, const void* b){
    return _cmp_antiguedad_paciente((paciente_t*) a, (paciente_t*) b);
}

// Primitivas

turno_t* turno_crear(){
    turno_t* turno = malloc(sizeof(turno_t));
    if(!turno) return NULL;
    turno->urgente = cola_crear();
    if(!turno->urgente){
        free(turno);
        return NULL;
    }
    turno->regular = heap_crear(cmp_antiguedad_paciente);
    if(!turno->regular){
        free(turno);
        cola_destruir(turno->urgente, NULL);
        return NULL;
    }
    turno->cant_espera_urg = 0;
    return turno;
}

size_t ver_cant_espera_reg(turno_t* turno){
    return heap_cantidad(turno->regular);
}

size_t ver_cant_espera_urg(turno_t* turno){
    return turno->cant_espera_urg;
}

bool encolar_turno_urg(turno_t* turno, paciente_t* paciente){
    if(cola_encolar(turno->urgente, paciente)){
        turno->cant_espera_urg++;
        return true;
    }
    return false;
}

bool encolar_turno_reg(turno_t* turno, paciente_t* paciente){
    return heap_encolar(turno->regular, paciente);
}

bool desencolar_turno_reg(turno_t* turno){
    return heap_desencolar(turno->regular);
}

bool desencolar_turno_urg(turno_t* turno){
    if(cola_desencolar(turno->urgente)){
        turno->cant_espera_urg--;
        return true;
    }
    return false;
}

void turno_destruir(turno_t* turno){
    cola_destruir(turno->urgente, paciente_destruir);
    heap_destruir(turno->regular, paciente_destruir);
    free(turno);
}