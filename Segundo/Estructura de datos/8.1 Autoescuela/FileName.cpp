#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include<unordered_map>
#include<set>

using namespace std;

using Estudiante = string;
using Profesor = string;
using Nota = int;


class AutoEscuela {

private:
	struct EstudianteInfo {
		Profesor profe;
		Nota nota;
	};

	unordered_map<Estudiante, EstudianteInfo> estudiantes;
	unordered_map<Profesor, set<Estudiante>> profesores;

public:

	/*COSTE : O(logE) donde E es el nº de Estudiantes */
	void alta(const Estudiante& estudiante, const Profesor& profesor) {
		if (estudiantes.find(estudiante) == estudiantes.end()) { //O(1)
			estudiantes[estudiante] = { profesor,0 }; //O(1)
			profesores[profesor].insert(estudiante); //O(logE) logarítmico respecto nº estudiantes, por el insert
		}
		else {
			if (estudiantes[estudiante].profe != profesor) {
				Profesor antiguo = estudiantes[estudiante].profe;
				profesores[antiguo].erase(estudiante); //O(logE) por el erase
				estudiantes[estudiante].profe = profesor;
				profesores[profesor].insert(estudiante);
			}
		}
	}

	// O(1)
	bool es_alumno(const Estudiante& estudiante, const Profesor& profesor) {
		if (estudiantes.find(estudiante) == estudiantes.end()) { return false; }
		//estudiante si está en la bd
		return estudiantes[estudiante].profe == profesor; //O(1) pq es unordered_map
	}

	//O(1)
	int puntuacion(const Estudiante& estudiante) {
		if (estudiantes.find(estudiante) == estudiantes.end()) throw std::domain_error("ESTUDIANTE NO ENCONTRADO");
		return estudiantes.at(estudiante).nota;
	}

	
	void actualizar(const Estudiante const& estudiante, int n) {
	}

	//O(E)
	vector<Estudiante> examen(const Profesor& profesor, int n) {
		vector<Estudiante> a_examen(profesores[profesor].size()); 
		for (auto alumno : profesores[profesor]) { //E vueltas
			if (estudiantes[alumno].nota >= n) {
				a_examen.push_back(alumno); //O(E) En el caso peor es lineal (si tengo q redimensionar el vector) y el coste del bucle sería O(E*E) = O(E^2)
				//Si quiero asegurarme de no redimensionar, declaro el vector con espacio suficiente
			}
		} //O(E*1)= O(E)
		return a_examen;
	}

	//O(logE)
	void aprobar(const Estudiante& a) {
		profesores[estudiantes[a].profe].erase(a); //O(logE)
		estudiantes.erase(a); //O(1)
	}
};


bool tratar_caso() {
	AutoEscuela autoescuela;
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