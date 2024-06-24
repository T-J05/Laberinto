#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

// Definimos una clase para crear y resolver laberintos
class Laberinto {
public:
    Laberinto(int ancho, int alto); // Constructor de la clase
    void generar(); // Método para generar el laberinto
    void imprimir(); // Método para imprimir el laberinto
    bool resolver(); // Método para resolver el laberinto
    void imprimirSolucion(); // Método para imprimir la solución del laberinto

private:
    // Estructura que representa una celda en el laberinto
    struct Celda {
        bool visitado = false; // Bandera para saber si la celda fue visitada
        bool arriba = true; // Pared superior
        bool abajo = true; // Pared inferior
        bool izquierda = true; // Pared izquierda
        bool derecha = true; // Pared derecha
        bool enSolucion = false; // Bandera para saber si la celda está en la solución
    };

    int ancho; // Ancho del laberinto
    int alto; // Alto del laberinto
    std::vector<std::vector<Celda>> grilla; // Matriz de celdas que representa el laberinto

    // Métodos privados para ayudar a generar y resolver el laberinto
    void eliminarPared(Celda& actual, Celda& siguiente, char direccion);
    bool resolverAux(int x, int y);
    void reiniciarVisitados();
};

// Constructor de la clase Laberinto
Laberinto::Laberinto(int ancho, int alto) : ancho(ancho), alto(alto) {
    grilla.resize(alto, std::vector<Celda>(ancho)); // Crear la grilla con las celdas
    srand(time(nullptr)); // Inicializar la semilla para la generación de números aleatorios
}

// Método para generar el laberinto usando un algoritmo de backtracking
void Laberinto::generar() {
    std::stack<std::pair<int, int>> pila; // Pila para llevar la cuenta de las celdas visitadas
    int inicioX = 0; // Coordenada X de inicio
    int inicioY = 0; // Coordenada Y de inicio
    grilla[inicioY][inicioX].visitado = true; // Marcar la celda de inicio como visitada
    pila.push({inicioX, inicioY}); // Agregar la celda de inicio a la pila

    while (!pila.empty()) {
        int x = pila.top().first; // Obtener la coordenada X de la celda actual
        int y = pila.top().second; // Obtener la coordenada Y de la celda actual

        // Lista de vecinos no visitados
        std::vector<std::pair<int, int>> vecinos;

        // Agregar vecinos no visitados a la lista
        if (x > 0 && !grilla[y][x - 1].visitado) vecinos.push_back({x - 1, y}); // Izquierda
        if (x < ancho - 1 && !grilla[y][x + 1].visitado) vecinos.push_back({x + 1, y}); // Derecha
        if (y > 0 && !grilla[y - 1][x].visitado) vecinos.push_back({x, y - 1}); // Arriba
        if (y < alto - 1 && !grilla[y + 1][x].visitado) vecinos.push_back({x, y + 1}); // Abajo

        if (!vecinos.empty()) {
            // Elegir un vecino al azar y eliminar la pared entre la celda actual y el vecino
            auto [nextX, nextY] = vecinos[rand() % vecinos.size()];

            if (nextX == x - 1) eliminarPared(grilla[y][x], grilla[nextY][nextX], 'I');
            else if (nextX == x + 1) eliminarPared(grilla[y][x], grilla[nextY][nextX], 'D');
            else if (nextY == y - 1) eliminarPared(grilla[y][x], grilla[nextY][nextX], 'A');
            else if (nextY == y + 1) eliminarPared(grilla[y][x], grilla[nextY][nextX], 'B');

            // Marcar el vecino como visitado y agregarlo a la pila
            grilla[nextY][nextX].visitado = true;
            pila.push({nextX, nextY});
        } else {
            // Si no hay vecinos no visitados, retroceder en la pila
            pila.pop();
        }
    }

    // Asegurar la entrada y salida del laberinto
    grilla[0][0].izquierda = false;  // Entrada
    grilla[alto - 1][ancho - 1].derecha = false;  // Salida

    reiniciarVisitados();  // Resetear el estado de visitado antes de resolver el laberinto
}

// Método para eliminar la pared entre dos celdas
void Laberinto::eliminarPared(Celda& actual, Celda& siguiente, char direccion) {
    // Dependiendo de la dirección, eliminar la pared adecuada
    switch (direccion) {
        case 'I':
            actual.izquierda = false; // Eliminar pared izquierda de la celda actual
            siguiente.derecha = false; // Eliminar pared derecha de la celda siguiente
            break;
        case 'D':
            actual.derecha = false; // Eliminar pared derecha de la celda actual
            siguiente.izquierda = false; // Eliminar pared izquierda de la celda siguiente
            break;
        case 'A':
            actual.arriba = false; // Eliminar pared superior de la celda actual
            siguiente.abajo = false; // Eliminar pared inferior de la celda siguiente
            break;
        case 'B':
            actual.abajo = false; // Eliminar pared inferior de la celda actual
            siguiente.arriba = false; // Eliminar pared superior de la celda siguiente
            break;
    }
}

// Método para resolver el laberinto usando backtracking
bool Laberinto::resolver() {
    return resolverAux(0, 0); // Comenzar la solución desde la celda de inicio
}

// Método auxiliar recursivo para resolver el laberinto
bool Laberinto::resolverAux(int x, int y) {
    if (x == ancho - 1 && y == alto - 1) { // Si se llega a la celda de salida
        grilla[y][x].enSolucion = true; // Marcar la celda como parte de la solución
        return true; // Laberinto resuelto
    }

    if (x >= 0 && x < ancho && y >= 0 && y < alto && !grilla[y][x].visitado) { // Si la celda es válida y no visitada
        grilla[y][x].visitado = true; // Marcar la celda como visitada

        // Intentar mover en las 4 direcciones
        if (!grilla[y][x].arriba && resolverAux(x, y - 1)) {
            grilla[y][x].enSolucion = true; // Marcar la celda como parte de la solución
            return true;
        }
        if (!grilla[y][x].derecha && resolverAux(x + 1, y)) {
            grilla[y][x].enSolucion = true; // Marcar la celda como parte de la solución
            return true;
        }
        if (!grilla[y][x].abajo && resolverAux(x, y + 1)) {
            grilla[y][x].enSolucion = true; // Marcar la celda como parte de la solución
            return true;
        }
        if (!grilla[y][x].izquierda && resolverAux(x - 1, y)) {
            grilla[y][x].enSolucion = true; // Marcar la celda como parte de la solución
            return true;
        }
    }
    return false; // Si no se puede resolver, regresar falso
}

// Método para reiniciar el estado de visitado de todas las celdas
void Laberinto::reiniciarVisitados() {
    for (int y = 0; y < alto; ++y) { // Recorrer todas las filas
        for (int x = 0; x < ancho; ++x) { // Recorrer todas las columnas
            grilla[y][x].visitado = false; // Reiniciar el estado de visitado
        }
    }
}

// Método para imprimir la solución del laberinto
void Laberinto::imprimirSolucion() {
    for (int y = 0; y < alto; ++y) {
        // Imprimir paredes superiores
        for (int x = 0; x < ancho; ++x) {
            std::cout << (grilla[y][x].arriba ? "#___" : "#   ");
        }
        std::cout << "#\n";

        // Imprimir paredes izquierdas y celdas
        for (int x = 0; x < ancho; ++x) {
            if (grilla[y][x].izquierda)
                std::cout << "|";
            else
                std::cout << " ";
            if (grilla[y][x].enSolucion)
                std::cout << " . ";
            else
                std::cout << "   ";
        }
        std::cout << "|\n";
    }

    // Imprimir paredes inferiores
    for (int x = 0; x < ancho; ++x) {
        std::cout << "#___";
    }
    std::cout << "#\n";
}

// Método para imprimir el laberinto generado
void Laberinto::imprimir() {
    for (int y = 0; y < alto; ++y) {
        // Imprimir paredes superiores
        for (int x = 0; x < ancho; ++x) {
            std::cout << (grilla[y][x].arriba ? "#___" : "#   ");
        }
        std::cout << "#\n";

        // Imprimir paredes izquierdas y celdas
        for (int x = 0; x < ancho; ++x) {
            std::cout << (grilla[y][x].izquierda ? "|   " : "    ");
        }
        std::cout << "|\n";
    }

    // Imprimir paredes inferiores
    for (int x = 0; x < ancho; ++x) {
        std::cout << "#___";
    }
    std::cout << "#\n";
}

// Función principal
int main() {
    int ancho;
    int alto;
    std::cout << "Digite el numero del ancho: ";
    std::cin >> ancho; // Leer el ancho del laberinto
    std::cout << "Digite el numero del alto: ";
    std::cin >> alto; // Leer el alto del laberinto

    Laberinto laberinto(ancho, alto); // Crear un laberinto con las dimensiones dadas
    laberinto.generar(); // Generar el laberinto
    laberinto.imprimir(); // Imprimir el laberinto

    std::cout << " \nLa solucion al laberinto\n";

    laberinto.resolver(); // Resolver el laberinto
    laberinto.imprimirSolucion(); // Imprimir la solución del laberinto

    return 0;
}