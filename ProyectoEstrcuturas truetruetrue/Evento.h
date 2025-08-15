#ifndef EVENTO_H
#define EVENTO_H

#include "Persona.h"
#include "Especialista.h"
#include "Fecha.h"
#include "Hora.h"
#include <iostream>
#include <string> // <-- necesario para std::string

class Evento {
private:
    int id;
    Persona paciente;
    Especialista especialista;
    Fecha fechaAtencion;
    Hora horaAtencion;
    std::string direccionHospital;
    std::string last_modified; // Timestamp de última modificación
public:
    Evento(int id, const Persona& pac, const Especialista& esp, const Fecha& f, const Hora& h, const std::string& direccionHospital, const std::string& last_modified = "");
    int getId() const;
    void mostrar() const;
    Persona getPaciente() const;
    Especialista getEspecialista() const;
    Fecha getFechaAtencion() const;
    Hora getHoraAtencion() const;
    std::string getDireccionHospital() const;
    std::string getLastModified() const;
    void setLastModified(const std::string& lm);

    // NUEVO: Devuelve la clave de fecha y hora en formato AAAAMMDDHHmm
    std::string getClaveFechaHora() const;
};

#endif