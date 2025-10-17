#pragma once
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>

// Clase generica de matriz dinamica 2D con gestion manual de memoria (T**)
// Implementa la Regla de los Cinco: ctor/dtor, copia, asignacion, movimiento
template <typename T>
class MatrizDinamica {
private:
    T **datos;
    int filas;
    int columnas;

    void liberar() noexcept {
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

    void reservar_e_inicializar(int f, int c) {
        if (f < 0 || c < 0) throw std::invalid_argument("Dimensiones negativas");
        filas = f;
        columnas = c;
        if (filas == 0 || columnas == 0) {
            datos = nullptr;
            return;
        }
        datos = new T*[filas];
        try {
            for (int i = 0; i < filas; ++i) {
                datos[i] = new T[columnas];
                for (int j = 0; j < columnas; ++j) datos[i][j] = T();
            }
        } catch (...) {
            // Limpieza segura ante excepcion durante la reserva
            for (int i = 0; i < filas; ++i) {
                if (datos && datos[i]) delete[] datos[i];
            }
            delete[] datos;
            datos = nullptr; filas = 0; columnas = 0;
            throw;
        }
    }

public:
    // Constructores
    MatrizDinamica() : datos(nullptr), filas(0), columnas(0) {}

    // Constructor: crea matriz f x c e inicializa con T()
    MatrizDinamica(int f, int c) : datos(nullptr), filas(0), columnas(0) {
        reservar_e_inicializar(f, c);
    }

    // Destructor: libera memoria
    ~MatrizDinamica() { liberar(); }

    // Constructor de copia (copia profunda)
    MatrizDinamica(const MatrizDinamica &o) : datos(nullptr), filas(0), columnas(0) {
        reservar_e_inicializar(o.filas, o.columnas);
        for (int i = 0; i < filas; ++i)
            for (int j = 0; j < columnas; ++j)
                datos[i][j] = o.datos[i][j];
    }

    // Constructor de movimiento
    MatrizDinamica(MatrizDinamica &&o) noexcept : datos(o.datos), filas(o.filas), columnas(o.columnas) {
        o.datos = nullptr; o.filas = 0; o.columnas = 0;
    }

    // Operador de asignacion por copia (idioma copia-e-intercambio)
    MatrizDinamica &operator=(const MatrizDinamica &o) {
        if (this == &o) return *this;
        MatrizDinamica tmp(o);
        swap(tmp);
        return *this;
    }

    // Operador de asignacion por movimiento
    MatrizDinamica &operator=(MatrizDinamica &&o) noexcept {
        if (this == &o) return *this;
        liberar();
        datos = o.datos; filas = o.filas; columnas = o.columnas;
        o.datos = nullptr; o.filas = 0; o.columnas = 0;
        return *this;
    }

    void swap(MatrizDinamica &o) noexcept {
        std::swap(datos, o.datos);
        std::swap(filas, o.filas);
        std::swap(columnas, o.columnas);
    }

    // Acceso a elemento con validacion
    T &at(int i, int j) {
        if (i < 0 || i >= filas || j < 0 || j >= columnas) throw std::out_of_range("Indice fuera de rango");
        return datos[i][j];
    }
    const T &at(int i, int j) const {
        if (i < 0 || i >= filas || j < 0 || j >= columnas) throw std::out_of_range("Indice fuera de rango");
        return datos[i][j];
    }

    // Acceso sin comprobacion (para rendimiento interno)
    T &operator()(int i, int j) { return datos[i][j]; }
    const T &operator()(int i, int j) const { return datos[i][j]; }

    // Dimensiones
    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }

    // Redimension con copia y fill a T(); valida y maneja nuevos/menores tamanos
    void redimensionar(int nuevaF, int nuevaC) {
        if (nuevaF < 0 || nuevaC < 0) throw std::invalid_argument("Dimensiones negativas en redimensionar");
        T **nuevo = nullptr;
        if (nuevaF > 0 && nuevaC > 0) {
            nuevo = new T*[nuevaF];
            try {
                for (int i = 0; i < nuevaF; ++i) {
                    nuevo[i] = new T[nuevaC];
                    for (int j = 0; j < nuevaC; ++j) nuevo[i][j] = T();
                }
            } catch (...) {
                for (int i = 0; i < nuevaF; ++i) {
                    if (nuevo && nuevo[i]) delete[] nuevo[i];
                }
                delete[] nuevo;
                throw;
            }

            const int minF = (filas < nuevaF) ? filas : nuevaF;
            const int minC = (columnas < nuevaC) ? columnas : nuevaC;
            for (int i = 0; i < minF; ++i)
                for (int j = 0; j < minC; ++j)
                    nuevo[i][j] = datos[i][j];
        }

        // Sustituir almacenamiento
        liberar();
        datos = nuevo; filas = nuevaF; columnas = nuevaC;
    }
};

// Multiplicacion matricial generica. Soporta tipos mixtos devolviendo el tipo comun
// C = A x B, donde A(f x n), B(n x c), C(f x c)
template <typename TA, typename TB>
auto multiplicar(const MatrizDinamica<TA> &A, const MatrizDinamica<TB> &B)
    -> MatrizDinamica<std::common_type_t<TA, TB>> {
    if (A.getColumnas() != B.getFilas())
        throw std::invalid_argument("Dimensiones incompatibles para multiplicacion");

    using TC = std::common_type_t<TA, TB>;
    MatrizDinamica<TC> C(A.getFilas(), B.getColumnas());
    for (int i = 0; i < A.getFilas(); ++i) {
        for (int j = 0; j < B.getColumnas(); ++j) {
            TC acumulado = TC();
            for (int k = 0; k < A.getColumnas(); ++k) {
                acumulado += static_cast<TC>(A(i, k)) * static_cast<TC>(B(k, j));
            }
            C(i, j) = acumulado;
        }
    }
    return C;
}
