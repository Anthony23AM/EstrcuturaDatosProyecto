#ifndef CASE1_H
#define CASE1_H

#include <string>
#include "Persona.h"
#include "Especialista.h"
#include "Evento.h"
#include "Fecha.h"
#include "Hora.h"
#include <vector>
#include "ListaSimple.h"
#include "Listasdc.h"
#include "CitasIO.h"

#include "DiasFestivos.h"
#include <map>

void generarTurno(
    std::string& rutaBase,
    ListaSimple<Persona>& listaPacientes,
    ListaSimple<Especialista>& listaEspecialistas,
    ListaDC<Evento>& listaTurnos,
    std::map<std::string, std::vector<Especialista>>& especialistasPorHospital,
    int& contadorTurno
);

#endif // CASE1_H
