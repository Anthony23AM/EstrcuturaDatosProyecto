#pragma once
#include "Persona.h"
#include "Especialista.h"
#include "Evento.h"
#include "ListaSimple.h"
#include "Listasdc.h"

extern ListaSimple<Persona> listaPacientes;
extern ListaSimple<Especialista> listaEspecialistas;
extern ListaDC<Evento> listaTurnos;
