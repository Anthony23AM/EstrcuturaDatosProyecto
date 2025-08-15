#ifndef VALIDAR_H
#define VALIDAR_H

#include <string>
#include "Fecha.h"

class Validar {
public:
    static std::string leerNombreOApellido(const std::string& campo);
    static std::string leerSoloNumeros(const std::string& mensaje, int maxLen);
    static int leerEnteroSeguro(const std::string& mensaje);
    static bool validarCedula(const std::string& cedula);
    static Fecha leerFecha(const std::string& mensaje);
    static bool validarCorreo(const std::string& correo);
    static std::string soloLetras(const std::string& mensaje);
    // Añadido para validación de edad máxima
    static bool validarEdadMaxima(const Fecha& nacimiento, const Fecha& actual, int maxAnios = 115);
    static bool validarEdadMaximaCita(const Fecha& nacimiento, const Fecha& cita, int maxAnios = 115);
    void generarQRyAbrir(const std::string& datos);
    void generarQRyAbrirEscaneable(const std::string& datos); // NUEVO: QR real escaneable
    void generarQRyAbrirConLibreria(const std::string& datos);
};

#endif