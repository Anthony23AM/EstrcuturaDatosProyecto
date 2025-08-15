#ifndef LISTASIMPLE_H
#define LISTASIMPLE_H

#include <functional>
#include "NodoSimple.h"

template <typename T>
class ListaSimple {
private:
    NodoSimple<T>* cabeza;
    void mostrarRecursivoNodo(NodoSimple<T>* nodo, const std::function<void(const T&)>& accion) const;
public:
    ListaSimple();
    ~ListaSimple();

    void agregar(const T& dato);
    bool existe(const std::function<bool(const T&)>& criterio) const;
    T* buscar(const std::function<bool(const T&)>& criterio) const;
    bool eliminar(const std::function<bool(const T&)>& criterio);
    void mostrar(const std::function<void(const T&)>& accion) const;
    void mostrarRecursivo(const std::function<void(const T&)>& accion) const;
};

#endif