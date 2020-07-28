#include "heap.h"
#include "cola.h"
#include "doctor.h"
#include "turno.h"
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

size_t ver_cant_espera(turno_t* turno){
    return turno->cant_espera_urg + heap_cantidad(turno->regular);
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

paciente_t* desencolar_turno_reg(turno_t* turno){
    return heap_desencolar(turno->regular);
}

paciente_t* desencolar_turno_urg(turno_t* turno){
    paciente_t* paciente = cola_desencolar(turno->urgente);
    if(!paciente) return NULL;
    turno->cant_espera_urg--;
    return paciente;
}

cola_t* ver_cola_turnos_urgentes(turno_t* turno){
    return turno->urgente;
}

heap_t* ver_heap_turnos_regulares(turno_t* turno){
    return turno->regular;
}

void turno_destruir(turno_t* turno){
    // No les paso funcion de destruccion porque asumo que solo vamos a destruir
    // esta estructura cuando esten vacias ambas colas.
    cola_destruir(turno->urgente, NULL);
    heap_destruir(turno->regular, NULL);
    free(turno);
}

bool especialidad_pertenece_a_hash_turnos(hash_t* hash_turnos, char* especialidad){
    return hash_pertenece(hash_turnos, especialidad);
}

