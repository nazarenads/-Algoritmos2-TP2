#include <stdlib.h>
#include "abb.h"

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
// Pre: se creo el doctor
// Post: se devolvio la especialidad del doctor
char* doctor_ver_especialidad(doctor_t* doctor);

// Devuelve el nombre del doctor
// Pre: el doctor fue creado
// Post: se devolvio el nombre del doctor
char* doctor_ver_nombre(doctor_t* doctor);

// Devuelve la cantidad de pacientes que atendio el doctor
// Pre: el doctor fue creado
// Post: se devolvio la cantiad de pacientes que atendio el doctor
size_t doctor_cant_atendidos(doctor_t* doctor);

//Destruye al doctor
void doctor_destruir(doctor_t* doctor);

// Recibe el vector de campos del csv y crea un doctor_t usando esos campos
void* crear_doctor_desde_csv(char** campos, void* extra);

// Aumenta la cantidad de pacientes que atendio un doctor
// Pre: el doctor fue creado
// Post: se aumento la cantidad de pacientes atendidos del doctor
void doctor_aumentar_cant_atendidos(doctor_t* doctor);