#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "heap.h"
#define TAM_INI 10
#define FACTOR_REDIMENSION 2

/* *****************************************************************
 *                        ESTRUCTURAS
 * *****************************************************************/

struct heap{
    void** datos;
    size_t cant;
    size_t tam;
    cmp_func_t cmp;
};

/* *****************************************************************
 *                    FUNCIONES AUXILIARES
 * *****************************************************************/
void swap (void** datos, size_t pos_x, size_t pos_y) {
	void* aux_x = datos[pos_x];
	datos[pos_x] = datos[pos_y];
	datos[pos_y] = aux_x;
}

size_t calcular_pos_padre(size_t hijo){
    return (hijo - 1) / 2;
}

size_t calcular_pos_hijo(size_t padre, size_t n, size_t cant){
    //si la pos hijo no existe en el array devuelve al padre
    if (2 * padre + n > cant){
        return padre;
    }
    return 2 * padre + n;
}

void upheap(void** datos, size_t hijo, cmp_func_t cmp){
    if(hijo == 0) return;
    size_t padre = calcular_pos_padre(hijo);
    if(cmp(datos[padre], datos[hijo]) < 0){
        swap(datos, padre, hijo);
        upheap(datos, padre, cmp);
    }
}

size_t calcular_max(void** datos, cmp_func_t cmp, size_t padre, size_t hijo_izq, size_t hijo_der, size_t cant){
    size_t max = padre;
    if( (hijo_izq < cant) && (cmp(datos[hijo_izq], datos[max]) > 0) )
        max = hijo_izq;
    if( (hijo_der < cant) && (cmp(datos[hijo_der], datos[max]) > 0) )
        max = hijo_der;
    return max;
}

void downheap(void** datos, size_t tam, size_t padre, cmp_func_t cmp){
    if(padre >= tam) return;
    size_t hijo_izq = calcular_pos_hijo(padre, 1, tam);
    size_t hijo_der = calcular_pos_hijo(padre, 2, tam);
    size_t max = calcular_max(datos, cmp, padre, hijo_izq, hijo_der, tam);
    if(max != padre){
        swap(datos, padre, max);
        downheap(datos, tam, max, cmp);
    }
}

bool heap_redimensionar(heap_t* heap, size_t nuevo_tam){
    void* datos_nuevo = realloc(heap->datos, nuevo_tam * sizeof(void*));
    if (nuevo_tam > 0 && !datos_nuevo){
        return false;
    }
	heap->tam = nuevo_tam;
    heap->datos	= datos_nuevo;
    return true;
}

void heapify(void** datos, size_t n, cmp_func_t cmp){
	for(size_t i = n/2; i >= 0 && i < n; i--){
        downheap(datos, n, i, cmp);
	}
}

bool heap_esta_lleno(heap_t* heap){
    return heap->cant == heap->tam;
}

bool heap_hay_que_achicar(heap_t* heap){
    return heap->cant * 4 <= heap->tam;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;
    heap->datos = calloc(1, sizeof(void*) * TAM_INI);
    if(!heap->datos){
        free(heap);
        return NULL;
    }
    heap->cant = 0;
    heap->tam = TAM_INI;
    heap->cmp = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if(!heap) return NULL;

    void** copia_arreglo = malloc(sizeof(void*) * n);
	if (!copia_arreglo) return NULL;
	for(size_t i = 0; i<n; i++){
		copia_arreglo[i] = arreglo[i];
	}
    heap->datos = copia_arreglo;
    heap->cmp = cmp;
    heap->tam = n;
    heap->cant = n;
    heapify(heap->datos, heap->cant, cmp);
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if(destruir_elemento){
        for(int i = 0; i < heap->tam; i++){
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
    if (!elem) return false;
    if (heap_esta_lleno(heap)) {
        bool redimension = heap_redimensionar(heap, heap->tam*FACTOR_REDIMENSION);
        if (!redimension) return false;
    }
    heap->datos[heap->cant] = elem;
    heap->cant++;
    upheap(heap->datos, heap->cant-1, heap->cmp);
    return true;
}

void* heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)){
        return NULL;
    }
    return heap->datos[0];
}

void *heap_desencolar(heap_t *heap){
    if(heap_esta_vacio(heap)) return NULL;

    if(heap_hay_que_achicar(heap)){
        heap_redimensionar(heap, heap->tam / FACTOR_REDIMENSION);
    }
    void* primero = heap->datos[0];
    swap(heap->datos, 0, heap->cant - 1);
    heap->cant--;
    downheap(heap->datos, heap->cant, 0, heap->cmp);
    return primero;
}

/* *****************************************************************
 *                    HEAP SORT
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heap_t* heap = heap_crear_arr(elementos, cant, cmp);
    size_t pos_ult = cant - 1;
    for(size_t i = 0; i < cant; i++){
        swap(heap->datos, 0, pos_ult);
        downheap(heap->datos, pos_ult, 0, cmp);
        pos_ult--;
    }
    for(size_t i = 0; i < cant; i++){
        elementos[i] = heap->datos[i];
    }
    heap_destruir(heap, NULL);
}