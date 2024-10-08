#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// Función que calcula la suma de una sublista
int suma_parcial(const vector<int>& lista, int inicio, int fin) {
    int suma = 0;
    for (int i = inicio; i < fin; i++) {
        suma += lista[i];
    }
    return suma;
}

int main() {
    // Generar una lista de números ejemplo
    int n = 10;  // número de elementos en la lista
    vector<int> lista(n);

    // Inicializar la lista con valores del 1 al n
    for (int i = 0; i < n; i++) {
        lista[i] = i + 1;
    }

    // Variable para almacenar la suma total
    int suma_total = 0;

    // Definir el número de hilos y la partición
    int num_hilos = 4;
    omp_set_num_threads(num_hilos);  // Establecer el número de hilos

    // Suma total usando Fork-Join
    #pragma omp parallel
    {
        int id_hilo = omp_get_thread_num();  // Obtener el ID del hilo
        int n_hilos = omp_get_num_threads(); // Número total de hilos

        // Dividir la lista en partes iguales entre los hilos
        int tam_parte = n / n_hilos;
        int inicio = id_hilo * tam_parte;
        int fin = (id_hilo == n_hilos - 1) ? n : inicio + tam_parte;

        // Cada hilo calcula su suma parcial
        int suma_local = suma_parcial(lista, inicio, fin);

        // Imprimir la suma parcial de cada sublista
        #pragma omp critical
        {
            cout << "Hilo " << id_hilo << ": Suma parcial de la sublista (" 
                 << inicio << " a " << fin - 1 << ") = " << suma_local << endl;
        }

        // Combinar las sumas locales en la suma total
        #pragma omp critical
        {
            suma_total += suma_local;
        }
    }

    // Mostrar la suma total
    cout << "La suma total es: " << suma_total << endl;

    return 0;
}