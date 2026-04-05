#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()

using namespace std;

// Definicion struct solicitado
struct Certamen {
    string tipo;
    string ramo;
    int nro_preguntas;
    string *preguntas;
    string *respuestas;
};

/*****
* Certamen* extraccionDatos
******
* Extrae la informacion de archivos de texto y la almacena en memoria dinamica.
******
* Input:
* int nroArchivos : Cantidad de certamenes (archivos) a cargar.
******
* Returns:
* Certamen*, Puntero al primer elemento del arreglo dinamico de certamenes.
*****/
Certamen *extraccionDatos(int nroArchivos) {
    Certamen *certamenes = new Certamen[nroArchivos];
    string nombreArchivo;

    for (int i = 0; i < nroArchivos; i++) {
        cout << "Ingrese el nombre del archivo " << i + 1 << ":" << endl << ">";
        cin >> nombreArchivo;

        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo " << nombreArchivo << ". Saliendo..." << endl;
            exit(1); 
        }

        // Lectura de cabecera
        getline(archivo, certamenes[i].tipo);
        if (certamenes[i].tipo.empty() || certamenes[i].tipo == "\r") {
            getline(archivo, certamenes[i].tipo);
        }
        
        getline(archivo, certamenes[i].ramo);
        
        string nroStr;
        getline(archivo, nroStr);
        certamenes[i].nro_preguntas = stoi(nroStr);

        // Asignacion de memoria para los arreglos internos
        certamenes[i].preguntas = new string[certamenes[i].nro_preguntas];
        certamenes[i].respuestas = new string[certamenes[i].nro_preguntas];

        // Lectura de preguntas y respuestas
        for (int j = 0; j < certamenes[i].nro_preguntas; j++) {
            getline(archivo, certamenes[i].preguntas[j]);
            if (certamenes[i].tipo.find("Desarrollo") == string::npos) {
                getline(archivo, certamenes[i].respuestas[j]);
            } else {
                certamenes[i].respuestas[j] = "";
            }
        }
        archivo.close();
    }
    return certamenes;
}

/*****
* void nroCertTipo
******
* Cuenta la cantidad de certamenes por tipo en cada ramo usando punteros.
******
* Input:
* int* tipoIntroC : Arreglo contador para Introduccion al Calculo
* int* tipoAlgrGeo : Arreglo contador para Algebra y Geometria
* int* tipoIntroF : Arreglo contador para Introduccion a la Fisica
* int* tipoCalc : Arreglo contador para Calculo en una Variable
* int* tipoAlgrL : Arreglo contador para Algebra Lineal
* int* tipoMeca : Arreglo contador para Fisica General: Mecanica
* Certamen* certamenes : Arreglo dinamico con todos los certamenes
* int nroArchivos : Cantidad total de certamenes cargados
******
* Returns:
* void, No retorna valor, modifica variables por referencia de puntero
*****/
void nroCertTipo(int* tipoIntroC, int* tipoAlgrGeo, int* tipoIntroF, 
                 int* tipoCalc, int* tipoAlgrL, int* tipoMeca, 
                 Certamen *certamenes, int nroArchivos) {
    
    for (int i = 0; i < nroArchivos; i++) {
        int indiceTipo = 0;
        if (certamenes[i].tipo.find("Alternativas") != string::npos) indiceTipo = 0;
        else if (certamenes[i].tipo.find("Desarrollo") != string::npos) indiceTipo = 1;
        else if (certamenes[i].tipo.find("Verdadero y Falso") != string::npos) indiceTipo = 2;
        else if (certamenes[i].tipo.find("Completacion") != string::npos) indiceTipo = 3;

        // Busqueda por fragmentos seguros sin tildes
        if (certamenes[i].ramo.find("al C") != string::npos && certamenes[i].ramo.find("Introducc") != string::npos) tipoIntroC[indiceTipo]++;
        else if (certamenes[i].ramo.find("Geometr") != string::npos) tipoAlgrGeo[indiceTipo]++;
        else if (certamenes[i].ramo.find("la F") != string::npos && certamenes[i].ramo.find("Introducc") != string::npos) tipoIntroF[indiceTipo]++;
        else if (certamenes[i].ramo.find("una Variable") != string::npos) tipoCalc[indiceTipo]++;
        else if (certamenes[i].ramo.find("Lineal") != string::npos) tipoAlgrL[indiceTipo]++;
        else if (certamenes[i].ramo.find("Mec") != string::npos) tipoMeca[indiceTipo]++;
    }
}

/*****
* void nroCertRamo
******
* Cuenta la cantidad de certamenes totales por ramo usando referencias.
******
* Input:
* int& nroIntroC : Referencia a contador de Intro. al Calculo
* int& nroAlgrGeo : Referencia a contador de Algebra y Geometria
* int& nroIntroF : Referencia a contador de Intro. a la Fisica
* int& nroCalc : Referencia a contador de Calculo en una Variable
* int& nroAlgrL : Referencia a contador de Algebra Lineal
* int& nroMeca : Referencia a contador de Mecanica
* Certamen* certamenes : Arreglo dinamico con todos los certamenes
* int nroArchivos : Cantidad total de certamenes cargados
******
* Returns:
* void, No retorna valor, modifica contadores mediante referencias
*****/
void nroCertRamo(int &nroIntroC, int &nroAlgrGeo, int &nroIntroF,
                 int &nroCalc, int &nroAlgrL, int &nroMeca,
                 Certamen *certamenes, int nroArchivos) {
    
    nroIntroC = 0; nroAlgrGeo = 0; nroIntroF = 0; nroCalc = 0; nroAlgrL = 0; nroMeca = 0;
    
    for (int i = 0; i < nroArchivos; i++) {
        // Busqueda por fragmentos seguros sin tildes
        if (certamenes[i].ramo.find("al C") != string::npos && certamenes[i].ramo.find("Introducc") != string::npos) nroIntroC++;
        else if (certamenes[i].ramo.find("Geometr") != string::npos) nroAlgrGeo++;
        else if (certamenes[i].ramo.find("la F") != string::npos && certamenes[i].ramo.find("Introducc") != string::npos) nroIntroF++;
        else if (certamenes[i].ramo.find("una Variable") != string::npos) nroCalc++;
        else if (certamenes[i].ramo.find("Lineal") != string::npos) nroAlgrL++;
        else if (certamenes[i].ramo.find("Mec") != string::npos) nroMeca++;
    }
}

/*****
* void preguntaRandom
******
* Despliega el menu principal, permite elegir ramo y tipo, y muestra preguntas aleatorias.
******
* Input:
* Certamen* certamenes : Arreglo dinamico con todos los certamenes
* int nroArchivos : Cantidad total de certamenes cargados
******
* Returns:
* void, Modifica la salida estandar (consola)
*****/
void preguntaRandom(Certamen *certamenes, int nroArchivos) {
    string opcionRamo = "";
    
    while (opcionRamo != "0") {
        int cIntroC = 0, cAlgrGeo = 0, cIntroF = 0, cCalc = 0, cAlgrL = 0, cMeca = 0;
        nroCertRamo(cIntroC, cAlgrGeo, cIntroF, cCalc, cAlgrL, cMeca, certamenes, nroArchivos);

        cout << "\nTiene esta cantidad de certamenes:" << endl;
        cout << "a) " << cIntroC << " Introduccion al Calculo" << endl;
        cout << "b) " << cAlgrGeo << " Algebra y Geometria" << endl;
        cout << "c) " << cIntroF << " Introduccion a la Fisica" << endl;
        cout << "d) " << cCalc << " Calculo en una Variable" << endl;
        cout << "e) " << cAlgrL << " Algebra Lineal" << endl;
        cout << "f) " << cMeca << " Fisica General: Mecanica" << endl;

        cout << "\nIngrese la letra del ramo a revisar o 0 para salir:" << endl << ">";
        cin >> opcionRamo;

        if (opcionRamo == "0") break;

        // Mapeamos la opcion a fragmentos sin tildes
        string nombreRamoBuscado = "";
        if (opcionRamo == "a") nombreRamoBuscado = "al C"; 
        else if (opcionRamo == "b") nombreRamoBuscado = "Geometr"; 
        else if (opcionRamo == "c") nombreRamoBuscado = "la F"; 
        else if (opcionRamo == "d") nombreRamoBuscado = "una Variable"; 
        else if (opcionRamo == "e") nombreRamoBuscado = "Lineal"; 
        else if (opcionRamo == "f") nombreRamoBuscado = "Mec"; 

        string opcionTipo = "";
        cout << "\nIngrese letra del tipo de certamen (a: Alternativas, b: Desarrollo, c: V/F, d: Completacion) o 0 para salir:" << endl << ">";
        cin >> opcionTipo;
        if (opcionTipo == "0") continue;

        string nombreTipoBuscado = "";
        if (opcionTipo == "a") nombreTipoBuscado = "Alternativas";
        else if (opcionTipo == "b") nombreTipoBuscado = "Desarrollo";
        else if (opcionTipo == "c") nombreTipoBuscado = "Verdadero y Falso";
        else if (opcionTipo == "d") nombreTipoBuscado = "Completacion";

        int indexEncontrado = -1;
        for(int i = 0; i < nroArchivos; i++) {
            if (certamenes[i].ramo.find(nombreRamoBuscado) != string::npos && 
                certamenes[i].tipo.find(nombreTipoBuscado) != string::npos) {
                indexEncontrado = i;
                break;
            }
        }

        if (indexEncontrado == -1) {
            cout << "No hay certamenes disponibles para esta combinacion." << endl;
            continue; 
        }

        string avanzar = "1";
        while (avanzar == "1") {
            int maxPreguntas = certamenes[indexEncontrado].nro_preguntas;
            int randomIdx = rand() % maxPreguntas; 

            cout << "\nEnunciado: " << certamenes[indexEncontrado].preguntas[randomIdx] << endl;
            cout << "Ingrese 1 para avanzar y 0 para salir:" << endl << ">";
            cin >> avanzar;
            if (avanzar == "0") break;

            if (certamenes[indexEncontrado].tipo.find("Desarrollo") != string::npos) {
                cout << "Pregunta de desarrollo, no hay respuesta disponible" << endl;
            } else {
                cout << "La respuesta correcta es: " << certamenes[indexEncontrado].respuestas[randomIdx] << endl;
            }
            
            cout << "Ingrese 1 para avanzar y 0 para salir:" << endl << ">";
            cin >> avanzar;
        }
    }
}

int main() {
    // Generador de numeros aleatorios inicializado
    srand(time(0)); 

    cout << "Ingrese el numero de certamenes:\n>";
    int nroArchivos;
    cin >> nroArchivos;
    
    // 1. Extraer datos
    Certamen *misCertamenes = extraccionDatos(nroArchivos);

    // 2. Menu y aleatoriedad
    preguntaRandom(misCertamenes, nroArchivos);

    // 3. Limpieza estricta de memoria para evitar descuentos
    for (int i = 0; i < nroArchivos; i++) {
        delete[] misCertamenes[i].preguntas;
        delete[] misCertamenes[i].respuestas;
    }
    delete[] misCertamenes;

    return 0;
}
