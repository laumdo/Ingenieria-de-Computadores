// Nombre del alumno Laura Martín del Olmo
// Usuario del Juez FAL-C052


#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

typedef struct {
    string nombre;
    int canicas;
}tJugador;

// función que resuelve el problema
tJugador resolver(vector<tJugador>& v, int ini, int fin) {
    if (ini == fin - 2) {
        if (v[ini].canicas > v[ini+1].canicas || v[ini].canicas == v[ini+1].canicas) {
            v[ini].canicas += v[ini+1].canicas/2;
            return v[ini];
        }
        else {
            v[ini+1].canicas += v[ini].canicas/2;
            return v[ini+1];
        }
    }
    int mitad = (ini + fin) / 2;
    tJugador iz = resolver(v, ini, mitad);
    tJugador der = resolver(v, mitad, fin);
    if (iz.canicas > der.canicas || iz.canicas == der.canicas) {
        iz.canicas += der.canicas / 2;
        return iz;
    }
    else {
        der.canicas += iz.canicas / 2;
        return der;
    }

}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // leer los datos de la entrada
    int n;
    cin >> n;

    if (! std::cin)
        return false;
    
    vector<tJugador> datos(n);

    for (int i = 0; i < n; i++) {
        cin >> datos[i].nombre;
        cin >> datos[i].canicas;
    }
    tJugador sol = resolver(datos,0,n);
    
    // escribir sol
    cout << sol.nombre << " " << sol.canicas << endl;
    
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