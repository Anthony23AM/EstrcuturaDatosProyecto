#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <conio.h>
#include <windows.h>
#include <thread>
#include <algorithm> // para rotate
#include <set>
#include <map>
#include <filesystem>
#include "Validar.h"
#include "Persona.h"
#include "Especialista.h"
#include "Evento.h"
#include "ListaSimple.h"
#include "Listasdc.h"
#include "Fecha.h"
#include "Hora.h"
#include "DiasFestivos.h"
#include "BPlusTree.h"
#include "Marquesina.h"
#include "CitasIO.h"
#include <string>
#include "Utilidades.h"
#include "case1.h"
#include "case2.h"
#include "case4.h"
#include "case9.h"
#include "case6.h"
#include "case11.h"
using namespace std;

// Mapa global de especialistas por hospital
std::map<std::string, std::vector<Especialista>> especialistasPorHospital;
ListaSimple<Persona> listaPacientes;
ListaSimple<Especialista> listaEspecialistas;
ListaDC<Evento> listaTurnos;
std::string rutaBase;


bool turnosCargados = false;

// --- NUEVO: Contador secuencial para ID de turnos ---
int contadorTurno = 180000;


int main(){
    // Cambiar página de códigos a UTF-8 automáticamente
    system("chcp 65001 > nul");
    // Configurar la consola para UTF-8 (soporte de tildes)
    SetConsoleOutputCP(CP_UTF8);
    // Solicitar ruta base al inicio
    std::string rutaBase;
    std::cout << "Ingrese la ruta base donde se guardarán y cargarán los archivos de citas (ejemplo: C:/miBackup/): ";
    std::getline(std::cin, rutaBase);
    if (!rutaBase.empty() && rutaBase.back() != '/' && rutaBase.back() != '\\') {
        rutaBase += '/';
    }
    // Descargar obligatoriamente los turnos de MongoDB al txt antes de cargar (sincronización inicial)
    std::string comandoDescarga = "python ./Conexion_MongoDB/descargar_turnos_mongo.py \"" + rutaBase + "\"";
    int resDescarga = system(comandoDescarga.c_str());
    if (resDescarga != 0) {
        std::cout << "\nERROR: No se pudo sincronizar con la base de datos MongoDB.\n";
        std::cout << "Verifique su conexión a internet o la configuración de la base de datos.\n";
        std::cout << "No se permite continuar sin datos actualizados.\n";
        system("pause");
        return 1;
    }
    // Cargar especialistas globales al inicio desde la ruta base
    cargarEspecialistas(rutaBase + "especialistas.txt");
    Fecha fechaActual = obtenerFechaActualSistema();
    std::thread th_marquesina(mostrar_marquesina);
    DiasFestivos diasFestivos(fechaActual.getAnio());
    // Verificar si moduloCitas.txt existe en la ruta base
    if (!std::filesystem::exists(rutaBase + "moduloCitas.txt")) {
        guardarModuloCitas(rutaBase);
    } else {
        cargarCitasArchivo(rutaBase + "moduloCitas.txt", rutaBase);
    }
    int opcion;
    do {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        vector<string> opcionesMenu = {
            "Generar turno",//case1
            "Mostrar lista de turnos",//case2
            "Buscar persona",//case3
            "Eliminar Turno",//case4
            "Mostrar lista de especialistas",//case5
            "Cargar datos de turnos",//case6
            "Guardar lista de turnos",//case7
            "Ayuda",//case8
            "Generar QR PDF de cita existente",//case9
            "Arbol de citas",//case10
            "Buscar cita por hash",//case11
            "Chat",//case12
            "Salir"//default
        };
        opcion = menuConFlechas(opcionesMenu);
        switch (opcion) {
            case 1: { // Generar turno
                generarTurno(rutaBase, listaPacientes, listaEspecialistas, listaTurnos, especialistasPorHospital, contadorTurno);
                break;
            }
            case 2:{//mostrar lista de turnos
                case2(rutaBase, fechaActual);
                break;
            }
            case 3: {//Buscar paciente
                // Solo buscar por cédula
                string ced;
                ced = Validar::leerSoloNumeros("Cedula: ", 11);
                Persona* p = listaPacientes.buscar([&](const Persona& p) { return p.getCedula() == ced; });
                if (p) {
                    p->mostrar();
                    cout << "\n--- Citas del paciente ---\n";
                    bool tieneCitas = false;
                    listaTurnos.mostrar([&](const Evento& e) {
                        if (e.getPaciente().getCedula() == ced) {
                            e.mostrar();
                            cout << "-----------------------------\n";
                            tieneCitas = true;
                        }
                    });
                    if (!tieneCitas) cout << "No tiene citas registradas.\n";
                } else {
                    cout << "Paciente no encontrado.\n";
                }
                system("pause");
                break;
            }
            case 4: {
                case4(rutaBase);
                break;
            }
            case 5: {//Mostrar especialistas por hospital
                cout << "\n------------------ ESPECIALISTAS POR HOSPITAL ------------------\n";
                mostrarEspecialistasPorHospital();
                system("pause");
                break;
            }
            case 6: {//Cargar datos en archivos
                case6(rutaBase);
                break;
            }
            case 7: {//Guardar datos en archivo
                std::cout << "¿Desea guardar en otra ruta? (s/n): ";
                char guardarOtraRuta;
                do {
                    guardarOtraRuta = _getch();
                } while (guardarOtraRuta != 's' && guardarOtraRuta != 'n');
                std::cout << guardarOtraRuta << std::endl;
                std::string rutaBackup = rutaBase + obtenerFechaHoraActual() + ".txt";
                if (guardarOtraRuta == 's') {
                    std::cout << "Ingrese la ruta completa donde desea guardar el backup de citas: ";
                    std::getline(std::cin, rutaBackup);
                }
                guardarCitasArchivo(rutaBackup);
                std::cout << "Backup de citas guardado en: " << rutaBackup << std::endl;
                // Sincronizar SIEMPRE con el archivo principal de la ruta base
                {
                    std::string archivoTurnos = rutaBase + "moduloCitas.txt";
                    std::ifstream f(archivoTurnos);
                    if (f && f.peek() != std::ifstream::traits_type::eof()) {
                        system(("python sincronizar_turnos_total.py " + archivoTurnos).c_str());
                    } else {
                        std::cout << "No se sincroniza: el archivo de turnos está vacío o no existe.\n";
                    }
                }
                system("pause");
                break;
            }
            case 8: { // Ayuda
                mostrarGuiaGeneral();
                system("pause");
                break;
            }
            case 9: {
                case9(rutaBase);
                break;
            }
            case 10:{
                // Explicación de uso de BPlusTree
                std::cout << "Al cargar o agregar una cita (Evento), inserta la cédula o el ID en el BPlusTree.\n";
                std::cout << "Cuando quieras buscar citas por cédula, puedes consultar el árbol para saber si existe y luego buscar en la lista.\n\n";

                // Crear los árboles
                BPlusTree arbolCedulas;
                BPlusTree arbolIDs;
                listaTurnos.mostrar([&](const Evento& e) {
                    // Poblar ambos árboles
                    try {
                        arbolCedulas.insertar(std::stoi(e.getPaciente().getCedula()));
                    } catch (...) {
                        // Si la cédula no es numérica, ignorar
                    }
                    arbolIDs.insertar(e.getId());
                });
                while (true) {
                    vector<string> opcionesMA = {
                        "Imprimir árbol de cédulas",
                        "Imprimir árbol de IDs",
                        "Volver al menú principal"
                    };
                    int seleccionMA = menuConFlechas(opcionesMA);
                    if (seleccionMA == 1) {
                        std::cout << "\nÁrbol BPlus de cédulas:\n";
                        arbolCedulas.imprimir();
                        system("pause");
                    } else if (seleccionMA == 2) {
                        std::cout << "\nÁrbol BPlus de IDs:\n";
                        arbolIDs.imprimir();
                        system("pause");
                    } else if (seleccionMA == 3) {
                        break;
                    }
                }
                break;
            }
            case 11:{
                case11();
                break;
            }
            case 12: { // Chat
                std::cout << "Iniciando cliente de chat...\n";
                int resultado = system("start python ./Chat_hamachi/cliente.py");
                if (resultado != 0) {
                    std::cout << "\nERROR: No se pudo iniciar el cliente de chat.\n";
                    std::cout << "Verifique que Python esté instalado y la ruta del script sea correcta.\n";
                }
                system("pause");
                break;
            }
            case 13: // Salir
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción no válida.\n";
                break;
        }
    } while (opcion != 13); // "Salir" es opción 12
    ejecutando = false;
    th_marquesina.join();
    return 0;
}





