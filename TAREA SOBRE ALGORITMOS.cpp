#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// ==========================================
// 1. FUNCIONES DE LA INTERFAZ VISUAL
// ==========================================

// Muestra el panel inicial con las instrucciones del programa
void mostrarBienvenida() {
    cout << "\n=================================================================\n";
    cout << "             HOLA! BIENVENIDO AL SISTEMA DE ORDENAMIENTO         \n";
    cout << "=================================================================\n";
    cout << " ESTE PROGRAMA ESTA ELABORADO PARA:\n";
    cout << " Generar listas de numeros al azar y organizarlos de menor a \n";
    cout << " mayor utilizando diferentes metodos, denominados algoritmos,\n";
    cout << " para evaluar su eficiencia y velocidad.\n\n";
    cout << " NOTA (Pasos a seguir para utilizar el sistema):\n";
    cout << "  1. Entra a 'Gestion de Datos' para crear tu lista paso a paso.\n";
    cout << "  2. Ve a 'Opciones de Ordenamiento' para probar un solo metodo.\n";
    cout << "  3. En 'Analisis y Rendimiento' podras hacer carreras de velocidad.\n";
    cout << "  4. Ejecucion Automatica: Selecciona esta opcion para que el programa\n";
    cout << "     realice todo el trabajo por ti. Primero generara los datos, luego\n";
    cout << "     medira el tiempo de todos los algoritmos en una competencia, y\n";
    cout << "     finalmente te mostrara los resultados ordenados en un solo paso.\n";
    cout << "=================================================================\n\n";
}

// Imprime los numeros en pantalla (con un limite para no saturar la consola)
void displayArray(const vector<int>& arr, int limit = 50) {
    cout << "[ ";
    for (size_t i = 0; i < arr.size() && i < limit; i++) {
        cout << arr[i] << (i < arr.size() - 1 && i < limit - 1 ? ", " : "");
    }
    cout << " ]\n";
    if (arr.size() > limit) {
        cout << "... (Hay " << arr.size() - limit << " numeros mas ocultos)\n";
    }
}

// ==========================================
// 2. ALGORITMOS DE ORDENAMIENTO
// ==========================================

// --- ALGORITMO BUBBLE SORT ---
// Compara de dos en dos y empuja el mayor hacia el final de la lista
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// --- ALGORITMO INSERTION SORT ---
// Toma un numero y lo arrastra hacia la izquierda hasta encontrar su lugar
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// --- ALGORITMO SELECTION SORT ---
// Busca el numero mas pequeno de toda la lista y lo pone al inicio
void selectionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) { min_idx = j; }
        }
        swap(arr[i], arr[min_idx]);
    }
}

// --- ALGORITMO MERGE SORT (Funciones auxiliares) ---
// Divide la lista por la mitad repetidamente y luego une las partes ya ordenadas
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// --- ALGORITMO QUICK SORT (Funciones auxiliares) ---
// Elige un numero guia (pivote) y pone los menores a un lado y mayores al otro
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// --- ALGORITMO COUNTING SORT ---
// Ordena los datos contando cuantas veces se repite cada numero
void countingSort(vector<int>& arr) {
    if (arr.empty()) return;
    int max_val = arr[0];
    for (int num : arr) { if (num > max_val) max_val = num; }

    vector<int> count(max_val + 1, 0);
    vector<int> output(arr.size());

    for (int num : arr) count[num]++;
    for (int i = 1; i <= max_val; i++) count[i] += count[i - 1];

    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;
    }
    for (size_t i = 0; i < arr.size(); i++) arr[i] = output[i];
}

// ==========================================
// 3. MEDICIÓN DE TIEMPO
// ==========================================

// Calcula los milisegundos exactos que tarda un algoritmo en terminar
double measureTime(int option, vector<int>& arr) {
    auto start = high_resolution_clock::now();

    switch(option) {
        case 1: bubbleSort(arr); break;
        case 2: insertionSort(arr); break;
        case 3: selectionSort(arr); break;
        case 4: mergeSort(arr, 0, arr.size() - 1); break;
        case 5: quickSort(arr, 0, arr.size() - 1); break;
        case 6: countingSort(arr); break;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    return duration.count() / 1000.0;
}

// ==========================================
// 4. MÓDULOS DE LOS MENÚS SECUNDARIOS
// ==========================================

// Submenu para crear y visualizar los numeros al azar
void subMenuDatos(vector<int>& original_arr) {
    int opcion;
    do {
        cout << "\n--- [ SUBMENU: GESTION DE DATOS ] ---\n";
        cout << " 1. Generar numeros aleatorios\n";
        cout << " 2. Ver numeros actuales en memoria\n";
        cout << " 3. <-- Regresar al Menu Principal\n";
        cout << " Elija uno de los numeros brindados en las opciones.\n";
        cout << " Opcion: ";

        // Evita que el programa falle si el usuario escribe letras
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch(opcion) {
            case 1: {
                int n, max_val;
                cout << "\nCantidad de numeros a generar: "; cin >> n;
                cout << "Valor maximo permitido (ej. 1000): "; cin >> max_val;

                if (n <= 0 || max_val <= 0 || max_val > 1000000) {
                    cout << "[-] Valores invalidos. Maximo 1,000,000.\n"; break;
                }

                // Generador de numeros aleatorios
                original_arr.resize(n);
                random_device rd; mt19937 gen(rd());
                uniform_int_distribution<> distrib(0, max_val);
                for (int i = 0; i < n; i++) original_arr[i] = distrib(gen);

                cout << "\n[+] EXITO: " << n << " numeros guardados.\n";
                displayArray(original_arr);
                break;
            }
            case 2:
                if (original_arr.empty()) cout << "\n[-] Memoria vacia. Genera datos primero (Opcion 1).\n";
                else { cout << "\n"; displayArray(original_arr, 100); }
                break;
            case 3: cout << "\n[ Regresando... ]\n"; break;
            default: cout << "\n[-] Opcion no valida.\n";
        }
    } while (opcion != 3);
}

// Submenu para elegir un algoritmo especifico y ordenar la lista
void subMenuOrdenamiento(const vector<int>& original_arr, vector<int>& last_sorted_arr) {
    if (original_arr.empty()) {
        cout << "\n[-] ALERTA: No hay datos para ordenar. Ve a Gestion de Datos primero.\n";
        return;
    }

    int opcion;
    do {
        cout << "\n--- [ SUBMENU: OPCIONES DE ORDENAMIENTO ] ---\n";
        cout << " 1. Elegir algoritmo y ordenar\n";
        cout << " 2. Ver el ultimo resultado ordenado\n";
        cout << " 3. <-- Regresar al Menu Principal\n";
        cout << " Elija uno de los numeros brindados en las opciones.\n";
        cout << " Opcion: ";

        if (!(cin >> opcion)) {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue;
        }

        switch(opcion) {
            case 1: {
                cout << "\n ALGORITMOS DISPONIBLES:\n";
                cout << "  1. Bubble Sort      4. Merge Sort\n";
                cout << "  2. Insertion Sort   5. Quick Sort\n";
                cout << "  3. Selection Sort   6. Counting Sort\n";
                cout << " Elige el metodo (1-6): ";
                int method;
                if (!(cin >> method) || method < 1 || method > 6) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "[-] Metodo invalido.\n"; break;
                }

                if (original_arr.size() > 20000 && method <= 3)
                    cout << "\n[!] Procesando un algoritmo lento para muchos datos. Espera...\n";

                // Copia la lista original para ordenarla sin borrar la generada al azar
                last_sorted_arr = original_arr;
                double t = measureTime(method, last_sorted_arr);

                cout << "\n[+] ORDENAMIENTO COMPLETADO en " << fixed << setprecision(4) << t << " ms\n";
                displayArray(last_sorted_arr);
                break;
            }
            case 2:
                if (last_sorted_arr.empty()) cout << "\n[-] No has ordenado nada todavia.\n";
                else { cout << "\n"; displayArray(last_sorted_arr, 100); }
                break;
            case 3: cout << "\n[ Regresando... ]\n"; break;
            default: cout << "\n[-] Opcion no valida.\n";
        }
    } while (opcion != 3);
}

// Submenu para poner a competir todos los metodos al mismo tiempo
void subMenuAnalisis(const vector<int>& original_arr) {
    if (original_arr.empty()) {
        cout << "\n[-] ALERTA: No hay datos para analizar. Ve a Gestion de Datos primero.\n";
        return;
    }

    cout << "\n--- [ SUBMENU: ANALISIS Y RENDIMIENTO ] ---\n";
    cout << "Ejecutando competencia entre algoritmos...\n";

    if (original_arr.size() > 20000) cout << "[!] Esto tomara algo de tiempo debido a la cantidad de datos.\n";

    string names[] = {"Bubble Sort", "Insertion Sort", "Selection Sort", "Merge Sort", "Quick Sort", "Counting Sort"};
    vector<pair<double, string>> resultados;

    // Pone a prueba cada algoritmo uno por uno y guarda su tiempo
    for (int i = 1; i <= 6; i++) {
        vector<int> temp_arr = original_arr;
        double t = measureTime(i, temp_arr);
        resultados.push_back(make_pair(t, names[i-1]));
    }

    sort(resultados.begin(), resultados.end());

    // Imprime la tabla final de ganadores
    cout << "\n=============================================\n";
    cout << " POSICION | ALGORITMO          | TIEMPO (ms)  \n";
    cout << "=============================================\n";
    for(size_t i = 0; i < resultados.size(); i++) {
        cout << "   #" << i+1 << "    | " << left << setw(18) << resultados[i].second << " | " << fixed << setprecision(4) << resultados[i].first << " ms\n";
    }
    cout << "=============================================\n";
    cout << "\n[ Presiona Enter para regresar al Menu Principal ]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
}

// Submenu que unifica los pasos de generacion, medicion y ordenamiento de golpe
void subMenuEjecucionCompleta(vector<int>& original_arr, vector<int>& last_sorted_arr) {
    cout << "\n--- [ EJECUCION AUTOMATICA (TODO DE GOLPE) ] ---\n";
    cout << " INSTRUCCION OBLIGATORIA: Para que el sistema pueda realizar\n";
    cout << " todo el proceso de forma automatica, es estrictamente necesario\n";
    cout << " que ingreses los dos valores que se te piden a continuacion.\n\n";

    int n, max_val;
    cout << " Paso 1: Cantidad de numeros a generar: ";
    if (!(cin >> n)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }

    cout << " Paso 2: Valor maximo permitido (ej. 1000): ";
    if (!(cin >> max_val)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }

    if (n <= 0 || max_val <= 0 || max_val > 1000000) {
        cout << "\n[-] Valores invalidos. Cancelando operacion.\n";
        return;
    }

    // Paso 1: Crear los numeros al azar
    original_arr.resize(n);
    random_device rd; mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, max_val);
    for (int i = 0; i < n; i++) original_arr[i] = distrib(gen);

    cout << "\n[+] DATOS GENERADOS CON EXITO:\n";
    displayArray(original_arr);

    // Paso 2: Medir los tiempos de todos los algoritmos
    cout << "\n[+] PROCESANDO BENCHMARK DE ALGORITMOS...\n";
    if (original_arr.size() > 20000) cout << "    (Calculando miles de datos, por favor espera un momento)\n";

    string names[] = {"Bubble Sort", "Insertion Sort", "Selection Sort", "Merge Sort", "Quick Sort", "Counting Sort"};
    vector<pair<double, string>> resultados;
    for (int i = 1; i <= 6; i++) {
        vector<int> temp_arr = original_arr;
        double t = measureTime(i, temp_arr);
        resultados.push_back(make_pair(t, names[i-1]));
    }
    sort(resultados.begin(), resultados.end());

    cout << "\n=============================================\n";
    cout << " POSICION | ALGORITMO          | TIEMPO (ms)  \n";
    cout << "=============================================\n";
    for(size_t i = 0; i < resultados.size(); i++) {
        cout << "   #" << i+1 << "    | " << left << setw(18) << resultados[i].second << " | " << fixed << setprecision(4) << resultados[i].first << " ms\n";
    }
    cout << "=============================================\n";

    // Paso 3: Ordenar la lista final y mostrarla
    cout << "\n[+] MUESTRA DEL ARREGLO FINAL ORDENADO:\n";
    last_sorted_arr = original_arr;
    quickSort(last_sorted_arr, 0, last_sorted_arr.size() - 1);
    displayArray(last_sorted_arr, 100);

    cout << "\n[ Presiona Enter para regresar al Menu Principal ]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
}

// ==========================================
// 5. BUCLE PRINCIPAL (INICIO DEL PROGRAMA)
// ==========================================

// Punto de partida de toda la aplicacion
int main() {
    vector<int> original_arr;
    vector<int> last_sorted_arr;
    int main_choice;

    mostrarBienvenida();

    // Bucle para mantener la consola abierta hasta que se elija salir
    do {
        cout << "\n=================================================\n";
        cout << " [ MENU PRINCIPAL ] \n";
        cout << " Datos en Memoria: " << (original_arr.empty() ? "Vacio" : to_string(original_arr.size()) + " numeros listos") << "\n";
        cout << "-------------------------------------------------\n";
        cout << " 1. Gestion de Datos (Crear los numeros)\n";
        cout << " 2. Opciones de Ordenamiento (Organizar)\n";
        cout << " 3. Analisis y Rendimiento (Carrera de velocidad)\n";
        cout << " 4. Ejecucion Automatica (Hacer todo de golpe)\n";
        cout << " 5. Salir del programa\n";
        cout << "=================================================\n";
        cout << " Elija uno de los numeros brindados en las opciones.\n";
        cout << " Opcion: ";

        if (!(cin >> main_choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Envia al usuario al menu correspondiente segun su eleccion
        switch (main_choice) {
            case 1: subMenuDatos(original_arr); break;
            case 2: subMenuOrdenamiento(original_arr, last_sorted_arr); break;
            case 3: subMenuAnalisis(original_arr); break;
            case 4: subMenuEjecucionCompleta(original_arr, last_sorted_arr); break;
            case 5: cout << "\nSaliendo del programa. Hasta pronto!\n"; break;
            default: cout << "\n[-] Opcion no valida. Intenta con un numero del 1 al 5.\n";
        }
    } while (main_choice != 5);

    return 0;
}