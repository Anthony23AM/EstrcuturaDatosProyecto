#include "Evento.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Evento::Evento(int id, const Persona& pac, const Especialista& esp, const Fecha& f, const Hora& h, const std::string& direccionHospital, const std::string& last_modified)
    : id(id), paciente(pac), especialista(esp), fechaAtencion(f), horaAtencion(h), direccionHospital(direccionHospital), last_modified(last_modified) {}
std::string Evento::getLastModified() const {
    return last_modified;
}

void Evento::setLastModified(const std::string& lm) {
    last_modified = lm;
}
std::string Evento::getDireccionHospital() const {
    return direccionHospital;
}

int Evento::getId() const { return id; }
Persona Evento::getPaciente() const { return paciente; }
Especialista Evento::getEspecialista() const { return especialista; }
Fecha Evento::getFechaAtencion() const { return fechaAtencion; }
Hora Evento::getHoraAtencion() const { return horaAtencion; }

void Evento::mostrar() const {
    std::cout << "Turno ID: " << id << std::endl;
    std::cout << "Paciente: ";
    paciente.mostrar();
    std::cout << "Especialista: ";
    if (especialista.getId() != 0)
        std::cout << "ID: " << especialista.getId() << "\n";
    std::cout << "Nombre: " << especialista.getNombre() << "\nApellido: " << especialista.getApellido() << "\nEspecialidad: " << especialista.getEspecialidad() << std::endl;
    std::cout << "Fecha de atencion: ";
    fechaAtencion.mostrar();
    std::cout << " | Hora: ";
    horaAtencion.mostrar();
    std::cout << std::endl;
}

// NUEVO: Devuelve la clave de fecha y hora en formato AAAAMMDDHHmm
std::string Evento::getClaveFechaHora() const {
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << fechaAtencion.getAnio()
        << std::setw(2) << std::setfill('0') << fechaAtencion.getMes()
        << std::setw(2) << std::setfill('0') << fechaAtencion.getDia()
        << std::setw(2) << std::setfill('0') << horaAtencion.getHora()
        << std::setw(2) << std::setfill('0') << horaAtencion.getMinuto();
    return oss.str();
}