#ifndef NODOSIMPLE_H
#define NODOSIMPLE_H

template <typename T>
class NodoSimple {
public:
    T dato;
    NodoSimple* siguiente;
    NodoSimple(const T& d) : dato(d), siguiente(nullptr) {}
};

#endif