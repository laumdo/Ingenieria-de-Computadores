#include "Arbin.h"
#include <iostream>
#include <string>

using namespace std;


/**
  COMPLEJIDAD: Explica aquí justificadamente cuál es la complejidad de
			   esta función
			   ***A COMPLETAR***
*/
struct tSol {
	int equipos;
	int excursionistas;
};

tSol excursionistas(const Arbin<int>& a) {
	if (a.esVacio()) {
		return { 0,0 };
	}
	else if (a.hijoIz().esVacio() && a.hijoDer().esVacio()) {
		if (a.raiz() != 0) return { 1,a.raiz() };
		else return { 0,0 };
	}
	else {
		tSol iz = excursionistas(a.hijoIz());
		tSol der = excursionistas(a.hijoDer());
		tSol sol;
		sol.equipos = iz.equipos + der.equipos;
		sol.excursionistas = max(iz.excursionistas, der.excursionistas)+a.raiz();
		if (a.raiz() != 0 && iz.equipos == 0 && der.equipos == 0) {
			//sol.excursionistas += a.raiz();
			//if () {
				sol.equipos = 1;
			//}
		}
		return sol;
	}
}


Arbin<int> lee_arbol(istream& in) {
	char c;
	in >> c;

	if (c == '.') {
		return Arbin<int>(); // Nodo vacío
	}
	else if (c == '(') {
		Arbin<int> iz = lee_arbol(in);
		char op;
		in >> op; // Leer '*'
		Arbin<int> dr = lee_arbol(in);
		in >> c; // Leer ')'
		return Arbin<int>(iz, 0, dr); // Asumiendo que el nodo raíz contiene un valor neutro (por ejemplo, 0)
	}

	return Arbin<int>();
}


int main() {
	Arbin<int> arbol;
	while (cin.peek() != EOF) {
		tSol sol = excursionistas(lee_arbol(cin));
		cout << sol.equipos << " " << sol.excursionistas << endl;
		string resto_linea;
		getline(cin, resto_linea);
		if (cin.peek() != EOF) cout << endl;
	}
	return 0;
}
