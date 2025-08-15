#include "Especialista.h"
#include <iostream>

Especialista::Especialista() : id(0), nombre(""), apellido(""), cedula(""), especialidad("") {}

Especialista::Especialista(int id, const std::string& nom, const std::string& ape, const std::string& ced, const std::string& esp)
    : id(id), nombre(nom), apellido(ape), cedula(ced), especialidad(esp) {}

int Especialista::getId() const { return id; }
std::string Especialista::getNombre() const { return nombre; }
std::string Especialista::getApellido() const { return apellido; }
std::string Especialista::getCedula() const { return cedula; }
std::string Especialista::getEspecialidad() const { return especialidad; }

void Especialista::mostrar() const {
    std::cout << "ID: " << id << "\nNombre: " << nombre << "\nApellido: " << apellido
        << "\nEspecialidad: " << especialidad << std::endl;
}
