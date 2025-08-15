#include "ListaSimple.h"
#include "Listasdc.h"
#include "Evento.h"
#include "Fecha.h"
#include "Hora.h"
#include "Especialista.h"
#include "Persona.h"
#include "DiasFestivos.h"
#include "Validar.h"
#include "CitasIO.h"
#include <iostream>
#include <string>
using namespace std;

extern ListaDC<Evento> listaTurnos;

void limpiarListaTurnos();
void cargarCitasArchivo(const std::string& archivo, const std::string& rutaBase);
int calcularEdad(const Fecha& nacimiento, const Fecha& actual);

void case2(const std::string& rutaBase, const Fecha& fechaActual) {
    // Descargar turnos actualizados desde MongoDB antes de mostrar (siempre con la ruta base)
    std::string comandoDescarga = "python ./Conexion_MongoDB/descargar_turnos_mongo.py \"" + rutaBase + "\"";
    system(comandoDescarga.c_str());
    // Limpiar la lista de turnos antes de cargar para evitar duplicados
    limpiarListaTurnos();
    cargarCitasArchivo(rutaBase + "moduloCitas.txt", rutaBase);
    cout << "\n------------------ LISTA DE TURNOS ------------------\n";
    listaTurnos.mostrarOrdenado(
        [](const Evento& a, const Evento& b) {
            return a.getClaveFechaHora() < b.getClaveFechaHora();
        },
        [&](const Evento& ev) {
            cout << "Turno ID: " << ev.getId() << endl;
            cout << "Paciente: " << ev.getPaciente().getNombre() << " " << ev.getPaciente().getApellido()
                << " | Cedula: " << ev.getPaciente().getCedula()
                << " | Telefono: " << ev.getPaciente().getTelefono()
                << " | Edad: " << calcularEdad(ev.getPaciente().getFechaNacimiento(), fechaActual) << endl;
            cout << "Especialista: " << ev.getEspecialista().getNombre() << " " << ev.getEspecialista().getApellido()
                << " | Especialidad: " << ev.getEspecialista().getEspecialidad() << endl;
            cout << "Hospital: " << ev.getDireccionHospital() << endl;
            cout << "Fecha de atencion: "; ev.getFechaAtencion().mostrar();
            cout << " | Hora: "; ev.getHoraAtencion().mostrar();
            cout << "\n-----------------------------------------------------\n";
        }
    );
    system("pause");
}
