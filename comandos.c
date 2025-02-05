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
        printf(CANT_PACIENTES_ENCOLADOS, ver_cant_espera(turno_urgente), parametros[ESPECIALIDAD]);
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
        printf(CANT_PACIENTES_ENCOLADOS, ver_cant_espera(turno_regular), parametros[ESPECIALIDAD]);
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

/* ATENDER SIGUIENTE */


void atender_siguiente(char** parametros, clinica_t* clinica){
    abb_t* abb_doctores = clinica_ver_doctores(clinica);
    hash_t* hash_turnos = clinica_ver_turnos(clinica);
    if (!abb_pertenece(abb_doctores, parametros[NOMBRE])){
        printf(ENOENT_DOCTOR, parametros[NOMBRE]);
        return;
    }
    doctor_t* doctor = abb_obtener(abb_doctores, parametros[NOMBRE]);
    char* especialidad = doctor_ver_especialidad(doctor);
    turno_t* turno = hash_obtener(hash_turnos, especialidad);
    cola_t* cola_urg = ver_cola_turnos_urgentes(turno);
    if (!cola_esta_vacia(cola_urg)){
        paciente_t* paciente_a_atender = desencolar_turno_urg(turno);
        if (!paciente_a_atender) {
            printf(ENOENT_DESENCOLAR);
            return;
        }
        printf(PACIENTE_ATENDIDO, paciente_nombre(paciente_a_atender));
        printf(CANT_PACIENTES_ENCOLADOS, ver_cant_espera(turno), especialidad);
        doctor_aumentar_cant_atendidos(doctor);
        return;
    }
    heap_t* heap_reg =  ver_heap_turnos_regulares(turno);
    if (heap_esta_vacio(heap_reg)) {
        printf(SIN_PACIENTES);
        return;
    }
    paciente_t* paciente_a_atender = desencolar_turno_reg(turno);
    if (!paciente_a_atender) {
        printf(ENOENT_DESENCOLAR);
        return;
    }
    printf(PACIENTE_ATENDIDO, paciente_nombre(paciente_a_atender));
    printf(CANT_PACIENTES_ENCOLADOS, ver_cant_espera(turno), especialidad);
    doctor_aumentar_cant_atendidos(doctor);
}

/* INFORME DOCTORES */

void imprimir_doctor(const char* nombre, doctor_t* doctor, size_t numero){
    printf(INFORME_DOCTOR, numero, nombre, doctor_ver_especialidad(doctor), doctor_cant_atendidos(doctor));
}

bool imprimir(const char* dato, void* valor, void* extra){
    imprimir_doctor(dato, valor, *(size_t*)extra += 1);
    return false;
}

bool contar_docs(const char* dato, void* valor, void* extra){
    *(size_t*)extra += 1;
    return true;
}

void imprimir_informe_doctores(char** parametros, clinica_t* clinica){
    char* inicio = parametros[0];
    char* fin = parametros[1];
    if(!inicio || !fin){
        printf(ENOENT_PARAMS, "INFORME");
        return;
    }
    if(strcmp(inicio, "") == 0) inicio = "A";
    if(strcmp(fin, "") == 0) fin = "Z";
    abb_t* abb_doctores = clinica_ver_doctores(clinica);
    size_t cantidad_doctores = 0;
    abb_in_order_por_rango(abb_doctores, inicio, fin, contar_docs, &cantidad_doctores);
    printf(DOCTORES_SISTEMA, cantidad_doctores);
    size_t numero = 0;
    abb_in_order_por_rango(abb_doctores, inicio, fin, imprimir, &numero);
}
