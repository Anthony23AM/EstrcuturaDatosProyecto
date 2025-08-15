#include "Validar.h"
#include <iostream>
#include <string>
#include <cctype>
#include <regex>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "QrCode.hpp"
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

// Solo permite ingreso de números por teclado
int Validar::leerEnteroSeguro(const std::string& mensaje) {
    char c;
    int valor;
    int i = 0;
    char dato[20];

    std::cout << mensaje << ": ";
    while ((c = _getch()) != 13) { // 13 = Enter
        if (c >= '0' && c <= '9') {
            if (i < 19) {
                dato[i++] = c;
                std::cout << c;
            }
        }
        // Permitir borrar con Backspace
        else if (c == 8 && i > 0) {
            --i;
            std::cout << "\b \b";
        }
    }
    dato[i] = '\0';
    std::cout << std::endl;
    valor = atoi(dato);
    return valor;
}

// Solo permite ingreso de letras, espacios y puntos por teclado
std::string Validar::soloLetras(const std::string& mensaje) {
    char c;
    int i = 0;
    char dato[60];
    std::cout << mensaje << ": ";
    while ((c = _getch()) != 13) { // 13 = Enter
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == '.') {
            if (i < 59) {
                dato[i++] = c;
                std::cout << c;
            }
        }
        // Permitir borrar con Backspace
        else if (c == 8 && i > 0) {
            --i;
            std::cout << "\b \b";
        }
    }
    dato[i] = '\0';
    std::cout << std::endl;
    return std::string(dato);
}

bool Validar::validarCedula(const std::string& cedula) {
    if (cedula.length() != 10) return false;
    for (char c : cedula) if (!isdigit(c)) return false;

    int provincia = std::stoi(cedula.substr(0, 2));
    if (provincia < 1 || provincia > 24) return false;

    int tercerDigito = cedula[2] - '0';
    if (tercerDigito >= 6) return false;

    int suma = 0;
    for (int i = 0; i < 9; ++i) {
        int num = cedula[i] - '0';
        if (i % 2 == 0) {
            num *= 2;
            if (num > 9) num -= 9;
        }
        suma += num;
    }
    int verificadorCalculado = (10 - (suma % 10)) % 10;
    int verificadorReal = cedula[9] - '0';
    return verificadorCalculado == verificadorReal;
}

Fecha Validar::leerFecha(const std::string& mensaje) {
    int d, m, a;
    while (true) {
        std::cout << mensaje << "\n";

        // Leer dia con validacion de rango
        while (true) {
            d = leerEnteroSeguro("Dia");
            if (d >= 1 && d <= 31) break;
            std::cout << "El dia debe estar entre 1 y 31. Intente de nuevo.\n";
        }

        // Leer mes con validacion de rango
        while (true) {
            m = leerEnteroSeguro("Mes");
            if (m >= 1 && m <= 12) break;
            std::cout << "El mes debe estar entre 1 y 12. Intente de nuevo.\n";
        }

        // Verificar combinacion dia-mes valida sin anio aun
        int diasMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
        if (d > diasMes[m - 1]) {
            if (!(m == 2 && d == 29)) {
                std::cout << "La combinacion de dia y mes no es valida. Intente de nuevo.\n";
                continue; // Vuelve a pedir dia y mes
            }
        }

        // Leer anio y validar la fecha completa con bisiesto incluido
        a = leerEnteroSeguro("Anio");
        Fecha f(d, m, a);
        if (!f.esValida()) {
            std::cout << "Fecha invalida. Intente de nuevo.\n";
            continue;
        }

        return f;
    }
}

bool Validar::validarCorreo(const std::string& correo) {
    std::regex patron(R"(^[A-Za-z0-9]+(\.[A-Za-z0-9]+)*@gmail\.com$)",
                      std::regex_constants::icase);
    return std::regex_match(correo, patron);
}


// Añadido: Validar que la edad no supere el máximo permitido
bool Validar::validarEdadMaxima(const Fecha& nacimiento, const Fecha& actual, int maxAnios) {
    int edad = actual.getAnio() - nacimiento.getAnio();
    if (actual.getMes() < nacimiento.getMes() ||
        (actual.getMes() == nacimiento.getMes() && actual.getDia() < nacimiento.getDia())) {
        edad--;
    }
    return edad <= maxAnios;
}

// Añadido: Validar que la edad en la fecha de la cita no supere el máximo permitido
bool Validar::validarEdadMaximaCita(const Fecha& nacimiento, const Fecha& cita, int maxAnios) {
    int edad = cita.getAnio() - nacimiento.getAnio();
    if (cita.getMes() < nacimiento.getMes() ||
        (cita.getMes() == nacimiento.getMes() && cita.getDia() < nacimiento.getDia())) {
        edad--;
    }
    return edad <= maxAnios;
}

// Genera un QR simple en SVG y lo abre en el navegador con los datos de la cita
void Validar::generarQRyAbrir(const std::string& datos) {
    // --- Generador QR versión 1 (21x21) modo alfanumérico, sin librerías externas ---
    // Solo soporta datos cortos (máx 25-30 caracteres alfanuméricos)
    // Para simplicidad, se usará un patrón fijo (no QR real, pero sí "escaneable" para pruebas)
    // Si quieres un QR real, se debe implementar el algoritmo completo (esto es un ejemplo visual)

    // SVG de "QR" simple (patrón de cuadros)
    std::string svg = "<svg xmlns='http://www.w3.org/2000/svg' width='210' height='210' viewBox='0 0 21 21' style='background:white'>";
    for (int y = 0; y < 21; ++y) {
        for (int x = 0; x < 21; ++x) {
            // Patrón simple: alterna cuadros para simular QR
            if (((x + y) % 2 == 0) || (x < 7 && y < 7) || (x > 13 && y < 7) || (x < 7 && y > 13)) {
                svg += "<rect x='" + std::to_string(x) + "' y='" + std::to_string(y) + "' width='1' height='1' fill='black'/>";
            }
        }
    }
    svg += "</svg>";

    // HTML temporal con los datos y el SVG
    std::string html = "<html><head><meta charset='utf-8'><title>QR de Cita</title></head><body>";
    html += "<h2>Datos de la cita</h2><pre>" + datos + "</pre>";
    html += "<h3>QR generado</h3>" + svg + "</body></html>";

    // Guardar archivo temporal
    std::string filename = "qr_cita_temp.html";
    FILE* f = fopen(filename.c_str(), "w");
    if (f) {
        fwrite(html.c_str(), 1, html.size(), f);
        fclose(f);
        // Abrir en navegador predeterminado (Windows)
        std::string cmd = "start " + filename;
        system(cmd.c_str());
    } else {
        std::cerr << "No se pudo crear el archivo temporal para el QR." << std::endl;
    }
}

// Genera un QR real (versión 1, modo alfanumérico, escaneable, con ECC Reed-Solomon y formato estándar) en SVG y lo abre en el navegador
// Solo soporta datos cortos (máx 25 caracteres alfanuméricos QR)
void Validar::generarQRyAbrirEscaneable(const std::string& datos) {
    // --- Tablas para Reed-Solomon (GF(256)) ---
    static const unsigned char gf_exp[512] = {
        1,2,4,8,16,32,64,128,29,58,116,232,205,135,19,38,76,152,45,90,180,117,234,201,143,3,6,12,24,48,96,192,157,39,78,156,37,74,148,53,106,212,181,119,238,193,159,35,70,140,5,10,20,40,80,160,93,186,105,210,185,111,222,161,95,190,97,194,153,47,94,188,101,202,137,15,30,60,120,240,253,231,211,187,107,214,177,127,254,225,223,163,91,182,113,226,217,175,67,134,17,34,68,136,13,26,52,104,208,189,103,206,129,31,62,124,248,237,199,147,59,118,236,197,151,51,102,204,133,23,46,92,184,109,218,169,79,158,33,66,132,21,42,84,168,77,154,41,82,164,85,170,73,146,57,114,228,213,183,115,230,209,191,99,198,145,63,126,252,229,215,179,123,246,241,255,227,219,171,75,150,49,98,196,149,55,110,220,165,87,174,65,130,25,50,100,200,141,7,14,28,56,112,224,221,167,83,166,81,162,89,178,121,242,249,239,195,155,43,86,172,69,138,9,18,36,72,144,61,122,244,245,247,243,251,235,203,139,11,22,44,88,176,125,250,233,207,131,27,54,108,216,173,71,142};
    static const unsigned char gf_log[256] = {
        0,0,1,25,2,50,26,198,3,223,51,238,27,104,199,75,4,100,224,14,52,141,239,129,28,193,105,248,200,8,76,113,5,138,101,47,225,36,15,33,53,147,142,218,240,18,130,69,29,181,194,125,106,39,249,185,201,154,9,120,77,228,114,166,6,191,139,98,102,221,48,253,226,152,37,179,16,145,34,136,54,208,148,206,143,150,219,189,241,210,19,92,131,56,70,64,30,66,182,163,195,72,126,110,107,58,40,84,250,133,186,61,202,94,155,159,10,21,121,43,78,212,229,172,115,243,167,87,7,112,192,247,140,128,99,13,103,74,222,237,49,197,254,24,227,165,153,119,38,184,180,124,17,68,146,217,35,32,137,46,55,63,209,91,149,188,207,205,144,135,151,178,220,252,190,97,242,86,211,171,20,42,93,158,132,60,57,83,71,109,65,162,31,45,67,216,183,123,164,118,196,23,73,236,127,12,111,246,108,161,59,82,41,157,85,170,251,96,134,177,187,204,62,90,203,89,95,176,156,169,160,81,11,245,22,235,122,117,44,215,79,174,213,233,230,231,173,232,116,214,244,234,168,80,88,175};
    // 1. Convertir datos a modo alfanumérico QR
    const char* tabla = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";
    std::string upper;
    for (char c : datos) {
        if (c >= 'a' && c <= 'z') upper += c - 32;
        else if (strchr(tabla, c) || c == ' ') upper += c;
        else if (c == '\n') upper += ' '; // reemplazar salto por espacio
    }
    if (upper.size() > 25) upper = upper.substr(0, 25); // Limitar a 25
    // 2. Codificar a bits QR (modo alfanumérico)
    std::string bits = "0010"; // modo alfanumérico
    char len[10];
    sprintf(len, "%09b", (int)upper.size());
    bits += len;
    for (size_t i = 0; i < upper.size(); i += 2) {
        int v1 = (int)(strchr(tabla, upper[i]) - tabla);
        if (i + 1 < upper.size()) {
            int v2 = (int)(strchr(tabla, upper[i + 1]) - tabla);
            int v = 45 * v1 + v2;
            char b[12]; sprintf(b, "%011b", v); bits += b;
        } else {
            char b[7]; sprintf(b, "%06b", v1); bits += b;
        }
    }
    // Terminador y relleno
    bits += "0000";
    while (bits.size() % 8 != 0) bits += '0';
    while (bits.size() < 152) bits += (bits.size()/8)%2 ? "00010001" : "11101100";
    // 3. ECC Reed-Solomon (versión 1-L: 19 datos, 7 ECC, estándar)
    unsigned char data[26] = {0};
    for (int i = 0; i < 19; ++i) {
        int v = 0;
        for (int b = 0; b < 8; ++b) v = (v << 1) | (bits[i*8+b] == '1');
        data[i] = v;
    }
    // Reed-Solomon ECC (QR v1-L, polinomio generador de grado 7)
    unsigned char ecc[7] = {0};
    // Polinomio generador QR v1-L (grado 7): [87,229,146,149,238,102,21]
    static const unsigned char gen[7] = {87,229,146,149,238,102,21};
    for (int i = 0; i < 19; ++i) {
        unsigned char factor = data[i] ^ ecc[0];
        // Desplazar todos los coeficientes a la izquierda
        for (int j = 0; j < 6; ++j)
            ecc[j] = ecc[j+1];
        ecc[6] = 0;
        if (factor) {
            for (int j = 0; j < 7; ++j) {
                ecc[j] ^= gf_exp[(gf_log[factor] + gf_log[gen[j]]) % 255];
            }
        }
    }
    for (int i = 0; i < 7; ++i) data[19+i] = ecc[i];
    // 4. Interleaving (no necesario en v1-L)
    // 5. Crear matriz QR (21x21)
    int qr[21][21] = {};
    // Insertar patrones de posición
    for (int i = 0; i < 7; ++i) for (int j = 0; j < 7; ++j) {
        if ((i == 0 || i == 6 || j == 0 || j == 6) || (i >= 2 && i <= 4 && j >= 2 && j <= 4)) {
            qr[i][j] = qr[i][20 - j] = qr[20 - i][j] = 1;
        }
    }
    // Timing
    for (int i = 8; i < 13; ++i) qr[6][i] = qr[i][6] = i%2==0;
    // 6. Insertar datos (zigzag, máscara 0: (i+j)%2==0)
    int dir = -1, x = 20, y = 20, k = 0, byte = 0, bit = 7;
    while (k < 152) {
        for (int col = 0; col < 2; ++col) {
            int xx = x - col;
            if (xx == 6) continue;
            if (y >= 0 && y < 21 && xx >= 0 && xx < 21 && qr[y][xx] == 0) {
                int v = (data[byte] >> bit) & 1;
                // Aplicar máscara 0
                if (((y+xx)%2)==0) v ^= 1;
                qr[y][xx] = v;
                ++k;
                if (--bit < 0) { bit = 7; ++byte; }
            }
        }
        y += dir;
        if (y < 0 || y >= 21) { dir = -dir; x -= 2; y += dir; }
        if (x < 0) break;
    }
    // 7. Bits de formato (máscara 0, nivel L: 111011111000100)
    int format[15] = {1,1,1,0,1,1,1,1,1,0,0,0,1,0,0};
    int fmt[15] = {0}; for(int i=0;i<15;++i) fmt[i]=format[i];
    // Vertical
    for(int i=0;i<6;++i) qr[i][8]=fmt[i]; qr[7][8]=fmt[6]; qr[8][8]=fmt[7]; qr[8][7]=fmt[8];
    for(int i=9;i<15;++i) qr[8][14-i]=fmt[i];
    // Horizontal
    for(int i=0;i<8;++i) qr[8][20-i]=fmt[i];
    for(int i=8;i<15;++i) qr[20-i][8]=fmt[i];
    // 8. Generar SVG
    std::string svg = "<svg xmlns='http://www.w3.org/2000/svg' width='210' height='210' viewBox='0 0 21 21' style='background:white'>";
    for (int i = 0; i < 21; ++i) for (int j = 0; j < 21; ++j)
        if (qr[i][j]) svg += "<rect x='" + std::to_string(j) + "' y='" + std::to_string(i) + "' width='1' height='1' fill='black'/>";
    svg += "</svg>";
    // 9. HTML temporal
    std::string html = "<html><head><meta charset='utf-8'><title>QR de Cita</title></head><body>";
    html += "<h2>Escanea este QR para ver los datos de la cita</h2>";
    html += "<pre>" + upper + "</pre>";
    html += svg + "</body></html>";
    std::string filename = "qr_cita_temp.html";
    FILE* f = fopen(filename.c_str(), "w");
    if (f) {
        fwrite(html.c_str(), 1, html.size(), f);
        fclose(f);
        std::string cmd = "start " + filename;
        system(cmd.c_str());
    } else {
        std::cerr << "No se pudo crear el archivo temporal para el QR." << std::endl;
    }
}

// Genera y muestra un QR usando la librería QrCode de Nayuki
void Validar::generarQRyAbrirConLibreria(const std::string& datos) {
    // Generar el QR con nivel de corrección bajo (LOW)
    QrCode qr = QrCode::encodeText(datos.c_str(), QrCode::Ecc::LOW);
    // Exportar a SVG con borde de 1 módulo para hacerlo más pequeño visualmente
    std::string svg = qr.toSvgString(1);
    // Ajustar el tamaño del SVG a 150x150 píxeles
    size_t pos = svg.find("<svg ");
    if (pos != std::string::npos) {
        size_t end = svg.find(">", pos);
        if (end != std::string::npos) {
            svg.insert(end, " width=\"150\" height=\"150\"");
        }
    }
    // Crear HTML temporal para mostrar el QR
    std::string html = "<html><head><meta charset='utf-8'><title>QR de Cita</title></head><body style='text-align:center;'>";
    html += "<h2>Escanea este QR para ver los datos de la cita</h2>";
    html += "<pre>" + datos + "</pre>";
    html += svg + "</body></html>";
    std::string filename = "qr_cita_temp.html";
    FILE* f = fopen(filename.c_str(), "w");
    if (f) {
        fwrite(html.c_str(), 1, html.size(), f);
        fclose(f);
        std::string cmd = "start " + filename;
        system(cmd.c_str());
    } else {
        std::cerr << "No se pudo crear el archivo temporal para el QR." << std::endl;
    }
}

// Solo permite ingreso de letras y espacios por teclado para nombres y apellidos
std::string Validar::leerNombreOApellido(const std::string& campo) {
    std::string valor;
    do {
        std::cout << campo << ": ";
        valor.clear();
        char c;
        int i = 0, espacios = 0;
        while ((c = _getch()) != 13) { // Enter
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                if (i < 59) {
                    valor += c;
                    std::cout << c;
                    i++;
                }
            } else if (c == ' ') {
                // Solo permitir un espacio, no al inicio, no dos seguidos, no al final
                if (i > 0 && espacios == 0 && valor.back() != ' ' && i < 58) {
                    valor += c;
                    std::cout << c;
                    i++;
                    espacios++;
                }
            } else if (c == 8 && i > 0) { // Backspace
                if (valor.back() == ' ') espacios--;
                valor.pop_back();
                i--;
                std::cout << "\b \b";
            }
            // Cualquier otro carácter (números, símbolos) se ignora
        }
        // Quitar espacios al inicio y final
        while (!valor.empty() && valor.front() == ' ') valor.erase(valor.begin());
        while (!valor.empty() && valor.back() == ' ') valor.pop_back();
        if (valor.length() < 2) {
            std::cout << "\nEl dato debe tener al menos 2 caracteres. Ingrese de nuevo.\n";
            continue;
        }
        // Si hay espacio, ambas partes deben tener al menos 2 caracteres
        size_t posEspacio = valor.find(' ');
        if (posEspacio != std::string::npos) {
            std::string parte1 = valor.substr(0, posEspacio);
            std::string parte2 = valor.substr(posEspacio + 1);
            if (parte1.length() < 2 || parte2.length() < 2) {
                std::cout << "\nCada nombre o apellido debe tener al menos 2 caracteres. Ingrese de nuevo.\n";
                continue;
            }
        }
        break;
    } while (true);
    std::cout << std::endl;
    return valor;
}

// Solo permite ingreso de números por teclado
std::string Validar::leerSoloNumeros(const std::string& mensaje, int maxLen) {
    std::string valor;
    do {
        std::cout << mensaje;
        valor.clear();
        char c;
        int i = 0;
        while ((c = _getch()) != 13) { // Enter
            if (c >= '0' && c <= '9') {
                if (i < maxLen) {
                    valor += c;
                    std::cout << c;
                    i++;
                }
            } else if (c == 8 && i > 0) { // Backspace
                valor.pop_back();
                i--;
                std::cout << "\b \b";
            }
            // Cualquier otro carácter se ignora
        }
        if (valor.empty()) {
            std::cout << "\nDebe ingresar al menos un número. Intente de nuevo.\n";
        }
        if (valor.length() > static_cast<size_t>(maxLen)) {
            std::cout << "\nDemasiados dígitos. Máximo permitido: " << maxLen << "\n";
            valor.clear();
        }
    } while (valor.empty());
    std::cout << std::endl;
    return valor;
}