#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <vector>
#include <iostream>

class NodoBPlus {
public:
    std::vector<int> claves;
    std::vector<NodoBPlus*> hijos;
    bool hoja;
    NodoBPlus* siguiente;
    NodoBPlus(bool hoja);
    ~NodoBPlus();
};

class BPlusTree {
    NodoBPlus* raiz;
public:
    BPlusTree();
    ~BPlusTree();
    void insertar(int clave);
    void imprimir();
private:
    NodoBPlus* insertarRec(NodoBPlus* nodo, int clave, int& promovido, NodoBPlus*& nuevoHijo);
    void imprimirNiveles();
    void liberar(NodoBPlus* nodo);
};

#endif