#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

/*****
* struct Persona
******
* Representa a un integrante de un equipo con sus atributos básicos[cite: 2, 8].
*****/
struct Persona {
    string nombre;
    int rol;      // 1: entrenador; 0: jugador[cite: 8]
    float poder;  // Guardamos el poder real, pero si es entrenador no suma al total[cite: 8]
};

/*****
* struct Jugador
******
* Nodo para la lista enlazada que contiene una Persona[cite: 2, 8].
*****/
struct Jugador {
    Persona elemento;
    Jugador *siguiente;
};

/*****
* class Equipo
******
* Implementa una lista enlazada para gestionar los integrantes de un equipo[cite: 1, 5, 8].
*****/
class Equipo {
private:
    Jugador *head;
    Jugador *tail;
    Jugador *curr;
    string nombre;
    unsigned int listSize;
    unsigned int pos;
    unsigned int maximo;

public:
    /*****
    * Constructor Equipo
    ******
    * Inicializa un nuevo equipo con un tamaño máximo definido[cite: 1, 8].
    *****/
    Equipo(int max = 7) {
        head = new Jugador;
        head->siguiente = NULL;
        tail = head;
        curr = head;
        listSize = 0;
        pos = 0;
        maximo = max;
    }

    /*****
    * Destructor Equipo
    ******
    * Libera la memoria de la lista enlazada para evitar fugas[cite: 3, 8].
    *****/
    ~Equipo() {
        limpiar();
        delete head;
    }

    /*****
    * void limpiar
    ******
    * Elimina todos los nodos de la lista (excepto el head)[cite: 3, 5, 8].
    *****/
    void limpiar() {
        Jugador *aux = head->siguiente;
        while (aux != NULL) {
            Jugador *borrar = aux;
            aux = aux->siguiente;
            delete borrar;
        }
        head->siguiente = NULL;
        tail = head;
        listSize = 0;
    }

    

    void setNombre(string n) { nombre = n; }
    string getNombre() { return nombre; }

    /*****
    * int agregar_companero
    ******
    * Inserta una nueva persona al equipo si hay espacio[cite: 2, 8].
    *****/
    int agregar_companero(string n, int r, float p) {
        if (listSize >= maximo) return -1;
        
        Jugador *nuevo = new Jugador;
        nuevo->elemento.nombre = n;
        nuevo->elemento.rol = r;
        nuevo->elemento.poder = p; 
        nuevo->siguiente = NULL;
        
        tail->siguiente = nuevo;
        tail = nuevo;
        listSize++;
        return listSize - 1;
    }

    /*****
    * int calcular_poder
    ******
    * Suma el poder de combate. Ignora el poder de los entrenadores (rol == 1)[cite: 2, 8].
    *****/
    int calcular_poder() {
        int suma = 0;
        Jugador *aux = head->siguiente;
        while (aux != NULL) {
            if (aux->elemento.rol == 0) { // Solo suma a los jugadores[cite: 8]
                suma += (int)aux->elemento.poder;
            }
            aux = aux->siguiente;
        }
        return suma;
    }

    /*****
    * void imprimir_equipo
    ******
    * Muestra la información detallada del equipo[cite: 2, 8].
    *****/
    void imprimir_equipo() {
        cout << "Equipo: " << nombre << endl;
        Jugador *aux = head->siguiente;
        int i = 0;
        while (aux != NULL) {
            if (aux->elemento.rol == 1) {
                cout << "Persona " << i << ": " << aux->elemento.nombre << " true " << aux->elemento.poder << endl;
            } else {
                cout << "Persona " << i << ": " << aux->elemento.nombre << " " << aux->elemento.rol << " " << aux->elemento.poder << endl;
            }
            aux = aux->siguiente;
            i++;
        }
    }
};

/*****
* bool batallar
******
* Compara el poder de dos equipos. Otorga la victoria al de la izquierda si hay empate[cite: 8].
*****/
bool batallar(Equipo &a, Equipo &b) {
    return a.calcular_poder() >= b.calcular_poder();
}

/*****
* struct Competidor
******
* Nodo del árbol binario que representa un espacio en el torneo[cite: 6, 8].
*****/
struct Competidor {
    Equipo equipo;
    Competidor *izq;
    Competidor *der;
    bool ocupado;
};

/*****
* class Torneo
******
* Implementa un árbol binario para gestionar los enfrentamientos[cite: 1, 6, 8].
*****/
class Torneo {
private:
    Competidor *raiz;
    int nElems;

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

    /*****
    * void imprimir_nodos_match
    ******
    * Imprime los combates de un nivel específico con el formato requerido[cite: 8].
    *****/
    void imprimir_nodos_match(Competidor* nodo, int nivel_actual, int nivel_padre, bool& primero) {
        if (!nodo) return;
        if (nivel_actual == nivel_padre) {
            if (!primero) cout << " | ";
            
            if (nodo->izq && nodo->izq->ocupado)
                cout << nodo->izq->equipo.getNombre() << " " << nodo->izq->equipo.calcular_poder();
            else
                cout << "--";
            
            cout << " vs ";
            
            if (nodo->der && nodo->der->ocupado)
                cout << nodo->der->equipo.getNombre() << " " << nodo->der->equipo.calcular_poder();
            else
                cout << "--";

            primero = false;
            return;
        }
        imprimir_nodos_match(nodo->izq, nivel_actual + 1, nivel_padre, primero);
        imprimir_nodos_match(nodo->der, nivel_actual + 1, nivel_padre, primero);
    }

    void avanzar_ronda_rec(Competidor *nodo) {
        if (nodo == NULL) return;
        if (nodo->ocupado) return; 

        if (nodo->izq && nodo->izq->ocupado && nodo->der && nodo->der->ocupado) {
            if (batallar(nodo->izq->equipo, nodo->der->equipo))
                nodo->equipo = nodo->izq->equipo;
            else
                nodo->equipo = nodo->der->equipo;
            
            nodo->ocupado = true;
        } else {
            avanzar_ronda_rec(nodo->izq);
            avanzar_ronda_rec(nodo->der);
        }
    }

    void destruir_arbol(Competidor *nodo) {
        if (nodo == NULL) return;
        destruir_arbol(nodo->izq);
        destruir_arbol(nodo->der);
        delete nodo;
    }

public:
    Torneo() { raiz = NULL; nElems = 0; }
    
    ~Torneo() { destruir_arbol(raiz); }

    void crear_torneo(Equipo* equipos, int n) {
        nElems = n;
        int niveles = 0;
        int temp = n;
        while (temp > 1) { temp /= 2; niveles++; }
        raiz = construir_arbol_vacio(niveles);
        int index = 0;
        llenar_hojas(raiz, equipos, index);
    }

    /*****
    * void imprimir_bracket
    ******
    * Muestra el bracket nivel por nivel desde la raíz hacia las hojas[cite: 8].
    *****/
    void imprimir_bracket() {
        if (raiz == NULL) return;
        
        int h = 0;
        Competidor* temp = raiz;
        while(temp->izq != NULL) {
            h++;
            temp = temp->izq;
        }
        
        for (int i = 0; i <= h; i++) {
            if (i == 0) {
                if (raiz->ocupado) cout << raiz->equipo.getNombre() << " " << raiz->equipo.calcular_poder() << endl;
                else cout << "--" << endl;
            } else {
                Competidor* aux = raiz;
                for(int j = 0; j < i; j++) aux = aux->izq;
                
                if (aux->ocupado) cout << (1 << i) << endl; 
                
                bool primero = true;
                imprimir_nodos_match(raiz, 0, i - 1, primero);
                cout << endl;
            }
        }
    }

    /*****
    * void avanzar_ronda
    ******
    * Avanza una fase o termina el programa si se intenta avanzar con torneo finalizado[cite: 8].
    *****/
    void avanzar_ronda() {
        if (raiz == NULL) return;
        
        if (raiz->ocupado) {
            imprimir_bracket();
            exit(0);
        }
        
        avanzar_ronda_rec(raiz);
    }

    /*****
    * void buscar_k_rec
    ******
    * Búsqueda para imprimir equipos con poder >= k[cite: 6, 8].
    * Solo evalúa las hojas para no imprimir equipos duplicados que ganaron rondas.
    *****/
    void buscar_k_rec(Competidor *nodo, int k) {
        if (nodo == NULL) return;
        
        if (nodo->izq == NULL && nodo->der == NULL) {
            if (nodo->ocupado && nodo->equipo.calcular_poder() >= k) {
                nodo->equipo.imprimir_equipo();
            }
            return; 
        }
        
        buscar_k_rec(nodo->izq, k);
        buscar_k_rec(nodo->der, k);
    }

    void buscar_k(int k) { buscar_k_rec(raiz, k); }
};

/*****
* int main
******
* Carga los datos y gestiona el flujo del usuario[cite: 8].
*****/
int main() {
    ifstream archivo("Equipos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir Equipos.txt" << endl;
        return 1;
    }

    int n;
    if (!(archivo >> n)) return 1;

    Equipo *lista = new Equipo[n];

    for (int i = 0; i < n; i++) {
        int p; 
        string nombreE;
        archivo >> p >> nombreE;
        lista[i].limpiar();
        lista[i].setNombre(nombreE);
        
        for (int j = 0; j < p; j++) {
            string nomP; 
            int rol; 
            float pod;
            archivo >> nomP >> rol >> pod;
            lista[i].agregar_companero(nomP, rol, pod);
        }
    }
    archivo.close();

    Torneo t;
    t.crear_torneo(lista, n);

    int op;
    while (cin >> op) {
        if (op == 1) t.avanzar_ronda();
        else if (op == 2) t.imprimir_bracket();
        else if (op == 3) { int k; cin >> k; t.buscar_k(k); }
    }

    delete[] lista;
    return 0;
}
