#include "Fecha.h"
#include <ctime>

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

void Fecha::mostrar() const {
    std::cout << dia << "/" << mes << "/" << anio;
}

bool Fecha::esValida() const {
    if (anio < 1700 || anio > 4000) return false; // Cambiado: mínimo 1700, sin límite superior
    if (mes < 1 || mes > 12) return false;
    if (dia < 1) return false;
    int diasMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    bool bisiesto = (anio % 4 == 0 && (anio % 100 != 0 || anio % 400 == 0));
    if (bisiesto && mes == 2) return dia <= 29;
    return dia <= diasMes[mes - 1];
}

bool Fecha::operator>(const Fecha& f) const {
    if (anio != f.anio) return anio > f.anio;
    if (mes != f.mes) return mes > f.mes;
    return dia > f.dia;
}

bool Fecha::operator<(const Fecha& f) const {
    if (anio != f.anio) return anio < f.anio;
    if (mes != f.mes) return mes < f.mes;
    return dia < f.dia;
}

