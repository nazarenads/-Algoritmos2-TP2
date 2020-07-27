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





