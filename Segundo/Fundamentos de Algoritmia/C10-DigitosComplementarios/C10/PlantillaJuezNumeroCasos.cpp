// Nombre del alumno Laura Martín del Olmo
// Usuario del Juez FAL-C052


#include <iostream>
#include <iomanip>
#include <fstream>
#include<cmath>

using namespace std;


long long int complementario(long long int num, long long int& cont) {
    cont++;
    if (num < 10) {
        return 9 - (num % 10);
    }
    return (9 - (num % 10)) + complementario(num / 10, cont) * 10;
}

long long int invertido(long long int num, long long int& pot) {
    if (num < 10) {
        return num*pow(10,pot--); //no hace falta el --
    }

    return (((num % 10) * pow(10,pot--)) + invertido(num / 10, pot));
}

// función que resuelve el problema

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
void resuelveCaso() {
    // leer los datos de la entrada
    long long int num;
    cin >> num;
    long long int potencia=0;
    long long int c = complementario(num, potencia);
    potencia--;
    long long int i;
    if (num == 0) i = 9;
    //else if (num%10==0 /* && potencia>0 && num % 9 == 0*/) i = invertido(c,potencia) * pow(10, potencia);
    else i=invertido(c,potencia);
    cout << c << " " << i << endl;
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