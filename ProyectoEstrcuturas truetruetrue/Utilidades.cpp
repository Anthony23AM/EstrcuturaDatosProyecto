
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <conio.h>
#include <shellapi.h>
#ifndef SW_SHOWNORMAL
#define SW_SHOWNORMAL 1
#endif
#endif
#include "ListaSimple.h"
#include "Listasdc.h"
#include "Especialista.h"
#include "Evento.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <set>
#include <map>
#include <thread>
#include <filesystem>
using namespace std;

extern std::map<std::string, std::vector<Especialista>> especialistasPorHospital;
extern ListaSimple<Persona> listaPacientes;
extern ListaSimple<Especialista> listaEspecialistas;
extern ListaDC<Evento> listaTurnos;
extern std::string rutaBase;

int menuConFlechas(const vector<string>& opciones) {
    int seleccion = 0;
    char tecla;
    do {
        system("cls");
        std::cout << "\n==============================\n";
        std::cout << "  Sistema de gestion de turnos medicos\n";
        std::cout << "  Bienvenido al sistema\n";
        std::cout <<   "==============================\n\n";
        for (size_t i = 0; i < opciones.size(); ++i) {
            if (i == seleccion)
                std::cout << "-> ";
            else
                std::cout << "   ";
            std::cout << opciones[i] << std::endl;
        }
        tecla = _getch();
        if (tecla == 72 && seleccion > 0) seleccion--; // Flecha arriba
        else if (tecla == 80 && seleccion < (int)opciones.size() - 1) seleccion++; // Flecha abajo
        else if (tecla == 13) break; // Enter
    } while (true);
    return seleccion + 1; // Opciones empiezan en 1
}

void limpiarListaTurnos() {
    while (listaTurnos.eliminar([](const Evento&) { return true; }));
}

void mostrarGuiaGeneral() {
#ifdef _WIN32
#ifndef SW_SHOWNORMAL
#define SW_SHOWNORMAL 1
#endif
    ShellExecuteA(NULL, "open", "AyudaPrueba1.chm", NULL, NULL, SW_SHOWNORMAL);
#endif
}

void mostrarEspecialistasPorHospital() {
    if (especialistasPorHospital.empty()) {
        std::cout << "No hay especialistas asignados a hospitales.\n";
        return;
    }
    for (const auto& par : especialistasPorHospital) {
        std::cout << "\nHospital: " << par.first << std::endl;
        for (const auto& esp : par.second) {
            std::cout << "  ID: " << esp.getId()
                      << " | Nombre: " << esp.getNombre()
                      << " | Apellido: " << esp.getApellido()
                      << " | Cedula: " << esp.getCedula()
                      << " | Especialidad: " << esp.getEspecialidad() << std::endl;
        }
    }
}

void guardarEspecialistas(const std::string& archivo) {
    std::ofstream out(archivo, std::ios::out);
    if (!out) return;
    listaEspecialistas.mostrar([&](const Especialista& e) {
        out << e.getId() << '|'
            << e.getNombre() << '|'
            << e.getApellido() << '|'
            << e.getCedula() << '|'
            << e.getEspecialidad() << '\n';
    });
    out.close();
}

void mostrarEspecialistasRecursivo(const std::string& nombrePaciente, const std::string& apellidoPaciente) {
    cout << left << setw(8) << "ID"
        << setw(12) << "Nombre"
        << setw(14) << "Apellido"
        << setw(16) << "Especialidad" << endl;
    listaEspecialistas.mostrarRecursivo([&](const Especialista& e) {
        if (!(e.getNombre() == nombrePaciente && e.getApellido() == apellidoPaciente)) {
            cout << left << setw(8) << e.getId()
                << setw(12) << e.getNombre()
                << setw(14) << e.getApellido()
                << setw(16) << e.getEspecialidad() << endl;
        }
    });
}

void cargarEspecialistas(const std::string& archivo) {
    std::ifstream in(archivo);
    if (!in) return;
    std::string linea;
    while (getline(in, linea)) {
        std::stringstream ss(linea);
        std::string campo, nombre, apellido, cedula, especialidad;
        int id;
        getline(ss, campo, '|');
        id = stoi(campo);
        getline(ss, nombre, '|');
        getline(ss, apellido, '|');
        getline(ss, cedula, '|');
        getline(ss, especialidad, '\n');

        Especialista* existente = listaEspecialistas.buscar([&](const Especialista& e) {
            return e.getCedula() == cedula;
        });
        if (existente) {
            if (existente->getNombre() != nombre || existente->getApellido() != apellido) {
                cout << "Conflicto de especialista: " << existente->getNombre() << " " << existente->getApellido()
                     << " (cedula " << cedula << ") tiene conflicto con la base. Se elimina y se reemplaza por " 
                     << nombre << " " << apellido << ".\n";
                while (listaTurnos.eliminar([&](const Evento& ev) {
                    return ev.getEspecialista().getCedula() == cedula;
                }));
                listaEspecialistas.eliminar([&](const Especialista& e) {
                    return e.getCedula() == cedula;
                });
            } else {
                bool mismoRegistro = listaEspecialistas.existe([&](const Especialista& e) {
                    return e.getCedula() == cedula && e.getEspecialidad() == especialidad;
                });
                if (mismoRegistro) continue;
            }
        }
        listaEspecialistas.agregar(Especialista(id, nombre, apellido, cedula, especialidad));
    }
    in.close();
}
#include "Utilidades.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include <ctime>
#include <random>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif



Fecha obtenerFechaActualSistema() {
    std::time_t t = std::time(nullptr);
#ifdef _WIN32
    std::tm tm = {0};
    localtime_s(&tm, &t);
#else
    std::tm tm;
    localtime_r(&t, &tm);
#endif
    return Fecha(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

int generarIDUnico() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(100000, 999999);
    return dis(gen);
}

int calcularEdad(const Fecha& nacimiento, const Fecha& actual) {
    int edad = actual.getAnio() - nacimiento.getAnio();
    if (actual.getMes() < nacimiento.getMes() || (actual.getMes() == nacimiento.getMes() && actual.getDia() < nacimiento.getDia())) {
        edad--;
    }
    return edad;
}
