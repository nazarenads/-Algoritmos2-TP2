#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct doctor;
typedef struct doctor doctor_t;

/* ******************************************************************
 *                    PRIMITIVAS DE DOCTOR
 * *****************************************************************/

// Crea un doctor
doctor_t* doctor_crear(char* nombre, char* especialidad);

// Devuelve la especialidad del doctor
char* doctor_ver_especialidad(doctor_t* doctor);

// Devuelve la cantidad de pacientes que atendio el doctor
size_t doctor_cant_atendidos(doctor_t* doctor);

//Destruye al doctor
void doctor_destruir(doctor_t* doctor);