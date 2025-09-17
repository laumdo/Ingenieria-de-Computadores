/*
 * ---------------------------------------------------
 *                ESTRUCTURAS DE DATOS
 * ---------------------------------------------------
 *              Facultad de Inform�tica
 *         Universidad Complutense de Madrid
 * ---------------------------------------------------
 */


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
    void display_reverse(std::ostream& out) const;

    void display() const { display(std::cout); }
    void display_reverse() const { display_reverse(std::cout); }

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

void ListLinkedDouble::display_reverse(std::ostream& out) const {
    out << "[";
    if (head->prev != head) {
        out << head->prev->value;
        Node* current = head->prev->prev;
        while (current != head) {
            out << ", " << current->value;
            current = current->prev;
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

void ListLinkedDouble::add_to(int index, int val) {
}

void ListLinkedDouble::partition(int pivote) {
    Node* current=head->next;
    Node* ultimo = this->head->prev;

    while (current != ultimo) {
        Node* aux = current->next;
        if (current->value > pivote) {
            detach(current);
            attach(current, head);
        }
        current = aux;
    }
    if (current->value > pivote) {
        detach(current);
        attach(current, head);
    }
    return;
}

void ListLinkedDouble::interseccion(ListLinkedDouble& other) {
}

void ListLinkedDouble::eliminarNum(int num) {

}

void ListLinkedDouble::mezclar(ListLinkedDouble& lista2) {





}


void ListLinkedDouble::repetir(vector<int> rep) {


}


void ListLinkedDouble::unzip(ListLinkedDouble& zs) {


}

void tratar_caso() {
    ListLinkedDouble ns;
    //ListLinkedDouble zs;
    int aux;
    cin >> aux;
    while (aux != 0) {
        ns.push_back(aux);
        cin >> aux;
    }
    int pivote;
    cin >> pivote;
    /*cin >> aux;
    while (aux != 0) {
        zs.push_back(aux);
        cin >> aux;
    }*/
    if (!ns.empty()) {
        ns.partition(pivote);
    }
    
    ns.display();
    ns.display_reverse();
    //cout << endl;
    /*zs.display();
    cout << endl;*/

}

//---------------------------------------------------------------
// No modificar por debajo de esta linea
// --------------------------------------------------------------
//@ </answer>


int main() {
#ifndef DOMJUDGE
    std::ifstream in("sample.in");
    auto cinbuf = std::cin.rdbuf(in.rdbuf());
#endif
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) tratar_caso();

#ifndef DOMJUDGE
    std::cin.rdbuf(cinbuf);
    // Descomentar si se trabaja en Windows
     //system("PAUSE");
#endif
    return 0;
}