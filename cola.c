#include <stdlib.h>
#include "cola.h"

//Definicion de las estructuras

typedef struct nodo{
    void** dato;
    struct nodo* siguiente;
}nodo_t;

struct cola{
    nodo_t* primero;
    nodo_t* ultimo;
};

//Funciones auxiliares

nodo_t* crear_nodo(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if(nodo == NULL) return NULL;
    nodo->dato = valor;
    nodo->siguiente = NULL;
    return nodo;
}

//Definicion de las primitivas

cola_t* cola_crear(void){
    cola_t* cola = malloc(sizeof(cola_t));
    if(cola == NULL) return NULL;
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

bool cola_esta_vacia(const cola_t *cola){
    return cola->ultimo == NULL;
}

void* cola_ver_primero(const cola_t *cola){
    if(cola_esta_vacia(cola)){
        return NULL;
    }
    return cola->primero->dato;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)){
    while(cola->ultimo){
        void* dato_primero = cola_desencolar(cola);
        if(destruir_dato != NULL)
            destruir_dato(dato_primero);
    }
    free(cola);
}

bool cola_encolar(cola_t *cola, void* valor){
    nodo_t* nodo = crear_nodo(valor);
    if(cola_esta_vacia(cola)){
        cola->primero = nodo;
        cola->ultimo = nodo;
        return true;
    }
    cola->ultimo->siguiente = nodo;
    cola->ultimo = nodo;
    return true;
}

void* cola_desencolar(cola_t *cola){
    if(cola_esta_vacia(cola))
        return NULL;
    nodo_t* primero_anterior = cola->primero;
    void* dato_primero_anterior = primero_anterior->dato;
    cola->primero = primero_anterior->siguiente;
    if(primero_anterior->siguiente == NULL)
        cola->ultimo = NULL;
    free(primero_anterior);
    return dato_primero_anterior;
}
