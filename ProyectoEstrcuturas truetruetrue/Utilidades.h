#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
#include "Fecha.h"


int menuConFlechas(const std::vector<std::string>& opciones);
void limpiarListaTurnos();
void mostrarGuiaGeneral();
void mostrarEspecialistasPorHospital();
void guardarEspecialistas(const std::string& archivo);
void mostrarEspecialistasRecursivo(const std::string& nombrePaciente = "", const std::string& apellidoPaciente = "");
void cargarEspecialistas(const std::string& archivo);
Fecha obtenerFechaActualSistema();
int generarIDUnico();
int calcularEdad(const Fecha& nacimiento, const Fecha& actual);

#endif // UTILIDADES_H
