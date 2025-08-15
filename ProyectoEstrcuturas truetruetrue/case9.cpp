#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Validar.h"
#include "Evento.h"
#include "Listasdc.h"
using namespace std;

extern ListaDC<Evento> listaTurnos;

void case9(const std::string& rutaBase) {
    if (listaTurnos.existe([](const Evento& ev) { return true; }) == false) {
        std::cout << "No hay citas registradas.\n";
    } else {
        std::vector<Evento> citas;
        listaTurnos.mostrar([&](const Evento& ev) {
            citas.push_back(ev);
        });
        std::cout << "Citas disponibles:\n";
        for (size_t i = 0; i < citas.size(); ++i) {
            std::cout << (i+1) << ") Turno ID: " << citas[i].getId()
                      << " | Paciente: " << citas[i].getPaciente().getNombre() << " " << citas[i].getPaciente().getApellido()
                      << " | Fecha: ";
            citas[i].getFechaAtencion().mostrar();
            std::cout << " | Especialista: " << citas[i].getEspecialista().getNombre() << " " << citas[i].getEspecialista().getApellido() << std::endl;
        }
        int seleccion = 0;
        do {
            std::cout << "Seleccione el número de cita para generar el QR PDF: ";
            seleccion = Validar::leerEnteroSeguro("");
        } while (seleccion < 1 || seleccion > citas.size());
        const Evento& cita = citas[seleccion-1];
        std::stringstream datosQR;
        datosQR << "Turno ID: " << cita.getId() << " ";
        datosQR << "Paciente: " << cita.getPaciente().getNombre() << " " << cita.getPaciente().getApellido() << " ";
        datosQR << "Cedula: " << cita.getPaciente().getCedula() << " ";
        datosQR << "Especialista: " << cita.getEspecialista().getNombre() << " " << cita.getEspecialista().getApellido() << " ";
        datosQR << "Especialidad: " << cita.getEspecialista().getEspecialidad() << " ";
        datosQR << "Fecha: " << cita.getFechaAtencion().getDia() << "/" << cita.getFechaAtencion().getMes() << "/" << cita.getFechaAtencion().getAnio() << " ";
        datosQR << "Hora: " << (cita.getHoraAtencion().getHora() < 10 ? "0" : "") << cita.getHoraAtencion().getHora() << ":" << (cita.getHoraAtencion().getMinuto() < 10 ? "0" : "") << cita.getHoraAtencion().getMinuto() << " ";
        datosQR << "Hospital: " << cita.getDireccionHospital() << " ";
        datosQR << "Direccion: " << cita.getDireccionHospital() << " ";
        std::stringstream nombrePDF;
        nombrePDF << rutaBase << "QR_Cita_" << cita.getFechaAtencion().getDia() << "-" << cita.getFechaAtencion().getMes() << "-" << cita.getFechaAtencion().getAnio() << ".pdf";
        std::string comandoPDF = "python generar_qr_pdf.py \"" + datosQR.str() + "\" \"" + nombrePDF.str() + "\"";
        system(comandoPDF.c_str());
        std::cout << "QR PDF generado: " << nombrePDF.str() << std::endl;
    }
    system("pause");
}
