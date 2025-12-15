#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <cmath>
#include <string>
#include <windows.h>

using namespace std;

// Estructura para representar el estado del puzzle
struct Estado {
    vector<vector<int>> tablero;
    int fila_vacia;
    int col_vacia;
    int g; // Costo desde el inicio
    int h; // Heurística (distancia Manhattan)
    int f; // f = g + h
    vector<string> movimientos;
    
    // Constructor por defecto - ORDEN CORRECTO DE INICIALIZACION
    Estado() : tablero(3, vector<int>(3, 0)), fila_vacia(0), col_vacia(0), g(0), h(0), f(0) {}
    
    // Operador para comparar estados en la cola de prioridad
    bool operator>(const Estado& otro) const {
        return f > otro.f;
    }
};

// Función para mostrar el tablero
void mostrarTablero(const vector<vector<int>>& tablero) {
    cout << "  +---+---+---+" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "  |";
        for (int j = 0; j < 3; j++) {
            if (tablero[i][j] == 0) {
                cout << "   |";
            } else {
                cout << " " << tablero[i][j] << " |";
            }
        }
        cout << endl;
        cout << "  +---+---+---+" << endl;
    }
}

// Función para calcular la distancia Manhattan
int calcularManhattan(const vector<vector<int>>& tablero, const vector<vector<int>>& objetivo) {
    int distancia = 0;
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int valor = tablero[i][j];
            if (valor != 0) {
                // Encontrar posición objetivo de este valor
                for (int x = 0; x < 3; x++) {
                    for (int y = 0; y < 3; y++) {
                        if (objetivo[x][y] == valor) {
                            distancia += abs(i - x) + abs(j - y);
                        }
                    }
                }
            }
        }
    }
    
    return distancia;
}

// Función para convertir tablero a string (para comparación)
string tableroAString(const vector<vector<int>>& tablero) {
    string s = "";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            s += to_string(tablero[i][j]);
        }
    }
    return s;
}

// Función para verificar si dos tableros son iguales
bool sonIguales(const vector<vector<int>>& t1, const vector<vector<int>>& t2) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (t1[i][j] != t2[i][j]) {
                return false;
            }
        }
    }
    return true;
}

// Función para generar estados vecinos
vector<Estado> generarVecinos(const Estado& actual, const vector<vector<int>>& objetivo) {
    vector<Estado> vecinos;
    
    // Movimientos posibles: arriba, abajo, izquierda, derecha
    int movimientos_fila[] = {-1, 1, 0, 0};
    int movimientos_col[] = {0, 0, -1, 1};
    string nombres_mov[] = {"ARRIBA", "ABAJO", "IZQUIERDA", "DERECHA"};
    
    for (int i = 0; i < 4; i++) {
        int nueva_fila = actual.fila_vacia + movimientos_fila[i];
        int nueva_col = actual.col_vacia + movimientos_col[i];
        
        // Verificar si el movimiento es válido
        if (nueva_fila >= 0 && nueva_fila < 3 && nueva_col >= 0 && nueva_col < 3) {
            Estado nuevo_estado;
            nuevo_estado.tablero = actual.tablero;
            
            // Intercambiar espacio vacío con la pieza
            swap(nuevo_estado.tablero[actual.fila_vacia][actual.col_vacia],
                 nuevo_estado.tablero[nueva_fila][nueva_col]);
            
            nuevo_estado.fila_vacia = nueva_fila;
            nuevo_estado.col_vacia = nueva_col;
            nuevo_estado.g = actual.g + 1;
            nuevo_estado.h = calcularManhattan(nuevo_estado.tablero, objetivo);
            nuevo_estado.f = nuevo_estado.g + nuevo_estado.h;
            nuevo_estado.movimientos = actual.movimientos;
            nuevo_estado.movimientos.push_back(nombres_mov[i]);
            
            vecinos.push_back(nuevo_estado);
        }
    }
    
    return vecinos;
}

// Algoritmo A* para resolver el 8-puzzle
bool resolverPuzzle(Estado& inicial, const vector<vector<int>>& objetivo) {
    priority_queue<Estado, vector<Estado>, greater<Estado>> cola_abierta;
    set<string> visitados;
    
    inicial.h = calcularManhattan(inicial.tablero, objetivo);
    inicial.f = inicial.g + inicial.h;
    
    cola_abierta.push(inicial);
    int nodos_explorados = 0;
    
    cout << "\n[BUSQUEDA] Iniciando algoritmo A*..." << endl;
    cout << "  Heuristica: Distancia Manhattan" << endl;
    cout << "  Estado inicial - h = " << inicial.h << endl << endl;
    
    while (!cola_abierta.empty()) {
        Estado actual = cola_abierta.top();
        cola_abierta.pop();
        
        nodos_explorados++;
        
        // Mostrar progreso cada 100 nodos
        if (nodos_explorados % 100 == 0) {
            cout << "\r  Nodos explorados: " << nodos_explorados 
                 << " | Profundidad: " << actual.g 
                 << " | Cola: " << cola_abierta.size();
            cout.flush();
        }
        
        // Verificar si es el estado objetivo
        if (sonIguales(actual.tablero, objetivo)) {
            cout << "\r  Nodos explorados: " << nodos_explorados 
                 << " | Profundidad: " << actual.g 
                 << " | Cola: " << cola_abierta.size() << endl;
            cout << "\n[EXITO] Solucion encontrada!" << endl;
            cout << "  Numero de movimientos: " << actual.movimientos.size() << endl;
            cout << "  Nodos explorados: " << nodos_explorados << endl << endl;
            
            inicial = actual;
            return true;
        }
        
        string tablero_str = tableroAString(actual.tablero);
        if (visitados.count(tablero_str)) {
            continue;
        }
        visitados.insert(tablero_str);
        
        // Generar vecinos
        vector<Estado> vecinos = generarVecinos(actual, objetivo);
        for (const Estado& vecino : vecinos) {
            string vecino_str = tableroAString(vecino.tablero);
            if (!visitados.count(vecino_str)) {
                cola_abierta.push(vecino);
            }
        }
    }
    
    cout << "\n[ERROR] No se encontro solucion." << endl;
    return false;
}

// Función para ingresar configuración personalizada
void ingresarConfiguracion(vector<vector<int>>& tablero, int& fila_vacia, int& col_vacia) {
    cout << "\nIngresa los numeros del tablero (0 para espacio vacio)" << endl;
    cout << "Ingresa fila por fila, separados por espacios:" << endl;
    
    set<int> numeros_usados;
    
    for (int i = 0; i < 3; i++) {
        cout << "  Fila " << (i + 1) << ": ";
        for (int j = 0; j < 3; j++) {
            cin >> tablero[i][j];
            
            if (tablero[i][j] < 0 || tablero[i][j] > 8) {
                cout << "[ERROR] Numero invalido. Debe estar entre 0 y 8." << endl;
                j--;
                continue;
            }
            
            if (numeros_usados.count(tablero[i][j])) {
                cout << "[ERROR] Numero repetido. Intenta de nuevo." << endl;
                j--;
                continue;
            }
            
            numeros_usados.insert(tablero[i][j]);
            
            if (tablero[i][j] == 0) {
                fila_vacia = i;
                col_vacia = j;
            }
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    
    cout << "=================================================================" << endl;
    cout << "              RESOLVEDOR DE 8-PUZZLE CON A*" << endl;
    cout << "=================================================================" << endl;
    cout << "    Trabajo Practico No. 4 - Punto 1" << endl;
    cout << "    Programacion Avanzada - C++" << endl;
    cout << "=================================================================" << endl << endl;
    
    // Estado objetivo
    vector<vector<int>> objetivo = {
        {1, 2, 3},
        {8, 0, 4},
        {7, 6, 5}
    };
    
    cout << "[CONFIG] Estado objetivo:" << endl;
    mostrarTablero(objetivo);
    cout << endl;
    
    // Preguntar al usuario
    cout << "Selecciona una opcion:" << endl;
    cout << "  1. Usar configuracion de ejemplo (Fig. 1 del taller)" << endl;
    cout << "  2. Ingresar configuracion personalizada" << endl;
    cout << "\nOpcion: ";
    
    int opcion;
    cin >> opcion;
    
    Estado inicial;
    Estado estado_inicial_backup; // Para guardar el estado inicial original
    
    if (opcion == 1) {
        // Configuración del ejemplo (Fig. 1)
        inicial.tablero = {
            {5, 7, 2},
            {4, 1, 0},
            {3, 8, 6}
        };
        inicial.fila_vacia = 1;
        inicial.col_vacia = 2;
        inicial.g = 0;
        
        estado_inicial_backup = inicial;
    } else {
        ingresarConfiguracion(inicial.tablero, inicial.fila_vacia, inicial.col_vacia);
        inicial.g = 0;
        
        estado_inicial_backup = inicial;
    }
    
    cout << "\n[CONFIG] Estado inicial:" << endl;
    mostrarTablero(inicial.tablero);
    
    // Resolver el puzzle
    cout << "\n=================================================================" << endl;
    
    if (resolverPuzzle(inicial, objetivo)) {
        // Mostrar la secuencia de movimientos
        cout << "=================================================================" << endl;
        cout << "              SECUENCIA DE MOVIMIENTOS" << endl;
        cout << "=================================================================" << endl << endl;
        
        // Usar el backup del estado inicial
        Estado paso_actual = estado_inicial_backup;
        
        cout << "Paso 0 - INICIO:" << endl;
        mostrarTablero(paso_actual.tablero);
        cout << endl;
        
        // Simular cada movimiento
        for (size_t i = 0; i < inicial.movimientos.size(); i++) {
            string mov = inicial.movimientos[i];
            cout << "Paso " << (i + 1) << " - Mover pieza " << mov << ":" << endl;
            
            // Aplicar movimiento
            if (mov == "ARRIBA") {
                swap(paso_actual.tablero[paso_actual.fila_vacia][paso_actual.col_vacia],
                     paso_actual.tablero[paso_actual.fila_vacia - 1][paso_actual.col_vacia]);
                paso_actual.fila_vacia--;
            } else if (mov == "ABAJO") {
                swap(paso_actual.tablero[paso_actual.fila_vacia][paso_actual.col_vacia],
                     paso_actual.tablero[paso_actual.fila_vacia + 1][paso_actual.col_vacia]);
                paso_actual.fila_vacia++;
            } else if (mov == "IZQUIERDA") {
                swap(paso_actual.tablero[paso_actual.fila_vacia][paso_actual.col_vacia],
                     paso_actual.tablero[paso_actual.fila_vacia][paso_actual.col_vacia - 1]);
                paso_actual.col_vacia--;
            } else if (mov == "DERECHA") {
                swap(paso_actual.tablero[paso_actual.fila_vacia][paso_actual.col_vacia],
                     paso_actual.tablero[paso_actual.fila_vacia][paso_actual.col_vacia + 1]);
                paso_actual.col_vacia++;
            }
            
            mostrarTablero(paso_actual.tablero);
            cout << endl;
        }
        
        cout << "=================================================================" << endl;
        cout << "  RESUMEN:" << endl;
        cout << "  - Movimientos totales: " << inicial.movimientos.size() << endl;
        cout << "  - Solucion OPTIMA encontrada" << endl;
        cout << "=================================================================" << endl;
    }
    
    cout << "\nPresiona Enter para salir...";
    cin.ignore();
    cin.get();
    
    return 0;
}
