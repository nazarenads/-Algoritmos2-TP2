#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#include "lista.h"
#include "heap.h"
#include "cola.h"
#include "paciente.h"


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct turno;
typedef struct turno turno_t;

/* ******************************************************************
 *                    PRIMITIVAS DE TURNO
 * *****************************************************************/

// Crea un turno
turno_t* turno_crear();

// Devuelve la cantidad de pacientes que hay en espera para un turno
size_t ver_cant_espera(turno_t* turno);

// Agrega un paciente a la lista de espera de turnos urgentes
// Pre: el turno fue creado.
// Post: devolvio true si se pudo encolar al paciente. False en caso de error.
bool encolar_turno_urg(turno_t* turno, paciente_t* paciente);

// Agrega un paciente a la lista de espera de turnos regulares
// Pre: el turno fue creado.
// Post: devolvio true si se pudo encolar al paciente. False en caso de error.
bool encolar_turno_reg(turno_t* turno, paciente_t* paciente);

// Saca un paciente de la lista de espera de turnos regulares
// Pre: el turno fue creado.
// Post: el paciente ya no se encuentra dentro del turno.
paciente_t* desencolar_turno_reg(turno_t* turno);

// Saca un paciente de la lista de espera de turnos de urgencia
// Pre: el turno fue creado.
// Post: el paciente ya no se encuentra dentro del turno.
paciente_t* desencolar_turno_urg(turno_t* turno);

// Devuelve la cola de turnos urgentes 
cola_t* ver_cola_turnos_urgentes(turno_t* turno);

// Devuelve el heap de turnos regulares
heap_t* ver_heap_turnos_regulares(turno_t* turno);

// Destruye el turno
void turno_destruir(turno_t* turno);