#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

using namespace std;

// Constantes del derivador
const double L = 1.8;
const double tau_s = 0.004;  // 250 Hz
const double lambda0 = 1.3;
const double lambda1 = 1.85;
const double lambda2 = 2.79;
const double lambda3 = 6.48;
const int n = 3;  // Orden del derivador

// Clase para el Derivador de Orden Superior
class HighOrderDerivator {
private:
    double z0, z1, z2, z3;
    
    // Funcion phi segun la ecuacion del documento
    double phi(int i, double error) {
        double exponent = (double)(n - i) / (double)(n + 1);
        double lambda_vals[] = {lambda0, lambda1, lambda2, lambda3};
        double L_power = pow(L, (double)(i + 1) / (double)(n + 1));
        
        double sign_error = (error >= 0) ? 1.0 : -1.0;
        
        return -lambda_vals[i] * L_power * pow(abs(error), exponent) * sign_error;
    }
    
public:
    HighOrderDerivator() : z0(0), z1(0), z2(0), z3(0) {}
    
    // Actualizacion segun ecuaciones (2) del documento
    void update(double f_ref) {
        double error = z0 - f_ref;
        
        double phi0 = phi(0, error);
        double phi1 = phi(1, error);
        double phi2 = phi(2, error);
        double phi3 = phi(3, error);
        
        // Ecuaciones (2) exactas del documento
        double z0_new = z0 + tau_s * phi0 + tau_s * z1 + 
                        (tau_s * tau_s / 2.0) * z2 + 
                        (tau_s * tau_s * tau_s / 6.0) * z3;
                        
        double z1_new = z1 + tau_s * phi1 + tau_s * z2 + 
                        (tau_s * tau_s / 2.0) * z3;
                        
        double z2_new = z2 + tau_s * phi2 + tau_s * z3;
        
        double z3_new = z3 + tau_s * phi3;
        
        z0 = z0_new;
        z1 = z1_new;
        z2 = z2_new;
        z3 = z3_new;
    }
    
    double getZ0() const { return z0; }  // Seguimiento de f0
    double getZ1() const { return z1; }  // Primera derivada
    double getZ2() const { return z2; }  // Segunda derivada
    double getZ3() const { return z3; }  // Tercera derivada
    
    void reset() {
        z0 = z1 = z2 = z3 = 0;
    }
};

// Clase para la Planta G(s) = (7s^2 - 28s + 21)/(s^3 + 9.8s^2 + 30.65s + 30.1)
class Plant {
private:
    // Coeficientes continuos
    vector<double> num_cont = {7.0, -28.0, 21.0};      // 7s^2 - 28s + 21
    vector<double> den_cont = {1.0, 9.8, 30.65, 30.1}; // s^3 + 9.8s^2 + 30.65s + 30.1
    
    // Estados para ecuaciones de estado (forma controlable)
    vector<double> x;
    double u_prev;
    
public:
    Plant() {
        x.resize(3, 0.0);
        u_prev = 0.0;
    }
    
    // Metodo de integracion Runge-Kutta 4 para estabilidad
    double update(double u) {
        // Ecuaciones de estado de la planta en forma continua
        // x_punto = Ax + Bu, y = Cx + Du
        
        // Coeficientes de la ecuacion diferencial
        double a0 = -den_cont[3] / den_cont[0];
        double a1 = -den_cont[2] / den_cont[0];
        double a2 = -den_cont[1] / den_cont[0];
        
        double b0 = num_cont[2] / den_cont[0];
        double b1 = (num_cont[1] - b0 * den_cont[2]) / den_cont[0];
        double b2 = (num_cont[0] - b0 * den_cont[1] - b1 * den_cont[2]) / den_cont[0];
        
        // RK4 para integracion numerica
        vector<double> k1(3), k2(3), k3(3), k4(3), x_temp(3);
        
        // k1
        k1[0] = x[1];
        k1[1] = x[2];
        k1[2] = a0 * x[0] + a1 * x[1] + a2 * x[2] + u;
        
        // k2
        for (int i = 0; i < 3; i++) x_temp[i] = x[i] + 0.5 * tau_s * k1[i];
        k2[0] = x_temp[1];
        k2[1] = x_temp[2];
        k2[2] = a0 * x_temp[0] + a1 * x_temp[1] + a2 * x_temp[2] + u;
        
        // k3
        for (int i = 0; i < 3; i++) x_temp[i] = x[i] + 0.5 * tau_s * k2[i];
        k3[0] = x_temp[1];
        k3[1] = x_temp[2];
        k3[2] = a0 * x_temp[0] + a1 * x_temp[1] + a2 * x_temp[2] + u;
        
        // k4
        for (int i = 0; i < 3; i++) x_temp[i] = x[i] + tau_s * k3[i];
        k4[0] = x_temp[1];
        k4[1] = x_temp[2];
        k4[2] = a0 * x_temp[0] + a1 * x_temp[1] + a2 * x_temp[2] + u;
        
        // Actualizar estado
        for (int i = 0; i < 3; i++) {
            x[i] = x[i] + (tau_s / 6.0) * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]);
        }
        
        // Salida: y = Cx + Du
        double y = b0 * x[0] + b1 * x[1] + b2 * x[2];
        
        u_prev = u;
        return y;
    }
    
    void reset() {
        fill(x.begin(), x.end(), 0.0);
        u_prev = 0.0;
    }
};

// Generadores de senales de referencia
double stepSignal(double t) {
    // Escalon que comienza en t=1s con amplitud 1
    return (t >= 1.0) ? 1.0 : 0.0;
}

double rampSignal(double t) {
    // Rampa que comienza en t=1s con pendiente 0.5
    return (t >= 1.0) ? (t - 1.0) * 0.5 : 0.0;
}

double sinusoidalSignal(double t) {
    // Senoidal: sin(2*pi*f*t) con f=0.5 Hz, comienza en t=0.5s
    return (t >= 0.5) ? sin(2.0 * M_PI * 0.5 * t) : 0.0;
}

// Funcion principal de simulacion HIL
void runHILSimulation(int signalType, double simTime, const string& filename) {
    Plant plant;
    HighOrderDerivator derivator;
    
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error: No se pudo crear el archivo " << filename << endl;
        return;
    }
    
    // Encabezado del archivo con todas las senales requeridas
    outFile << "Tiempo,Referencia_f(t),Entrada_Planta_u(t),Salida_Planta_y(t),"
            << "z0_Seguimiento,z1_Derivada1,z2_Derivada2,z3_Derivada3" << endl;
    
    cout << "\n+============================================================+" << endl;
    cout << "|       SIMULACION HIL - Hardware in the Loop              |" << endl;
    cout << "|  Planta SISO + Derivador de Orden Superior               |" << endl;
    cout << "+============================================================+\n" << endl;
    
    cout << "Configuracion de la simulacion:" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << "- Planta: G(s) = (7s^2-28s+21)/(s^3+9.8s^2+30.65s+30.1)" << endl;
    cout << "- Frecuencia de muestreo: 250 Hz (Ts = 0.004 s)" << endl;
    cout << "- Metodo de discretizacion: Runge-Kutta 4 orden" << endl;
    cout << "- Constante de Lipschitz: L = " << L << endl;
    cout << "- Parametros lambda: [" << lambda0 << ", " << lambda1 << ", " 
         << lambda2 << ", " << lambda3 << "]" << endl;
    cout << "- Senal de referencia: ";
    
    if (signalType == 1) cout << "ESCALON UNITARIO (en t=1s)" << endl;
    else if (signalType == 2) cout << "RAMPA (pendiente=0.5, inicia en t=1s)" << endl;
    else cout << "SENOIDAL (f=0.5Hz, inicia en t=0.5s)" << endl;
    
    cout << "- Tiempo de simulacion: " << simTime << " segundos" << endl;
    cout << "------------------------------------------------------------\n" << endl;
    
    int totalSamples = static_cast<int>(simTime / tau_s);
    int progressStep = max(1, totalSamples / 50);
    
    auto startTime = chrono::high_resolution_clock::now();
    
    cout << "Ejecutando simulacion..." << endl;
    cout << "[";
    
    for (int k = 0; k < totalSamples; k++) {
        double t = k * tau_s;
        
        // 1. Generar senal de referencia f(t)
        double f_ref;
        if (signalType == 1) f_ref = stepSignal(t);
        else if (signalType == 2) f_ref = rampSignal(t);
        else f_ref = sinusoidalSignal(t);
        
        // 2. Actualizar derivador con la senal de referencia
        //    El derivador calcula z0 (seguimiento), z1, z2, z3 (derivadas)
        derivator.update(f_ref);
        
        // 3. Usar z0 como entrada a la planta (seguimiento de referencia)
        double plant_input = derivator.getZ0();
        
        // 4. Actualizar planta con la entrada
        double plant_output = plant.update(plant_input);
        
        // 5. Guardar todas las senales en el archivo
        outFile << fixed << setprecision(8)
                << t << ","
                << f_ref << ","
                << plant_input << ","
                << plant_output << ","
                << derivator.getZ0() << ","
                << derivator.getZ1() << ","
                << derivator.getZ2() << ","
                << derivator.getZ3() << endl;
        
        // Mostrar barra de progreso
        if (k % progressStep == 0) {
            cout << "=" << flush;
        }
    }
    
    cout << "] 100%" << endl;
    
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    
    outFile.close();
    
    cout << "\n+============================================================+" << endl;
    cout << "|              SIMULACION COMPLETADA                        |" << endl;
    cout << "+============================================================+" << endl;
    cout << "[OK] Muestras procesadas: " << totalSamples << endl;
    cout << "[OK] Tiempo de ejecucion: " << duration.count() << " ms" << endl;
    cout << "[OK] Datos guardados en: " << filename << endl;
    cout << "[OK] Formato: CSV con 8 columnas" << endl;
    cout << "\nColumnas del archivo:" << endl;
    cout << "  1. Tiempo (s)" << endl;
    cout << "  2. Referencia f(t)" << endl;
    cout << "  3. Entrada a la planta u(t)" << endl;
    cout << "  4. Salida de la planta y(t)" << endl;
    cout << "  5. z0 - Seguimiento de f0" << endl;
    cout << "  6. z1 - Primera derivada" << endl;
    cout << "  7. z2 - Segunda derivada" << endl;
    cout << "  8. z3 - Tercera derivada" << endl;
    cout << "============================================================\n" << endl;
}

int main() {
    cout << "\n+==============================================================+" << endl;
    cout << "|     TRABAJO PRACTICO No. 4 - Punto 2 (50%)                  |" << endl;
    cout << "|     Sistema HIL: Planta SISO + Derivador Orden Superior    |" << endl;
    cout << "|     Programacion Avanzada - Ing. Mecatronica               |" << endl;
    cout << "+==============================================================+\n" << endl;
    
    int option;
    bool validOption = false;
    
    do {
        cout << "+============================================+" << endl;
        cout << "|  SELECCIONE EL TIPO DE SENAL DE REFERENCIA |" << endl;
        cout << "+============================================+" << endl;
        cout << "  1. Senal Escalon Unitario" << endl;
        cout << "  2. Senal Rampa" << endl;
        cout << "  3. Senal Senoidal" << endl;
        cout << "\n-> Ingrese su opcion (1-3): ";
        
        cin >> option;
        
        if (cin.fail() || option < 1 || option > 3) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "\n[X] Opcion invalida. Por favor ingrese 1, 2 o 3.\n" << endl;
        } else {
            validOption = true;
        }
    } while (!validOption);
    
    double simTime;
    bool validTime = false;
    
    do {
        cout << "\n-> Ingrese el tiempo de simulacion en segundos (5-100): ";
        cin >> simTime;
        
        if (cin.fail() || simTime < 5 || simTime > 100) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[X] Tiempo invalido. Debe estar entre 5 y 100 segundos." << endl;
        } else {
            validTime = true;
        }
    } while (!validTime);
    
    // Generar nombre de archivo segun el tipo de senal
    string filename;
    if (option == 1) filename = "hil_escalon_output.txt";
    else if (option == 2) filename = "hil_rampa_output.txt";
    else filename = "hil_senoidal_output.txt";
    
    cout << "\n" << endl;
    
    // Ejecutar simulacion HIL
    runHILSimulation(option, simTime, filename);
    
    cout << "Puede analizar los resultados con Python, MATLAB o Excel." << endl;
    cout << "Ejemplo Python:" << endl;
    cout << "  import pandas as pd" << endl;
    cout << "  import matplotlib.pyplot as plt" << endl;
    cout << "  data = pd.read_csv('" << filename << "')" << endl;
    cout << "  data.plot(x='Tiempo', y=['Referencia_f(t)', 'Salida_Planta_y(t)'])" << endl;
    cout << "  plt.show()" << endl;
    
    cout << "\nPresione Enter para salir...";
    cin.ignore();
    cin.get();
    
    return 0;
}