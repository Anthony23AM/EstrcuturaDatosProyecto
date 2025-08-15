#include "DiasFestivos.h"

DiasFestivos::DiasFestivos(int anio) : anioObjetivo(anio) {
    cargarFeriados();
}

DiasFestivos::~DiasFestivos() {
}

int DiasFestivos::calcularDiaSemana(const Fecha& f) const {
    int d = f.getDia();
    int m = f.getMes();
    int y = f.getAnio();

    if (m < 3) {
        m += 12;
        y -= 1;
    }

    int k = y % 100;
    int j = y / 100;

    int h = (d + (13 * (m + 1)) / 5 + k + (k / 4) + (j / 4) + 5 * j) % 7;
    return h; // 0 = sabado, 1 = domingo, ..., 6 = viernes
}

std::string DiasFestivos::obtenerDiaSemana(const Fecha& f) const {
    const std::string dias[7] = {
        "Sabado", "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes"
    };
    return dias[calcularDiaSemana(f)];
}

Fecha DiasFestivos::calcularPascua(int anio) const {
    // Algoritmo de Meeus/Jones/Butcher
    int a = anio % 19;
    int b = anio / 100;
    int c = anio % 100;
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int l = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * l) / 451;
    int mes = (h + l - 7 * m + 114) / 31;
    int dia = ((h + l - 7 * m + 114) % 31) + 1;
    return Fecha(dia, mes, anio);
}

Fecha DiasFestivos::trasladarFeriado(const Fecha& f) const {
    int diaSemana = calcularDiaSemana(f);
    // 0 = sabado, 1 = domingo, 2 = lunes, 3 = martes, 4 = miercoles, 5 = jueves, 6 = viernes
    Fecha trasladada = f;
    if (diaSemana == 0) { // Sábado → viernes anterior
        trasladada = Fecha(f.getDia() - 1, f.getMes(), f.getAnio());
    }
    else if (diaSemana == 1) { // Domingo → lunes siguiente
        trasladada = Fecha(f.getDia() + 1, f.getMes(), f.getAnio());
    }
    else if (diaSemana == 3 || diaSemana == 4) { // Martes o miércoles → lunes anterior
        trasladada = Fecha(f.getDia() - (diaSemana - 2), f.getMes(), f.getAnio());
    }
    else if (diaSemana == 5) { // Jueves → viernes siguiente
        trasladada = Fecha(f.getDia() + 1, f.getMes(), f.getAnio());
    }
    // Validar que el feriado trasladado siga en el mismo año objetivo
    if (trasladada.getAnio() != anioObjetivo) return f;
    return trasladada;
}

void DiasFestivos::cargarFeriados() {
    totalFeriados = 0;
    int anio = anioObjetivo;

    // Solo feriados del año actual
    feriadosFijos[totalFeriados++] = Fecha(1, 1, anio+1);    // Año Nuevo
    feriadosFijos[totalFeriados++] = Fecha(1, 5, anio);    // Día del Trabajo
    feriadosFijos[totalFeriados++] = Fecha(24, 5, anio);   // Batalla del Pichincha
    feriadosFijos[totalFeriados++] = Fecha(10, 8, anio);   // Primer Grito de Independencia
    feriadosFijos[totalFeriados++] = Fecha(9, 10, anio);   // Independencia de Guayaquil
    feriadosFijos[totalFeriados++] = Fecha(2, 11, anio);   // Día de los Difuntos
    feriadosFijos[totalFeriados++] = Fecha(24, 12, anio);  // Nochebuena
    feriadosFijos[totalFeriados++] = Fecha(25, 12, anio);  // Navidad
    feriadosFijos[totalFeriados++] = Fecha(31, 12, anio);  // Nochevieja

    // Carnaval (lunes y martes antes de Pascua)
    Fecha pascua = calcularPascua(anio);
    int d = pascua.getDia();
    int m = pascua.getMes();
    int a = pascua.getAnio();
    // Lunes y martes de carnaval: 48 y 47 días antes de Pascua
    int diasCarnaval[2] = { -48, -47 };
    for (int i = 0; i < 2; ++i) {
        int diasRestar = -diasCarnaval[i];
        int dia = d, mes = m, anioC = a;
        for (int j = 0; j < diasRestar; ++j) {
            dia--;
            if (dia < 1) {
                mes--;
                if (mes < 1) { mes = 12; anioC--; }
                int diasMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
                bool bisiesto = (anioC % 4 == 0 && (anioC % 100 != 0 || anioC % 400 == 0));
                if (bisiesto && mes == 2) dia = 29;
                else dia = diasMes[mes - 1];
            }
        }
        if (anioC == anio) // Solo agregar si es del año actual
            feriadosFijos[totalFeriados++] = Fecha(dia, mes, anioC);
    }

    // Viernes Santo: 2 días antes de Pascua
    {
        int diasRestar = 2;
        int dia = d, mes = m, anioV = a;
        for (int j = 0; j < diasRestar; ++j) {
            dia--;
            if (dia < 1) {
                mes--;
                if (mes < 1) { mes = 12; anioV--; }
                int diasMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
                bool bisiesto = (anioV % 4 == 0 && (anioV % 100 != 0 || anioV % 400 == 0));
                if (bisiesto && mes == 2) dia = 29;
                else dia = diasMes[mes - 1];
            }
        }
        if (anioV == anio)
            feriadosFijos[totalFeriados++] = Fecha(dia, mes, anioV);
    }

    // Trasladar feriados según ley ecuatoriana
    for (int i = 0; i < totalFeriados; ++i) {
        feriadosFijos[i] = trasladarFeriado(feriadosFijos[i]);
    }
}

bool DiasFestivos::esFeriado(const Fecha& f) const {
    for (int i = 0; i < totalFeriados; ++i) {
        if (f.getDia() == feriadosFijos[i].getDia() &&
            f.getMes() == feriadosFijos[i].getMes() &&
            f.getAnio() == feriadosFijos[i].getAnio()) {
            return true;
        }
    }
    return false;
}

// Getters
int DiasFestivos::getTotalFeriados() const {
    return totalFeriados;
}

int DiasFestivos::getAnioObjetivo() const {
    return anioObjetivo;
}

Fecha DiasFestivos::getFeriado(int i) const {
    if (i >= 0 && i < totalFeriados) {
        return feriadosFijos[i];
    }
    return Fecha();
}

// Setters
void DiasFestivos::setTotalFeriados(int t) {
    totalFeriados = t;
}

void DiasFestivos::setAnioObjetivo(int anio) {
    anioObjetivo = anio;
    cargarFeriados(); // actualizamos los feriados si el anio cambia
}

void DiasFestivos::setFeriado(int i, const Fecha& f) {
    if (i >= 0 && i < 20) {
        feriadosFijos[i] = f;
    }
}