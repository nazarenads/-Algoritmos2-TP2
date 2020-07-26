#include "csv.h"
#include "clinica.h"
#include <string.h>

// Definicion de la estructura

struct clinica{
    hash_t* hash_pacientes;
    hash_t* hash_turnos;
    abb_t* abb_doctores;
};


// Funciones auxiliares

void destruir_dato_turno(void* dato){
    turno_destruir(dato);
}

void destruir_dato_doc(void* dato){
    doctor_destruir(dato);
}

void destruir_dato_paciente(void* dato){
    paciente_destruir(dato);
}

bool guardar_turno_en_hash(hash_t* hash, doctor_t* doctor){
    char* especialidad = doctor_ver_especialidad(doctor);
    turno_t* turno = turno_crear();
    return hash_guardar(hash, especialidad, turno);
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
    return abb_guardar(abb_doctores, nombre_doctor, doctor);
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
    return hash_guardar(hash, nombre, &antiguedad);
}


hash_t* paciente_hash_crear(lista_t* lista_pacientes){
    hash_t* hash_pacientes = hash_crear(destruir_dato_paciente);
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

// Primitivas

clinica_t* clinica_crear(char* csv_pacientes, char* csv_doctores){
    clinica_t* clinica = malloc(sizeof(clinica_t));
    if (!clinica) return NULL;
    lista_t* lista_pacientes = csv_crear_estructura(csv_pacientes, crear_paciente_desde_csv, NULL);
    if (!lista_pacientes) {
        free(clinica);
        return NULL;
    }
    lista_t* lista_doctores =  csv_crear_estructura(csv_doctores, crear_doctor_desde_csv, NULL);
    if (!lista_doctores) {
        free(clinica);
        lista_destruir(lista_pacientes, destruir_dato_paciente);
        return NULL;
    }
    clinica->hash_pacientes = paciente_hash_crear(lista_pacientes);
    if (!clinica->hash_pacientes) {
        free(clinica);
        lista_destruir(lista_pacientes, destruir_dato_paciente);
        lista_destruir(lista_doctores, destruir_dato_doc);
        return NULL;
    }
    clinica->abb_doctores = doctor_abb_crear(lista_doctores);
    if (!clinica->hash_pacientes) {
        free(clinica);
        lista_destruir(lista_pacientes, destruir_dato_paciente);
        lista_destruir(lista_doctores, destruir_dato_doc);
        hash_destruir(clinica->hash_pacientes);
        return NULL;
    }
    clinica->hash_turnos = turno_hash_crear(lista_doctores);
    if (!clinica->hash_pacientes) {
        free(clinica);
        lista_destruir(lista_pacientes, destruir_dato_paciente);
        lista_destruir(lista_doctores, destruir_dato_doc);
        hash_destruir(clinica->hash_pacientes);
        abb_destruir(clinica->abb_doctores);
        return NULL;
    }
    lista_destruir(lista_pacientes, destruir_dato_paciente);
    lista_destruir(lista_doctores, destruir_dato_doc);
    return clinica;
}

hash_t* clinica_ver_pacientes(clinica_t* clinica){
    return clinica->hash_pacientes;
}

hash_t* clinica_ver_turnos(clinica_t* clinica){
    return clinica->hash_turnos;
}

abb_t* clinica_ver_doctores(clinica_t* clinica){
    return clinica->abb_doctores;
}

void clinica_destruir(clinica_t* clinica){
    hash_destruir(clinica->hash_pacientes);
    hash_destruir(clinica->hash_turnos);
    abb_destruir(clinica->abb_doctores);
    free(clinica);
}