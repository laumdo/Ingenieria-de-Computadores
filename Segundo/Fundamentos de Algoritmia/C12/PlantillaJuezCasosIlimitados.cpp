// Nombre del alumno Laura Martín del Olmo
// Usuario del Juez FAL-C052


#include <iostream>
#include <iomanip>
#include <fstream>
#include<vector>

using namespace std;

// función que resuelve el problema
void resolver(vector<int> const& v, int ini, int fin,int& min) {
    if (ini == fin-1) {
        if (v[ini] < min) min = v[ini];
        return;
    }
    int m = (ini + fin) / 2;
    resolver(v, ini, m, min);
    resolver(v, m, fin, min);
    return;
}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // leer los datos de la entrada
    int n;
    cin >> n;

    if (! std::cin)
        return false;
    
    vector<int> v;
    int aux;
    for (int i = 0; i < n; i++) {
        cin >> aux;
        v.push_back(aux);
    }

    int sol=v[0];

    resolver(v, 0, v.size(), sol);
    cout << sol << endl;
    
    // escribir sol
    
    
    return true;
    
}

int main() {
    // Para la entrada por fichero.
    // Comentar para acepta el reto
    #ifndef DOMJUDGE
     std::ifstream in("datos.txt");
     auto cinbuf = std::cin.rdbuf(in.rdbuf()); //save old buf and redirect std::cin to casos.txt
     #endif 
    
    
    while (resuelveCaso())
        ;

    
    // Para restablecer entrada. Comentar para acepta el reto
     #ifndef DOMJUDGE // para dejar todo como estaba al principio
     std::cin.rdbuf(cinbuf);
     system("PAUSE");
     #endif
    
    return 0;
}