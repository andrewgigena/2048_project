// Integrantes:
// - Andrew Gigena, CI: 5111580-2, Email: mail@andrewgigena.dev
// - Horacio Duarte, CI: 5096812-7, Email: raulduarte45@hotmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

const int lado_tablero = 4;
const int espacio = 0;
const int maximo_puntaje = 2048;

// Genera un numero aleatoriamente de 2 o 4
int numero_aleatorio() {
  if (rand() % 2 == 1) {
    return 4;
  } else {
    return 2;
  }
}

// Agrega un numero en un espacio aleatorio en blanco dentro del tablero
void agregar_numero_al_tablero(int tablero[lado_tablero][lado_tablero]) {
  bool agregado = false;
  int x = 0, y = 0;

  do {
    x = rand() % lado_tablero;
    y = rand() % lado_tablero;

    if (tablero[x][y] == espacio) {
      tablero[x][y] = numero_aleatorio();
      agregado = true;
    }
  } while (agregado == false);
}

// Genera un nuevo tablero con espacios en blanco
void nuevo_tablero(int tablero[lado_tablero][lado_tablero]) {
  for (int x = 0; x < lado_tablero; x++) {
    for (int y = 0; y < lado_tablero; y++) {
      tablero[x][y] = espacio;
    }
  }
  agregar_numero_al_tablero(tablero);
  agregar_numero_al_tablero(tablero);
}

// Copia cada elemento del tablero en un nuevo tablero
void clonar_tablero(int tablero[lado_tablero][lado_tablero],
                    int clon[lado_tablero][lado_tablero]) {
  for (int x = 0; x < lado_tablero; x++) {
    for (int y = 0; y < lado_tablero; y++) {
      clon[x][y] = tablero[x][y];
    }
  }
}

// Comprueba si dos tableros son diferentes
bool son_tableros_diferentes(int tablero[lado_tablero][lado_tablero],
                             int clon[lado_tablero][lado_tablero]) {
  for (int x = 0; x < lado_tablero; x++) {
    for (int y = 0; y < lado_tablero; y++) {
      if (clon[x][y] != tablero[x][y]) {
        return true;
      }
    }
  }
  return false;
}

// Muestra el tablero en pantalla
void mostrar_tablero(int tablero[lado_tablero][lado_tablero]) {
  for (int x = 0; x < lado_tablero; x++) {
    for (int y = 0; y < lado_tablero; y++) {
      if (tablero[x][y] == espacio) {
        printf("||   ");
      } else {
        printf("||%2d ", tablero[x][y]);
      }
    }
    printf("||\n");
  }
  printf("\n");
}

// Comprueba si el tablero esta lleno validando que no haya espacios en blanco
// o que no haya fichas adyacentes con el mismo valor.
bool hay_movimientos_validos(int tablero[lado_tablero][lado_tablero]) {
  for (int x = 0; x < lado_tablero; x++) {
    for (int y = 0; y < lado_tablero; y++) {
      if (tablero[x][y] == espacio) {
        return true;
      }
      if (y > 0 && tablero[x][y] == tablero[x][y - 1]) {
        return true;
      }
      if (x > 0 && tablero[x][y] == tablero[x - 1][y]) {
        return true;
      }
    }
  }
  return false;
}

// Mover las fichas hacia la derecha
void mover_derecha(int tablero[lado_tablero][lado_tablero]) {
  // Para cada fila
  for (int x = 0; x < lado_tablero; x++) {
    int y, y_aux;

    // Movemos los ceros hacia la izquierda
    for (y = 3; y >= 0; y--) {
      for (y_aux = 0; y_aux < y; y_aux++) {
        if (tablero[x][y_aux] != 0 && tablero[x][y_aux + 1] == 0) {
          tablero[x][y_aux + 1] = tablero[x][y_aux];
          tablero[x][y_aux] = 0;
        }
      }
    }

    // Combinamos las fichas adyacentes
    for (y = 3; y > 0; y--) {
      if (tablero[x][y] == tablero[x][y - 1]) {
        // Combinar las fichas
        tablero[x][y] = tablero[x][y] * 2;

        // Desplazar el resto de la fila
        for (y_aux = y - 1; y_aux > 0; y_aux--) {
          tablero[x][y_aux] = tablero[x][y_aux - 1];
        }
        // Añadir un cero al principio de la fila
        tablero[x][0] = 0;
      }
    }
  }
}

// Mover las fichas hacia la izquierda
void mover_izquierda(int tablero[lado_tablero][lado_tablero]) {
  // Para cada fila
  for (int x = 0; x < lado_tablero; x++) {
    int y, y_aux;

    // Movemos los ceros hacia la derecha
    for (y = 0; y < lado_tablero; y++) {
      for (y_aux = 3; y_aux > y; y_aux--) {
        if (tablero[x][y_aux] != 0 && tablero[x][y_aux - 1] == 0) {
          tablero[x][y_aux - 1] = tablero[x][y_aux];
          tablero[x][y_aux] = 0;
        }
      }
    }

    // Combinamos las fichas adyacentes
    for (y = 0; y < 3; y++) {
      if (tablero[x][y] == tablero[x][y + 1]) {
        // Combinar las fichas
        tablero[x][y] = tablero[x][y] * 2;

        // Desplazar el resto de la fila
        for (y_aux = y + 1; y_aux < 3; y_aux++) {
          tablero[x][y_aux] = tablero[x][y_aux + 1];
        }
        // Añadir un cero al final de la fila
        tablero[x][3] = 0;
      }
    }
  }
}

// Mover las fichas hacia arriba
void mover_arriba(int tablero[lado_tablero][lado_tablero]) {
  // Para cada columna
  for (int y = 0; y < lado_tablero; y++) {
    int x, x_aux;

    // Movemos los ceros hacia abajo
    for (x = 0; x < lado_tablero; x++) {
      for (x_aux = 3; x_aux > x; x_aux--) {
        if (tablero[x_aux][y] != 0 && tablero[x_aux - 1][y] == 0) {
          tablero[x_aux - 1][y] = tablero[x_aux][y];
          tablero[x_aux][y] = 0;
        }
      }
    }

    // Combinamos las fichas adyacentes
    for (x = 0; x < 3; x++) {
      if (tablero[x][y] == tablero[x + 1][y]) {
        // Combinar las fichas
        tablero[x][y] = tablero[x][y] * 2;

        // Desplazar el resto de la columna
        for (x_aux = x + 1; x_aux < 3; x_aux++) {
          tablero[x_aux][y] = tablero[x_aux + 1][y];
        }
        // Añadir un cero al final de la columna
        tablero[3][y] = 0;
      }
    }
  }
}

// Mover las fichas hacia abajo
void mover_abajo(int tablero[lado_tablero][lado_tablero]) {
  // Para cada columna
  for (int y = 0; y < lado_tablero; y++) {
    int x, x_aux;

    // Movemos los ceros hacia arriba
    for (x = lado_tablero - 1; x >= 0; x--) {
      for (x_aux = 0; x_aux < x; x_aux++) {
        if (tablero[x_aux][y] != 0 && tablero[x_aux + 1][y] == 0) {
          tablero[x_aux + 1][y] = tablero[x_aux][y];
          tablero[x_aux][y] = 0;
        }
      }
    }

    // Combinamos las fichas adyacentes
    for (x = lado_tablero - 1; x > 0; x--) {
      if (tablero[x][y] == tablero[x - 1][y]) {
        // Combinar las fichas
        tablero[x][y] = tablero[x][y] * 2;

        // Desplazar el resto de la columna
        for (x_aux = x - 1; x_aux > 0; x_aux--) {
          tablero[x_aux][y] = tablero[x_aux - 1][y];
        }
        // Añadir un cero al principio de la columna
        tablero[0][y] = 0;
      }
    }
  }
}

// Calcula la puntuación del tablero buscando el valor más alto
int calcular_puntuacion(int tablero[][lado_tablero]) {
  int max_valor = 0;
  for (int x = 0; x < lado_tablero; x++) {
    for (int y = 0; y < lado_tablero; y++) {
      if (tablero[x][y] > max_valor) {
        max_valor = tablero[x][y];
      }
    }
  }
  return max_valor;
}

int main() {
  // Establecemos aleatoriedad
  srand(time(NULL));

  // Cargamos el tablero
  int tablero[lado_tablero][lado_tablero];
  int tablero_premovimiento[lado_tablero][lado_tablero];
  nuevo_tablero(tablero);

  // Comenzamos el juego
  printf("¡Bienvenido al juego 2048!\n  (R)egistrar (L)istado (J)ugar (D)atos"
"(E)liminar  (S)alir\n\n");
  printf("Utiliza las teclas para mover las fichas. Las fichas con el mismo "
         "número se suman en una cuando se tocan. ¡Súmalos para llegar a "
         "2048!\n\n");

  // Jugar
  int puntuacion_actual = 0;
  char letra;

  do {
    // Clono el tablero
    clonar_tablero(tablero, tablero_premovimiento);
    mostrar_tablero(tablero);
    printf(
        "Nuevo movimiento, Subir(W), Bajar(S), Izquierda(A), Derecha(D): \n");
    letra = getchar();

    switch (letra) {
    case 'D':
    case 'd':
      mover_derecha(tablero);
      break;
    case 'A':
    case 'a':
      mover_izquierda(tablero);
      break;
    case 'W':
    case 'w':
      mover_arriba(tablero);
      break;
    case 'S':
    case 's':
      mover_abajo(tablero);
      break;
    default:
      printf("Entrada inválida, vuelva a intentarlo:\n");
      break;
    }

    // Limpiar buffer
    while (letra != '\n')
      letra = getchar();

    if (!hay_movimientos_validos(tablero)) {
      break;
    }

    // Si el tablero cambió, agregamos un nuevo número
    if (son_tableros_diferentes(tablero, tablero_premovimiento)) {
      agregar_numero_al_tablero(tablero);
    }

    puntuacion_actual = calcular_puntuacion(tablero);
  } while (puntuacion_actual < maximo_puntaje);

  // Resulado del juego
  mostrar_tablero(tablero);
  if (puntuacion_actual >= maximo_puntaje) {
    printf("Ganaste, lograste un puntaje de %d \n", puntuacion_actual);
  } else {
    printf("Perdiste, lograste un puntaje de %d \n", puntuacion_actual);
  }

  return 0;
}
