#include "Listasdc.h"
#include "Evento.h"
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

// --- Implementación de quicksort puro para vector<T> ---
template <typename T>
void quicksort(std::vector<T>& vec, int left, int right, const std::function<bool(const T&, const T&)>& comp) {
    if (left >= right) return;
    int i = left, j = right;
    T pivot = vec[left + (right - left) / 2];
    while (i <= j) {
        while (comp(vec[i], pivot)) i++;
        while (comp(pivot, vec[j])) j--;
        if (i <= j) {
            std::swap(vec[i], vec[j]);
            i++;
            j--;
        }
    }
    if (left < j) quicksort(vec, left, j, comp);
    if (i < right) quicksort(vec, i, right, comp);
}

// --- Cifrado César para strings y números ---
std::string cifradoCesar(const std::string& texto, int desplazamiento) {
    std::string resultado = texto;
    for (char& c : resultado) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base + desplazamiento) % 26 + base;
        } else if (isdigit(c)) {
            c = (c - '0' + desplazamiento) % 10 + '0';
        }
        // Otros caracteres quedan igual
    }
    return resultado;
}

unsigned int hashDJB2(const std::string& texto) {
    unsigned int hash = 5381;
    for (char c : texto) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
    }
    return hash;
}

// Constructor
template <typename T>
ListaDC<T>::ListaDC() : cabeza(nullptr) {}

// Destructor
template <typename T>
ListaDC<T>::~ListaDC() {
    if (!cabeza) return;
    NodoDC<T>* actual = cabeza->siguiente;
    while (actual != cabeza) {
        NodoDC<T>* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    delete cabeza;
    cabeza = nullptr;
}

// Agregar al final
template <typename T>
void ListaDC<T>::agregar(const T& dato) {
    NodoDC<T>* nuevo = new NodoDC<T>(dato);
    if (!cabeza) {
        cabeza = nuevo;
        cabeza->siguiente = cabeza;
        cabeza->anterior = cabeza;
    } else {
        NodoDC<T>* ultimo = cabeza->anterior;
        ultimo->siguiente = nuevo;
        nuevo->anterior = ultimo;
        nuevo->siguiente = cabeza;
        cabeza->anterior = nuevo;
    }
}

// Buscar por criterio
template <typename T>
T* ListaDC<T>::buscar(const std::function<bool(const T&)>& criterio) const {
    if (!cabeza) return nullptr;
    NodoDC<T>* actual = cabeza;
    do {
        if (criterio(actual->dato)) return &(actual->dato);
        actual = actual->siguiente;
    } while (actual != cabeza);
    return nullptr;
}

// Existe por criterio
template <typename T>
bool ListaDC<T>::existe(const std::function<bool(const T&)>& criterio) const {
    return buscar(criterio) != nullptr;
}

// Eliminar por criterio
template <typename T>
bool ListaDC<T>::eliminar(const std::function<bool(const T&)>& criterio) {
    if (!cabeza) return false;
    NodoDC<T>* actual = cabeza;
    do {
        if (criterio(actual->dato)) {
            if (actual == cabeza && cabeza->siguiente == cabeza) {
                delete cabeza;
                cabeza = nullptr;
                return true;
            }
            if (actual == cabeza) cabeza = cabeza->siguiente;
            actual->anterior->siguiente = actual->siguiente;
            actual->siguiente->anterior = actual->anterior;
            delete actual;
            return true;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);
    return false;
}

// Mostrar todos
template <typename T>
void ListaDC<T>::mostrar(const std::function<void(const T&)>& accion) const {
    if (!cabeza) return;
    NodoDC<T>* actual = cabeza;
    do {
        accion(actual->dato);
        actual = actual->siguiente;
    } while (actual != cabeza);
}

// Mostrar ordenado por comparador usando quicksort puro
template <typename T>
void ListaDC<T>::mostrarOrdenado(const std::function<bool(const T&, const T&)>& comparador, const std::function<void(const T&)>& accion) const {
    if (!cabeza) return;
    std::vector<T> elementos;
    NodoDC<T>* actual = cabeza;
    do {
        elementos.push_back(actual->dato);
        actual = actual->siguiente;
    } while (actual != cabeza);
    quicksort(elementos, 0, elementos.size() - 1, comparador); // quicksort puro
    for (const auto& elem : elementos) {
        accion(elem);
    }
}

// --------- Guardar en TXT solo para Evento, usando cifrado César ---------
// --------- Guardar en TXT solo para Evento ---------
template <>
void ListaDC<Evento>::guardarBinario(const std::string& archivo) const {
    std::ofstream out(archivo, std::ios::out); // Cambiado a ios::out para sobrescribir
    if (!out) return;
    if (!cabeza) return;
    NodoDC<Evento>* actual = cabeza;
    do {
        const Evento& turno = actual->dato;
        const Persona& p = turno.getPaciente();
        const Especialista& e = turno.getEspecialista();
        const Fecha& f = turno.getFechaAtencion();
        const Hora& h = turno.getHoraAtencion();

        out << turno.getId() << '|'
            << p.getId() << '|'
            << p.getNombre() << '|'
            << p.getApellido() << '|'
            << p.getCedula() << '|'
            << p.getCorreo() << '|'
            << p.getTelefono() << '|'
            << p.getFechaNacimiento().getDia() << '/'
            << p.getFechaNacimiento().getMes() << '/'
            << p.getFechaNacimiento().getAnio() << '|'
            << e.getId() << '|'
            << e.getNombre() << '|'
            << e.getApellido() << '|'
            << e.getEspecialidad() << '|'
            << e.getCedula() << '|'
            << f.getDia() << '/'
            << f.getMes() << '/'
            << f.getAnio() << '|'
            << h.getHora() << ':'
            << h.getMinuto()
            << '\n';

        actual = actual->siguiente;
    } while (actual != cabeza);
    out.close();
}

// Instanciación explícita para Evento
template class ListaDC<Evento>;