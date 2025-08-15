#include "Persona.h"
#include <iostream>

Persona::Persona() : idCentroMedico(0), nombre(""), apellido(""), cedula(""), correo(""), telefono(""), fechaNacimiento(1,1,2000) {}

Persona::Persona(int id, const std::string& nom, const std::string& ape, const std::string& ced, const std::string& cor, const std::string& tel, const Fecha& fn)
    : idCentroMedico(id), nombre(nom), apellido(ape), cedula(ced), correo(cor), telefono(tel), fechaNacimiento(fn) {}

int Persona::getId() const { return idCentroMedico; }
std::string Persona::getNombre() const { return nombre; }
std::string Persona::getApellido() const { return apellido; }
std::string Persona::getCedula() const { return cedula; }
std::string Persona::getCorreo() const { return correo; }
std::string Persona::getTelefono() const { return telefono; }
Fecha Persona::getFechaNacimiento() const { return fechaNacimiento; }

void Persona::mostrar() const {
    std::cout << "Nombre: " << nombre << "\nApellido: " << apellido
        << "\nCedula: " << cedula << "\nCorreo: " << correo << "\nTelefono: " << telefono << "\nFecha Nacimiento: ";
    fechaNacimiento.mostrar();
    std::cout << std::endl;
}