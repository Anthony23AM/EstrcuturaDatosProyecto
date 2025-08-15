#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include "Fecha.h"

class Persona {
protected:
    int idCentroMedico;
    std::string nombre;
    std::string apellido;
    std::string cedula;
    std::string correo;
    std::string telefono;
    Fecha fechaNacimiento;
public:
    Persona();
    Persona(int id, const std::string& nom, const std::string& ape, const std::string& ced, const std::string& cor, const std::string& tel, const Fecha& fn);
    int getId() const;
    std::string getNombre() const;
    std::string getApellido() const;
    std::string getCedula() const;
    std::string getCorreo() const;
    std::string getTelefono() const;
    Fecha getFechaNacimiento() const;
    void mostrar() const;
};

#endif