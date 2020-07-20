#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "abb.h"
#include "pila.h"
#define CANT_INI 0

/* Definición del struct auxiliar nodo */
typedef struct nodo_abb {
    struct nodo_abb* izq;
    struct nodo_abb* der;
    char* clave;
    void* dato;
} nodo_abb_t;

/* Definición del struct abb */
struct abb {
    nodo_abb_t* raiz;
    size_t cant;
    abb_destruir_dato_t destruir_dato;
    abb_comparar_clave_t comparar_clave;
};

/* Definición del struct iterador externo */
struct abb_iter{
    pila_t* pila;
};

/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/

nodo_abb_t* nodo_crear(char* clave, void* dato){
    nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
    if(!nodo) return NULL;
    nodo->clave = clave;
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

nodo_abb_t* _buscar_nodo(nodo_abb_t* nodo, abb_comparar_clave_t cmp, const char* clave){
    if(!nodo) return NULL;
    if(cmp(clave, nodo->clave) == 0){
        return nodo;
    }
    if(cmp(clave, nodo->clave) < 0 && nodo->izq){
        return _buscar_nodo(nodo->izq, cmp, clave);
    }
    if(cmp(clave, nodo->clave) > 0 && nodo->der){
        return _buscar_nodo(nodo->der, cmp, clave);
    }
    return NULL;
}

nodo_abb_t* buscar_nodo(const abb_t* abb, const char* clave){
    return _buscar_nodo(abb->raiz, abb->comparar_clave, clave);
}

void destruir_nodo(nodo_abb_t* nodo, abb_destruir_dato_t destruir_dato){
    if(!nodo) return;
    if(nodo->izq){
        destruir_nodo(nodo->izq, destruir_dato);
    }
    if(nodo->der){
        destruir_nodo(nodo->der, destruir_dato);
    }
    if(destruir_dato){
        destruir_dato(nodo->dato);
    }
    free(nodo->clave);
    free(nodo);
}

nodo_abb_t* buscar_padre(abb_comparar_clave_t cmp, nodo_abb_t* nodo_actual, char* clave_hijo){
	if (!nodo_actual) return NULL;
    nodo_abb_t* padre = NULL;
    while(nodo_actual != NULL){
        if(cmp(clave_hijo, nodo_actual->clave) < 0){
            padre = nodo_actual;
            nodo_actual = nodo_actual->izq;
        }else if (cmp(clave_hijo, nodo_actual->clave) > 0){
            padre = nodo_actual;
            nodo_actual = nodo_actual->der;
        }else{
            break;
        }
    }
    return padre;
}

void insertar(nodo_abb_t* nodo_a_insertar, nodo_abb_t* raiz, abb_comparar_clave_t cmp) { 
    nodo_abb_t* padre = buscar_padre(cmp, raiz, nodo_a_insertar->clave);
    if (cmp(padre->clave, nodo_a_insertar->clave) < 0){
        padre->der = nodo_a_insertar;
    }else{
        padre->izq = nodo_a_insertar;
    }
}

nodo_abb_t* buscar_reemplazante(nodo_abb_t* nodo_actual){
    if(!nodo_actual->izq) return nodo_actual;
    nodo_abb_t* reemplazo = buscar_reemplazante(nodo_actual->izq);
    return reemplazo;
}

void apilar_hijos_izquierdos(pila_t* pila, nodo_abb_t* nodo){
    if(nodo == NULL) return;
    pila_apilar(pila, nodo);
    apilar_hijos_izquierdos(pila, nodo->izq);
}

void iterar_iter_interno(nodo_abb_t* nodo, bool visitar(const char *, void *, void *), void* extra, bool* seguir){
    if(!nodo || !*seguir) return;
    iterar_iter_interno(nodo->izq, visitar, extra, seguir);
    if(*seguir){
        if(!visitar(nodo->clave, nodo->dato, extra)){
            *seguir = false;
            return;
        }
    }
    iterar_iter_interno(nodo->der, visitar, extra, seguir);
}

bool es_hoja(nodo_abb_t* nodo){
    return !nodo->izq && !nodo->der;
}

bool tiene_un_hijo(nodo_abb_t* nodo){
    return (!nodo->izq && nodo->der) || (!nodo->der && nodo->izq);
}

bool tiene_dos_hijos(nodo_abb_t* nodo){
    return (nodo->der != NULL) && (nodo->izq != NULL);
}

bool es_raiz(char* clave_nodo, char* clave_raiz, abb_comparar_clave_t cmp){
    return cmp(clave_nodo, clave_raiz) == 0;
}

void borrar_hoja(nodo_abb_t* nodo_a_borrar, nodo_abb_t* padre, abb_t* arbol){
    if(es_raiz(nodo_a_borrar->clave, arbol->raiz->clave, arbol->comparar_clave)){
        arbol->raiz = NULL;
    }else if(padre && padre->der && arbol->comparar_clave(nodo_a_borrar->clave, padre->der->clave) == 0){
        padre->der = NULL;
    }else if(padre && padre->izq && arbol->comparar_clave(nodo_a_borrar->clave, padre->izq->clave) == 0){
        padre->izq = NULL;
    }
}

void borrar_nodo_con_un_hijo(nodo_abb_t* nodo_a_borrar, nodo_abb_t* padre, abb_t* arbol){
    if(es_raiz(nodo_a_borrar->clave, arbol->raiz->clave, arbol->comparar_clave)){
        if(arbol->raiz->izq != NULL){
            arbol->raiz = nodo_a_borrar->izq;
        }else{
            arbol->raiz = nodo_a_borrar->der;
        }
    }else if(padre->der && arbol->comparar_clave(nodo_a_borrar->clave, padre->der->clave) == 0 && nodo_a_borrar->izq){
        padre->der = nodo_a_borrar->izq;
    }else if(padre->der && arbol->comparar_clave(nodo_a_borrar->clave, padre->der->clave) == 0 && nodo_a_borrar->der){
        padre->der = nodo_a_borrar->der;
    }else if(padre->izq && arbol->comparar_clave(nodo_a_borrar->clave, padre->izq->clave) == 0 && nodo_a_borrar->izq){
        padre->izq = nodo_a_borrar->izq;
    }else if(padre->izq && arbol->comparar_clave(nodo_a_borrar->clave, padre->izq->clave) == 0 && nodo_a_borrar->der){
        padre->izq = nodo_a_borrar->der;
    }
}

void borrar_nodo_con_dos_hijos(nodo_abb_t* nodo_a_borrar, abb_t* arbol){
    nodo_abb_t* reemplazo = buscar_reemplazante(nodo_a_borrar->der);
    char* clave_reemplazo = strdup(reemplazo->clave);
    void* valor_reemplazo = reemplazo->dato;
    nodo_abb_t* padre_reemplazo = buscar_padre(arbol->comparar_clave, arbol->raiz, clave_reemplazo);
    if(es_hoja(reemplazo)){
        borrar_hoja(reemplazo, padre_reemplazo, arbol);
    } else if(tiene_un_hijo(reemplazo)){
        borrar_nodo_con_un_hijo(reemplazo, padre_reemplazo, arbol);
    }
    if(es_raiz(nodo_a_borrar->clave, arbol->raiz->clave, arbol->comparar_clave)){
        free(arbol->raiz->clave);
        arbol->raiz->clave = clave_reemplazo;
        arbol->raiz->dato = valor_reemplazo;
    }else{
        free(nodo_a_borrar->clave);
        nodo_a_borrar->dato = valor_reemplazo;
        nodo_a_borrar->clave = clave_reemplazo;
    }
    free(reemplazo->clave);
    free(reemplazo);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;
    abb->raiz = NULL;
    abb->cant = CANT_INI;
    abb->destruir_dato = destruir_dato;
    abb->comparar_clave = cmp;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol->raiz){
        char* copia_clave = strdup(clave);
        if(!copia_clave) return false;
        arbol->raiz = nodo_crear(copia_clave, dato);
        if(!arbol->raiz){
            free(copia_clave);
            return false;
        }
        arbol->cant++;
        return true;
    }
    nodo_abb_t* nodo = buscar_nodo(arbol, clave);
    if (nodo){
        void* dato_anterior = nodo->dato;
        nodo->dato = dato;
        if (arbol->destruir_dato){
            arbol->destruir_dato(dato_anterior);
        }
        return true;
    }
    char* copia_clave = strdup(clave);
    if(!copia_clave) return false;
    nodo = nodo_crear(copia_clave, dato);
    insertar(nodo, arbol->raiz, arbol->comparar_clave);
    arbol->cant++;
    return true;
}

void* abb_borrar(abb_t* arbol, const char* clave){
	if(!abb_pertenece(arbol, clave)){
        return NULL;
    }
	arbol->cant--;
	nodo_abb_t* nodo = buscar_nodo(arbol, clave);
    if(!nodo) return NULL;
	void* dato_nodo = nodo->dato;
    nodo_abb_t* padre = buscar_padre(arbol->comparar_clave, arbol->raiz, nodo->clave);
	if(es_hoja(nodo)){
        borrar_hoja(nodo, padre, arbol);
	}
    else if(tiene_un_hijo(nodo)){
        borrar_nodo_con_un_hijo(nodo, padre, arbol);
	}
    else if(tiene_dos_hijos(nodo)){
        borrar_nodo_con_dos_hijos(nodo, arbol);
        return dato_nodo;
    }
    free(nodo->clave);
    free(nodo);
    return dato_nodo;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    if (!abb_pertenece(arbol, clave)) return NULL;
    nodo_abb_t* nodo_a_buscar = buscar_nodo(arbol, clave);
    return nodo_a_buscar->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	if(!arbol->raiz){
        return false;
    }
	nodo_abb_t* nodo_a_buscar = buscar_nodo(arbol, clave);
    if(!nodo_a_buscar) return false;
	return arbol->comparar_clave(nodo_a_buscar->clave, clave) == 0;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void abb_destruir(abb_t *arbol){
    destruir_nodo(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}

/* *****************************************************************
 *                 PRIMITIVA DEL ITER INTERNO
 * *****************************************************************/

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    bool seguir = true;
    iterar_iter_interno(arbol->raiz, visitar, extra , &seguir);
}

/* *****************************************************************
 *                 PRIMITIVAS DEL ITER EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    pila_t* pila = pila_crear();
    if(!pila) return NULL;
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter){
        pila_destruir(pila);
        return NULL;
    }
    iter->pila = pila;
    if(arbol->raiz){
        apilar_hijos_izquierdos(iter->pila, arbol->raiz);
    }
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)){
        return false;
    }
    nodo_abb_t* nodo = pila_desapilar(iter->pila);
    if(nodo->der != NULL){
        pila_apilar(iter->pila, nodo->der);
        apilar_hijos_izquierdos(iter->pila, nodo->der->izq);
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)){
        return NULL;
    }
    nodo_abb_t* nodo = pila_ver_tope(iter->pila);
    return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}