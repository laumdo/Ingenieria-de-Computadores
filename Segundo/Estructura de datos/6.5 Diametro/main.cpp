#include "Arbin.h"
#include <iostream>
#include <string>

using namespace std;

struct tSol {
	int count;
	int max;
};
/**
  COMPLEJIDAD: Explica aquí justificadamente cuál es la complejidad de
  esta función
  ***A COMPLETAR***
*/
tSol diametro(const Arbin<char>& a) {
	if (a.esVacio()) {
		return { 0,0 };
	}
	else if (a.hijoIz().esVacio() && a.hijoDer().esVacio()) {
		return { 1,1 };
	}
	else {
		tSol iz = diametro(a.hijoIz());
		tSol der = diametro(a.hijoDer());
		tSol sol;
		int sube = max(iz.count, der.count)+1;
		int no_sube = iz.count + der.count;
		if (sube >= no_sube) {
			sol.max = sube;
			if (sol.max == iz.max + 1) sol.count = iz.count + 1;
			else sol.count = der.count + 1;
		}
		else {
			sol.max = no_sube + 1;
			sol.count = 0;
		}
		return sol;
	}
}

Arbin<char> lee_arbol(istream& in) {
	char c;
	in >> c;

	if (c == '.') {
		return Arbin<char>(); // Nodo vacío
	}
	else if (c == '(') {
		Arbin<char> iz = lee_arbol(in);
		char op;
		in >> op; // Leer '*'
		Arbin<char> dr = lee_arbol(in);
		in >> c; // Leer ')'
		return Arbin<char>(iz, 0, dr); // Asumiendo que el nodo raíz contiene un valor neutro (por ejemplo, 0)
	}

	return Arbin<char>();
}


int main() {
	Arbin<char> arbol;
	while (cin.peek() != EOF) {
		tSol sol = diametro(lee_arbol(cin));
		cout << sol.max << endl;
		string resto_linea;
		getline(cin, resto_linea);
		if (cin.peek() != EOF) cout << endl;
	}
	return 0;
}
