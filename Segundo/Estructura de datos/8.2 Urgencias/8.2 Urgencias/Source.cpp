#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <unordered_set>
#include <set>

using namespace std;

using Paciente = string;
using Medico = string;
using Gravedad = int;


class Urgencias {

private:
	struct InfoPaciente {
		Paciente paciente;
		Gravedad gravedad;
	};
	unordered_map<Paciente,Gravedad> pacientes;
	list<Paciente> espera;
	set<Paciente> recuperados;

public:


	void nuevo_paciente(const Paciente& paciente, const Gravedad& gravedad) {
		if (pacientes.find(paciente) != pacientes.end()) throw std::domain_error("Paciente repetido");
		else if(gravedad>3 || gravedad<1) throw std::domain_error("Gravedad incorrecta");
		pacientes[paciente] = { gravedad };
		
		//espera.push_back({paciente,gravedad});
		espera.push_back(paciente);
	}

	Gravedad gravedad_actual(const Paciente& paciente) {
		if(pacientes.find(paciente)==pacientes.end()) throw std::domain_error("Paciente inexistente");
		return pacientes.at(paciente);
	}

	Paciente siguiente() {
		if(pacientes.empty()) throw std::domain_error("No hay pacientes");

		Paciente sol;
		bool encontrado = false;
		bool max_prio = false;
		for (auto p:espera) {
			if (!max_prio && pacientes.at(p)==3) {
				encontrado = true;
				max_prio = true;
				sol = p;
			}
			else if (!encontrado && pacientes.at(p) == 2) {
				encontrado = true;
				sol = p;
			}
			else {
				sol = p;
			}
		}
		return sol;
	}

	void mejora(Paciente paciente) {
		if (pacientes.find(paciente) == pacientes.end()) throw std::domain_error("Paciente inexistente");
		if (pacientes.at(paciente) == 3) pacientes[paciente] = 2;
		else if (pacientes.at(paciente) == 2) pacientes[paciente] = 1;
		else {
			pacientes.erase(paciente);
			list<Paciente>::const_iterator it = pacientes.at(paciente);
			espera.erase(it);
			recuperados.insert(paciente);
		}
	}


	vector<Paciente> recuperados() {
		vector<Paciente> sol;
		for (auto p : recuperados) {
			sol.push_back(p);
		}
		return sol;
	}

};


bool tratar_caso() {
	Urgencias autoescuela;
	string linea;

	getline(cin, linea);

	if (!std::cin)
		return false;

	while (linea != "FIN") {

		stringstream ss(linea);
		string operacion;
		ss >> operacion;

		if (operacion == "alta") {
			string a, p;
			ss >> a >> p;
			try {
				autoescuela.alta(a, p);
			}
			catch (domain_error& de) {
				cout << de.what() << endl;
			}
		}
		else if (operacion == "es_alumno") {
			string a, p;
			ss >> a >> p;
			try {
				if (autoescuela.es_alumno(a, p)) {
					cout << a << " es alumno de " << p << endl;
				}
				else {
					cout << a << " no es alumno de " << p << endl;
				}
			}
			catch (domain_error& de) {
				cout << de.what() << endl;
			}
		}
		else if (operacion == "puntuacion") {
			string a; int p;
			ss >> a;
			try {
				p = autoescuela.puntuacion(a);
				cout << "Puntuacion de " << a << ": " << p << endl;
			}
			catch (domain_error& de) {
				cout << de.what() << endl;
			}
		}
		else if (operacion == "actualizar") {
			string a; int n;
			ss >> a >> n;
			try {
				autoescuela.actualizar(a, n);
			}
			catch (domain_error& de) {
				cout << de.what() << endl;
			}
		}
		else if (operacion == "examen") {
			string p;  int n;
			ss >> p >> n;
			try {
				vector<string> l = autoescuela.examen(p, n);
				cout << "Alumnos de " << p << " a examen: " << endl;
				for (size_t i = 0; i < l.size(); i++) {
					cout << l[i] << endl;
				}
			}
			catch (domain_error& de) {
				cout << de.what() << endl;
			}
		}
		else if (operacion == "aprobar") {
			string a;
			ss >> a;
			try {
				autoescuela.aprobar(a);
			}
			catch (domain_error& de) {
				cout << de.what() << endl;
			}
		}
		getline(cin, linea);
	}

	cout << "---" << endl;

	return true;
}



int main() {


#ifndef DOMJUDGE
	std::ifstream in("datos.in");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

	while (tratar_caso());

#ifndef DOMJUDGE
	std::cin.rdbuf(cinbuf);
#endif

}