#include "lista.h"
#include <stdlib.h>
#include <stddef.h>

/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

 typedef struct nodo {
	void* dato;
	struct nodo* prox;
} nodo_t;


nodo_t* crear_nodo_lista(void* valor) { 
	nodo_t* nodo = malloc(sizeof(nodo_t)); 
	if (!nodo) { 
		return NULL; 
	}
	nodo->dato = valor; 
	nodo->prox = NULL; 
	return nodo; 
}

/* Definición del struct lista */
struct lista {
	nodo_t* prim;
	nodo_t* ult;
	size_t largo;
};

/* Definición del struct lista_iter */
struct lista_iter {
	lista_t* lista;
	nodo_t* actual;
	nodo_t* anterior;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t* lista_crear(void){
	lista_t* lista = malloc(sizeof(lista_t));
	if (!lista) {
		free(lista);
		return NULL;
    }
	lista->prim = NULL;
	lista->ult = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t* lista){
	return !lista->prim && !lista->ult;
}

bool lista_insertar_primero(lista_t* lista, void* dato){
	nodo_t* nodo = crear_nodo_lista(dato);
	if(!nodo){
		return false;
	}
	if (lista_esta_vacia(lista)){
		lista->ult = nodo;
	}
	nodo_t* viejo_primero = lista->prim;
	nodo->prox = viejo_primero;
	lista->prim = nodo;
	lista->largo++;
	return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato){
	nodo_t* nodo = crear_nodo_lista(dato);
	if (!nodo)
		return false;
	if (lista_esta_vacia(lista)){
		lista->prim = nodo;
	}
	if (lista->ult){
		lista->ult->prox = nodo;
	}
	lista->ult = nodo;
	lista->largo++;
	return true;
}

void* lista_borrar_primero(lista_t* lista){
	if (lista_esta_vacia(lista)){
		return NULL;
	}
	nodo_t* primero = lista->prim;
	void* dato = primero->dato;
	//caso borde, si la lista tenia un solo elemento, tanto prim como ult deben quedar en NULL
	if (!primero->prox) {
		lista->ult = NULL;
	}
	nodo_t * nuevo_primero = primero->prox;
	lista->prim = nuevo_primero;
	lista->largo--;
	free(primero);
	return dato;
}

void* lista_ver_primero(const lista_t* lista){
	if(!lista->prim){
		return NULL;
	}
	return lista->prim->dato;
}

void* lista_ver_ultimo(const lista_t* lista){
	if(!lista->ult){
		return NULL;
	}
	return lista->ult->dato;
}

size_t lista_largo(const lista_t* lista){
	return lista->largo;
}

void lista_destruir(lista_t* lista, void (*destruir_dato)(void *)){
	while (!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if (destruir_dato) {
			destruir_dato(dato);
		}
	}
	free(lista);
}

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t* lista_iter_crear(lista_t* lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (!iter) {
		return NULL;
    }
	iter->lista = lista;
	iter->actual = lista->prim;
	iter->anterior = NULL;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t* iter){
	if (lista_iter_al_final(iter)) {
		return false;
	}
	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	return true;
}
void* lista_iter_ver_actual(const lista_iter_t* iter){
	if(lista_iter_al_final(iter)) 
		return NULL;
	return iter->actual->dato;
}
bool lista_iter_al_final(const lista_iter_t* iter){
	return !iter->actual; 
}
void lista_iter_destruir(lista_iter_t* iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t* iter, void* dato){
	nodo_t* nuevo_nodo = crear_nodo_lista(dato);
	if (!nuevo_nodo){
		return false;
	}
	// caso borde: estoy al final de la lista
	if (lista_iter_al_final(iter)){
		iter->lista->ult = nuevo_nodo;
	}
	// caso borde: estoy al principio de la lista 
	if (!iter->anterior) {
		iter->lista->prim = nuevo_nodo;
	} else {
		iter->anterior->prox = nuevo_nodo;
	}
	nuevo_nodo->prox = iter->actual;
	iter->actual = nuevo_nodo;
	iter->lista->largo++;
	return true;
}

void* lista_iter_borrar(lista_iter_t* iter){
	// casos borde: la lista esta vacia o ya estoy fuera de la lista
	if (lista_iter_al_final(iter)) {
		return NULL;
	}
	if(iter->anterior) {
		iter->anterior->prox = iter->actual->prox;
	}
	//caso borde: tener que borrar al final
	if(iter->actual == iter->lista->ult) {
		iter->lista->ult = iter->anterior;
	}
	// caso borde: tener que borrar al principio
	if(iter->actual == iter->lista->prim)  {
		iter->lista->prim = iter->actual->prox;
	}
	void* dato = iter->actual->dato;
	nodo_t* viejo_actual = iter->actual;
	iter->actual = iter->actual->prox;
	free(viejo_actual);
	iter->lista->largo--;
	return dato;
} 

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra){
	nodo_t* actual = lista->prim;
	bool continuar = true;
	while (actual && continuar) {
		continuar = visitar(actual->dato, extra);
		actual = actual->prox;
	}
}