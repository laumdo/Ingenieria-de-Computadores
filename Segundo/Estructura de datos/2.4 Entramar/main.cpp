#include "lista.h" 
#include <iostream>
using namespace std;

bool lee_lista(Lista<int>& lista) {
    int n_elems;
    cin >> n_elems;
    if (n_elems >= 0) {
        while (n_elems-- != 0) {
            int elem;
            cin >> elem;
            lista.pon_final(elem);
        }
        return true;
    }
    else {
        return false;
    }
}

bool ejecuta_caso() {
    Lista<int> lista;
    if (lee_lista(lista)) {
        Lista<int> otra;
        if (lee_lista(otra)) {
            lista.entramar(otra);
            lista.imprime1();
            cout << endl;
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

int main() {
    while (ejecuta_caso());
    return 0;
}

/*#include"lista.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include<vector>

using namespace std;

void resuelveCaso() {
    //int num_cases;
    //cin >> num_cases;


    //for (int i = 0; i < num_cases; ++i) {
    Lista<int> list1, list2;

    //this
    int n;
    cin >> n;
    vector<int> v1(n);
    for (int j = 0; j < n; ++j) cin >> v1[j];
    for (int j = 0; j < n; ++j) list1.pon_ppio(v1[n - 1 - j]);


    //other
    int m;
    cin >> m;
    vector<int> v2(m);
    for (int j = 0; j < m; ++j) cin >> v2[j];
    for (int j = 0; j < m; ++j) list2.pon_ppio(v2[m - 1 - j]);

    //entramar
    list1.entramar(list2);

    list1.display();
    //}

    //return;
}

int main() {
    // ajustes para que cin extraiga directamente de un fichero
#ifndef DOMJUDGE
    std::ifstream in("datos.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
    std::ofstream out("datos.out");
    auto coutbuf = std::cout.rdbuf(out.rdbuf());
#endif

    int numCasos;
    std::cin >> numCasos;
    for (int i = 0; i < numCasos; ++i)
        resuelveCaso();

    // para dejar todo como estaba al principio
#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    system("PAUSE");
#endif
    return 0;
}*/