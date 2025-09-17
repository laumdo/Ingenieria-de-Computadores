// Nombre del alumno Laura Martín del Olmo
// Usuario del Juez FAL-C052


#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

const int MAX_I = 20;
const int MAX_J = 19;

// Función que verifica si una asignación es válida
bool esValida(vector<int>& productosPorSupermercado, int supermercado) {
    return productosPorSupermercado[supermercado] < 3;
}

// Función de vuelta atrás con optimización
void vueltaAtrasOpt(int sol[], int k, int& cMejor, const int datos[MAX_I][MAX_J], int m, int n, vector<int>& productosPorSupermercado, int costeActual) {
    // Vector de soluciones: sol = (x0, x1, ..., xk), donde xi es el supermercado asignado al producto i.
    for (int i = 0; i < m; ++i) {
        sol[k] = i;
        if (esValida(productosPorSupermercado, i)) {
            productosPorSupermercado[i]++;
            int nuevoCoste = costeActual + datos[i][k];

            if (k==n-1) {
                if (nuevoCoste<cMejor) {
                    cMejor = nuevoCoste;
                }
            }
            else if (nuevoCoste < cMejor) {
                vueltaAtrasOpt(sol, k + 1, cMejor, datos, m, n, productosPorSupermercado, nuevoCoste);
            }

            productosPorSupermercado[i]--;
        }
    }
}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    // leer los datos de la entrada
    int m, n;
    cin >> m >> n;
    //vector<vector<int>> datos(m)(n);
    int datos[MAX_I][MAX_J];
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> datos[i][j];
        }
    }

    /*int costeM = INT_MAX;
    vector<int> sol(n);
    vector<int> super(m);
    for (int k = 0; k < super.size(); k++) {
        super[k] = 0;
    }*/

    //resolver(m,n,datos,0,costeM,sol,super);
    // escribir sol
    int sol[MAX_J] = { 0 };
    int cMejor = 100000; //NOTA: MAX_INT da compiler error
    vector<int> productosPorSupermercado(m,0);
    vueltaAtrasOpt(sol, 0, cMejor, datos, m, n, productosPorSupermercado, 0);

    if (cMejor == 100000) {
        cout << "Sin solucion factible" << endl;
    }
    else {
        cout << cMejor << endl;
    }
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