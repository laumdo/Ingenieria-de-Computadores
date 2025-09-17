// Nombre del alumno Laura Martín del Olmo
// Usuario del Juez FAL-C052


#include <iostream>
#include <iomanip>
#include <fstream>
#include<vector>

using namespace std;

//const int MAX = 100000;

typedef struct {
    bool po;
    int max;
    int min;
}tSol;

tSol parcial_ord(vector<int> const& v, int ini, int fin) {
    if (ini == fin - 1) {
        return{ true,v[ini],v[ini] };
    }
    else {
        int m = (ini + fin) / 2;
        tSol iz = parcial_ord(v, ini, m);
        tSol der = parcial_ord(v, m, fin);
        bool  b = iz.min <= der.min && der.max >= iz.max;
        return { b && iz.po && der.po, der.max, iz.min };
    }
}

//Función de generalización
//void pordenadoGeneralizacion(vector<int>const& v, int ini, int fin, bool& po, int& m, int& M) { //po, m y M son de salida
//    //Caso base:
//    if (ini==fin-1) {
//        po = true;
//        m = v[ini];
//        M = v[ini];
//        return;
//    }
//    //Caso recursivo:
//    int mitad = (ini + fin) / 2;
//    bool poIz, poDer;
//    int mIz, mDer, MIz, MDer;
//    pordenadoGeneralizacion(v, ini, mitad, po, m, M);
//    poIz = po;
//    mIz = m;
//    MIz = M;
//    pordenadoGeneralizacion(v, mitad, fin, po, m, M);
//    poDer = po;
//    mDer = m;
//    MDer = M;
//    bool b = mIz <= mDer && MDer >= MIz;
//    po = b && poIz && poDer;
//    return;
//}
//
//bool pordenado(vector<int>const& v) {
//    bool po;
//    int m, M;
//    pordenadoGeneralizacion(v, 0, v.size(),po,m,M);
//    return po;
//}

// función que resuelve el problema
//bool resolver(vector<int> const& v, int ini, int fin, int& min, int& minNuevo, int& max) {
//    if (ini + 1 == fin) { //si el vector tiene 2 elementos
//        
//        if (v[ini] < min && min==MAX) {
//            minNuevo = v[ini];
//            min = minNuevo;
//        }
//        else if (v[ini] < minNuevo) minNuevo = v[ini];
//
//        if (v[fin] > max) max = v[fin];
//        return v[ini]<=min /* && v[fin] >= max */&& v[ini] <= v[fin];
//    }
//    else {
//        int m = (ini + fin) / 2;
//        return resolver(v, m + 1, fin, min, minNuevo, max) && resolver(v, ini, m, min, minNuevo, max);
//    }
//}

// Resuelve un caso de prueba, leyendo de la entrada la
// configuración, y escribiendo la respuesta
bool resuelveCaso() {
    // leer los datos de la entrada
    vector<int> v;
    int num;
   /* int i = 0;

    int min = MAX;
    int minN = MAX;
    int max = 0;*/

    cin >> num;
    if (num==0)
        return false;
    
    while (num != 0) {
        v.push_back(num);
        /*v[i] = num;
        i++;*/
        cin >> num;
    }
    //v.resize(i);

    //bool sol = resolver(v,0,i-1,min,minN,max);
    //bool sol = pordenado(v);
    tSol sol = parcial_ord(v, 0, v.size());
    
    // escribir sol
    if (sol.po) cout << "SI\n";
    else cout << "NO\n";
    
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
