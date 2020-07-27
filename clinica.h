#include "paciente.h"
#include "doctor.h"
#include "turno.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct clinica;
typedef struct clinica clinica_t;

/* ******************************************************************
 *                    PRIMITIVAS DE CLINICA
 * *****************************************************************/

// Crea la clinica inicializando todos sus miembros
clinica_t* clinica_crear(char* csv_pacientes, char* csv_doctores);

// Destruye la clinica con todos sus miembros
void clinica_destruir(clinica_t* clinica);

// Devuelve el hash de pacientes de la clinica
hash_t* clinica_ver_pacientes(clinica_t* clinica);

// Devuelve el hash de turnos de la clinica
hash_t* clinica_ver_turnos(clinica_t* clinica);

// Devuelve el abb de doctores de la clinica
abb_t* clinica_ver_doctores(clinica_t* clinica);