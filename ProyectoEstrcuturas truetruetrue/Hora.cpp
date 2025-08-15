#include "Hora.h"
#include <iostream>

Hora::Hora(int h, int m) : hora(h), minuto(m) {}

int Hora::getHora() const { return hora; }
int Hora::getMinuto() const { return minuto; }

void Hora::mostrar() const {
    std::cout << (hora < 10 ? "0" : "") << hora << ":" << (minuto < 10 ? "0" : "") << minuto;
}

bool Hora::esValida() const {
    return hora >= 0 && hora < 24 && minuto >= 0 && minuto < 60;
}

bool Hora::operator==(const Hora& h) const {
    return hora == h.hora && minuto == h.minuto;
}