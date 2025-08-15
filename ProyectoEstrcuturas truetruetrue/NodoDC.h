#ifndef NODODC_H
#define NODODC_H

template <typename T>
class NodoDC {
public:
    T dato;
    NodoDC* siguiente;
    NodoDC* anterior;
    NodoDC(const T& d) : dato(d), siguiente(nullptr), anterior(nullptr) {}
};

#endif