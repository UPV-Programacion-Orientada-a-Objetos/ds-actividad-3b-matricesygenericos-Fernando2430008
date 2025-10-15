#pragma once
#include <cstddef>

template <typename T>
class MatrizDinamica {
private:
    T **datos;
    int filas;
    int columnas;

public:
    // Constructor: crea matriz f x c e inicializa con T()
    MatrizDinamica(int f, int c) : datos(nullptr), filas(f), columnas(c) {
        datos = new T*[filas];
        for (int i = 0; i < filas; ++i) {
            datos[i] = new T[columnas];
            for (int j = 0; j < columnas; ++j) datos[i][j] = T();
        }
    }

    // Destructor: libera memoria
    ~MatrizDinamica() {
        if (datos) {
            for (int i = 0; i < filas; ++i) delete[] datos[i];
            delete[] datos;
        }
        datos = nullptr; filas = 0; columnas = 0;
    }

    // Acceso a elemento
    T &at(int i, int j) { return datos[i][j]; }
    const T &at(int i, int j) const { return datos[i][j]; }

    // Dimensiones
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }

    // Redimension basica con copia y fill a T()
    void redimensionar(int nuevaF, int nuevaC) {
        T **nuevo = new T*[nuevaF];
        for (int i = 0; i < nuevaF; ++i) {
            nuevo[i] = new T[nuevaC];
            for (int j = 0; j < nuevaC; ++j) nuevo[i][j] = T();
        }
        int minF = (filas < nuevaF) ? filas : nuevaF;
        int minC = (columnas < nuevaC) ? columnas : nuevaC;
        for (int i = 0; i < minF; ++i)
            for (int j = 0; j < minC; ++j)
                nuevo[i][j] = datos[i][j];
        for (int i = 0; i < filas; ++i) delete[] datos[i];
        delete[] datos;
        datos = nuevo; filas = nuevaF; columnas = nuevaC;
    }
};
