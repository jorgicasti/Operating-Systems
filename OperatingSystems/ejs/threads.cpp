#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <mutex>



using namespace std;

mutex mtx;
// Variables compartidas entre los hilos
long long T1 = 0;   // Suma
long long T2 = 1;   // Producto
int T3 = 0;         // Número aleatorio

/* para part A
// Hilo 1: calcula la suma del array
void calcularSuma(const vector<int>& array) {
    T1 = 0;
    for (int num : array) {
        T1 += num;
    }
}*/

/*para part A
// Hilo 2: calcula el producto del array
void calcularProducto(const vector<int>& array) {
    T2 = 1;
    for (int num : array) {
        T2 *= num;
    }
}
*/


// Hilo 3: genera un número aleatorio entre -1000 y 1000
void generarNumeroAleatorio() {
    T3 = rand() % 2001 - 1000;
}

// Comparamos
bool comparar(pair<string, long long> a, pair<string, long long> b) {
    return a.second > b.second;
}

// Hilo 4: ordena los resultados T1, T2 y T3
void rankingResultados() {
    vector<pair<string, long long>> resultados;

    resultados.push_back({"T1 (Suma)", T1});
    resultados.push_back({"T2 (Producto)", T2});
    resultados.push_back({"T3 (Aleatorio)", T3});

    sort(resultados.begin(), resultados.end(), comparar);

    cout << "\nRanking de resultados (orden decreciente):\n";
    for (int i = 0; i < resultados.size(); i++) {
        cout << resultados[i].first << " = " << resultados[i].second << endl;
    }
}
//------------------------------------------------------------
// Nueva funcion para modificar la mitad del array -- PART B
void modificarArray(vector<int>& array) {
    int N = array.size();
    int cambios = N / 2;

    cout << "  -> Modificando " << cambios << " posiciones del array\n";

    for (int i = 0; i < cambios; i++) {
        int indice = rand() % N;
        int nuevoValor = rand() % 201 - 100;

        cout << "     Posicion " << indice
             << " cambia a " << nuevoValor << endl;

        array[indice] = nuevoValor;
    }
}


void calcularSuma(vector<int>& array) {
    cout << "[T1] Esperando mutex...\n";
    mtx.lock();

    cout << "[T1] Mutex adquirido\n";
    cout << "[T1] Modificando array\n";
    modificarArray(array);

    T1 = 0;
    for (int num : array) {
        T1 += num;
    }

    cout << "[T1] Suma calculada: " << T1 << endl;
    cout << "[T1] Liberando mutex\n";

    mtx.unlock();
}


void calcularProducto(vector<int>& array) {
    cout << "[T2] Esperando mutex...\n";
    mtx.lock();

    cout << "[T2] Mutex adquirido\n";
    cout << "[T2] Modificando array\n";
    modificarArray(array);

    T2 = 1;
    for (int num : array) {
        T2 *= num;
    }

    cout << "[T2] Producto calculado: " << T2 << endl;
    cout << "[T2] Liberando mutex\n";

    mtx.unlock();
}



int main() {
    srand(time(nullptr)); // Inicializar semilla aleatoria

    int N;
    cout << "Introduce el tamaño del array: ";
    cin >> N;

    vector<int> array(N);

    // Rellenar el array con valores aleatorios entre -100 y 100
    for (int i = 0; i < N; i++) {
        array[i] = rand() % 201 - 100;
    }

    cout << "\nArray generado:\n";
    for (int i = 0; i < N; i++) {
        cout << array[i] << " ";
    }
    cout << endl;

    // Crear los primeros 3 hilos
    //cref para part a 
    thread t1(calcularSuma, ref(array));
    thread t2(calcularProducto, ref(array));
    thread t3(generarNumeroAleatorio);

    // Esperar a que terminen
    t1.join();
    t2.join();
    t3.join();
    
    // Crear el cuarto hilo (ranking)
    thread t4(rankingResultados);
    t4.join();

    return 0;
}
