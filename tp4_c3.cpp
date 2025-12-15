#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>
#include <string>
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std;

// Función de activación sigmoide
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivada de la función sigmoide
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// Función para pausar la ejecución
void pausar(int milisegundos) {
    this_thread::sleep_for(chrono::milliseconds(milisegundos));
}

// Clase para la Red Neuronal
class NeuralNetwork {
private:
    int input_size;
    int hidden_size;
    int output_size;
    
    vector<vector<double>> weights_input_hidden;
    vector<vector<double>> weights_hidden_output;
    vector<double> bias_hidden;
    vector<double> bias_output;
    
    double learning_rate;
    
public:
    NeuralNetwork(int input, int hidden, int output, double lr = 0.1) {
        input_size = input;
        hidden_size = hidden;
        output_size = output;
        learning_rate = lr;
        
        // Inicializar pesos y bias con valores aleatorios
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(-1.0, 1.0);
        
        // Pesos entrada -> oculta
        weights_input_hidden.resize(input_size, vector<double>(hidden_size));
        for (int i = 0; i < input_size; i++) {
            for (int j = 0; j < hidden_size; j++) {
                weights_input_hidden[i][j] = dis(gen);
            }
        }
        
        // Pesos oculta -> salida
        weights_hidden_output.resize(hidden_size, vector<double>(output_size));
        for (int i = 0; i < hidden_size; i++) {
            for (int j = 0; j < output_size; j++) {
                weights_hidden_output[i][j] = dis(gen);
            }
        }
        
        // Bias
        bias_hidden.resize(hidden_size);
        bias_output.resize(output_size);
        for (int i = 0; i < hidden_size; i++) {
            bias_hidden[i] = dis(gen);
        }
        for (int i = 0; i < output_size; i++) {
            bias_output[i] = dis(gen);
        }
    }
    
    // Propagación hacia adelante
    vector<double> forward(const vector<double>& input, vector<double>& hidden_layer) {
        // Capa oculta
        hidden_layer.resize(hidden_size);
        for (int j = 0; j < hidden_size; j++) {
            double sum = bias_hidden[j];
            for (int i = 0; i < input_size; i++) {
                sum += input[i] * weights_input_hidden[i][j];
            }
            hidden_layer[j] = sigmoid(sum);
        }
        
        // Capa de salida
        vector<double> output(output_size);
        for (int j = 0; j < output_size; j++) {
            double sum = bias_output[j];
            for (int i = 0; i < hidden_size; i++) {
                sum += hidden_layer[i] * weights_hidden_output[i][j];
            }
            output[j] = sigmoid(sum);
        }
        
        return output;
    }
    
    // Entrenamiento con backpropagation
    void train(const vector<double>& input, const vector<double>& target, int epochs) {
        for (int epoch = 0; epoch < epochs; epoch++) {
            vector<double> hidden_layer;
            vector<double> output = forward(input, hidden_layer);
            
            // Calcular error en la salida
            vector<double> output_error(output_size);
            vector<double> output_delta(output_size);
            for (int i = 0; i < output_size; i++) {
                output_error[i] = target[i] - output[i];
                output_delta[i] = output_error[i] * sigmoid_derivative(output[i]);
            }
            
            // Calcular error en la capa oculta
            vector<double> hidden_error(hidden_size);
            vector<double> hidden_delta(hidden_size);
            for (int i = 0; i < hidden_size; i++) {
                hidden_error[i] = 0.0;
                for (int j = 0; j < output_size; j++) {
                    hidden_error[i] += output_delta[j] * weights_hidden_output[i][j];
                }
                hidden_delta[i] = hidden_error[i] * sigmoid_derivative(hidden_layer[i]);
            }
            
            // Actualizar pesos oculta -> salida
            for (int i = 0; i < hidden_size; i++) {
                for (int j = 0; j < output_size; j++) {
                    weights_hidden_output[i][j] += learning_rate * output_delta[j] * hidden_layer[i];
                }
            }
            
            // Actualizar bias de salida
            for (int i = 0; i < output_size; i++) {
                bias_output[i] += learning_rate * output_delta[i];
            }
            
            // Actualizar pesos entrada -> oculta
            for (int i = 0; i < input_size; i++) {
                for (int j = 0; j < hidden_size; j++) {
                    weights_input_hidden[i][j] += learning_rate * hidden_delta[j] * input[i];
                }
            }
            
            // Actualizar bias oculta
            for (int i = 0; i < hidden_size; i++) {
                bias_hidden[i] += learning_rate * hidden_delta[i];
            }
        }
    }
    
    // Predecir
    vector<double> predict(const vector<double>& input) {
        vector<double> hidden_layer;
        return forward(input, hidden_layer);
    }
};

// Función para convertir número a vector de entrada (codificación one-hot)
vector<double> numberToInput(int number) {
    vector<double> input(10, 0.0);
    input[number] = 1.0;
    return input;
}

// Función para determinar las clases de un número
vector<double> getClasses(int number) {
    vector<double> classes(4, 0.0);
    
    // Clase 0: Números pares (0, 2, 4, 6, 8)
    if (number % 2 == 0) {
        classes[0] = 1.0;
    }
    
    // Clase 1: Números impares (1, 3, 5, 7, 9)
    if (number % 2 != 0) {
        classes[1] = 1.0;
    }
    
    // Clase 2: Números primos (2, 3, 5, 7)
    if (number == 2 || number == 3 || number == 5 || number == 7) {
        classes[2] = 1.0;
    }
    
    // Clase 3: Números compuestos (4, 6, 8, 9)
    if (number == 4 || number == 6 || number == 8 || number == 9) {
        classes[3] = 1.0;
    }
    
    return classes;
}

// Función para obtener el nombre de la clase
string getClassName(int classIndex) {
    switch(classIndex) {
        case 0: return "Numeros pares";
        case 1: return "Numeros impares";
        case 2: return "Numeros primos";
        case 3: return "Numeros compuestos";
        default: return "Desconocido";
    }
}

// Función para mostrar barra de progreso
void mostrarProgreso(int actual, int total, string mensaje) {
    int barWidth = 50;
    float progreso = (float)actual / total;
    int pos = barWidth * progreso;
    
    cout << "\r" << mensaje << " [";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "=";
        else if (i == pos) cout << ">";
        else cout << " ";
    }
    cout << "] " << int(progreso * 100.0) << "% (" << actual << "/" << total << ")";
    cout.flush();
}

int main() {
    // Configurar consola para caracteres especiales
    SetConsoleOutputCP(CP_UTF8);
    
    cout << "=================================================================" << endl;
    cout << "    RED NEURONAL ARTIFICIAL - CLASIFICACION DE NUMEROS" << endl;
    cout << "=================================================================" << endl;
    cout << "    Trabajo Practico No. 4 - Punto 3" << endl;
    cout << "    Programacion Avanzada - C++" << endl;
    cout << "=================================================================" << endl << endl;
    
    // Obtener directorio actual
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    cout << "[INFO] Directorio de trabajo: " << buffer << endl << endl;
    
    pausar(500);
    
    // Mostrar información de la arquitectura
    cout << "=================================================================" << endl;
    cout << "    CONFIGURACION DE LA RED NEURONAL" << endl;
    cout << "=================================================================" << endl;
    cout << "  * Capa de entrada:  10 neuronas (codificacion one-hot)" << endl;
    cout << "  * Capa oculta:       8 neuronas (funcion sigmoide)" << endl;
    cout << "  * Capa de salida:    4 neuronas (4 clases)" << endl;
    cout << "  * Tasa de aprendizaje: 0.5" << endl;
    cout << "  * Epocas por numero:   1000" << endl;
    cout << "=================================================================" << endl << endl;
    
    pausar(500);
    
    // Mostrar las clases
    cout << "=================================================================" << endl;
    cout << "    CLASES DE CLASIFICACION" << endl;
    cout << "=================================================================" << endl;
    cout << "  Clase 0: Numeros pares       -> 0, 2, 4, 6, 8" << endl;
    cout << "  Clase 1: Numeros impares     -> 1, 3, 5, 7, 9" << endl;
    cout << "  Clase 2: Numeros primos      -> 2, 3, 5, 7" << endl;
    cout << "  Clase 3: Numeros compuestos  -> 4, 6, 8, 9" << endl;
    cout << "=================================================================" << endl << endl;
    
    pausar(800);
    
    // Crear red neuronal
    cout << "[ETAPA 1] Inicializando red neuronal..." << endl;
    NeuralNetwork nn(10, 8, 4, 0.5);
    cout << "[OK] Red neuronal creada exitosamente." << endl << endl;
    
    pausar(300);
    
    // Entrenar la red
    cout << "[ETAPA 2] Iniciando entrenamiento de la red neuronal..." << endl << endl;
    
    for (int i = 0; i < 10; i++) {
        vector<double> input = numberToInput(i);
        vector<double> target = getClasses(i);
        
        cout << "  Entrenando numero " << i << "...";
        cout.flush();
        
        nn.train(input, target, 1000);
        
        cout << " [COMPLETADO]" << endl;
        pausar(100);
    }
    
    cout << "\n[OK] Entrenamiento completado exitosamente." << endl << endl;
    pausar(500);
    
    // Intentar abrir el archivo
    cout << "=================================================================" << endl;
    cout << "[ETAPA 3] Buscando archivo digitos.txt..." << endl;
    cout << "=================================================================" << endl;
    
    string filenames[] = {
        "digitos.txt",
        ".\\digitos.txt",
        "C:\\Users\\cesar\\Desktop\\TP4_C1\\digitos.txt"
    };
    
    ifstream file;
    bool fileOpened = false;
    string usedFilename;
    
    for (const string& filename : filenames) {
        cout << "  Intentando abrir: " << filename << "...";
        cout.flush();
        pausar(200);
        
        file.open(filename);
        if (file.is_open()) {
            fileOpened = true;
            usedFilename = filename;
            cout << " [ENCONTRADO]" << endl;
            break;
        } else {
            cout << " [NO ENCONTRADO]" << endl;
        }
    }
    
    cout << endl;
    
    if (!fileOpened) {
        cout << "[ERROR] No se pudo abrir el archivo digitos.txt en ninguna ruta." << endl;
        cout << "[INFO] Directorio actual: " << buffer << endl << endl;
        
        cout << "Deseas crear un archivo digitos.txt de ejemplo? (s/n): ";
        char respuesta;
        cin >> respuesta;
        
        if (respuesta == 's' || respuesta == 'S') {
            ofstream outFile("digitos.txt");
            if (outFile.is_open()) {
                for (int i = 0; i <= 9; i++) {
                    outFile << i << endl;
                }
                outFile.close();
                cout << "[OK] Archivo digitos.txt creado exitosamente." << endl;
                cout << "[INFO] Por favor, ejecuta el programa nuevamente." << endl;
            } else {
                cerr << "[ERROR] No se pudo crear el archivo." << endl;
            }
        }
        
        cout << "\nPresiona Enter para salir...";
        cin.ignore();
        cin.get();
        return 1;
    }
    
    cout << "[OK] Archivo abierto: " << usedFilename << endl << endl;
    pausar(300);
    
    // Leer y contar números primero
    cout << "[ETAPA 4] Leyendo numeros del archivo..." << endl;
    vector<int> numeros;
    int number;
    
    while (file >> number) {
        if (number >= 0 && number <= 9) {
            numeros.push_back(number);
            cout << "  Numero leido: " << number << endl;
            pausar(100);
        } else {
            cout << "  [ADVERTENCIA] Numero " << number << " fuera de rango [0-9] - IGNORADO" << endl;
        }
    }
    
    file.close();
    
    cout << "\n[OK] Total de numeros validos leidos: " << numeros.size() << endl << endl;
    pausar(500);
    
    // Clasificar números
    cout << "=================================================================" << endl;
    cout << "[ETAPA 5] CLASIFICACION DE NUMEROS" << endl;
    cout << "=================================================================" << endl << endl;
    
    for (size_t idx = 0; idx < numeros.size(); idx++) {
        int num = numeros[idx];
        
        cout << "-----------------------------------------------------------" << endl;
        cout << "  NUMERO " << (idx + 1) << " de " << numeros.size() << ": " << num << endl;
        cout << "-----------------------------------------------------------" << endl;
        
        vector<double> input = numberToInput(num);
        
        cout << "  > Procesando entrada en la red neuronal..." << endl;
        pausar(150);
        
        vector<double> output = nn.predict(input);
        
        cout << "  > Analizando salidas de la red..." << endl;
        pausar(150);
        
        cout << "\n  RESULTADOS DE CLASIFICACION:" << endl;
        
        bool clasificado = false;
        for (int i = 0; i < 4; i++) {
            double confianza = output[i] * 100;
            
            if (output[i] > 0.5) {
                cout << "    [X] Clase " << i << ": " << getClassName(i) << endl;
                cout << "        Confianza: " << fixed << setprecision(2) << confianza << "%" << endl;
                clasificado = true;
            } else {
                cout << "    [ ] Clase " << i << ": " << getClassName(i) << endl;
                cout << "        Confianza: " << fixed << setprecision(2) << confianza << "%" << endl;
            }
        }
        
        if (!clasificado) {
            cout << "\n  [ADVERTENCIA] No se pudo clasificar con confianza suficiente." << endl;
        }
        
        cout << endl;
        pausar(300);
    }
    
    // Resumen final
    cout << "=================================================================" << endl;
    cout << "    RESUMEN FINAL" << endl;
    cout << "=================================================================" << endl;
    cout << "  Total de numeros procesados: " << numeros.size() << endl;
    cout << "  Archivo procesado: " << usedFilename << endl;
    cout << "  Estado: COMPLETADO EXITOSAMENTE" << endl;
    cout << "=================================================================" << endl << endl;
    
    cout << "Presiona Enter para salir...";
    cin.ignore();
    cin.get();
    
    return 0;
}
