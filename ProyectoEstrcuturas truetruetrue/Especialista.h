#ifndef ESPECIALISTA_H
#define ESPECIALISTA_H

#include <string>
#include <iostream>
#include "IMostrable.h"

class Especialista : public IMostrable {
private:
    int id;
    std::string nombre;
    std::string apellido;
    std::string cedula;
    std::string especialidad;
public:
    Especialista();
    Especialista(int id, const std::string& nom, const std::string& ape, const std::string& ced, const std::string& esp);
    int getId() const;
    std::string getNombre() const;
    std::string getApellido() const;
    std::string getCedula() const;
    std::string getEspecialidad() const;
    void mostrar() const override;
};

#endif