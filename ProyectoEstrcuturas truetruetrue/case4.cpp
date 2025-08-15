// --- Nueva función para eliminar cita solo por cédula ---
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <cstdlib> 
#include "CitasIO.h"
#include "ListaSimple.h"
#include "Listasdc.h"
#include "Evento.h"
#include "Persona.h"
#include "Validar.h"
using namespace std;

extern ListaSimple<Persona> listaPacientes;
extern ListaDC<Evento> listaTurnos;
void case4(const std::string& rutaBase) {
    bool eliminado = false;
    string ced = Validar::leerSoloNumeros("Cedula: ", 11);
    std::vector<Evento> turnosPaciente;
    listaTurnos.mostrar([&](const Evento& e) {
        if (e.getPaciente().getCedula() == ced) turnosPaciente.push_back(e);
    });
    if (turnosPaciente.empty()) {
        std::cout << "No se encontraron turnos para esa cédula.\n";
        eliminado = false;
    } else {
        std::cout << "Turnos encontrados para la cédula.\n";
        for (size_t i = 0; i < turnosPaciente.size(); ++i) {
            std::cout << (i+1) << ") ID Turno: " << turnosPaciente[i].getId() << " | Fecha: ";
            turnosPaciente[i].getFechaAtencion().mostrar();
            std::cout << " | Hora: "; turnosPaciente[i].getHoraAtencion().mostrar();
            std::cout << std::endl;
        }
        int seleccion = 0;
        do {
            std::cout << "Seleccione el número de turno a eliminar: ";
            seleccion = Validar::leerEnteroSeguro("");
        } while (seleccion < 1 || seleccion > turnosPaciente.size());
        int idTurnoEliminar = turnosPaciente[seleccion-1].getId();
        std::cout << "¿Está seguro que desea eliminar el turno seleccionado? (s/n): ";
        char conf;
        do { conf = _getch(); } while (conf != 's' && conf != 'n');
        std::cout << conf << std::endl;
        if (conf == 's') {
            listaTurnos.eliminar([&](const Evento& e) { return e.getId() == idTurnoEliminar; });
            std::string comandoEliminar = "python ./Conexion_MongoDB/eliminar_turno_mongo.py " + std::to_string(idTurnoEliminar);
            int res = system(comandoEliminar.c_str());
            if (res != 0) std::cout << "Error al eliminar en MongoDB.\n";
            // Si el paciente ya no tiene turnos, eliminarlo
            bool tieneMasTurnos = false;
            listaTurnos.mostrar([&](const Evento& e) { if (e.getPaciente().getCedula() == ced) tieneMasTurnos = true; });
            if (!tieneMasTurnos) {
                eliminado = listaPacientes.eliminar([&](const Persona& p) { return p.getCedula() == ced; });
            } else {
                eliminado = true;
            }
        } else {
            std::cout << "Eliminación cancelada.\n";
            eliminado = false;
        }
    }

    if (eliminado) {
        std::cout << "Eliminado.\n";
        guardarModuloCitas(rutaBase);
        // Subida inmediata tras eliminación: sincronizar con MongoDB
        std::string archivoTurnos = rutaBase + "moduloCitas.txt";
        std::ifstream f(archivoTurnos);
        if (f && f.peek() != std::ifstream::traits_type::eof()) {
            int resSync = system(("python ./Conexion_MongoDB/sincronizar_turnos_total.py " + archivoTurnos).c_str());
            if (resSync != 0) {
                std::cout << "\nERROR: No se pudo sincronizar la eliminación con MongoDB.\n";
                std::cout << "Los datos locales pueden diferir de la nube.\n";
            } else {
                std::cout << "Sincronización exitosa con la nube.\n";
            }
        } else {
            std::cout << "No se sincroniza: el archivo de turnos está vacío o no existe.\n";
        }
    } else {
        std::cout << "No encontrado.\n";
    }
    system("pause");
}