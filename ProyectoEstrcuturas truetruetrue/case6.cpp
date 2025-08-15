#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include "Validar.h"
#include "CitasIO.h"
using namespace std;

void guardarCitasArchivo(const std::string& nombreArchivo);
std::string obtenerFechaHoraActual();
void cargarCitasArchivo(const std::string& archivo, const std::string& rutaBase);

void case6(const std::string& rutaBase) {
    // Mostrar archivos de backup disponibles en la ruta base
    std::vector<std::string> archivosBackup;
    for (const auto& entry : std::filesystem::directory_iterator(rutaBase)) {
        if (entry.is_regular_file()) {
            std::string nombre = entry.path().filename().string();
            if (nombre.find(".txt") != std::string::npos && nombre != "moduloCitas.txt") {
                archivosBackup.push_back(nombre);
            }
        }
    }
    if (archivosBackup.empty()) {
        std::cout << "No hay archivos de backup disponibles en la ruta base.\n";
    } else {
        std::cout << "Archivos de backup disponibles en la ruta base:\n";
        for (size_t i = 0; i < archivosBackup.size(); ++i) {
            std::cout << (i+1) << ") " << archivosBackup[i] << std::endl;
        }
        int seleccion = 0;
        do {
            std::cout << "Seleccione el número de archivo a cargar: ";
            seleccion = Validar::leerEnteroSeguro("");
        } while (seleccion < 1 || seleccion > archivosBackup.size());
        std::string rutaArchivo = rutaBase + archivosBackup[seleccion-1];
        // Mensaje de advertencia y respaldo previo
        std::cout << "Se va a cargar el backup '" << archivosBackup[seleccion-1] << "'. Se creará un respaldo previo de los datos actuales.\n";
        std::string nombreRespaldo = rutaBase + "respaldo_" + obtenerFechaHoraActual() + ".txt";
        guardarCitasArchivo(nombreRespaldo);
        std::cout << "Respaldo creado en: " << nombreRespaldo << std::endl;
        // Cargar datos nuevos y actualizar moduloCitas.txt
        cargarCitasArchivo(rutaArchivo, rutaBase);
        std::cout << "Archivo cargado y datos actualizados desde: " << rutaArchivo << std::endl;
        // Sincronizar con MongoDB después de cargar backup solo si el archivo existe y no está vacío
        std::string archivoTurnos = rutaBase + "moduloCitas.txt";
        std::ifstream f(archivoTurnos);
        if (f && f.peek() != std::ifstream::traits_type::eof()) {
            system(("python Conexion_MongoDB/sincronizar_turnos_total.py " + archivoTurnos).c_str());
        } else {
            std::cout << "No se sincroniza: el archivo de turnos está vacío o no existe.\n";
        }
    }
    system("pause");
}
