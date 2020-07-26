#include "doctor.h"
#include "lista.h"


/* Definicion de la estructura */

struct doctor {
    char* nombre;
    char* especialidad;
    size_t cant_atendidos;
};

/* Primitivas */

doctor_t* doctor_crear(char* nombre, char* especialidad){
    doctor_t* doctor = malloc(sizeof(doctor_t));
    if(!doctor) return NULL;
    doctor->nombre = nombre;
    doctor->especialidad = especialidad;
    doctor->cant_atendidos = 0;
    return doctor;
}

void* crear_doctor_desde_csv(char** campos, void* extra){
    return doctor_crear(campos[0], campos[1]);
}

char* doctor_ver_especialidad(doctor_t* doctor){
    return doctor->especialidad;
}

char* doctor_ver_nombre(doctor_t* doctor){
    return doctor->nombre;
}

size_t doctor_cant_atendidos(doctor_t* doctor){
    return doctor->cant_atendidos;
}

void doctor_destruir(doctor_t* doctor){
    free(doctor);
}