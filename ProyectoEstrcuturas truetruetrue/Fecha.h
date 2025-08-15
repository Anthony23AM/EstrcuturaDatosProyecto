#ifndef FECHA_H
#define FECHA_H

#include <iostream>

class Fecha {
private:
    int dia, mes, anio;
public:
    Fecha(int d = 1, int m = 1, int a = 2000);
    int getDia() const;
    int getMes() const;
    int getAnio() const;
    void mostrar() const;
    bool esValida() const;
    bool operator>(const Fecha& f) const;
    bool operator<(const Fecha& f) const;
};

#endif