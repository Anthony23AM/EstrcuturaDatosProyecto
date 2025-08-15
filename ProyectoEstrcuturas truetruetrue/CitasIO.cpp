#include "CitasIO.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <filesystem>
#include "Evento.h"
#include "Persona.h"
#include "Especialista.h"
#include "ListaSimple.h"
#include "Listasdc.h"
#include "Fecha.h"
#include "Hora.h"
#include <map>
#include <string>

extern ListaSimple<Persona> listaPacientes;
extern ListaSimple<Especialista> listaEspecialistas;
extern ListaDC<Evento> listaTurnos;
extern std::map<std::string, std::vector<Especialista>> especialistasPorHospital;

std::string obtenerFechaHoraActual() {
    time_t t = time(nullptr);
    struct tm* now = localtime(&t);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", now);
    return std::string(buffer);
}

void guardarCitasArchivo(const std::string& nombreArchivo) {
    std::ofstream out(nombreArchivo);
    if (!out) return;
    listaTurnos.mostrar([&](const Evento& ev) {
        out << ev.getId() << '|'
            << ev.getPaciente().getId() << '|'
            << ev.getPaciente().getNombre() << '|'
            << ev.getPaciente().getApellido() << '|'
            << ev.getPaciente().getCedula() << '|'
            << ev.getPaciente().getCorreo() << '|'
            << ev.getPaciente().getTelefono() << '|'
            << ev.getPaciente().getFechaNacimiento().getDia() << '/' << ev.getPaciente().getFechaNacimiento().getMes() << '/' << ev.getPaciente().getFechaNacimiento().getAnio() << '|'
            << ev.getEspecialista().getId() << '|'
            << ev.getEspecialista().getNombre() << '|'
            << ev.getEspecialista().getApellido() << '|'
            << ev.getEspecialista().getEspecialidad() << '|'
            << ev.getEspecialista().getCedula() << '|'
            << ev.getFechaAtencion().getDia() << '/' << ev.getFechaAtencion().getMes() << '/' << ev.getFechaAtencion().getAnio() << '|'
            << ev.getHoraAtencion().getHora() << ':' << ev.getHoraAtencion().getMinuto() << '|'
            << ev.getDireccionHospital() << '|'
            << ev.getLastModified() << '\n';
    });
    out.close();
}

void guardarModuloCitas(const std::string& rutaBase) {
    guardarCitasArchivo(rutaBase + "moduloCitas.txt"); // Siempre usa la ruta base
}

void respaldarDatosPrevios() {
    extern std::string rutaBase;
    std::string nombre = rutaBase + "recuperacion_" + obtenerFechaHoraActual() + ".txt";
    guardarCitasArchivo(nombre);
}

void cargarCitasArchivo(const std::string& nombreArchivo, const std::string& rutaBase) {
    std::ifstream in(nombreArchivo);
    if (!in) return;
    respaldarDatosPrevios();
    while (listaTurnos.eliminar([](const Evento&) { return true; }));
    while (listaPacientes.eliminar([](const Persona&) { return true; }));
    while (listaEspecialistas.eliminar([](const Especialista&) { return true; }));
    std::string linea;
    int lineaNum = 0;
    while (getline(in, linea)) {
        ++lineaNum;
        std::stringstream ss(linea);
        std::string campo;
        int idTurno = 0;
        std::string nombre, apellido, cedula, correo, telefono;
        int diaN = 0, mesN = 0, anioN = 0;
        std::string nombreEsp, apellidoEsp, especialidad, cedulaEsp;
        int diaA = 0, mesA = 0, anioA = 0, hora = 0, minuto = 0;
        std::string direccionHospital;
        bool error = false;

        // idTurno
        if (!getline(ss, campo, '|') || campo.empty() || campo.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << "[Error] Línea " << lineaNum << ": idTurno inválido: '" << campo << "'. Se omite línea.\n";
            continue;
        }
        idTurno = stoi(campo);
        // nombre
        if (!getline(ss, nombre, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": nombre faltante.\n"; continue; }
        // apellido
        if (!getline(ss, apellido, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": apellido faltante.\n"; continue; }
        // cedula
        if (!getline(ss, cedula, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": cedula faltante.\n"; continue; }
        // correo
        if (!getline(ss, correo, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": correo faltante.\n"; continue; }
        // telefono
        if (!getline(ss, telefono, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": telefono faltante.\n"; continue; }
        // fecha nacimiento
        if (!getline(ss, campo, '|') || sscanf(campo.c_str(), "%d/%d/%d", &diaN, &mesN, &anioN) != 3) {
            std::cerr << "[Error] Línea " << lineaNum << ": fecha nacimiento inválida: '" << campo << "'.\n"; continue;
        }
        // nombreEsp
        if (!getline(ss, nombreEsp, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": nombreEsp faltante.\n"; continue; }
        // apellidoEsp
        if (!getline(ss, apellidoEsp, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": apellidoEsp faltante.\n"; continue; }
        // especialidad
        if (!getline(ss, especialidad, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": especialidad faltante.\n"; continue; }
        // cedulaEsp
        if (!getline(ss, cedulaEsp, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": cedulaEsp faltante.\n"; continue; }
        // fecha atencion
        if (!getline(ss, campo, '|') || sscanf(campo.c_str(), "%d/%d/%d", &diaA, &mesA, &anioA) != 3) {
            std::cerr << "[Error] Línea " << lineaNum << ": fecha atencion inválida: '" << campo << "'.\n"; continue;
        }
        // hora:minuto
        if (!getline(ss, campo, '|') || sscanf(campo.c_str(), "%d:%d", &hora, &minuto) != 2) {
            std::cerr << "[Error] Línea " << lineaNum << ": hora inválida: '" << campo << "'.\n"; continue;
        }
        // direccionHospital
        if (!getline(ss, campo, '|')) { std::cerr << "[Error] Línea " << lineaNum << ": direccionHospital faltante.\n"; continue; }
        direccionHospital = campo;
        // lastModified (opcional)
        std::string lastModified = "";
        if (getline(ss, campo, '\n')) lastModified = campo;

        Persona pacienteNuevo(0, nombre, apellido, cedula, correo, telefono, Fecha(diaN, mesN, anioN));
        Especialista especialistaNuevo(0, nombreEsp, apellidoEsp, cedulaEsp, especialidad);
        Fecha fechaAtencion(diaA, mesA, anioA);
        Hora horaAtencion(hora, minuto);
        Evento turno(idTurno, pacienteNuevo, especialistaNuevo, fechaAtencion, horaAtencion, direccionHospital, lastModified);
        listaPacientes.agregar(pacienteNuevo);
        listaEspecialistas.agregar(especialistaNuevo);
        // Solo agregar si el ID no existe ya en la lista
        bool idRepetido = false;
        listaTurnos.mostrar([&](const Evento& ev) {
            if (ev.getId() == idTurno) idRepetido = true;
        });
        if (!idRepetido) {
            listaTurnos.agregar(turno);
            // Actualizar el contador global si el idTurno es mayor
            extern int contadorTurno;
            if (idTurno >= contadorTurno) {
                contadorTurno = idTurno + 1;
            }
        } else {
            std::cerr << "[Advertencia] ID de turno repetido: " << idTurno << ". No se agrega el turno duplicado.\n";
        }
        if (!direccionHospital.empty()) {
            auto& listaEsp = especialistasPorHospital[direccionHospital];
            bool existe = false;
            for (const auto& esp : listaEsp) {
                if (esp.getId() == especialistaNuevo.getId() &&
                    esp.getCedula() == especialistaNuevo.getCedula() &&
                    esp.getNombre() == especialistaNuevo.getNombre() &&
                    esp.getApellido() == especialistaNuevo.getApellido() &&
                    esp.getEspecialidad() == especialistaNuevo.getEspecialidad()) {
                    existe = true;
                    break;
                }
            }
            if (!existe) {
                listaEsp.push_back(especialistaNuevo);
            }
        }
    }
    in.close();
    guardarModuloCitas(rutaBase);
}
