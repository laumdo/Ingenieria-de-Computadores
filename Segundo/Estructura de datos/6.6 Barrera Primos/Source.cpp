/*
 * ---------------------------------------------------
 *                ESTRUCTURAS DE DATOS
 * ---------------------------------------------------
 *              Manuel Montenegro Montes
 *              Facultad de Informática
 *         Universidad Complutense de Madrid
 * ---------------------------------------------------
 */

 /*
  * Se añaden iteradores con respecto a la versión anterior.
  */

#ifndef __BINTREE_H
#define __BINTREE_H

#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <stack>
#include <fstream>
#include <algorithm>

using namespace std;

template <class T> class BinTree {

private:
    struct TreeNode;
    using NodePointer = std::shared_ptr<TreeNode>;

public:
    BinTree() : root_node(nullptr) {}

    BinTree(const T& elem)
        : root_node(std::make_shared<TreeNode>(nullptr, elem, nullptr)) {}

    BinTree(const BinTree& left, const T& elem, const BinTree& right)
        : root_node(std::make_shared<TreeNode>(left.root_node, elem,
            right.root_node)) {}

    bool empty() const { return root_node == nullptr; }

    const T& root() const {
        assert(root_node != nullptr);
        return root_node->elem;
    }

    BinTree left() const {
        assert(root_node != nullptr);
        BinTree result;
        result.root_node = root_node->left;
        return result;
    }

    BinTree right() const {
        assert(root_node != nullptr);
        BinTree result;
        result.root_node = root_node->right;
        return result;
    }

    void display(std::ostream& out) const { display_node(root_node, out); }

    template <typename U> void preorder(U func) const {
        preorder(root_node, func);
    }

    template <typename U> void inorder(U func) const {
        std::stack<NodePointer> st;

        descend_and_push(root_node, st);

        while (!st.empty()) {
            NodePointer current = st.top();
            st.pop();

            func(current->elem);

            descend_and_push(current->right, st);
        }
    }

    template <typename U> void postorder(U func) const {
        postorder(root_node, func);
    }

    template <typename U> void levelorder(U func) const;

    class iterator {
    public:
        const T& operator*() const {
            assert(!st.empty());
            return st.top()->elem;
        }

        const T* operator->() const { return &operator*(); }

        iterator& operator++() {
            assert(!st.empty());
            NodePointer top = st.top();
            st.pop();
            BinTree::descend_and_push(top->right, st);
            return *this;
        }

        iterator operator++(int) {
            iterator previous = *this;
            ++(*this);
            return previous;
        }

        bool operator==(const iterator& other) const { return st == other.st; }

        bool operator!=(const iterator& other) const {
            return !this->operator==(other);
        }

    private:
        friend class BinTree;

        iterator() {}

        iterator(const NodePointer& root) { BinTree::descend_and_push(root, st); }

        std::stack<NodePointer> st;
    };

    iterator begin() const { return iterator(root_node); }

    iterator end() const { return iterator(); }

private:
    struct TreeNode {
        TreeNode(const NodePointer& left, const T& elem, const NodePointer& right)
            : elem(elem), left(left), right(right) {}

        T elem;
        NodePointer left, right;
    };

    NodePointer root_node;

    static void display_node(const NodePointer& root, std::ostream& out) {
        if (root == nullptr) {
            out << ".";
        }
        else {
            out << "(";
            display_node(root->left, out);
            out << " " << root->elem << " ";
            display_node(root->right, out);
            out << ")";
        }
    }

    static void descend_and_push(const NodePointer& node,
        std::stack<NodePointer>& st);

    template <typename U> static void preorder(const NodePointer& node, U func);

    template <typename U> static void postorder(const NodePointer& node, U func);
};

template <typename T>
void BinTree<T>::descend_and_push(const NodePointer& node,
    std::stack<NodePointer>& st) {
    NodePointer current = node;
    while (current != nullptr) {
        st.push(current);
        current = current->left;
    }
}

template <typename T>
template <typename U>
void BinTree<T>::preorder(const NodePointer& node, U func) {
    if (node != nullptr) {
        func(node->elem);
        preorder(node->left, func);
        preorder(node->right, func);
    }
}

template <typename T>
template <typename U>
void BinTree<T>::postorder(const NodePointer& node, U func) {
    if (node != nullptr) {
        postorder(node->left, func);
        postorder(node->right, func);
        func(node->elem);
    }
}

template <typename T>
template <typename U>
void BinTree<T>::levelorder(U func) const {
    std::queue<NodePointer> pending;
    if (root_node != nullptr) {
        pending.push(root_node);
    }
    while (!pending.empty()) {
        NodePointer current = pending.front();
        pending.pop();
        func(current->elem);
        if (current->left != nullptr) {
            pending.push(current->left);
        }
        if (current->right != nullptr) {
            pending.push(current->right);
        }
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const BinTree<T>& tree) {
    tree.display(out);
    return out;
}

template <typename T> BinTree<T> read_tree(std::istream& in) {
    char c;
    in >> c;
    if (c == '.') {
        return BinTree<T>();
    }
    else {
        assert(c == '(');
        BinTree<T> left = read_tree<T>(in);
        T elem;
        in >> elem;
        BinTree<T> right = read_tree<T>(in);
        in >> c;
        assert(c == ')');
        BinTree<T> result(left, elem, right);
        return result;
    }
}

#endif

struct barreraPrimo {
    int multiplo;
    int profundidad;
};

bool es_primo(int num, const vector<bool>& check_primo) {
    if (num >= 0 && num < check_primo.size()) {
        return check_primo[num];  // Retorna si el número es primo o no
    }
    return false;  // Si el número está fuera del rango, lo tratamos como no primo
}

#include<queue>
barreraPrimo calcular_barrera(const BinTree<int>& tree, vector<bool> check_primo) {
    queue<pair<BinTree<int>, int>> pendientes; //arbol, profundidad
    if (tree.empty()) return { -1,-1 };
    pendientes.push({ tree,0 });
    barreraPrimo result;

    while (!pendientes.empty()) {
        auto [next_tree, prof] = pendientes.front();
        //BinTree<int> next_tree = pendientes.front();
        pendientes.pop();
        //HAY Q COMPROBAR QUE EL ARBOL NO ES VACIO
        if (!tree.empty()) {
            if (!es_primo(next_tree.root(), check_primo)) { // vector booleanos tam 5000 que diga si es prim o no.
                // Condicion para añadir arriba en vez de hacer dos if en el else de abajo: !next_tree.empty() &&
                if (next_tree.root() % 7 == 0) { //multiplo de 7
                    result.multiplo = next_tree.root();
                    result.profundidad = prof+1;
                    return result;
                }
                else {
                    if (!next_tree.left().empty())pendientes.push({ next_tree.left(),prof + 1 });
                    if (!next_tree.right().empty()) pendientes.push({ next_tree.right(), prof + 1 });
                    // Si pidiesen prioridad a hijos derechos, se inserta antes el hijo derecho
                }
            }
        }
    }
    return { -1,-1 };

}

void criba_eratostenes(vector<bool>& check_primo, int limite) {
    // Inicializamos el vector con `true`
    fill(check_primo.begin(), check_primo.end(), true);

    // El 0 y el 1 no son primos
    check_primo[0] = check_primo[1] = false;

    for (int i = 2; i * i <= limite; ++i) {
        if (check_primo[i]) {
            for (int j = i * i; j <= limite; j += i) {
                check_primo[j] = false;  // Marcamos como no primo
            }
        }
    }
}
void resuelveCaso(vector<bool> check_primo) {
    BinTree<int> tree = read_tree<int>(cin);
    barreraPrimo sol = calcular_barrera(tree, check_primo);
    if (sol.multiplo == -1 && sol.profundidad == -1) cout << "NO HAY\n";
    else cout << sol.multiplo << " " << sol.profundidad << endl;

}


int main() {
#ifndef DOMJUDGE
    std::ifstream in("diametro.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
    std::ofstream out("diametro.out");
    auto coutbuf = std::cout.rdbuf(out.rdbuf());
#endif

    int numCasos;
    cin >> numCasos;
    int limite = 5000;  // Limite para la criba (puedes ajustarlo según necesites)
    vector<bool> check_primo(limite + 1);  // El tamaño del vector depende del límite
    criba_eratostenes(check_primo, limite);  // Llamamos a la criba para llenar el vector
    for (int i = 0; i < numCasos; ++i)
        resuelveCaso(check_primo);

#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);
    system("PAUSE");
#endif
    return 0;
}