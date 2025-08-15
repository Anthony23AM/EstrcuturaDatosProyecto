#include "ListaSimple.h"
#include "Persona.h"
#include "Especialista.h"

template <typename T>
ListaSimple<T>::ListaSimple() : cabeza(nullptr) {}

template <typename T>
ListaSimple<T>::~ListaSimple() {
    NodoSimple<T>* actual = cabeza;
    while (actual) {
        NodoSimple<T>* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

template <typename T>
void ListaSimple<T>::agregar(const T& dato) {
    NodoSimple<T>* nuevo = new NodoSimple<T>(dato);
    if (!cabeza) {
        cabeza = nuevo;
    } else {
        NodoSimple<T>* actual = cabeza;
        while (actual->siguiente) actual = actual->siguiente;
        actual->siguiente = nuevo;
    }
}

template <typename T>
bool ListaSimple<T>::existe(const std::function<bool(const T&)>& criterio) const {
    NodoSimple<T>* actual = cabeza;
    while (actual) {
        if (criterio(actual->dato)) return true;
        actual = actual->siguiente;
    }
    return false;
}

template <typename T>
T* ListaSimple<T>::buscar(const std::function<bool(const T&)>& criterio) const {
    NodoSimple<T>* actual = cabeza;
    while (actual) {
        if (criterio(actual->dato)) return &(actual->dato);
        actual = actual->siguiente;
    }
    return nullptr;
}

template <typename T>
bool ListaSimple<T>::eliminar(const std::function<bool(const T&)>& criterio) {
    NodoSimple<T>* actual = cabeza;
    NodoSimple<T>* anterior = nullptr;
    while (actual) {
        if (criterio(actual->dato)) {
            if (anterior) anterior->siguiente = actual->siguiente;
            else cabeza = actual->siguiente;
            delete actual;
            return true;
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    return false;
}

template <typename T>
void ListaSimple<T>::mostrar(const std::function<void(const T&)>& accion) const {
    NodoSimple<T>* actual = cabeza;
    while (actual) {
        accion(actual->dato);
        actual = actual->siguiente;
    }
}

template <typename T>
void ListaSimple<T>::mostrarRecursivo(const std::function<void(const T&)>& accion) const {
    mostrarRecursivoNodo(cabeza, accion);
}

template <typename T>
void ListaSimple<T>::mostrarRecursivoNodo(NodoSimple<T>* nodo, const std::function<void(const T&)>& accion) const {
    if (!nodo) return;
    accion(nodo->dato);
    mostrarRecursivoNodo(nodo->siguiente, accion);
}



// Instanciaciones explícitas
template class ListaSimple<Persona>;
template class ListaSimple<Especialista>;