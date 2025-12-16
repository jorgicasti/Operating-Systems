#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <algorithm>
#include <chrono>

using namespace std;

// Variables globales compartidas
mutex mtx;  // Para sincronización
vector<int> arr;  // El array compartido
long long T1, T2, T3;  // Resultados de los hilos
bool isPartB = false;  // Bandera para indicar si es Parte A o B

// Función para generar número aleatorio en un rango
int generarRandom(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Función del Hilo 1: Calcular suma
void calcularSuma() {
    long long suma = 0;
    
    if (isPartB) {
        // PARTE B: Modificar mitad del array y luego sumar
        mtx.lock();  // Bloquear acceso al array
        
        cout << "Hilo 1 (T1) va a modificar la mitad del array...\n";
        
        // Seleccionar aleatoriamente la mitad de los elementos
        int n = arr.size();
        vector<bool> seleccionados(n, false);
        int modificados = 0;
        
        while (modificados < n / 2) {
            int idx = generarRandom(0, n - 1);
            if (!seleccionados[idx]) {
                seleccionados[idx] = true;
                arr[idx] = generarRandom(-100, 100);  // Nuevo valor aleatorio
                modificados++;
            }
        }
        
        cout << "Hilo 1 modifico " << modificados << " elementos\n";
        
        // Calcular suma después de modificar
        for (int num : arr) {
            suma += num;
        }
        
        mtx.unlock();  // Liberar acceso al array
    } else {
        // PARTE A: Solo calcular suma sin modificar
        for (int num : arr) {
            suma += num;
        }
    }
    
    T1 = suma;
    cout << "Hilo 1 terminado. T1 (suma) = " << T1 << endl;
}

// Función del Hilo 2: Calcular producto
void calcularProducto() {
    long long producto = 1;
    
    if (isPartB) {
        // PARTE B: Modificar mitad del array y luego multiplicar
        mtx.lock();  // Bloquear acceso al array
        
        cout << "Hilo 2 (T2) va a modificar la mitad del array...\n";
        
        // Seleccionar aleatoriamente la mitad de los elementos
        int n = arr.size();
        vector<bool> seleccionados(n, false);
        int modificados = 0;
        
        while (modificados < n / 2) {
            int idx = generarRandom(0, n - 1);
            if (!seleccionados[idx]) {
                seleccionados[idx] = true;
                arr[idx] = generarRandom(-100, 100);  // Nuevo valor aleatorio
                modificados++;
            }
        }
        
        cout << "Hilo 2 modifico " << modificados << " elementos\n";
        
        // Calcular producto después de modificar
        producto = 1;
        for (int num : arr) {
            producto *= num;
        }
        
        mtx.unlock();  // Liberar acceso al array
    } else {
        // PARTE A: Solo calcular producto sin modificar
        producto = 1;
        for (int num : arr) {
            producto *= num;
        }
    }
    
    T2 = producto;
    cout << "Hilo 2 terminado. T2 (producto) = " << T2 << endl;
}

// Función del Hilo 3: Generar número aleatorio
void generarRandomNumber() {
    T3 = generarRandom(-1000, 1000);
    cout << "Hilo 3 terminado. T3 (random) = " << T3 << endl;
}

// Función del Hilo 4: Ordenar resultados
void ordenarResultados() {
    // Esperar a que T1, T2, T3 estén listos
    this_thread::sleep_for(chrono::milliseconds(100));
    
    // Crear vector con pares (valor, nombre)
    vector<pair<long long, string>> resultados;
    resultados.push_back({T1, "T1"});
    resultados.push_back({T2, "T2"});
    resultados.push_back({T3, "T3"});
    
    // Ordenar de mayor a menor
    sort(resultados.begin(), resultados.end(), 
         [](const pair<long long, string>& a, const pair<long long, string>& b) {
             return a.first > b.first;
         });
    
    cout << "\n=== RESULTADOS ORDENADOS ===" << endl;
    cout << "Orden (de mayor a menor): ";
    for (const auto& res : resultados) {
        cout << res.second << "(" << res.first << ") ";
    }
    cout << endl;
}

// Función principal
int main() {
    int N;
    int opcion;
    
    cout << "=== PROGRAMA DE HILOS CON ARRAY COMPARTIDO ===\n\n";
    
    // Pedir tamaño del array
    cout << "Ingrese el tamaño del array (N): ";
    cin >> N;
    
    // Inicializar array con valores aleatorios
    arr.resize(N);
    for (int i = 0; i < N; i++) {
        arr[i] = generarRandom(-100, 100);
    }
    
    // Mostrar array inicial
    cout << "\nArray inicial (primeros 10 elementos): ";
    for (int i = 0; i < min(10, N); i++) {
        cout << arr[i] << " ";
    }
    if (N > 10) cout << "...";
    cout << endl;
    
    // Seleccionar parte A o B
    cout << "\nSeleccione la parte a ejecutar:\n";
    cout << "1. Parte A (sin sincronizacion)\n";
    cout << "2. Parte B (con sincronizacion)\n";
    cout << "Opcion: ";
    cin >> opcion;
    
    if (opcion == 2) {
        isPartB = true;
        cout << "\n=== EJECUTANDO PARTE B (CON SINCRONIZACION) ===\n";
    } else {
        cout << "\n=== EJECUTANDO PARTE A (SIN SINCRONIZACION) ===\n";
    }
    
    cout << "\nCreando 4 hilos...\n";
    
    // Crear los 4 hilos
    thread hilo1(calcularSuma);
    thread hilo2(calcularProducto);
    thread hilo3(generarRandomNumber);
    thread hilo4(ordenarResultados);
    
    // Esperar a que todos los hilos terminen
    hilo1.join();
    hilo2.join();
    hilo3.join();
    hilo4.join();
    
    // Mostrar array final si es Parte B
    if (isPartB) {
        cout << "\nArray final (primeros 10 elementos): ";
        for (int i = 0; i < min(10, N); i++) {
            cout << arr[i] << " ";
        }
        if (N > 10) cout << "...";
        cout << endl;
    }
    
    cout << "\n=== PROGRAMA TERMINADO ===\n";
    
    return 0;
}