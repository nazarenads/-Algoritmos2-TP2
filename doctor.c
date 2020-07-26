#include "doctor.h"
#include "abb.h"
#include "lista.h"
#include <string.h>


/* Definicion de la estructura */

struct doctor {
    char* nombre;
    char* especialidad;
    size_t cant_atendidos;
};

/* Funciones auxiliares */

void destruir_dato_doc(void* dato){
    doctor_destruir(dato);
}

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