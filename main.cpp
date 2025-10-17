#include <iostream>
#include <iomanip>
#include <exception>
#include "MatrizDinamica.h"

// Helper: imprimir matriz generica
template <typename T>
void imprimir(const MatrizDinamica<T> &M, int precision = 1) {
    for (int i = 0; i < M.getFilas(); ++i) {
        std::cout << "| ";
        for (int j = 0; j < M.getColumnas(); ++j) {
            if constexpr (std::is_floating_point_v<T>) {
                std::cout << std::fixed << std::setprecision(precision) << M.at(i, j) << " | ";
            } else {
                std::cout << M.at(i, j) << " | ";
            }
        }
        std::cout << "\n";
    }
}

int main() {
    try {
        std::cout << "--- Sistema de Analisis de Transformaciones Lineales ---\n\n";

        // Prueba con tipo int
        std::cout << ">> Prueba de Matriz ENTERA (Matriz A) <<\n";
        std::cout << "Creando Matriz A (Tipo INT) de 2x3...\n\n";
        MatrizDinamica<int> A(2, 3);

        std::cout << "Ingresar valores para A (2x3):\n";
        for (int i = 0; i < A.getFilas(); ++i) {
            for (int j = 0; j < A.getColumnas(); ++j) {
                std::cout << "A[" << i << "," << j << "]: ";
                int v; std::cin >> v;
                A.at(i, j) = v;
            }
        }

        std::cout << "\n>> Redimensionando Matriz A <<\n";
        std::cout << "Redimensionando A a 3x3. Datos conservados:\n";
        A.redimensionar(3, 3);
        imprimir(A, 0);

        // Prueba con tipo float y multiplicacion generica
        std::cout << "\n>> Prueba de Multiplicacion (Tipo FLOAT) <<\n";
        std::cout << "Creando Matriz B (Tipo FLOAT) de 3x2...\n";
        MatrizDinamica<float> B(3, 2);
        // Valores de ejemplo segun enunciado
        B.at(0,0) = 1.5f; B.at(0,1) = 0.5f;
        B.at(1,0) = 2.0f; B.at(1,1) = 1.0f;
        B.at(2,0) = 1.0f; B.at(2,1) = 2.5f;

        std::cout << "B[0,0]: 1.5, B[0,1]: 0.5\n";
        std::cout << "B[1,0]: 2.0, B[1,1]: 1.0\n";
        std::cout << "B[2,0]: 1.0, B[2,1]: 2.5\n\n";

        std::cout << "Matriz C = A(3x3) x B(3x2) ...\n\n";
        auto C = multiplicar(A, B); // tipo comun float
        std::cout << "Matriz C (Resultado 3x2, Tipo FLOAT):\n";
        imprimir(C, 1);

        std::cout << "\nLiberando memoria de todas las matrices...\n";
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
