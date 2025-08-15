#include "case1.h"
#include "Validar.h"
#include "Utilidades.h"
#include <iostream>
#include <fstream>
#include <random>
#include <set>
#include <map>
#include <thread>
#include <algorithm>
#include <filesystem>
#include <conio.h>
#include <windows.h>


using namespace std;

// Definición explícita para evitar problemas de linkage
void generarTurno(
    std::string& rutaBase,
    ListaSimple<Persona>& listaPacientes,
    ListaSimple<Especialista>& listaEspecialistas,
    ListaDC<Evento>& listaTurnos,
    std::map<std::string, std::vector<Especialista>>& especialistasPorHospital,
    int& contadorTurno
)
{
    // 1. Captura y validación de datos del paciente
    std::string nombre = Validar::leerNombreOApellido("Nombre");
    std::string apellido = Validar::leerNombreOApellido("Apellido");
    std::string cedula;
    Persona* pacienteExistente = nullptr;
    Especialista* especialistaCoincide = nullptr;
    do {
        cedula = Validar::leerSoloNumeros("Cedula: ", 11);
        if (!Validar::validarCedula(cedula)) {
            std::cout << "Cedula ecuatoriana invalida. Intente de nuevo.\n";
            cedula.clear();
            continue;
        }
        pacienteExistente = listaPacientes.buscar([&](const Persona& p) {
            return p.getNombre() == nombre && p.getApellido() == apellido && p.getCedula() == cedula;
        });
        if (pacienteExistente) break;
        bool existeCedulaPaciente = listaPacientes.existe([&](const Persona& p) {
            return p.getCedula() == cedula;
        });
        if (existeCedulaPaciente) {
            std::cout << "La cedula ya esta registrada con otro nombre o apellido. Intente con otra.\n";
            cedula.clear();
            continue;
        }
        especialistaCoincide = listaEspecialistas.buscar([&](const Especialista& e) {
            return e.getCedula() == cedula;
        });
        if (especialistaCoincide) {
            if (especialistaCoincide->getNombre() != nombre || especialistaCoincide->getApellido() != apellido) {
                std::cout << "La cedula pertenece a un especialista, pero el nombre o apellido no coincide. No se puede registrar.\n";
                cedula.clear();
                continue;
            }
            break;
        }
        break;
    } while (cedula.empty());

    std::string correo, telefono;
    Fecha fnac;
    int idPaciente;
    Fecha fechaActual = obtenerFechaActualSistema();
    if (pacienteExistente) {
        correo = pacienteExistente->getCorreo();
        telefono = pacienteExistente->getTelefono();
        fnac = pacienteExistente->getFechaNacimiento();
        idPaciente = pacienteExistente->getId();
    } else {
        do {
            std::cout << "Correo: ";
            std::getline(std::cin, correo);
            if (!Validar::validarCorreo(correo)) {
                std::cout << "Correo invalido. Debe tener formato usuario@dominio.ext\n";
            }
        } while (!Validar::validarCorreo(correo));
        telefono = Validar::leerSoloNumeros("Telefono: ", 16);
        do {
            fnac = Validar::leerFecha("Fecha de nacimiento");
            if (fnac > fechaActual) {
                std::cout << "La fecha de nacimiento no puede ser futura respecto a la fecha actual del sistema.\n";
            } else if (!Validar::validarEdadMaxima(fnac, fechaActual, 115)) {
                std::cout << "La edad del paciente no puede ser mayor a 115 años respecto a la fecha actual.\n";
            }
        } while (fnac > fechaActual || !Validar::validarEdadMaxima(fnac, fechaActual, 115));
        Especialista* especialistaCoincideParaPaciente = listaEspecialistas.buscar([&](const Especialista& e) {
            return e.getCedula() == cedula && e.getNombre() == nombre && e.getApellido() == apellido;
        });
        if (especialistaCoincideParaPaciente) {
            idPaciente = especialistaCoincideParaPaciente->getId();
        } else {
            idPaciente = generarIDUnico();
        }
        listaPacientes.agregar(Persona(idPaciente, nombre, apellido, cedula, correo, telefono, fnac));
    }

    // 2. Selección de hospital
    std::string direccionHospital = "";
    bool direccionAceptada = false;
    std::string provincia;
    do {
        provincia = Validar::soloLetras("Ingrese la provincia para buscar hospitales: ");
        std::string provinciaLower = provincia;
        std::transform(provinciaLower.begin(), provinciaLower.end(), provinciaLower.begin(), ::tolower);
        if (provinciaLower != "pichincha") {
            std::cout << "Solo se permite en Pichincha.\n";
        }
    } while (provincia != "Pichincha" && provincia != "pichincha");
    std::string comando = "python python.py";
    system(comando.c_str());
    // Leer hospitales.txt y mostrar opciones al usuario
    std::ifstream hospitalesFile("hospitales.txt");
    std::vector<std::string> hospitales;
    std::string lineaHospital;
    while (std::getline(hospitalesFile, lineaHospital)) {
        std::string lineaLower = lineaHospital;
        std::transform(lineaLower.begin(), lineaLower.end(), lineaLower.begin(), ::tolower);
        if (lineaLower.find("pichincha") != std::string::npos && lineaHospital.find('|') != std::string::npos) {
            hospitales.push_back(lineaHospital);
        }
    }
    hospitalesFile.close();


    // --- FUNCIONES AUXILIARES PARA GESTIÓN DE ESPECIALISTAS ---

    // Lambda para cargar especialistas desde archivo
    auto cargarEspecialistasDesdeArchivo = [&](const std::string& archivo, ListaSimple<Especialista>& lista, std::map<std::string, std::vector<Especialista>>& mapa) {
        std::ifstream file(archivo);
        if (!file.is_open()) return;
        std::string linea;
        std::set<int> idsGlobales;
        while (std::getline(file, linea)) {
            if (linea.empty()) continue;
            std::istringstream iss(linea);
            std::string idStr, nombre, apellido, cedula, especialidad, hospital;
            if (std::getline(iss, idStr, '|') && std::getline(iss, nombre, '|') && std::getline(iss, apellido, '|') &&
                std::getline(iss, cedula, '|') && std::getline(iss, especialidad, '|') && std::getline(iss, hospital)) {
                int id = 0;
                try { id = std::stoi(idStr); } catch (...) { continue; }
                if (id <= 0 || idsGlobales.count(id)) continue; // ID único global
                Especialista esp(id, nombre, apellido, cedula, especialidad);
                lista.agregar(esp);
                mapa[hospital].push_back(esp);
                idsGlobales.insert(id);
            }
        }
        file.close();
    };

    // Lambda para guardar especialistas en archivo
    auto guardarEspecialistasEnArchivo = [&](const std::string& archivo, const std::map<std::string, std::vector<Especialista>>& mapa) {
        std::ofstream file(archivo, std::ios::trunc);
        for (const auto& par : mapa) {
            for (const auto& esp : par.second) {
                file << esp.getId() << '|' << esp.getNombre() << '|' << esp.getApellido() << '|' << esp.getCedula() << '|' << esp.getEspecialidad() << '|' << par.first << '\n';
            }
        }
        file.close();
    };

    // Lambda para generar 5 especialistas únicos para un hospital
    auto generarEspecialistasParaHospital = [&](const std::string& hospital, std::set<int>& idsUsadosGlobal, ListaSimple<Especialista>& lista, std::vector<Especialista>& especialistas) {
        std::set<std::string> cedulasUsadas, nombresUsados, apellidosUsados, especialidadesUsadas;
        std::vector<std::string> nombres = {"Juan", "Ana", "Luis", "Maria", "Pedro", "Sofia", "Carlos", "Lucia", "Miguel", "Valeria"};
        std::vector<std::string> apellidos = {"Perez", "Lopez", "Martinez", "Gomez", "Torres", "Ramirez", "Vega", "Castillo", "Mendoza", "Cruz"};
        std::vector<std::string> especialidades = {"Cardiologia", "Pediatria", "Dermatologia", "Neurologia", "Traumatologia", "Ginecologia", "Oftalmologia", "Urologia", "Oncologia", "Psiquiatria"};
        std::random_device rd;
        std::mt19937 gen(rd());
        for (size_t i = 0; i < 5; ++i) {
            int id;
            do {
                id = 170000 + gen() % 100000;
            } while (idsUsadosGlobal.count(id));
            idsUsadosGlobal.insert(id);
            std::string nombre, apellido, cedula, especialidad;
            do { nombre = nombres[gen() % nombres.size()]; } while (nombresUsados.count(nombre));
            nombresUsados.insert(nombre);
            do { apellido = apellidos[gen() % apellidos.size()]; } while (apellidosUsados.count(apellido));
            apellidosUsados.insert(apellido);
            do { cedula = std::to_string(1000000000 + gen() % 900000000); } while (cedulasUsadas.count(cedula));
            cedulasUsadas.insert(cedula);
            do { especialidad = especialidades[gen() % especialidades.size()]; } while (especialidadesUsadas.count(especialidad));
            especialidadesUsadas.insert(especialidad);
            Especialista nuevoEsp(id, nombre, apellido, cedula, especialidad);
            especialistas.push_back(nuevoEsp);
            lista.agregar(nuevoEsp);
        }
    };

    // --- FLUJO PRINCIPAL DE GESTIÓN DE ESPECIALISTAS ---

    // 1. Cargar especialistas desde archivo si existen
    cargarEspecialistasDesdeArchivo(rutaBase + "especialistas.txt", listaEspecialistas, especialistasPorHospital);

    // 2. Generar especialistas solo para hospitales que no tengan
    std::set<int> idsUsadosGlobal;
    for (const auto& par : especialistasPorHospital) {
        for (const auto& esp : par.second) {
            idsUsadosGlobal.insert(esp.getId());
        }
    }
    for (const auto& hospital : hospitales) {
        // Si el hospital no tiene especialistas, generarlos
        if (especialistasPorHospital[hospital].size() < 5) {
            std::vector<Especialista> nuevos;
            generarEspecialistasParaHospital(hospital, idsUsadosGlobal, listaEspecialistas, nuevos);
            especialistasPorHospital[hospital].insert(especialistasPorHospital[hospital].end(), nuevos.begin(), nuevos.end());
        }
        // Limitar a solo 5 especialistas por hospital (por si el archivo tenía más)
        if (especialistasPorHospital[hospital].size() > 5) {
            especialistasPorHospital[hospital].resize(5);
        }
    }

    // 3. Guardar todos los especialistas en archivo después de generar
    guardarEspecialistasEnArchivo(rutaBase + "especialistas.txt", especialistasPorHospital);

    // 4. Selección de hospital por usuario
    while (!direccionAceptada && !hospitales.empty()) {
        std::cout << "Seleccione el hospital (ingrese el número):\n";
        for (size_t idx = 0; idx < hospitales.size(); ++idx) {
            size_t pos1 = hospitales[idx].find('|');
            size_t pos2 = hospitales[idx].find('|', pos1 + 1);
            std::string nombre = hospitales[idx].substr(pos1 + 1, pos2 - pos1 - 1);
            std::cout << (idx+1) << ". " << nombre << std::endl;
        }
        int seleccion = 0;
        do {
            std::cout << "Número de hospital: ";
            seleccion = Validar::leerEnteroSeguro("");
        } while (seleccion < 1 || seleccion > hospitales.size());
        direccionHospital = hospitales[seleccion - 1];
        // Extraer latitud y longitud
        size_t posLat = direccionHospital.rfind('|', direccionHospital.rfind('|') - 1);
        size_t posLon = direccionHospital.rfind('|');
        std::string lat = direccionHospital.substr(posLat + 1, posLon - posLat - 1);
        std::string lon = direccionHospital.substr(posLon + 1);
        std::string url = "https://www.google.com/maps/dir/?api=1&destination=" + lat + "," + lon;
        ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, 1);
        std::cout << "¿Aceptar esta dirección y continuar con el registro? (s/n): ";
        char respuesta;
        do {
            respuesta = _getch();
        } while (respuesta != 's' && respuesta != 'n');
        std::cout << respuesta << std::endl;
        if (respuesta == 's') {
            direccionAceptada = true;
        } else {
            std::cout << "Puede seleccionar otro hospital.\n";
        }
    }
    if (hospitales.empty()) {
        std::cout << "No se encontraron hospitales en esa provincia.\n";
        return;
    }

    // 5. Selección de especialista y generación de turno
    std::string hospitalSeleccionado = direccionHospital;
    // Mostrar solo los 5 especialistas del hospital seleccionado
    std::cout << "\nLista de especialistas disponibles en el hospital seleccionado:\n";
    const auto& especialistasHospital = especialistasPorHospital[hospitalSeleccionado];
    for (const auto& esp : especialistasHospital) {
        std::cout << esp.getId() << "\t" << esp.getNombre() << "\t" << esp.getApellido() << "\t" << esp.getEspecialidad() << std::endl;
    }
    int idEsp;
    Especialista* esp = nullptr;
    bool especialistaDisponible = false;
    Fecha fatencion;
    Hora hatencion;
    DiasFestivos diasFestivos(fechaActual.getAnio());
    do {
        idEsp = Validar::leerEnteroSeguro("Ingrese ID del especialista: ");
        // Buscar solo entre los especialistas del hospital seleccionado
        auto it = std::find_if(especialistasHospital.begin(), especialistasHospital.end(), [&](const Especialista& e) {
            return e.getId() == idEsp && !(e.getNombre() == nombre && e.getApellido() == apellido);
        });
        if (it == especialistasHospital.end()) {
            std::cout << "ID de especialista no valido o corresponde al mismo nombre y apellido del paciente. Intente de nuevo.\n";
            continue;
        }
        esp = const_cast<Especialista*>(&(*it));
        especialistaDisponible = true;
        bool turnoGenerado = false;
        do {
            fatencion = Validar::leerFecha("Fecha de atencion");
            std::string diaSemana = diasFestivos.obtenerDiaSemana(fatencion);
            if (fatencion < fnac) {
                std::cout << "La fecha de atencion no puede ser antes de la fecha de nacimiento.\n";
            }
            else if (fatencion < fechaActual) {
                std::cout << "La fecha de atencion no puede ser anterior a la fecha actual del sistema.\n";
            }
            else if (!Validar::validarEdadMaximaCita(fnac, fatencion, 115)) {
                std::cout << "La edad del paciente en la fecha de la cita no puede ser mayor a 115 años.\n";
            }
            else if (
                (fatencion.getDia() == 1 && fatencion.getMes() == 1) ||
                (fatencion.getMes() == 12 &&
                    (fatencion.getDia() == 23 || fatencion.getDia() == 24 ||
                    fatencion.getDia() == 25 || fatencion.getDia() == 31)) ||
                (fatencion.getMes() == 11 &&
                    (fatencion.getDia() == 2 || fatencion.getDia() == 3 || fatencion.getDia() == 4))
            ) {
                std::cout << "No se puede agendar citas en esta fecha especial (no laborable). Intente con otra fecha.\n";
            }
            else if (diasFestivos.esFeriado(fatencion)) {
                std::cout << "No se puede agendar citas en dias feriados. Intente con otra fecha.\n";
            }
            else if (diaSemana == "Sabado" || diaSemana == "Domingo") {
                std::cout << "No se puede agendar citas en sabado ni domingo. Intente con otra fecha.\n";
            }
            else {
                int hh, mm;
                do {
                    hh = Validar::leerEnteroSeguro("Hora (9-18): ");
                } while (hh < 9 || hh > 17);
                do {
                    mm = Validar::leerEnteroSeguro("Minuto (0-59): ");
                } while (mm < 0 || mm > 59);
                Hora hatencion(hh, mm);
                if (!hatencion.esValida()) {
                    std::cout << "Hora invalida.\n";
                    continue;
                }
				bool pacienteYaTieneTurno = listaTurnos.existe([&](const Evento& ev) {
    			bool mismaFecha = ev.getFechaAtencion().getDia() == fatencion.getDia() &&
        		ev.getFechaAtencion().getMes() == fatencion.getMes() &&
        		ev.getFechaAtencion().getAnio() == fatencion.getAnio();
    			bool mismoPaciente = ev.getPaciente().getCedula() == cedula;
    			if (mismaFecha && mismoPaciente) {
        			int minutosEv = ev.getHoraAtencion().getHora() * 60 + ev.getHoraAtencion().getMinuto();
        			int minutosNuevo = hatencion.getHora() * 60 + hatencion.getMinuto();
        // Si la diferencia es menor a 60 minutos, está en conflicto
        			return std::abs(minutosEv - minutosNuevo) < 60;
    			}
    			return false;
				});
				if (pacienteYaTieneTurno) {
    				std::cout << "El paciente ya tiene una cita en esa fecha dentro de 1 hora de diferencia. Solo puede agendar fuera de ese rango.\n";
    				continue;
				}
                bool personaOcupada = listaTurnos.existe([&](const Evento& ev) {
                    bool mismaFecha = ev.getFechaAtencion().getDia() == fatencion.getDia() &&
                        ev.getFechaAtencion().getMes() == fatencion.getMes() &&
                        ev.getFechaAtencion().getAnio() == fatencion.getAnio();
                    bool mismaHora = ev.getHoraAtencion().getHora() == hatencion.getHora() &&
                        ev.getHoraAtencion().getMinuto() == hatencion.getMinuto();
                    bool esMismaPersona = (ev.getPaciente().getCedula() == cedula) || (ev.getEspecialista().getCedula() == cedula);
                    return mismaFecha && mismaHora && esMismaPersona;
                });
                if (personaOcupada) {
                    std::cout << "Esta persona ya tiene un turno (como paciente o como especialista) en esa fecha y hora.\n";
                    continue;
                }
                bool ocupado = listaTurnos.existe([&](const Evento& ev) {
                    if (ev.getEspecialista().getId() != idEsp) return false;
                    if (ev.getFechaAtencion().getDia() != fatencion.getDia() ||
                        ev.getFechaAtencion().getMes() != fatencion.getMes() ||
                        ev.getFechaAtencion().getAnio() != fatencion.getAnio()) return false;
                    int minutosEv = ev.getHoraAtencion().getHora() * 60 + ev.getHoraAtencion().getMinuto();
                    int minutosNuevo = hatencion.getHora() * 60 + hatencion.getMinuto();
                    return std::abs(minutosEv - minutosNuevo) < 60;
                });
                if (ocupado) {
                    std::cout << "El especialista ya tiene un turno en esa fecha y hora.\n";
                    continue;
                }
                bool existeTurno = listaTurnos.existe([&](const Evento& ev) {
                    return ev.getPaciente().getId() == idPaciente &&
                        ev.getEspecialista().getId() == idEsp &&
                        ev.getFechaAtencion().getDia() == fatencion.getDia() &&
                        ev.getFechaAtencion().getMes() == fatencion.getMes() &&
                        ev.getFechaAtencion().getAnio() == fatencion.getAnio() &&
                        ev.getHoraAtencion().getHora() == hatencion.getHora() &&
                        ev.getHoraAtencion().getMinuto() == hatencion.getMinuto();
                });
                if (existeTurno) {
                    std::cout << "El paciente ya tiene un turno con este especialista en esa fecha y hora.\n";
                    continue;
                }
                // --- NUEVO: ID de turno secuencial ---
                int idTurno = contadorTurno++;
                // Obtener timestamp actual en formato ISO8601
                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                std::tm tm_now;
#ifdef _WIN32
                localtime_s(&tm_now, &now_c);
#else
                localtime_r(&now_c, &tm_now);
#endif
                char buf[32];
                std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &tm_now);
                std::string lastModified(buf);
                Evento nuevoTurno(idTurno, Persona(idPaciente, nombre, apellido, cedula, correo, telefono, fnac), *esp, fatencion, hatencion, direccionHospital, lastModified);
                listaTurnos.agregar(nuevoTurno);
                std::cout << "Turno generado exitosamente. ID de turno: " << idTurno << std::endl;
                // Mostrar QR al generar la cita (ahora escaneable)
                std::stringstream datosQR;
                datosQR << "Turno ID: " << idTurno << " ";
                datosQR << "Paciente: " << nombre << " " << apellido << " ";
                datosQR << "Cedula: " << cedula << " ";
                datosQR << "Especialista: " << esp->getNombre() << " " << esp->getApellido() << " ";
                datosQR << "Especialidad: " << esp->getEspecialidad() << " ";
                datosQR << "Fecha: " << fatencion.getDia() << "/" << fatencion.getMes() << "/" << fatencion.getAnio() << " ";
                datosQR << "Hora: " << (hatencion.getHora() < 10 ? "0" : "") << hatencion.getHora() << ":" << (hatencion.getMinuto() < 10 ? "0" : "") << hatencion.getMinuto() << " ";
                datosQR << "Hospital: " << direccionHospital << " ";
                datosQR << "Direccion: " << direccionHospital << " ";
                Validar vqr; vqr.generarQRyAbrirConLibreria(datosQR.str());
                // Guardar solo el QR como PDF usando Python
                std::string nombrePDF = rutaBase + "QR_Turno_" + std::to_string(idTurno) + ".pdf";
                std::string comandoPDF = "python generar_qr_pdf.py \"" + datosQR.str() + "\" \"" + nombrePDF + "\"";
                system(comandoPDF.c_str());
                std::cout << "QR guardado como PDF: " << nombrePDF << std::endl;
                guardarModuloCitas(rutaBase);
                // --- Insertar turno en MongoDB con los mismos datos del QR ---
                std::stringstream comandoMongo;
                comandoMongo << "python Conexion_MongoDB/insertar_turno_mongo.py "
                    << idTurno << " "
                    << '"' << nombre << '"' << " "
                    << '"' << apellido << '"' << " "
                    << '"' << cedula << '"' << " "
                    << '"' << correo << '"' << " "
                    << '"' << telefono << '"' << " "
                    << '"' << esp->getNombre() << '"' << " "
                    << '"' << esp->getApellido() << '"' << " "
                    << '"' << esp->getEspecialidad() << '"' << " "
                    << '"' << fatencion.getDia() << "/" << fatencion.getMes() << "/" << fatencion.getAnio() << '"' << " "
                    << '"' << (hatencion.getHora() < 10 ? "0" : "") << hatencion.getHora() << ":" << (hatencion.getMinuto() < 10 ? "0" : "") << hatencion.getMinuto() << '"' << " "
                    << '"' << direccionHospital << '"';
                int resMongo = system(comandoMongo.str().c_str());
                if (resMongo != 0) {
                    std::cout << "\nERROR: No se pudo guardar el turno en MongoDB.\n";
                    std::cout << "El turno solo se guardó localmente.\n";
                } else {
                    std::cout << "Turno sincronizado con la nube.\n";
                }
                system("pause");
                turnoGenerado = true;
            }
        } while (!turnoGenerado);
    } while (!especialistaDisponible);
}
