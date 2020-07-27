#include <string.h>
#include <stdio.h>
#include "comandos.h"
#define TURNO_URG "URGENTE"
#define TURNO_REG "REGULAR"
#define NOMBRE 0
#define ESPECIALIDAD 1
#define URGENCIA 2


/* PEDIR TURNO */

void pedir_turno_urgente(char** parametros, clinica_t* clinica){
    hash_t* hash_pacientes = clinica_ver_pacientes(clinica);
    hash_t* hash_turnos = clinica_ver_turnos(clinica);
    if(!hash_pertenece(hash_pacientes, parametros[NOMBRE])){
        printf(ENOENT_PACIENTE, parametros[NOMBRE]);
        return;
    }
    if(!hash_pertenece(hash_turnos, parametros[ESPECIALIDAD])){
        printf(ENOENT_ESPECIALIDAD, parametros[ESPECIALIDAD]);
        return;
    }
    turno_t* turno_urgente = hash_obtener(hash_turnos, parametros[ESPECIALIDAD]);
    paciente_t* paciente = hash_obtener(hash_pacientes, parametros[NOMBRE]);
    if(encolar_turno_urg(turno_urgente, paciente)){
        printf(PACIENTE_ENCOLADO, parametros[NOMBRE]);
        printf(CANT_PACIENTES_ENCOLADOS, ver_cant_espera_urg(turno_urgente), parametros[ESPECIALIDAD]);
    }
}

void pedir_turno_regular(char** parametros, clinica_t* clinica){
    hash_t* hash_pacientes = clinica_ver_pacientes(clinica);
    hash_t* hash_turnos = clinica_ver_turnos(clinica);
    if(!hash_pertenece(hash_pacientes, parametros[NOMBRE])){
        printf(ENOENT_PACIENTE, parametros[NOMBRE]);
        return;
    }
    if(!hash_pertenece(hash_turnos, parametros[ESPECIALIDAD])){
        printf(ENOENT_ESPECIALIDAD, parametros[ESPECIALIDAD]);
        return;
    }
    turno_t* turno_regular = hash_obtener(hash_turnos, parametros[ESPECIALIDAD]);
    paciente_t* paciente = hash_obtener(hash_pacientes, parametros[NOMBRE]);
    if(encolar_turno_reg(turno_regular, paciente)){
        printf(PACIENTE_ENCOLADO, parametros[NOMBRE]);
        printf(CANT_PACIENTES_ENCOLADOS, ver_cant_espera_reg(turno_regular), parametros[ESPECIALIDAD]);
    }
}

void pedir_turno(char** parametros, clinica_t* clinica){
    if(strcmp(parametros[URGENCIA], TURNO_URG) == 0){
        pedir_turno_urgente(parametros, clinica);
    }else if(strcmp(parametros[URGENCIA], TURNO_REG) == 0){
        pedir_turno_regular(parametros, clinica);
    }else{
        printf(ENOENT_URGENCIA, parametros[URGENCIA]);
    }
}
