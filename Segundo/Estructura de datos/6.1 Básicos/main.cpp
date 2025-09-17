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
	int nodos;
	int hojas;
	int altura;
};

tSol num_nodos(const Arbin<int>& a) {
	if (a.esVacio()) {
		return { 0,0,0 };
	}
	else if (a.hijoIz().esVacio() && a.hijoDer().esVacio()) {
		return { 1,1,1 };
	}
	else {
		tSol iz = num_nodos(a.hijoIz());
		tSol der = num_nodos(a.hijoDer());
		tSol sol;
		sol.nodos = iz.nodos + der.nodos + 1;
		sol.hojas = iz.hojas + der.hojas;
		sol.altura = max(iz.altura, der.altura) + 1;
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
		tSol sol = num_nodos(lee_arbol(cin));
		cout << sol.nodos << " " << sol.hojas << " " << sol.altura << endl;
		string resto_linea;
		getline(cin, resto_linea);
		if (cin.peek() != EOF) cout << endl;
	}
	return 0;
}
