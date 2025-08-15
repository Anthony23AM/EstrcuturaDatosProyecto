#pragma once
#include <string>

void guardarCitasArchivo(const std::string& nombreArchivo);
void guardarModuloCitas(const std::string& rutaBase);
void respaldarDatosPrevios();
void cargarCitasArchivo(const std::string& nombreArchivo, const std::string& rutaBase);
std::string obtenerFechaHoraActual();
