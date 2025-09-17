#include <iostream>
#include <cassert>
#include <fstream>
#include<vector>

using namespace std;

class ListLinkedDouble {
private:
    struct Node {
        int value;
        Node* next;
        Node* prev;
    };

public:
    ListLinkedDouble() : num_elems(0) {
        head = new Node;
        head->next = head;
        head->prev = head;
    }

    ListLinkedDouble(const ListLinkedDouble& other) : ListLinkedDouble() {
        copy_nodes_from(other);
        num_elems = other.num_elems;
    }

    ~ListLinkedDouble() { delete_nodes(); }

    void entramar(ListLinkedDouble& other);
    void push_front(const int& elem) {
        Node* new_node = new Node{ elem, head->next, head };
        head->next->prev = new_node;
        head->next = new_node;
        num_elems++;
    }

    void push_back(const int& elem) {
        Node* new_node = new Node{ elem, head, head->prev };
        head->prev->next = new_node;
        head->prev = new_node;
        num_elems++;
    }

    void pop_front() {
        assert(num_elems > 0);
        Node* target = head->next;
        head->next = target->next;
        target->next->prev = head;
        delete target;
        num_elems--;
    }

    void pop_back() {
        assert(num_elems > 0);
        Node* target = head->prev;
        target->prev->next = head;
        head->prev = target->prev;
        delete target;
        num_elems--;
    }

    int size() const { return num_elems; }

    bool empty() const { return num_elems == 0; };

    const int& front() const {
        assert(num_elems > 0);
        return head->next->value;
    }

    int& front() {
        assert(num_elems > 0);
        return head->next->value;
    }

    const int& back() const {
        assert(num_elems > 0);
        return head->prev->value;
    }

    int& back() {
        assert(num_elems > 0);
        return head->prev->value;
    }

    const int& operator[](int index) const {
        assert(0 <= index && index < num_elems);
        Node* result_node = nth_node(index);
        return result_node->value;
    }

    int& operator[](int index) {
        assert(0 <= index && index < num_elems);
        Node* result_node = nth_node(index);
        return result_node->value;
    }

    ListLinkedDouble& operator=(const ListLinkedDouble& other) {
        if (this != &other) {
            delete_nodes();
            head = new Node;
            head->next = head->prev = head;
            copy_nodes_from(other);
            num_elems = other.num_elems;
        }
        return *this;
    }

    void display(std::ostream& out) const;

    void display() const { display(std::cout); }


    // Nuevo metodo
    void add_to(int index, int val);
    void partition(int pivote);
    void my_display(std::ostream& out) const;
    void interseccion(ListLinkedDouble& other);
    void eliminarNum(int num);
    void mezclar(ListLinkedDouble& lista2);
    void repetir(vector<int> rep);
    void unzip(ListLinkedDouble& zs);

private:
    Node* head;
    int num_elems;

    Node* nth_node(int n) const;
    void delete_nodes();
    void copy_nodes_from(const ListLinkedDouble& other);

    // Nuevos metodos
    static void attach(Node* node, Node* before);
    static void detach(Node* node);
};

ListLinkedDouble::Node* ListLinkedDouble::nth_node(int n) const {
    int current_index = 0;
    Node* current = head->next;

    while (current_index < n && current != head) {
        current_index++;
        current = current->next;
    }

    return current;
}

void ListLinkedDouble::delete_nodes() {
    Node* current = head->next;
    while (current != head) {
        Node* target = current;
        current = current->next;
        delete target;
    }

    delete head;
}

void ListLinkedDouble::copy_nodes_from(const ListLinkedDouble& other) {
    Node* current_other = other.head->next;
    Node* last = head;

    while (current_other != other.head) {
        Node* new_node = new Node{ current_other->value, head, last };
        last->next = new_node;
        last = new_node;
        current_other = current_other->next;
    }
    head->prev = last;
}

void ListLinkedDouble::display(std::ostream& out) const {
    out << "[";
    if (head->next != head) {
        out << head->next->value;
        Node* current = head->next->next;
        while (current != head) {
            out << ", " << current->value;
            current = current->next;
        }
    }
    out << "]";
    out << "\n";
}

void ListLinkedDouble::my_display(std::ostream& out) const {
    if (head->next != head) {
        out << head->next->value;
        Node* current = head->next->next;
        while (current != head) {
            out << " " << current->value;
            current = current->next;
        }
    }
}

std::ostream& operator<<(std::ostream& out, const ListLinkedDouble& l) {
    l.display(out);
    return out;
}

//@ <answer>
//---------------------------------------------------------------
// Modificar a partir de aqui
// --------------------------------------------------------------

void ListLinkedDouble::attach(Node* node, Node* after) { //after forma parte de la lista, queremos introducir node antes de after
    after->prev->next = node; // 1
    node->prev = after->prev; // 2
    node->next = after; // 4
    after->prev = node; // 3
    //Da igual el orden de 4 y 3
}

void ListLinkedDouble::detach(Node* node) { //el nodo recibido por parámetro tiene que ser sacado de la lista
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->prev = nullptr;
    node->next = nullptr;
    //node->next = node->prev = nullptr;
}

void ListLinkedDouble::entramar(ListLinkedDouble& other) {

    Node* current = this->head->next;
    Node* currentOther = other.head->next;
    while (current != this->head && currentOther != other.head) {
        Node* aux = current->next;
        Node* aux2 = currentOther->next;

        other.detach(currentOther);
        attach(currentOther, aux);
        current = aux;
        currentOther = aux2;

    }
    while (currentOther != other.head) {
        Node* aux3= currentOther->next;
        other.detach(currentOther);
        attach(currentOther, this->head);
        currentOther = aux3;
    }
}


void resuelveCaso() {
    //int num_cases;
    //cin >> num_cases;


    //for (int i = 0; i < num_cases; ++i) {
        ListLinkedDouble list1, list2;

        //this
        int n;
        cin >> n;
        vector<int> v1(n);
        for (int j = 0; j < n; ++j) cin >> v1[j];
        for (int j = 0; j < n; ++j) list1.push_front(v1[n - 1 - j]);


        //other
        int m;
        cin >> m;
        vector<int> v2(m);
        for (int j = 0; j < m; ++j) cin >> v2[j];
        for (int j = 0; j < m; ++j) list2.push_front(v2[m - 1 - j]);

        //entramar
        list1.entramar(list2);

        list1.display();
    //}

    //return;
}

/*
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