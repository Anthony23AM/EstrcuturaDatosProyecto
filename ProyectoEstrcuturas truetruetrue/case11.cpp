#include "case11.h"
#include "globals.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class HashTable {
	static const int SIZE = 10;
	vector<string> table[SIZE];
public:
	int hash(const string& key) {
		int sum = 0;
		for (char c : key) sum += c;
		return sum % SIZE;
	}
	void insert(const string& key) {
		int idx = hash(key);
		table[idx].push_back(key);
	}
	// Nuevo método para obtener cédulas por hash
	const vector<string>& getByHash(int hashValue) const {
		if (hashValue >= 0 && hashValue < SIZE)
			return table[hashValue];
		static vector<string> vacio;
		return vacio;
	}
	bool search(const string& key) {
		int idx = hash(key);
		for (const string& val : table[idx]) {
			if (val == key) return true;
		}
		return false;
	}
	void display() {
		for (int i = 0; i < SIZE; ++i) {
			cout << i << ": ";
			for (const string& val : table[i]) cout << val << " ";
			cout << endl;
		}
	}
};

void case11() {
    static HashTable hashPacientes;
    int opHash;
    string nombreHash;

    // Llenar el hash con las cédulas de todos los pacientes al iniciar
    listaPacientes.mostrar([&](const Persona& p) {
        hashPacientes.insert(p.getCedula());
    });

	// Lambda para insertar paciente
	auto insertarPaciente = [&]() {
		cout << "Ingrese el nombre del paciente: ";
		getline(cin, nombreHash);
		hashPacientes.insert(nombreHash);
		cout << "Paciente insertado." << endl;
	};

	// Lambda para mostrar la tabla hash
	auto mostrarTabla = [&]() {
		cout << "\nTabla hash actual:\n";
		hashPacientes.display();
	};

	// Lambda para buscar hash por cédula
	auto buscarHashPorCedula = [&]() {
		string cedula;
		cout << "Ingrese la cédula del paciente: ";
		getline(cin, cedula);
		Persona* p = listaPacientes.buscar([&](const Persona& per) { return per.getCedula() == cedula; });
		if (p) {
			int hashValue = hashPacientes.hash(cedula);
			cout << "El hash para la cédula " << cedula << " es: " << hashValue << endl;
		} else {
			cout << "Paciente no encontrado." << endl;
		}
	};

	// Lambda para buscar por hash y mostrar citas
	auto buscarPorHashMostrarCitas = [&]() {
		int hashValue;
		cout << "Ingrese el hash del paciente: ";
		cin >> hashValue;
		cin.ignore();
		const vector<string>& posiblesCedulas = hashPacientes.getByHash(hashValue);
		if (posiblesCedulas.empty()) {
			cout << "No hay pacientes con ese hash." << endl;
			return;
		}
		cout << "Cédulas asociadas al hash " << hashValue << ":\n";
		for (size_t i = 0; i < posiblesCedulas.size(); ++i) {
			cout << i+1 << ". " << posiblesCedulas[i] << endl;
		}
		cout << "Seleccione el número de la cédula para ver sus citas: ";
		size_t seleccion;
		cin >> seleccion;
		cin.ignore();
		if (seleccion < 1 || seleccion > posiblesCedulas.size()) {
			cout << "Selección inválida." << endl;
			return;
		}
		string cedula = posiblesCedulas[seleccion-1];
		Persona* p = listaPacientes.buscar([&](const Persona& per) { return per.getCedula() == cedula; });
		if (p) {
			p->mostrar();
			cout << "\n--- Citas del paciente ---\n";
			bool tieneCitas = false;
			listaTurnos.mostrar([&](const Evento& e) {
				if (e.getPaciente().getCedula() == cedula) {
					e.mostrar();
					cout << "-----------------------------\n";
					tieneCitas = true;
				}
			});
			if (!tieneCitas) cout << "No tiene citas registradas.\n";
		} else {
			cout << "No se encontró información adicional del paciente con cédula " << cedula << ".\n";
		}
	};

	do {
		cout << "\n--- Menú de Pacientes (Hash) ---\n";
		cout << "1. Mostrar tabla hash" << endl;
		cout << "2. Buscar hash del paciente usando la cédula" << endl;
		cout << "3. Buscar paciente por hash y mostrar citas" << endl;
		cout << "0. Volver al menú principal" << endl;
		cout << "Seleccione una opción: ";
		cin >> opHash;
		cin.ignore();
		switch (opHash) {
			case 1:
				mostrarTabla();
				break;
			case 2:
				buscarHashPorCedula();
				break;
			case 3:
				buscarPorHashMostrarCitas();
				break;
			case 0:
				cout << "Volviendo al menú principal..." << endl;
				break;
			default:
				cout << "Opción inválida." << endl;
		}
	} while (opHash != 0);
}
