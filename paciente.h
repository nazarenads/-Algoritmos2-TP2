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
paciente_t* paciente_crear(char* nombre, int antiguedad);

// Devuelve el nombre del paciente
// Pre: el paciente fue creado
// Post: se devolvio el nombre del paciente
char* paciente_nombre(paciente_t* paciente);

// Devuelve la antiguedad del paciente
// Pre: el paciente fue creado
// Post: se devovlio la antiguedad del paciente
size_t paciente_antiguedad(paciente_t* paciente);

// Destruye el paciente
// Post: se destruyo al paciente
void paciente_destruir(paciente_t* paciente);

// Recibe el vector de campos del csv y crea un paciente_t usando esos campos
void* crear_paciente_desde_csv(char** campos, void* extra);