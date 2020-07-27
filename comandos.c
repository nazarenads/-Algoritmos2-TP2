#include <string.h>
#include <stdio.h>
#include "comandos.h"
#define TURNO_URG "URGENTE"
#define TURNO_REG "REGULAR"


/* PEDIR TURNO */

bool pedir_turno_urgente(char** parametros, clinica_t* clinica){
    hash_t* hash_pacientes = clinica_ver_pacientes(clinica);
    hash_t* hash_turnos = clinica_ver_turnos(clinica);
    if(!hash_pertenece(hash_pacientes, parametros[0])){
        // lanzar errror no existe le paciente 'NOMBRE_PACIENTE'
        return false;
    }
    if(!hash_pertenece(hash_turnos, parametros[1])){
        // lanzar error no existe la especialidad 'NOMBRE_ESPECIALIDAD'
        return false;
    }
    turno_t* turno_urgente = hash_obtener(hash_turnos, parametros[1]);
    paciente_t* paciente = hash_obtener(hash_pacientes, parametros[0]);
    return encolar_turno_urg(turno_urgente, paciente);
}

bool pedir_turno_regular(char** parametros, clinica_t* clinica){
    hash_t* hash_pacientes = clinica_ver_pacientes(clinica);
    hash_t* hash_turnos = clinica_ver_turnos(clinica);
    if(!hash_pertenece(hash_pacientes, parametros[0])){
        // lanzar errror no existe le paciente 'NOMBRE_PACIENTE'
        return false;
    }
    if(!hash_pertenece(hash_turnos, parametros[1])){
        // lanzar error no existe la especialidad 'NOMBRE_ESPECIALIDAD'
        return false;
    }
    turno_t* turno_regular = hash_obtener(hash_turnos, parametros[1]);
    paciente_t* paciente = hash_obtener(hash_pacientes, parametros[0]);
    return encolar_turno_reg(turno_regular, paciente);
}

void pedir_turno(char** parametros, clinica_t* clinica){
    if(strcmp(parametros[2], TURNO_URG) == 0){
        if( pedir_turno_urgente(parametros, clinica) ){
            // Imprimir salida de de encolado con exito
        }
    }else if(strcmp(parametros[2], TURNO_REG) == 0){
        if( pedir_turno_regular(parametros, clinica) ){
            // Imprimir salida de de encolado con exito
        }
    }else{
        // Lanzar error "grado de urgencia no identificado ('parametros[2]')"
    }
}


/* ATENDER SIGUIENTE */

char* buscar_especialidad_doctor(char* nombre_doctor, abb_t* abb_doctores){
    doctor_t* doctor = abb_obtener(abb_doctores, nombre_doctor);
    return doctor_ver_especialidad(doctor);
}

void atender_siguiente(char* parametros, clinica_t* clinica){
    abb_t* abb_doctores = clinica_ver_doctores(clinica);
    hash_t* hash_turnos = clinica_ver_turnos(clinica);
    if (!abb_pertenece(abb_doctores, parametros[0])){
        // Lanzar error ENOENT_DOCTOR
    }
    char* especialidad = buscar_especialidad_doctor(parametros[0], abb_doctores);
    turno_t* turno = hash_obtener(hash_turnos, especialidad);
    cola_t* cola_urg = ver_cola_turnos_urgentes(turno);
    if (!cola_esta_vacia(cola_urg)){
        paciente_t* paciente_a_atender = desencolar_turno_urg(cola_urg);
        if (!paciente_a_atender) {
            //Lanzar error genérico
        }
        // Imprimir salida de PACIENTE_ATENDIDO 
        // Imprimir CANT_PACIENTES_ENCOLADOS para la especialidad
        return;
    }
    heap_t* heap_reg =  ver_heap_turnos_regulares(turno);
    paciente_t* paciente_a_atender = desencolar_turno_reg(heap_reg);
    if (!paciente_a_atender) {
        // Lanzar error genérico
        // Imprimir salida de PACIENTE_ATENDIDO 
        // Imprimir CANT_PACIENTES_ENCOLADOS para la especialidad
    }
}


