#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Estructuras de la Unidad 1 y 2[cite: 2, 8]
struct Persona {
    string nombre;
    int rol; // 1: entrenador; 0: jugador
    float poder;
};

struct Jugador {
    Persona elemento;
    Jugador *siguiente;
};

// Clase Equipo implementada como Lista Enlazada[cite: 5, 8]
class Equipo {
private:
    Jugador *head, *tail;
    string nombre_equipo;
    unsigned int listSize, maximo;

public:
    Equipo() { 
        head = tail = new Jugador;
        head->siguiente = NULL;
        listSize = 0;
        maximo = 7; 
    }

    void setNombre(string n) { nombre_equipo = n; }
    string getNombre() { return nombre_equipo; }

    /*****
    * int agregar_companero
    * Inserta una persona al final de la lista del equipo.
    *****/
    int agregar_companero(string nombre, int rol, float poder) {
        if (listSize >= maximo) return -1;
        Jugador *nuevo = new Jugador;
        nuevo->elemento.nombre = nombre;
        nuevo->elemento.rol = rol;
        nuevo->elemento.poder = poder;
        nuevo->siguiente = NULL;
        
        tail->siguiente = nuevo;
        tail = nuevo;
        listSize++;
        return listSize - 1;
    }

    int calcular_poder() {
        int suma = 0;
        Jugador *aux = head->siguiente;
        while (aux != NULL) {
            suma += (int)aux->elemento.poder;
            aux = aux->siguiente;
        }
        return suma;
    }

    void imprimir_equipo() {
        cout << "Equipo: " << nombre_equipo << endl;
        Jugador *aux = head->siguiente;
        int i = 0;
        while (aux != NULL) {
            cout << "Persona " << i << ": " << aux->elemento.nombre << " " 
                 << aux->elemento.rol << " " << aux->elemento.poder << endl;
            aux = aux->siguiente;
            i++;
        }
    }
};

// Estructura para el Arbol Binario del Torneo[cite: 6, 8]
struct Competidor {
    Equipo equipo;
    Competidor *izq;
    Competidor *der;
    bool ocupado;
};

class Torneo {
private:
    Competidor *raiz;

    // Crea la estructura jerarquica del arbol vacio[cite: 6]
    Competidor* construir_arbol_vacio(int niveles) {
        Competidor *nuevo = new Competidor;
        nuevo->izq = nuevo->der = NULL;
        nuevo->ocupado = false;
        if (niveles > 0) {
            nuevo->izq = construir_arbol_vacio(niveles - 1);
            nuevo->der = construir_arbol_vacio(niveles - 1);
        }
        return nuevo;
    }

    void llenar_hojas(Competidor *nodo, Equipo* equipos, int &index) {
        if (nodo == NULL) return;
        if (nodo->izq == NULL && nodo->der == NULL) {
            nodo->equipo = equipos[index++];
            nodo->ocupado = true;
            return;
        }
        llenar_hojas(nodo->izq, equipos, index);
        llenar_hojas(nodo->der, equipos, index);
    }

public:
    Torneo() { raiz = NULL; }

    void crear_torneo(Equipo* equipos, int n) {
        int niveles = 0;
        int temp = n;
        while (temp > 1) { temp /= 2; niveles++; }
        raiz = construir_arbol_vacio(niveles);
        int index = 0;
        llenar_hojas(raiz, equipos, index);
    }

    // Recorrido In-orden o similar para imprimir bracket[cite: 6, 8]
    void imprimir_bracket_rec(Competidor *nodo) {
        if (nodo == NULL) return;
        if (nodo->izq != NULL && nodo->der != NULL) {
            if (nodo->izq->ocupado && nodo->der->ocupado) {
                cout << nodo->izq->equipo.getNombre() << " " << nodo->izq->equipo.calcular_poder() 
                     << " vs " << nodo->der->equipo.getNombre() << " " << nodo->der->equipo.calcular_poder() << " | ";
            } else {
                imprimir_bracket_rec(nodo->izq);
                imprimir_bracket_rec(nodo->der);
            }
        }
    }

    void imprimir_bracket() {
        if (raiz == NULL) return;
        if (raiz->ocupado && raiz->izq == NULL) {
            cout << raiz->equipo.getNombre() << " " << raiz->equipo.calcular_poder() << endl;
        } else {
            imprimir_bracket_rec(raiz);
            cout << endl;
        }
    }

    void avanzar_ronda_rec(Competidor *nodo) {
        if (nodo == NULL || nodo->izq == NULL) return;
        if (nodo->izq->ocupado && nodo->der->ocupado) {
            // Regla: si empatan, gana el de la izquierda
            if (nodo->izq->equipo.calcular_poder() >= nodo->der->equipo.calcular_poder())
                nodo->equipo = nodo->izq->equipo;
            else
                nodo->equipo = nodo->der->equipo;
            nodo->ocupado = true;
            nodo->izq->ocupado = false;
            nodo->der->ocupado = false;
        } else {
            avanzar_ronda_rec(nodo->izq);
            avanzar_ronda_rec(nodo->der);
        }
    }

    void avanzar_ronda() { avanzar_ronda_rec(raiz); }

    void imprimir_por_poder(Competidor *nodo, int k) {
        if (nodo == NULL) return;
        if (nodo->ocupado && nodo->equipo.calcular_poder() >= k) {
            nodo->equipo.imprimir_equipo();
        }
        imprimir_por_poder(nodo->izq, k);
        imprimir_por_poder(nodo->der, k);
    }

    void buscar_k(int k) { imprimir_por_poder(raiz, k); }
};

int main() {
    ifstream archivo("Equipos.txt");
    if (!archivo.is_open()) {
        cout << "Error: No se encontro Equipos.txt" << endl;
        return 1;
    }

    int n;
    archivo >> n;
    Equipo *lista = new Equipo[n];

    for (int i = 0; i < n; i++) {
        int p; string nombreE;
        archivo >> p >> nombreE;
        lista[i].setNombre(nombreE);
        for (int j = 0; j < p; j++) {
            string nomP; int rol; float pod;
            archivo >> nomP >> rol >> pod;
            lista[i].agregar_companero(nomP, rol, pod);
        }
    }
    archivo.close();

    Torneo t;
    t.crear_torneo(lista, n);

    int op;
    // Interfaz de consola solicitada
    while (cin >> op) {
        if (op == 1) { t.avanzar_ronda(); t.imprimir_bracket(); }
        else if (op == 2) { t.imprimir_bracket(); }
        else if (op == 3) { int k; cin >> k; t.buscar_k(k); }
    }

    delete[] lista;
    return 0;
}