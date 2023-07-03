// Integrantes:
// - Andrew Gigena, CI: 5111580-2, Email: mail@andrewgigena.dev
// - Horacio Duarte, CI: 5096812-7, Email: raulduarte45@hotmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAXIMOS_JUGADORES 10
#define LADO_TABLERO 4
#define ESPACIO 0
#define MAXIMO_PUNTAJE 2048

struct Jugador {
  bool activado;
  char alias[6];
  char nombre[21];
  char apellido[21];
  int edad;
  int puntuacion;
};

struct AdministradorJuego {
  int tablero[LADO_TABLERO][LADO_TABLERO];
  int tablero_premovimiento[LADO_TABLERO][LADO_TABLERO];
  int tablero_clonado[LADO_TABLERO][LADO_TABLERO];

  // Función para jugar
  int jugar() {
    // Cargamos el tablero
    nuevo_tablero();

    // Comenzamos el juego
    printf("Utiliza las teclas para mover las fichas. Las fichas con el mismo "
           "número se suman en una cuando se tocan. ¡Súmalos para llegar a "
           "2048!\n");

    // Jugar
    int puntuacion_actual = 0;
    char letra;
    bool continuar_jugando = true;
    do {
      clonar_tablero();
      mostrar_tablero();
      printf("Nuevo movimiento, Subir(W), Bajar(S), Izquierda(A), Derecha(D), "
             "Rendirse(R): \n");
      scanf("%c", &letra);

      switch (letra) {
      case 'D':
      case 'd':
        mover_derecha();
        break;
      case 'A':
      case 'a':
        mover_izquierda();
        break;
      case 'W':
      case 'w':
        mover_arriba();
        break;
      case 'S':
      case 's':
        mover_abajo();
        break;
      case 'R':
      case 'r':
        continuar_jugando = false;
        break;
      default:
        printf("Entrada inválida, vuelva a intentarlo:\n");
        break;
      }

      if (!hay_movimientos_validos()) {
        break;
      }

      // Si el tablero cambió, agregamos un nuevo número
      if (son_tableros_diferentes()) {
        agregar_numero_al_tablero();
      }

      puntuacion_actual = calcular_puntuacion();
    } while (puntuacion_actual < MAXIMO_PUNTAJE && continuar_jugando == true);

    // Resulado del juego
    mostrar_tablero();
    if (puntuacion_actual >= MAXIMO_PUNTAJE) {
      printf("Ganaste, lograste un puntaje de %d \n", puntuacion_actual);
    } else {
      printf("Perdiste, lograste un puntaje de %d \n", puntuacion_actual);
    }

    return puntuacion_actual;
  }

  // Genera un numero aleatoriamente de 2 o 4
  int numero_aleatorio() {
    if (rand() % 2 == 1) {
      return 4;
    } else {
      return 2;
    }
  }

  // Agrega un numero en un espacio aleatorio en blanco dentro del tablero
  void agregar_numero_al_tablero() {
    bool agregado = false;
    int x = 0, y = 0;

    do {
      x = rand() % LADO_TABLERO;
      y = rand() % LADO_TABLERO;

      if (tablero[x][y] == ESPACIO) {
        tablero[x][y] = numero_aleatorio();
        agregado = true;
      }
    } while (agregado == false);
  }

  // Genera un nuevo tablero con espacios en blanco
  void nuevo_tablero() {
    for (int x = 0; x < LADO_TABLERO; x++) {
      for (int y = 0; y < LADO_TABLERO; y++) {
        tablero[x][y] = ESPACIO;
      }
    }
    agregar_numero_al_tablero();
    agregar_numero_al_tablero();
  }

  // Copia cada elemento del tablero en un nuevo tablero
  void clonar_tablero() {
    for (int x = 0; x < LADO_TABLERO; x++) {
      for (int y = 0; y < LADO_TABLERO; y++) {
        tablero_clonado[x][y] = tablero[x][y];
      }
    }
  }

  // Comprueba si dos tableros son diferentes
  bool son_tableros_diferentes() {
    for (int x = 0; x < LADO_TABLERO; x++) {
      for (int y = 0; y < LADO_TABLERO; y++) {
        if (tablero_clonado[x][y] != tablero[x][y]) {
          return true;
        }
      }
    }
    return false;
  }

  // Muestra el tablero en pantalla
  void mostrar_tablero() {
    for (int x = 0; x < LADO_TABLERO; x++) {
      for (int y = 0; y < LADO_TABLERO; y++) {
        if (tablero[x][y] == ESPACIO) {
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
  bool hay_movimientos_validos() {
    for (int x = 0; x < LADO_TABLERO; x++) {
      for (int y = 0; y < LADO_TABLERO; y++) {
        if (tablero[x][y] == ESPACIO) {
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
  void mover_derecha() {
    // Para cada fila
    for (int x = 0; x < LADO_TABLERO; x++) {
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
  void mover_izquierda() {
    // Para cada fila
    for (int x = 0; x < LADO_TABLERO; x++) {
      int y, y_aux;

      // Movemos los ceros hacia la derecha
      for (y = 0; y < LADO_TABLERO; y++) {
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
  void mover_arriba() {
    // Para cada columna
    for (int y = 0; y < LADO_TABLERO; y++) {
      int x, x_aux;

      // Movemos los ceros hacia abajo
      for (x = 0; x < LADO_TABLERO; x++) {
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
  void mover_abajo() {
    // Para cada columna
    for (int y = 0; y < LADO_TABLERO; y++) {
      int x, x_aux;

      // Movemos los ceros hacia arriba
      for (x = LADO_TABLERO - 1; x >= 0; x--) {
        for (x_aux = 0; x_aux < x; x_aux++) {
          if (tablero[x_aux][y] != 0 && tablero[x_aux + 1][y] == 0) {
            tablero[x_aux + 1][y] = tablero[x_aux][y];
            tablero[x_aux][y] = 0;
          }
        }
      }

      // Combinamos las fichas adyacentes
      for (x = LADO_TABLERO - 1; x > 0; x--) {
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
  int calcular_puntuacion() {
    int max_valor = 0;
    for (int x = 0; x < LADO_TABLERO; x++) {
      for (int y = 0; y < LADO_TABLERO; y++) {
        if (tablero[x][y] > max_valor) {
          max_valor = tablero[x][y];
        }
      }
    }
    return max_valor;
  }
};

struct AdministradorJugadores {
  struct Jugador jugadores[MAXIMOS_JUGADORES];
  Jugador jugador_seleccionado;
  int num_jugadores = 0;

  // init
  void inicialiar() {
    for (int i = 0; i > MAXIMOS_JUGADORES; i++) {
      jugadores[i].activado = false;
    }
  }

  // Opcion (L)istar
  void listar_jugadores() {
    printf("  Alias  || Puntaje  \n");
    for (int i = 0; i < num_jugadores; i++) {
      if (jugadores[i].activado == true) {
        printf("   %s     ||   %d   \n", jugadores[i].alias,
               jugadores[i].puntuacion);
      }
    }
  }

  // Opcion (D)atos
  bool listar_jugador() {
    char alias[6];
    printf("Ingrese el alias del jugador a buscar:\n");
    scanf("%s", alias);

    // Buscar al jugador por el alias
    Jugador *jugador = NULL;
    for (int i = 0; i < num_jugadores; i++) {
      if (strcmp(jugadores[i].alias, alias) == 0 &&
          jugadores[i].activado == true) {
        jugador = &jugadores[i];
        break;
      }
    }

    // Verificar si el jugador existe
    if (jugador == NULL) {
      printf("El alias del jugador no existe en el sistema.\n");
      return false;
    }

    // Mostrar todos los datos del jugador
    printf("Nombre: %s\n", jugador->nombre);
    printf("Apellido: %s\n", jugador->apellido);
    printf("Edad: %d\n", jugador->edad);
    printf("Puntaje: %d\n", jugador->puntuacion);

    return true;
  }

  // Opcion (J)ugar
  bool seleccionar_jugador() {
    char alias[6];
    printf("Ingrese el alias del jugador que participará: ");
    scanf("%s", alias);

    // Buscar al jugador por el alias
    struct Jugador *jugador = NULL;
    for (int i = 0; i < num_jugadores; i++) {
      if (strcmp(jugadores[i].alias, alias) == 0) {
        jugador = &jugadores[i];
        break;
      }
    }

    // Verificar si el jugador existe
    if (jugador == NULL) {
      printf("El alias del jugador no existe en el sistema.\n");
      return false;
    }

    // Mostrar mensaje personalizado al jugador
    printf("¡Bienvenido, %s %s! ¡Buena suerte en el juego 2048!\n",
           jugador->nombre, jugador->apellido);

    jugador_seleccionado = *jugador;
    return true;
  }

  // Opcion (J)ugar
  void sumar_puntuacion_jugador_seleccionado(int puntuacion) {
    for (int i = 0; i < num_jugadores; i++) {
      if (strcmp(jugadores[i].alias, jugador_seleccionado.alias) == 0) {
        jugadores[i].puntuacion += puntuacion;
        break;
      }
    }
  }

  // Opcion (R)egistrar
  bool registrar_jugador() {
    Jugador nuevo_jugador;
    if (num_jugadores == MAXIMOS_JUGADORES) {
      printf("No hay lugar disponible para registrar más jugadores.\n");
      return false;
    }

    printf("Ingrese los datos del nuevo jugador\n");
    printf("Alias: ");
    scanf("%s", nuevo_jugador.alias);
    for (int i = 0; i < MAXIMOS_JUGADORES; i++) {
      if (jugadores[i].activado == true &&
          strcmp(nuevo_jugador.alias, jugadores[i].alias) == 0) {
        printf("Ya existe este alias\n");
        nuevo_jugador.activado = false;
        break;
      } else {
        printf("Nombre: ");
        scanf("%s", nuevo_jugador.nombre);
        printf("Apellido: ");
        scanf("%s", nuevo_jugador.apellido);
        printf("Edad: ");
        scanf("%d", &nuevo_jugador.edad);
        nuevo_jugador.puntuacion = 0;
        nuevo_jugador.activado = true;
        jugadores[num_jugadores] = nuevo_jugador;
        num_jugadores++;
        printf("Jugador registrado correctamente\n");
        break;
      }
    }

    return true;
  }

  // Opcion (E)liminar
  void eliminar_jugador() {
    bool borrado = false;
    char alias[6];

    printf("Ingrese el alias del jugador a eliminar\n");
    scanf("%s", alias);

    for (int i = 0; i < MAXIMOS_JUGADORES; i++) {
      if (jugadores[i].activado == true &&
          strcmp(jugadores[i].alias, alias) == 0) {
        jugadores[i].activado = false;
        borrado = true;
        break;
      }
    }

    if (borrado == true) {
      printf("Borrado exitoso\n");
    } else {
      printf("Jugador no ha sido encontrado.\n");
    }
  }
};

void print_separador() {
  printf("========================================================\n");
}

int main() {
  // Establecemos aleatoriedad
  srand(time(NULL));

  AdministradorJuego admin_juego;
  AdministradorJugadores admin_jugadores;
  admin_jugadores.inicialiar();

  char letramenu;
  bool salir = true;
  int puntuacion = 0;
  bool seleccion_correcta;
  printf("¡Bienvenido al juego 2048!\n");
  do {
    print_separador();
    printf("(R)egistrar (L)istado (J)ugar (D)atos (E)liminar (S)alir\n");
    print_separador();
    letramenu = getchar();
    switch (letramenu) {
    case 'R':
    case 'r':
      printf("Se ha seleccionado: Registrar\n");
      print_separador();
      if (admin_jugadores.num_jugadores < 10) {
        admin_jugadores.registrar_jugador();
      } else {
        printf(
            "Maximo de 10 jugadores permitido, borre para registrar jugador\n");
      }
      break;
    case 'L':
    case 'l':
      printf("Se ha seleccionado: Listado\n");
      print_separador();
      admin_jugadores.listar_jugadores();
      break;
    case 'J':
    case 'j':
      printf("Se ha seleccionado: Jugar\n");
      print_separador();
      seleccion_correcta = admin_jugadores.seleccionar_jugador();
      if (seleccion_correcta == true) {
        puntuacion = admin_juego.jugar();
        admin_jugadores.sumar_puntuacion_jugador_seleccionado(puntuacion);
      }
      break;
    case 'D':
    case 'd':
      printf("Se ha seleccionado: Datos\n");
      print_separador();
      admin_jugadores.listar_jugador();
      break;
    case 'E':
    case 'e':
      printf("Se ha seleccionado: Eliminar\n");
      print_separador();
      admin_jugadores.eliminar_jugador();
      break;
    case 'S':
    case 's':
      print_separador();
      salir = false;
      printf("Nos vemos pronto!:\n");
      break;
    default:
      print_separador();
      printf("Entrada inválida, vuelva a intentarlo:\n");
      break;
    }
    // Limpiar buffer
    while (letramenu != '\n')
      letramenu = getchar();
  } while (salir == true);

  return 0;
}
