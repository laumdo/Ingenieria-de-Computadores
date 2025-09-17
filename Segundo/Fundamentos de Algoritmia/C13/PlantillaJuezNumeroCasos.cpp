// Nombre del alumno Laura Martín del Olmo
// Usuario del Juez FAL-C052


#include <iostream>
#include <iomanip>
#include <fstream>
#include<vector>

using namespace std;

// función que resuelve el problema
char resolver(vector<char> const& v,const char letraIni, int ini, int fin) {
    //Caso Base:
    if (ini==fin-1) { //2 elementos
        if (v[ini]-letraIni!=ini) {
            return v[ini]-1;
        }
        else{ //si no son letras que están seguidas
            return v[ini] + 1; //se devuelve la que debería ser la sigueinte del primero (podría ser la anterior al último)
        }
    }
    //Caso recursivo:
    else {
        int m = (ini + fin) / 2;
        if (v[m] - letraIni == m) { // significa que hasta la mitad no se ha descuadrado, por lo tanto la solución estará a la derecha
            return resolver(v,letraIni, m, fin);
        }
        else { //sino, miro por la izquierda
            return resolver(v,letraIni, ini, m);
        }
    }
}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    // leer los datos de la entrada
    char ini, fin;
    cin >> ini >> fin;

    vector<char> v(fin-ini);

    char aux;
    for (int i = 0; i < v.size(); i++) {
        cin >> aux;
        v[i] = aux;
        //v.push_back(aux);
    }
    
    char sol;
    /*if (v[0] != ini) {
        sol = ini;
    }
    else if (v[v.size()-1] != fin) {
        sol = fin;
    }
    else {
        resolver(v,ini, 0, v.size(), sol);
    }*/
    sol=resolver(v, ini, 0, v.size());

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