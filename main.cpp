#include <iostream>
#include <iomanip>

template <typename T>
class MatrizDinamica {
private:
	T **datos;
	int filas;
	int columnas;

public:
	MatrizDinamica(int f, int c) : datos(nullptr), filas(f), columnas(c) {
		// Asignacion de memoria 2D con punteros
		datos = new T*[filas];
		for (int i = 0; i < filas; ++i) {
			datos[i] = new T[columnas];
			// Inicializacion a T() (cero para tipos numericos)
			for (int j = 0; j < columnas; ++j) {
				datos[i][j] = T();
			}
		}
	}

	~MatrizDinamica() {
		// Liberacion de memoria en orden inverso
		if (datos) {
			for (int i = 0; i < filas; ++i) {
				delete[] datos[i];
			}
			delete[] datos;
		}
		datos = nullptr;
		filas = 0;
		columnas = 0;
	}

	// Acceso directo (sin validaciones extra)
	T &at(int i, int j) { return datos[i][j]; }
	const T &at(int i, int j) const { return datos[i][j]; }

	int getFilas() const { return filas; }
	int getColumnas() const { return columnas; }

	// Redimension basica: conserva datos y llena nuevos con T()
	void redimensionar(int nuevaF, int nuevaC) {
		// Crear nueva matriz
		T **nuevo = new T*[nuevaF];
		for (int i = 0; i < nuevaF; ++i) {
			nuevo[i] = new T[nuevaC];
			for (int j = 0; j < nuevaC; ++j) {
				nuevo[i][j] = T();
			}
		}

		// Copiar datos en rango menor
		int minF = (filas < nuevaF) ? filas : nuevaF;
		int minC = (columnas < nuevaC) ? columnas : nuevaC;
		for (int i = 0; i < minF; ++i) {
			for (int j = 0; j < minC; ++j) {
				nuevo[i][j] = datos[i][j];
			}
		}

		// Liberar matriz antigua
		for (int i = 0; i < filas; ++i) {
			delete[] datos[i];
		}
		delete[] datos;

		// Actualizar punteros y dimensiones
		datos = nuevo;
		filas = nuevaF;
		columnas = nuevaC;
	}
};

int main() {
	std::cout << "--- Sistema de Analisis de Transformaciones Lineales (Parcial) ---\n\n";

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
	for (int i = 0; i < A.getFilas(); ++i) {
		std::cout << "| ";
		for (int j = 0; j < A.getColumnas(); ++j) {
			std::cout << A.at(i, j) << " | ";
		}
		std::cout << "\n";
	}

	// Prueba con tipo float (sin multiplicacion aun)
	std::cout << "\n>> Prueba de Matriz (Tipo FLOAT) <<\n";
	std::cout << "Creando Matriz B (Tipo FLOAT) de 3x2...\n";
	MatrizDinamica<float> B(3, 2);
	// Valores de ejemplo segun enunciado
	B.at(0,0) = 1.5f; B.at(0,1) = 0.5f;
	B.at(1,0) = 2.0f; B.at(1,1) = 1.0f;
	B.at(2,0) = 1.0f; B.at(2,1) = 2.5f;

	std::cout << "Matriz B (3x2, float):\n";
	for (int i = 0; i < B.getFilas(); ++i) {
		std::cout << "| ";
		for (int j = 0; j < B.getColumnas(); ++j) {
			std::cout << std::fixed << std::setprecision(1) << B.at(i, j) << " | ";
		}
		std::cout << "\n";
	}

	std::cout << "\n(Multiplicacion A x B se implementara mas adelante)\n";

	std::cout << "\nLiberando memoria de todas las matrices...\n";
	// Destructores (se llaman automáticamente al final de main)
	return 0;
}
