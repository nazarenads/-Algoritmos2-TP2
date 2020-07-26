#include "paciente.h"
#include "doctor.h"
#include "turno.h"
#include <string.h>

void destruir_dato_turno(void* dato){
    turno_destruir(dato);
}


void destruir_dato_doc(void* dato){
    doctor_destruir(dato);
}


bool guardar_turno_en_hash(hash_t* hash, doctor_t* doctor){
    char* especialidad = doctor_ver_especialidad(doctor);
    turno_t* turno = turno_crear();
    bool guardado = hash_guardar(hash, especialidad, (void*)turno);
    if (!guardado) return false;
    return true;
}

hash_t* turno_hash_crear(lista_t* lista_doctores){
    hash_t* hash_turnos = hash_crear(destruir_dato_turno);
    if (!hash_turnos) return NULL;
    lista_iter_t* iter = lista_iter_crear(lista_doctores);
    while (!lista_iter_al_final(iter)){
        doctor_t* doctor = lista_iter_ver_actual(iter);
        bool guardado = guardar_turno_en_hash(hash_turnos, doctor);
        if (!guardado) return NULL;
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    // no destruyo la lista de doctores porque se va a usar para el abb también
    return hash_turnos;
}

bool guardar_doctor_en_abb(abb_t* abb_doctores, doctor_t* doctor){
    char* nombre_doctor = doctor_ver_nombre(doctor);
    bool guardado = abb_guardar(abb_doctores, nombre_doctor, doctor);
    if (!guardado) return false;
    return true;
}

abb_t* doctor_abb_crear(lista_t* lista_doctores){
    abb_t* abb_doctores = abb_crear(strcmp, destruir_dato_doc);
    if (!abb_doctores) return NULL;
    lista_iter_t* iter = lista_iter_crear(lista_doctores);
    while (!lista_iter_al_final(iter)){
        doctor_t* doctor = lista_iter_ver_actual(iter);
        bool guardado = guardar_doctor_en_abb(abb_doctores, doctor);
        if (!guardado) return NULL;
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    // no destruyo la lista de doctores porque se va a usar para los turnos también
    return abb_doctores;
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