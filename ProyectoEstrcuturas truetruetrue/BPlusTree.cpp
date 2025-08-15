#include "BPlusTree.h"
#include <queue>
#include <iomanip>
#include <algorithm>
#include <iostream>

const int ORDEN = 3;

NodoBPlus::NodoBPlus(bool hoja) : hoja(hoja), siguiente(nullptr) {}
NodoBPlus::~NodoBPlus() {
    // No liberar hijos aquí, solo recursos propios si los hubiera
}

BPlusTree::BPlusTree() : raiz(nullptr) {}
BPlusTree::~BPlusTree() {
    liberar(raiz);
}

void BPlusTree::liberar(NodoBPlus* nodo) {
    if (!nodo) return;
    for (auto h : nodo->hijos) liberar(h);
    delete nodo;
}

void BPlusTree::insertar(int clave) {
    if (!raiz) {
        raiz = new NodoBPlus(true);
        raiz->claves.push_back(clave);
        return;
    }
    int promovido = -1;
    NodoBPlus* nuevoHijo = nullptr;
    NodoBPlus* nuevaRaiz = insertarRec(raiz, clave, promovido, nuevoHijo);
    if (nuevaRaiz) {
        NodoBPlus* nueva = new NodoBPlus(false);
        nueva->claves.push_back(promovido);
        nueva->hijos.push_back(raiz);
        nueva->hijos.push_back(nuevoHijo);
        raiz = nueva;
    }
}

NodoBPlus* BPlusTree::insertarRec(NodoBPlus* nodo, int clave, int& promovido, NodoBPlus*& nuevoHijo) {
    if (nodo->hoja) {
        auto it = std::upper_bound(nodo->claves.begin(), nodo->claves.end(), clave);
        nodo->claves.insert(it, clave);
        if (nodo->claves.size() < ORDEN) return nullptr;
        int mitad = ORDEN / 2;
        NodoBPlus* nuevo = new NodoBPlus(true);
        nuevo->claves.assign(nodo->claves.begin() + mitad, nodo->claves.end());
        nodo->claves.erase(nodo->claves.begin() + mitad, nodo->claves.end());
        nuevo->siguiente = nodo->siguiente;
        nodo->siguiente = nuevo;
        promovido = nuevo->claves[0];
        nuevoHijo = nuevo;
        return nuevo;
    }
    size_t i = 0;
    while (i < nodo->claves.size() && clave >= nodo->claves[i]) ++i;
    int promovidoHijo = -1;
    NodoBPlus* nuevoHijoHijo = nullptr;
    NodoBPlus* split = insertarRec(nodo->hijos[i], clave, promovidoHijo, nuevoHijoHijo);
    if (!split) return nullptr;
    nodo->claves.insert(nodo->claves.begin() + i, promovidoHijo);
    nodo->hijos.insert(nodo->hijos.begin() + i + 1, nuevoHijoHijo);
    if (nodo->claves.size() < ORDEN) return nullptr;
    int mitad = ORDEN / 2;
    promovido = nodo->claves[mitad];
    NodoBPlus* nuevo = new NodoBPlus(false);
    nuevo->claves.assign(nodo->claves.begin() + mitad + 1, nodo->claves.end());
    nodo->claves.erase(nodo->claves.begin() + mitad, nodo->claves.end());
    nuevo->hijos.assign(nodo->hijos.begin() + mitad + 1, nodo->hijos.end());
    nodo->hijos.erase(nodo->hijos.begin() + mitad + 1, nodo->hijos.end());
    nuevoHijo = nuevo;
    return nuevo;
}

void BPlusTree::imprimir() {
    imprimirNiveles();
}

void BPlusTree::imprimirNiveles() {
    if (!raiz) return;
    std::queue<NodoBPlus*> q;
    q.push(raiz);

    while (!q.empty()) {
        int n = q.size();
        std::vector<NodoBPlus*> nivelActual;
        bool esHoja = false;
        int anchoNivel = 0;

        for (int i = 0; i < n; ++i) {
            NodoBPlus* nodo = q.front(); q.pop();
            nivelActual.push_back(nodo);
            if (nodo->hoja) esHoja = true;
        }

        // Calcular ancho total del nivel
        for (auto nodo : nivelActual) {
            int anchoNodo = 2 + nodo->claves.size() * 3; // [x|y|z]
            anchoNivel += anchoNodo;
            if (&nodo != &nivelActual.back()) anchoNivel += 2; // espacio entre nodos
        }

        int anchoConsola = 80; // Puedes ajustar este valor según tu consola
        int espacios = (anchoConsola - anchoNivel) / 2;
        if (espacios < 0) espacios = 0;

        std::cout << std::string(espacios, ' ');

        if (esHoja) {
            for (size_t i = 0; i < nivelActual.size(); ++i) {
                std::cout << "[";
                for (size_t j = 0; j < nivelActual[i]->claves.size(); ++j) {
                    std::cout << nivelActual[i]->claves[j];
                    if (j + 1 < nivelActual[i]->claves.size()) std::cout << "|";
                }
                std::cout << "]";
                if (i + 1 < nivelActual.size()) std::cout << "->";
            }
            std::cout << std::endl;
            break;
        } else {
            for (size_t i = 0; i < nivelActual.size(); ++i) {
                NodoBPlus* nodo = nivelActual[i];
                std::cout << "[";
                for (size_t j = 0; j < nodo->claves.size(); ++j) {
                    std::cout << std::setw(2) << nodo->claves[j];
                    if (j + 1 < nodo->claves.size()) std::cout << "|";
                }
                std::cout << "]";
                if (i + 1 < nivelActual.size()) std::cout << "  ";
                for (auto h : nodo->hijos) q.push(h);
            }
            std::cout << std::endl;
        }
    }
}