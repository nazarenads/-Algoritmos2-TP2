#include <stdlib.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct paciente;
typedef struct paciente paciente_t;

/* ******************************************************************
 *                    PRIMITIVAS DE PACIENTE
 * *****************************************************************/

// Crea un paciente
paciente_t* paciente_crear(char* nombre, void* antiguedad);

// Devuelve el nombre del paciente
char* paciente_nombre(paciente_t* paciente);

// Devuelve la antiguedad del paciente
char* paciente_antiguedad(paciente_t* paciente);

// Destruye el paciente
void paciente_destruir(paciente_t* paciente);