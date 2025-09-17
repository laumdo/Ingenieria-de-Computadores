// Nombre del alumno Laura Martín del Olmo
// Usuario del Juez FAL-C052


#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

// función que resuelve el problema
/*int resolver(vector<int> const& v, int ini, int fin) {
    if (ini == fin - 1) {
        return ini;
    }
    else if (ini!=0 && ini == fin - 2) {
        return fin - 1;
    }
    int m = (ini + fin) / 2;
    if (v[m - 1] != v[m] && v[m + 1] != v[m]) {
        return m;
    }
    else {
        if ((m % 2 != 0 && v[m + 1] == v[m]) || (m % 2 == 0 && v[m] != v[m + 1])) {
            return resolver(v, ini, m);
        }
        else {
            return resolver(v, m, fin);
        }
    }
    
}*/

int resolver(vector<int> const& v, int ini, int fin) {
    if (ini == fin) {
        return ini;
    }
    else if (ini == fin - 1) {
        if (ini % 2 == 0) return ini;
        else return fin;
    }
    int m = (ini + fin) / 2;
    if (v[m - 1] != v[m] && v[m + 1] != v[m]) {
        return m;
    }
    else {
        if ((m % 2 != 0 && v[m + 1] == v[m]) || (m % 2 == 0 && v[m] == v[m - 1])) {
            return resolver(v, ini, m);
        }
        else {
            return resolver(v, m, fin);
        }
    }

}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    // leer los datos de la entrada
    int n;
    cin >> n;
    vector<int> v(n);

    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }
    
    int sol = resolver(v,0,n-1);
    // escribir sol
    cout << sol << endl;
    
}

int main() {
    // Para la entrada por fichero.
    // Comentar para acepta el reto
    #ifndef DOMJUDGE
     std::ifstream in("datos.txt");
     auto cinbuf = std::cin.rdbuf(in.rdbuf()); //save old buf and redirect std::cin to casos.txt
     #endif 
    
    
    int numCasos;
    std::cin >> numCasos;
    for (int i = 0; i < numCasos; ++i)
        resuelveCaso();

    
    // Para restablecer entrada. Comentar para acepta el reto
     #ifndef DOMJUDGE // para dejar todo como estaba al principio
     std::cin.rdbuf(cinbuf);
     system("PAUSE");
     #endif
    
    return 0;
}