#ifndef LISTASDC_H
#define LISTASDC_H

#include <functional>
#include <string>
#include <vector> // <-- agregado
#include <algorithm> // <-- agregado
#include "NodoDC.h"

template <typename T>
class ListaDC {
private:
    NodoDC<T>* cabeza;
public:
    ListaDC();
    ~ListaDC();

    void agregar(const T& dato);
    bool existe(const std::function<bool(const T&)>& criterio) const;
    T* buscar(const std::function<bool(const T&)>& criterio) const;
    bool eliminar(const std::function<bool(const T&)>& criterio);
    void mostrar(const std::function<void(const T&)>& accion) const;

    // Mostrar ordenado por criterio de comparación
    void mostrarOrdenado(const std::function<bool(const T&, const T&)>& comparador, const std::function<void(const T&)>& accion) const;

    // Método para guardar en binario (solo para Evento)
    void guardarBinario(const std::string& archivo) const;
};

#endif