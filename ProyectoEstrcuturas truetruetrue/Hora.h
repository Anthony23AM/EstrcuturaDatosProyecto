#ifndef HORA_H
#define HORA_H

#include <iostream>

class Hora {
private:
    int hora, minuto;
public:
    Hora(int h = 0, int m = 0);
    int getHora() const;
    int getMinuto() const;
    void mostrar() const;
    bool esValida() const;
    bool operator==(const Hora& h) const;
};

#endif