#ifndef DIASFESTIVOS_H
#define DIASFESTIVOS_H

#include "Fecha.h"
#include <string>

class DiasFestivos {
private:
    Fecha feriadosFijos[20];
    int totalFeriados;
    int anioObjetivo;

    void cargarFeriados();
    int calcularDiaSemana(const Fecha& f) const;
    Fecha calcularPascua(int anio) const;
    Fecha trasladarFeriado(const Fecha& f) const;
public:
    DiasFestivos(int anio);
    ~DiasFestivos();

    std::string obtenerDiaSemana(const Fecha& f) const;
    bool esFeriado(const Fecha& f) const;

    // Getters
    int getTotalFeriados() const;
    int getAnioObjetivo() const;
    Fecha getFeriado(int i) const;

    // Setters
    void setTotalFeriados(int t);
    void setAnioObjetivo(int anio);
    void setFeriado(int i, const Fecha& f);
};

#endif