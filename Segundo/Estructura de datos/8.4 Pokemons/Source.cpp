#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include<fstream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

using Pokemon = string;
using Entrenador = string;
using Nivel = int;


class CentroPokemon {

private:

	struct InfoEntrenador {
		Pokemon nombre;
		Nivel nivel;
	};

	unordered_map<Entrenador, list<InfoEntrenador>> entrenadores;
public:


	void capturar_pokemon(Entrenador& e, Pokemon& p, Nivel  n) {

		if (entrenadores.find(e) == entrenadores.end()) {
			list<InfoEntrenador> l;
			l.push_back({ p,n });
			entrenadores.insert({ e,l });
		}
		else {
			list<InfoEntrenador> info = entrenadores[e];
			for (InfoEntrenador i : info) {
				if (i.nombre == p) {

					throw std::invalid_argument("Pokemon Repetido");
				}
				else if (i.nivel == n) {
					throw std::invalid_argument("Nivel Repetido");
				}
			}

			entrenadores[e].push_back({ p,n });
		}
	}

	InfoEntrenador  vencer_pokemon(Entrenador& e, Nivel n) const {

		if (entrenadores.find(e) == entrenadores.end()) {
			throw std::invalid_argument("Entrenador no Existente");
		}
		else if (entrenadores.at(e).empty()) {
			throw std::invalid_argument("No tiene Pokemons");
		}
		else {
			list<InfoEntrenador> info = entrenadores.at(e);
			InfoEntrenador mayor_nivel = info.front();
			for (InfoEntrenador i : info) {
				if (i.nivel > mayor_nivel.nivel) {
					mayor_nivel = i;
				}
			}
			if (mayor_nivel.nivel < n) {
				throw std::invalid_argument("El pokemon no puede ser vencido");
			}
			return mayor_nivel;
		}
	}

	int dia_de_liberacion(Pokemon& p) {

		int sol = 0;
		for (auto it : entrenadores) {
			if (it.first == p) {
				sol++;
			}
		}
		return sol;
	}

	Pokemon gimnasio(Entrenador& e) {

		if (entrenadores.find(e) == entrenadores.end()) {
			throw std::invalid_argument("Entrenador no Existente");
		}
		else if (entrenadores.at(e).empty()) {
			throw std::invalid_argument("No tiene Pokemons");
		}
		else {
			Pokemon p = entrenadores[e].front().nombre;
			entrenadores[e].pop_front();
			return p;
		}
	}
};


bool tratar_caso() {
	CentroPokemon gimnasio;
	string linea;

	getline(cin, linea);

	if (!std::cin)
		return false;

	while (linea != "FIN") {

		stringstream ss(linea);
		string operacion;
		ss >> operacion;

		if (operacion == "capturar_pokemon") {
			string a, p;
			int c;
			ss >> a >> p >> c;
			try {
				gimnasio.capturar_pokemon(a, p, c);
			}
			catch (invalid_argument& de) {
				cout << de.what() << endl;
			}
		}
		else if (operacion == "dia_de_liberacion") {
			string p;
			ss >> p;
			try {
				cout << "Liberados " << gimnasio.dia_de_liberacion(p) << " pokemon " << p << endl;

			}
			catch (invalid_argument& de) {
				cout << de.what() << endl;
			}
		}
		else if (operacion == "vencer_pokemon") {
			string a; int p;
			ss >> a >> p;
			try {
				cout << a << " utiliza el pokemon " << gimnasio.vencer_pokemon(a, p).nombre << " con nivel " << gimnasio.vencer_pokemon(a, p).nivel << " para vencer a un nivel " << p << endl;
			}
			catch (invalid_argument& de) {
				cout << de.what() << endl;
			}
		}
		else if (operacion == "gimnasio") {
			string a;
			ss >> a;
			try {
				cout << a << " deja el pokemon " << gimnasio.gimnasio(a) << " en un gimnasio" << endl;
			}
			catch (invalid_argument& de) {
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