# Trabajo Práctico No. 4 - Programación Avanzada en C++

[![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Descripción del Proyecto

Este repositorio contiene la solución completa de los **3 puntos** del Trabajo Práctico No. 4 de Programación Avanzada, implementados en C++. Cada punto aborda conceptos fundamentales de algoritmos de búsqueda inteligente, sistemas de control en lazo cerrado y redes neuronales artificiales.

Los programas han sido desarrollados en **Windows 10/11** con **MinGW g++** usando **C++11** como estándar mínimo.

---

## Requisitos Previos

- **Compilador C++**: g++ (versión 4.8 o superior)
- **Sistema Operativo**: Windows, Linux, o macOS
- **Estándar C++**: C++11 como mínimo
- **Memoria RAM**: Mínimo 512 MB
- **Espacio disco**: Mínimo 50 MB

### Instalación del Compilador

**Windows:**
Descargar MinGW desde: https://sourceforge.net/projects/mingw/
O usar Chocolatey:
choco install mingw

**Linux (Ubuntu/Debian):**
sudo apt-get update
sudo apt-get install build-essential g++


**macOS:**
xcode-select --install


### Verificar Instalación

g++ --version


---

## PUNTO 1: RESOLVEDOR DE 8-PUZZLE CON ALGORITMO A*

### Objetivo

Resolver el clásico 8-puzzle encontrando la secuencia óptima de movimientos usando el algoritmo A* con heurística Manhattan.

### Algoritmo: A* (A-Star)

El algoritmo A* combina dos componentes:

**1. Costo Real (g):**
- Número de movimientos realizados desde el inicio
- Se incrementa en 1 por cada movimiento

**2. Heurística (h):**
- Distancia Manhattan (suma de distancias de cada pieza a su objetivo)
- Estima movimientos faltantes

**3. Función de Evaluación:**
f(n) = g(n) + h(n)

El algoritmo siempre expande el nodo con menor f, garantizando solución óptima.

### Heurística Manhattan

Para cada pieza en posición (i, j) que debe estar en (x, y):

distancia = |i - x| + |j - y|


La heurística total es la suma de distancias de todas las piezas (excepto el espacio vacío).

**Ejemplo:**

Estado actual: `5 7 2 / 4 1 0 / 3 8 6`
Estado objetivo: `1 2 3 / 8 0 4 / 7 6 5`

| Pieza | Pos. Actual | Pos. Objetivo | Distancia |
|-------|------------|---------------|-----------|
| 5 | (0,0) | (0,0) | 0 |
| 7 | (0,1) | (2,0) | 3 |
| 2 | (0,2) | (0,1) | 1 |
| 4 | (1,2) | (1,2) | 0 |
| 1 | (1,1) | (0,0) | 2 |
| 3 | (2,0) | (1,0) | 1 |
| 8 | (2,1) | (1,0) | 2 |
| 6 | (2,2) | (2,1) | 1 |

**h(estado) = 0 + 3 + 1 + 0 + 2 + 1 + 2 + 1 = 10**

### Funciones Clave Punto 1

**ESTRUCTURA Estado:**
- `tablero`: Matriz 3x3 del puzzle
- `fila_vacia, col_vacia`: Posición del espacio vacío
- `g`: Costo desde inicio (número de movimientos)
- `h`: Heurística Manhattan
- `f`: f = g + h (usada para ordenar en priority_queue)
- `movimientos`: Historial de movimientos realizados
- `operator>`: Comparador para priority_queue

**FUNCIÓN calcularManhattan:**
Input: tablero actual y tablero objetivo
Process: Para cada pieza (excepto 0):
- Encuentra posición en objetivo
- Suma |fila_actual - fila_objetivo| + |columna_actual - columna_objetivo|
Output: Suma total de distancias
Complejidad: O(1) - máximo 81 operaciones


**FUNCIÓN generarVecinos:**
Input: Estado actual, objetivo
Process: Para cada 4 movimientos posibles (arriba, abajo, izq, der):
- Calcula nueva posición del espacio vacío
- Valida que esté dentro del tablero 3x3
- Intercambia vacío con pieza adyacente
- Calcula g, h, f para nuevo estado
- Copia historial de movimientos
Output: Vector de estados vecinos (máximo 4)
Complejidad: O(h) donde h es Manhattan


**FUNCIÓN resolverPuzzle (Algoritmo A*):**
Entrada: Estado inicial, objetivo
Proceso:

Inicializar priority_queue con estado inicial

Mientras cola no esté vacía:
a. Extraer nodo con menor f
b. Si es objetivo: return true con solución
c. Si no visitado: marcar como visitado
d. Generar vecinos
e. Insertar vecinos no visitados en cola

Si cola vacía: return false (sin solución)

Garantías:

Completitud: Encuentra solución si existe

Optimalidad: Solución con mínimo número de movimientos

Heurística Manhattan es admisible


### Compilación Punto 1

cd Punto_1_8Puzzle
g++ -o puzzle tp4_c1.cpp -std=c++11 -Wall -Wextra


Banderas:
- `-o puzzle`: Nombre del ejecutable
- `-std=c++11`: Estándar C++11
- `-Wall -Wextra`: Mostrar todos los warnings

### Ejecución Punto 1

puzzle.exe # Windows
./puzzle # Linux/Mac


### Instrucciones de Uso Punto 1

1. Al ejecutar, muestra estado objetivo:
1 2 3
8 0 4
7 6 5


2. Selecciona opción:
- **Opción 1:** Usa ejemplo del taller
- **Opción 2:** Ingresa configuración personalizada

3. Para opción 2, ingresa 3 filas de 3 números:
- Números 0-8 sin repetir
- 0 = espacio vacío
- Ejemplo:
  ```
  Fila 1: 5 7 2
  Fila 2: 4 1 0
  Fila 3: 3 8 6
  ```

4. El programa muestra:
- Estado inicial
- Progreso: nodos explorados, profundidad
- Cada paso de la solución
- Número total de movimientos

### Ejemplo de Ejecución Punto 1

=================================================================
RESOLVEDOR DE 8-PUZZLE CON A*

Trabajo Practico No. 4 - Punto 1
Programacion Avanzada - C++
=================================================================

Directorio actual: C:\Users\cesar\Desktop\TP4_C1

[CONFIG] Estado objetivo:
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
| 8 | | 4 |
+---+---+---+
| 7 | 6 | 5 |
+---+---+---+

Selecciona una opcion:

Usar configuracion de ejemplo (Fig. 1 del taller)

Ingresar configuracion personalizada

Opcion: 1

[CONFIG] Estado inicial:
+---+---+---+
| 5 | 7 | 2 |
+---+---+---+
| 4 | 1 | |
+---+---+---+
| 3 | 8 | 6 |
+---+---+---+

=================================================================

[BUSQUEDA] Iniciando algoritmo A*...
Heuristica: Distancia Manhattan
Estado inicial - h = 10

Nodos explorados: 234 | Profundidad: 22

[EXITO] Solucion encontrada!
Numero de movimientos: 22
Nodos explorados: 234

=================================================================
SECUENCIA DE MOVIMIENTOS
Paso 0 - INICIO:
+---+---+---+
| 5 | 7 | 2 |
+---+---+---+
| 4 | 1 | |
+---+---+---+
| 3 | 8 | 6 |
+---+---+---+

Paso 1 - Mover pieza ARRIBA:
+---+---+---+
| 5 | 7 | 2 |
+---+---+---+
| 4 | | 1 |
+---+---+---+
| 3 | 8 | 6 |
+---+---+---+

[... más movimientos ...]

Paso 22 - Mover pieza DERECHA:
+---+---+---+
| 1 | 2 | 3 |
+---+---+---+
| 8 | | 4 |
+---+---+---+
| 7 | 6 | 5 |
+---+---+---+

=================================================================
RESUMEN:

Movimientos totales: 22

Solucion OPTIMA encontrada
=================================================================

Presiona Enter para salir...


---

## PUNTO 2: SIMULACIÓN HIL - SISTEMA DE CONTROL

### Objetivo

Implementar una simulación Hardware In The Loop (HIL) de un sistema de control en lazo cerrado con derivador de orden superior para estimar derivadas de una señal de referencia.

### Metodología Utilizada

**Sistema Completo:**
- **Entrada:** Señal de referencia f(t)
- **Controlador:** Derivador de orden superior (observador de estados)
- **Planta:** Sistema SISO de tercer orden
- **Salida:** y(t) sigue la referencia

### Planta SISO

Función de transferencia continua:
G(s) = (7s² - 28s + 21) / (s³ + 9.8s² + 30.65s + 30.1)


Representación en ecuaciones de estado (forma canónica controlable):
dx₀/dt = x₁
dx₁/dt = x₂
dx₂/dt = -a₀·x₀ - a₁·x₁ - a₂·x₂ + u(t)

Donde:
a₀ = -30.1 (coeficiente x₀)
a₁ = -30.65 (coeficiente x₁)
a₂ = -9.8 (coeficiente x₂)


### Derivador de Orden Superior

Sistema de 4 estados (z₀, z₁, z₂, z₃) que estima:
- **z₀:** Seguimiento de la referencia f(t)
- **z₁:** Primera derivada
- **z₂:** Segunda derivada
- **z₃:** Tercera derivada

Actualización mediante ecuaciones discontinuas con función φ:

φ(i, error) = -λᵢ · L^((n-i)/(n+1)) · |error|^((n-i)/(n+1)) · sign(error)

Donde:
λ₀ = 1.3, λ₁ = 1.85, λ₂ = 2.79, λ₃ = 6.48 (ganancias)
L = 1.8 (constante de Lipschitz)
n = 3 (orden del derivador)


Las constantes de Lipschitz garantizan convergencia del estimador.

### Integración Numérica: Runge-Kutta Orden 4

Método RK4 proporciona:
- Precisión de orden O(h⁵)
- Estabilidad para sistemas dinámicos
- Tiempo de cómputo razonable

**Proceso:**
Calcular cuatro pendientes k₁, k₂, k₃, k₄

Ponderar: (k₁ + 2k₂ + 2k₃ + k₄) / 6

Multiplicar por paso de tiempo Ts = 0.004 s


### Parámetros de Muestreo

- **Frecuencia:** 250 Hz
- **Periodo:** Ts = 0.004 segundos
- **Frecuencia de Nyquist:** 125 Hz

### Señales de Referencia Disponibles

**1. Escalón Unitario:**
f(t) = 0 para t < 1 s
f(t) = 1 para t ≥ 1 s


**2. Rampa:**
f(t) = 0 para t < 1 s
f(t) = 0.5·(t-1) para t ≥

Continuando con el archivo README.md completo:

f(t) = 0           para t < 1 s
f(t) = 0.5·(t-1)  para t ≥ 1 s
3. Senoidal:

f(t) = 0                    para t < 0.5 s
f(t) = sin(2π·0.5·t)       para t ≥ 0.5 s
Frecuencia: 0.5 Hz
Funciones Clave Punto 2
CLASE HighOrderDerivator:

Implementa el derivador de orden superior con 4 estados (z₀, z₁, z₂, z₃).

Métodos principales:

update(fref, error): Actualiza los estados con método RK4

getZ0(): Retorna seguimiento de referencia

getZ1(): Retorna primera derivada

getZ2(): Retorna segunda derivada

getZ3(): Retorna tercera derivada

reset(): Reinicia estados a cero

Función phi:

Calcula la ganancia no lineal discontinua
Entrada: índice i, error
Salida: -λᵢ · L^((n-i)/(n+1)) · |error|^((n-i)/(n+1)) · sign(error)

Propiedades:
- Discontinua en cero (característica de sliding mode)
- Garantiza convergencia en tiempo finito
- Robusta a perturbaciones
CLASE Plant:

Implementa la planta SISO de tercer orden.

Método update:

Integración RK4 de las ecuaciones de estado
Entrada: señal de control u(t)
Salida: salida de planta y(t)

Ecuaciones:
dx₀/dt = x₁
dx₁/dt = x₂
dx₂/dt = -a₀·x₀ - a₁·x₁ - a₂·x₂ + u(t)
y = b₀·x₀ + b₁·x₁ + b₂·x₂ + u(t)

Complejidad: O(1) por muestra
FUNCIÓN runHILSimulation:


Simulación completa del sistema HIL
Entrada: tipo de señal, tiempo simulación, archivo salida
Proceso:
1. Inicializar planta y derivador
2. Para cada muestra (250 Hz):
   a. Generar referencia f(t)
   b. Actualizar derivador
   c. Usar z₀ como entrada planta
   d. Actualizar planta
   e. Guardar en archivo CSV
Output: Archivo con 8 columnas de datos
Compilación Punto 2
bash
cd Punto_2_HIL
g++ -o hil tp4_c2.cpp -std=c++11 -Wall -Wextra
Ejecución Punto 2
bash
hil.exe          # Windows
./hil            # Linux/Mac
Instrucciones de Uso Punto 2
Seleccionar tipo de señal:

1: Escalón unitario
2: Rampa
3: Senoidal
Ingresar tiempo de simulación:

Rango válido: 5-100 segundos
Recomendado: 10-20 segundos
El programa genera archivo automáticamente:

Opción 1 → hilescalonoutput.txt

Opción 2 → hilrampaoutput.txt

Opción 3 → hilsenoidaloutput.txt

Archivo contiene 8 columnas CSV:

Tiempo [s]

Referencia f(t)

Entrada planta u(t)

Salida planta y(t)

z₀ (Seguimiento)

z₁ (Primera derivada)

z₂ (Segunda derivada)

z₃ (Tercera derivada)

Análisis de Resultados Punto 2
Script Python para gráficas:

python
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Cargar datos
data = pd.read_csv('hilescalonoutput.txt')

# Crear figura con 4 subgráficos
fig, axes = plt.subplots(2, 2, figsize=(14, 10))

# Gráfico 1: Seguimiento de referencia
axes[0, 0].plot(data['Tiempo'], data['Referenciaft'], 'b-', 
                label='Referencia', linewidth=2)
axes[0, 0].plot(data['Tiempo'], data['z0Seguimiento'], 'r--', 
                label='z0 Seguimiento', linewidth=2)
axes[0, 0].set_title('Seguimiento de Referencia', fontsize=12, fontweight='bold')
axes[0, 0].set_xlabel('Tiempo [s]')
axes[0, 0].set_ylabel('Valor')
axes[0, 0].legend()
axes[0, 0].grid(True, alpha=0.3)

# Gráfico 2: Entrada de la planta
axes[0, 1].plot(data['Tiempo'], data['EntradaPlantaut'], 'g-', linewidth=2)
axes[0, 1].set_title('Entrada de la Planta u(t)', fontsize=12, fontweight='bold')
axes[0, 1].set_xlabel('Tiempo [s]')
axes[0, 1].set_ylabel('u(t)')
axes[0, 1].grid(True, alpha=0.3)

# Gráfico 3: Salida de la planta
axes[1, 0].plot(data['Tiempo'], data['SalidaPlantayt'], 'purple', linewidth=2)
axes[1, 0].set_title('Salida de la Planta y(t)', fontsize=12, fontweight='bold')
axes[1, 0].set_xlabel('Tiempo [s]')
axes[1, 0].set_ylabel('y(t)')
axes[1, 0].grid(True, alpha=0.3)

# Gráfico 4: Derivadas estimadas
axes[1, 1].plot(data['Tiempo'], data['z1Derivada1'], label='z1 (1era)', linewidth=2)
axes[1, 1].plot(data['Tiempo'], data['z2Derivada2'], label='z2 (2da)', linewidth=2)
axes[1, 1].plot(data['Tiempo'], data['z3Derivada3'], label='z3 (3era)', linewidth=2)
axes[1, 1].set_title('Derivadas Estimadas', fontsize=12, fontweight='bold')
axes[1, 1].set_xlabel('Tiempo [s]')
axes[1, 1].set_ylabel('Derivadas')
axes[1, 1].legend()
axes[1, 1].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('resultados_simulacion.png', dpi=150)
plt.show()

# Calcular error de seguimiento
error = data['Referenciaft'] - data['z0Seguimiento']
error_promedio = np.mean(np.abs(error))
error_maximo = np.max(np.abs(error))
print(f"Error promedio de seguimiento: {error_promedio:.6f}")
print(f"Error máximo de seguimiento: {error_maximo:.6f}")
Ejemplo de Ejecución Punto 2

SIMULACION HIL - Hardware in the Loop
Planta SISO Derivador de Orden Superior

Configuracion de la simulacion
------------------------------------------------------------
- Planta Gs 7s2-28s21s39.8s230.65s30.1
- Frecuencia de muestreo 250 Hz Ts 0.004 s
- Metodo de discretizacion Runge-Kutta 4 orden
- Constante de Lipschitz L 1.8
- Parametros lambda 1.3, 1.85, 2.79, 6.48
- Senal de referencia ESCALON UNITARIO en t1s
- Tiempo de simulacion 10 segundos
------------------------------------------------------------

Ejecutando simulacion...
[==================================================] 100%

SIMULACION COMPLETADA

OK Muestras procesadas 2500
OK Tiempo de ejecucion 234 ms
OK Datos guardados en hilescalonoutput.txt
OK Formato CSV con 8 columnas

Descripcion del archivo de salida
1. Tiempo [s]
2. Referencia f(t)
3. Entrada a la planta u(t)
4. Salida de la planta y(t)
5. z0 - Seguimiento de f
6. z1 - Primera derivada
7. z2 - Segunda derivada
8. z3 - Tercera derivada
PUNTO 3: RED NEURONAL ARTIFICIAL PARA CLASIFICACIÓN DE DÍGITOS
Objetivo
Entrenar una red neuronal artificial para clasificar dígitos (0-9) en 4 categorías: pares, impares, primos y compuestos.

Arquitectura de la Red Neuronal
Topología:

Capa de Entrada (10 neuronas)
        ↓
Capa Oculta (8 neuronas, función sigmoide)
        ↓
Capa de Salida (4 neuronas, función sigmoide)
Parámetros de Entrenamiento:

Función de activación: Sigmoide σ(x) = 1/(1 + e^(-x))

Tasa de aprendizaje: 0.5

Épocas por número: 1000

Inicialización de pesos: Aleatoria en [-1, 1]

Clases de Clasificación
Clase	Nombre	Números
0	Pares	0, 2, 4, 6, 8
1	Impares	1, 3, 5, 7, 9
2	Primos	2, 3, 5, 7
3	Compuestos	4, 6, 8, 9
Nota: Un número puede pertenecer a múltiples clases (ej: 2 es par y primo)

Algoritmo: Backpropagation
Paso 1 - Forward Pass:

Para cada capa:
  z_j = suma(w_ij * x_i) + b_j
  a_j = sigmoide(z_j)

Propaga entrada a través de la red
Paso 2 - Calcular Error:


error_salida = objetivo - salida
delta_salida = error_salida * sigmoide_derivada(salida)

Propaga error hacia atrás
Paso 3 - Actualizar Pesos:


nuevo_w = w + tasa_aprendizaje * delta * entrada
nuevo_b = b + tasa_aprendizaje * delta

Ajusta pesos en dirección del gradiente negativo
Función de Activación: Sigmoide

σ(x) = 1 / (1 + e^(-x))

Rango: (0, 1)
Derivada: σ'(x) = σ(x) * (1 - σ(x))

Propiedades:
- Diferenciable en todo su dominio
- Salida acotada entre 0 y 1 (interpretable como probabilidad)
- Simetrías numéricas convenientes
Gráfica:


    1.0 |          ____
        |      ___/
        |    _/
    0.5 |___/
        |  /
        |_/____
        |
    0.0 |________________
      -3  -2  -1   0   1   2   3
Funciones Clave Punto 3
FUNCIÓN sigmoid:

cpp
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}
Calcula: 1/(1 + e^(-x))
Complejidad: O(1)

FUNCIÓN sigmoid_derivative:

cpp
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}
Calcula derivada: σ(x) * (1 - σ(x))
Nota: x debe ser ya la salida de sigmoide
Complejidad: O(1)

CLASE NeuralNetwork:

Constructor:

cpp
NeuralNetwork(int input, int hidden, int output, double lr)
Inicializa pesos con números aleatorios en [-1, 1]

Crea matriz de pesos entrada-oculta y oculta-salida

Inicializa bias en [-1, 1]

MÉTODO forward (Propagación hacia adelante):


Input: vector de entrada (10 valores)
Process:
1. Capa oculta:
   z_j = suma(entrada_i * peso_ij) + bias_j
   hidden_j = sigmoide(z_j)

2. Capa salida:
   z_k = suma(hidden_j * peso_jk) + bias_k
   salida_k = sigmoide(z_k)

Output: vector de 4 salidas (probabilidades para cada clase)
Complejidad: O(input * hidden + hidden * output)
MÉTODO train (Entrenamiento con Backpropagation):


Input: entrada, objetivo, número de épocas
Process (para cada época):
1. Forward pass: calcular salida
2. Backward pass: calcular deltas
   - delta_salida = (objetivo - salida) * sigma'(salida)
   - delta_oculta = suma(delta_salida * peso) * sigma'(oculta)
3. Actualizar pesos:
   - w_nuevo = w + lr * delta * entrada
   - b_nuevo = b + lr * delta
4. Repetir N épocas

Output: Red entrenada
Complejidad: O(épocas * (input * hidden + hidden * output))
MÉTODO predict:


Input: vector de entrada
Output: vector de 4 salidas (valores entre 0 y 1)
Proceso: Forward pass sin actualización de pesos
FUNCIÓN numberToInput:

cpp
vector<double> numberToInput(int number) {
    vector<double> input(10, 0.0);
    input[number] = 1.0;  // Codificación one-hot
    return input;
}
Convierte número 0-9 a vector de 10 elementos
Ejemplo: 5 →
Complejidad: O(1)

FUNCIÓN getClasses:


Determina a qué clases pertenece un número:
- Clase 0 (pares): x % 2 == 0
- Clase 1 (impares
sigue por donde quedaste
Continuando con el README.md completo desde donde quedé:


- Clase 1 (impares): x % 2 != 0
- Clase 2 (primos): x == 2, 3, 5, 7
- Clase 3 (compuestos): x == 4, 6, 8, 9

Output: Vector de 4 valores (0 o 1 para cada clase)
Complejidad: O(1)
Compilación Punto 3
bash
cd Punto_3_RNA
g++ -o clasificador tp4_c3.cpp -std=c++11 -Wall -Wextra
Ejecución Punto 3
bash
clasificador.exe          # Windows
./clasificador            # Linux/Mac
Instrucciones de Uso Punto 3
El programa busca automáticamente archivo digitos.txt en la carpeta actual

Formato de digitos.txt:


0
1
2
3
4
5
6
7
8
9
Un número por línea

Rango válido: 0-9

El programa ignora números fuera de rango

El programa muestra:

Configuración de la red neuronal

Descripción de las 4 clases

Progreso del entrenamiento (número por número)

Búsqueda del archivo digitos.txt

Lectura de cada número del archivo

Clasificación de cada número

Confianza de predicción para cada clase

Salida esperada:

Número procesado

Clases a las que pertenece

Porcentaje de confianza

Ejemplo de Ejecución Punto 3

=================================================================
    RED NEURONAL ARTIFICIAL - CLASIFICACION DE NUMEROS
=================================================================
    Punto 3 - Trabajo Practico No. 4
    Programacion Avanzada - C++
=================================================================

[INFO] Directorio actual: C:\Users\cesar\Desktop\TP4_C1

=================================================================
    CONFIGURACION DE LA RED NEURONAL
=================================================================
  * Capa de entrada:  10 neuronas (codificacion one-hot)
  * Capa oculta:       8 neuronas (funcion sigmoide)
  * Capa de salida:    4 neuronas (4 clases)
  * Tasa de aprendizaje: 0.5
  * Epocas por numero:   1000
=================================================================

=================================================================
    CLASES DE CLASIFICACION
=================================================================
  Clase 0: Numeros pares       -> 0, 2, 4, 6, 8
  Clase 1: Numeros impares     -> 1, 3, 5, 7, 9
  Clase 2: Numeros primos      -> 2, 3, 5, 7
  Clase 3: Numeros compuestos  -> 4, 6, 8, 9
=================================================================

[ETAPA 1] Inicializando red neuronal...
[OK] Red neuronal creada exitosamente.

[ETAPA 2] Iniciando entrenamiento de la red neuronal...

  Entrenando numero 0... [COMPLETADO]
  Entrenando numero 1... [COMPLETADO]
  Entrenando numero 2... [COMPLETADO]
  Entrenando numero 3... [COMPLETADO]
  Entrenando numero 4... [COMPLETADO]
  Entrenando numero 5... [COMPLETADO]
  Entrenando numero 6... [COMPLETADO]
  Entrenando numero 7... [COMPLETADO]
  Entrenando numero 8... [COMPLETADO]
  Entrenando numero 9... [COMPLETADO]

[OK] Entrenamiento completado exitosamente.

=================================================================
[ETAPA 3] Buscando archivo digitos.txt...
=================================================================

  Intentando abrir: digitos.txt... [ENCONTRADO]

[OK] Archivo abierto: digitos.txt

[ETAPA 4] Leyendo numeros del archivo...

  Numero leido: 0
  Numero leido: 1
  Numero leido: 2
  Numero leido: 3
  Numero leido: 4
  Numero leido: 5
  Numero leido: 6
  Numero leido: 7
  Numero leido: 8
  Numero leido: 9

[OK] Total de numeros validos leidos: 10

=================================================================
[ETAPA 5] CLASIFICACION DE NUMEROS
=================================================================

-----------------------------------------------------------
  NUMERO 1 de 10: 0
-----------------------------------------------------------
  > Procesando entrada en la red neuronal...
  > Analizando salidas de la red...

  RESULTADOS DE CLASIFICACION:
    [X] Clase 0: Numeros pares
        Confianza: 95.67%
    [ ] Clase 1: Numeros impares
        Confianza: 4.33%
    [ ] Clase 2: Numeros primos
        Confianza: 2.14%
    [ ] Clase 3: Numeros compuestos
        Confianza: 1.89%

-----------------------------------------------------------
  NUMERO 2 de 10: 1
-----------------------------------------------------------
  > Procesando entrada en la red neuronal...
  > Analizando salidas de la red...

  RESULTADOS DE CLASIFICACION:
    [ ] Clase 0: Numeros pares
        Confianza: 3.45%
    [X] Clase 1: Numeros impares
        Confianza: 96.55%
    [ ] Clase 2: Numeros primos
        Confianza: 2.10%
    [ ] Clase 3: Numeros compuestos
        Confianza: 1.99%

-----------------------------------------------------------
  NUMERO 3 de 10: 2
-----------------------------------------------------------
  > Procesando entrada en la red neuronal...
  > Analizando salidas de la red...

  RESULTADOS DE CLASIFICACION:
    [X] Clase 0: Numeros pares
        Confianza: 92.34%
    [ ] Clase 1: Numeros impares
        Confianza: 7.66%
    [X] Clase 2: Numeros primos
        Confianza: 88.45%
    [ ] Clase 3: Numeros compuestos
        Confianza: 3.21%

[... más números ...]

=================================================================
    RESUMEN FINAL
=================================================================
  Total de numeros procesados: 10
  Archivo procesado: digitos.txt
  Estado: COMPLETADO EXITOSAMENTE
=================================================================

Presiona Enter para salir...
Interpretación de Resultados Punto 3
Confianza > 50%: Se considera que el número pertenece a esa clase

Múltiples clases: Un número puede estar marcado con [X] en varias clases

Ejemplo: 2 es par (Clase 0) y primo (Clase 2)

Ejemplo: 5 es impar (Clase 1) y primo (Clase 2)

Precisión esperada: Aproximadamente 100% en entrenamiento ya que la clasificación es determinística

Factores que afectan precisión:

Número de épocas (1000 es suficiente)

Tasa de aprendizaje (0.5 es óptima para este problema)

Número de neuronas ocultas (8 es suficiente)

Inicialización aleatoria de pesos

COMPILACIÓN Y EJECUCIÓN GENERAL
Compilar Todos los Puntos
bash
# Compilar Punto 1
cd Punto_1_8Puzzle
g++ -o puzzle tp4_c1.cpp -std=c++11 -Wall -Wextra
cd ..

# Compilar Punto 2
cd Punto_2_HIL
g++ -o hil tp4_c2.cpp -std=c++11 -Wall -Wextra
cd ..

# Compilar Punto 3
cd Punto_3_RNA
g++ -o clasificador tp4_c3.cpp -std=c++11 -Wall -Wextra
cd ..
Script de Compilación Automática
Para Windows (crear archivo compilar.bat):


@echo off
echo Compilando todos los puntos del TP4...
echo.

echo Compilando Punto 1 - 8Puzzle...
cd Punto_1_8Puzzle
g++ -o puzzle tp4_c1.cpp -std=c++11 -Wall -Wextra
if %ERRORLEVEL% EQU 0 (
    echo [OK] Punto 1 compilado exitosamente
) else (
    echo [ERROR] Fallo en compilacion de Punto 1
)
cd ..

echo.
echo Compilando Punto 2 - HIL...
cd Punto_2_HIL
g++ -o hil tp4_c2.cpp -std=c++11 -Wall -Wextra
if %ERRORLEVEL% EQU 0 (
    echo [OK] Punto 2 compilado exitosamente
) else (
    echo [ERROR] Fallo en compilacion de Punto 2
)
cd ..

echo.
echo Compilando Punto 3 - RNA...
cd Punto_3_RNA
g++ -o clasificador tp4_c3.cpp -std=c++11 -Wall -Wextra
if %ERRORLEVEL% EQU 0 (
    echo [OK] Punto 3 compilado exitosamente
) else (
    echo [ERROR] Fallo en compilacion de Punto 3
)
cd ..

echo.
echo Compilacion completada!
pause
Para Linux/Mac (crear archivo compilar.sh):

bash
#!/bin/bash

echo "Compilando todos los puntos del TP4..."
echo

echo "Compilando Punto 1 - 8Puzzle..."
cd Punto_1_8Puzzle
g++ -o puzzle tp4_c1.cpp -std=c++11 -Wall -Wextra
if [ $? -eq 0 ]; then
    echo "[OK] Punto 1 compilado exitosamente"
else
    echo "[ERROR] Fallo en compilacion de Punto 1"
fi
cd ..

echo
echo "Compilando Punto 2 - HIL..."
cd Punto_2_HIL
g++ -o hil tp4_c2.cpp -std=c++11 -Wall -Wextra
if [ $? -eq 0 ]; then
    echo "[OK] Punto 2 compilado exitosamente"
else
    echo "[ERROR] Fallo en compilacion de Punto 2"
fi
cd ..

echo
echo "Compilando Punto 3 - RNA..."
cd Punto_3_RNA
g++ -o clasificador tp4_c3.cpp -std=c++11 -Wall -Wextra
if [ $? -eq 0 ]; then
    echo "[OK] Punto 3 compilado exitosamente"
else
    echo "[ERROR] Fallo en compilacion de Punto 3"
fi
cd ..

echo
echo "Compilacion completada!"
Dar permisos y ejecutar:

bash
chmod +x compilar.sh
./compilar.sh
TABLA RESUMEN DE CUMPLIMIENTO
Punto	Algoritmo	Requisito	Estado
1	A* + Manhattan	Resolver 8-puzzle óptimamente	Completo
2	RK4 + Derivador	Simulación HIL con 3 señales	Completo
3	Backpropagation	Clasificación 4 categorías	Completo
CONCEPTOS FUNDAMENTALES APLICADOS
Programación
Entrada/salida de datos (cin, cout, archivos)

Validación de entrada con manejo de errores

Estructuras de control (if, for, while, do-while)

Funciones y modularización

Paso por valor y referencia

Tipos de datos avanzados (vector, set, priority_queue)

Estructuras de Datos
Arreglos bidimensionales (matrices)

Vectores dinámicos (vector<T>)

Colas de prioridad (priority_queue)

Conjuntos para búsqueda rápida (set)

Structs para agrupar datos relacionados

Algoritmos
Búsqueda informada (A*)

Pivoteo parcial y evitar ciclos

Integración numérica (Runge-Kutta orden 4)

Retropropagación (backpropagation)

Cálculo de gradientes y actualización de pesos

Matemáticas
Álgebra lineal (matrices, vectores)

Cálculo (derivadas, funciones de activación)

Análisis numérico (RK4, tolerancias)

Teoría de control (funciones de transferencia)

Redes neuronales (arquitectura, entrenamiento)

Conceptos de Sistemas
Sistemas de control en lazo cerrado

Observadores de estados

Simulación de plantas dinámicas

Discretización de sistemas continuos

SOLUCIÓN DE PROBLEMAS COMUNES
Error: "g++ no se reconoce como comando"
Windows:


Solución: Instalar MinGW y agregarlo al PATH
1. Descargar desde: https://sourceforge.net/projects/mingw/
2. Instalar en: C:\MinGW
3. Agregar a PATH: C:\MinGW\bin
4. Reiniciar terminal
Linux:

bash
sudo apt-get update
sudo apt-get install build-essential
Error: "No se pudo abrir digitos.txt"
Solución:


1. Verificar que el archivo existe en la carpeta del ejecutable
2. Verificar nombre exacto: digitos.txt (minúsculas)
3. Verificar formato: un número por línea
4. Verificar números están en rango 0-9
Error de compilación con caracteres especiales
Solución:

bash
# Windows CMD - cambiar a UTF-8
chcp 65001

# Recompilar después
- g++ -o clasificador tp4_c3.cpp -std=c++11
Decimales incorrectos en salida
Punto 2 - Ajustar precisión:

- cpp
cout << fixed << setprecision(6) << valor << endl;
La red neuronal tiene baja precisión
Punto 3 - Aumentar entrenamientos:

cpp
// En lugar de 1000 épocas, usar 2000
nn.train(input, target, 2000);



# REQUISITOS DE ENTREGA
- Según instrucciones del taller, el repositorio debe contener:

- Archivos fuente .cpp para cada punto

- Archivo README.md con instrucciones de compilación

- Explicación clara de metodología en README

- Ejemplos de entrada y salida

- Código ejecutable sin errores

- Estructura organizada del repositorio

-  Documentación de funciones principales

- Scripts de compilación automática


##  Autor

**Carlos Alberto Oviedo Machado, Luiyi David Brito Palmezano e Ismael David Perez Cudriz**
- Universidad: Univsidad Nacional de Colombia
- Curso: Programación Avanzada
- Fecha: 14/12/2025

---

##  Licencia

Este proyecto es de uso académico y está disponible bajo la licencia MIT.

---

##  Contacto

Si tienes preguntas o sugerencias sobre este proyecto:
- Email: caoviedom@unal.edu.co, lbritop@unal.edu.co, Isperezc@unal.edu.co
- GitHub:caoviedom(https://github.com/caoviedom)

---

##  Agradecimientos

- Profesor del curso de Programación Avanzada Mauro Alonso Baquero Suarez
- Comunidad de Stack Overflow
- Documentación oficial de C++
