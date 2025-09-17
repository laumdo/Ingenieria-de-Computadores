#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include<fstream>
#include <unordered_map>
#include <unordered_set>
#include <map>
using namespace std;

using Pokemon = string;
using Entrenador = string;
using Nivel = int;


class CentroPokemon {

private:

	struct InfoEntrenador {
		unordered_set<Pokemon> pokemon_capturados;
		map<int, Pokemon> niveles;
	};

	unordered_map<Entrenador, InfoEntrenador> entrenadores;
public:


	void  capturar_pokemon(Entrenador& e, Pokemon& p, Nivel  n) {

		
	}

	InfoEntrenador  vencer_pokemon(Entrenador& e, Nivel n) const {

		
	}

	int dia_de_liberacion(Pokemon& p) {

		
	}

	Pokemon gimnasio(Entrenador& e) {

		
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



/*int main() {


#ifndef DOMJUDGE
	std::ifstream in("datos.in");
	auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif

	while (tratar_caso());

#ifndef DOMJUDGE
	std::cin.rdbuf(cinbuf);
#endif

}*/