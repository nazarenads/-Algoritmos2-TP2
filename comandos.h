#include "turno.h"
#include "paciente.h"
#include "doctor.h"
#include "clinica.h"
#include "mensajes.h"

// Se reciben los parametros del comando PEDIR_TURNO. Se procesa la entrada y se añade
// al paciente a la lista de espera correspondiente
// Pre: se ejecuto el comando PEDIR_TURNO
// Post: se agrego al paciente a la lista de espera correspondiente y se imprime
// el mensaje correspondiente. En caso de fallar se imprime el mensaje de error adecuado.
void pedir_turno(char** parametros, clinica_t* clinica);

// Se reciben los parametros del comando ATENDER_SIGUIENTE. Se procesa la entrada y se
// atiende al proximo paciente dentro de la lista de espera
// Pre: se ejecuto el comando ATENDER_SIGUIENTE
// Post: se atendio al paciente que seguia. Se imprime el mensaje adecuado al atender.
// En caso de fallar, se lanza el mensaje de error correspondiete.
void atender_siguiente(char** parametros, clinica_t* clinica);

// Se reciben los parametros del comando INFORME. Se procesa la entrada y se imprime
// un informe de los doctores dentro del rango especificado.
// Pre: se ejecuto el comando INFORME
// Post: se imprime el informe correspondiente de los doctores que estan dentro
// del rango pasado por parametro. Se lanza un mensaje de error en caso de algun fallo.
void imprimir_informe_doctores(char** parametros, clinica_t* clinica);