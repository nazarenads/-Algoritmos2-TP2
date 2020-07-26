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