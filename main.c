#include "hashmap.h"   // lollll
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BARRA "-------------------------------------------------------"

struct HashTable {
  int size;
  struct Bucket **buckets;
};

struct Bucket {
  char *key;
  void *value;
  struct Bucket *next;
};

struct HashMap {
  Pair **buckets;
  long size;     // cantidad de datos/pairs en la tabla
  long capacity; // capacidad de la tabla
  long current;  // indice del ultimo dato accedido
};

typedef struct {
  char item[31];
} inventario;

typedef struct {
  char nombre[31];
} nombres;
                            ///////////// ruben: struct inventario y una lista llamada igual?
typedef struct Jugador{
  char nombre[31];
  unsigned short puntos;
  int items;
  List *inventario;
  struct Jugador *historial;
} Jugador;


void mostrarMenu() {
  puts(BARRA);
  printf("                   SEBA´S ADVENTURE 64\n");
  puts(BARRA);
  printf("\nSeleccione una opción:\n\n1. Crear perfil de jugador\n2. Mostrar "
         "perfil de jugador\n3. Agregar ítem a jugador\n4. Eliminar ítem a "
         "jugador\n5. Agregar puntos de habilidad al jugador\n6. Mostrar "
         "jugadores con un ítem específico\n7. Deshacer última acción del "
         "jugador\n8. Exportar datos de jugadores a archivo de texto\n9. "
         "Cargar datos de jugadores desde un archivo de texto\n0. Salir\n\n");
  puts(BARRA);
}

void listaNombres(List *nombres, char *nombre) {
  inventario *i = (inventario *)malloc(sizeof(inventario));
  strcpy(i->item, nombre);
  pushBack(nombres, i);
}

void crearPerfil(Jugador *player, HashMap *map, List *nombres, char *nombre) {
  strcpy(player->nombre, nombre);
  player->puntos = 0;
  // player->inventario = createList();
  insertMap(map, player->nombre, player);
  listaNombres(nombres, nombre);
}

void mostrarItems(Pair *casilla) {
  List *node = ((Jugador *)casilla->value)->inventario;
  unsigned short cont = 1;

  if (firstList(node) == NULL) {
    printf("No tiene\n");
  }
  for (char *a = firstList(node); a != NULL; a = nextList(node)) {
    printf("Item %hu: %s\n", cont, a);
    cont++;
  }
}

void mostrarPerfil(char *nombre, HashMap *map) {
  printf("\n");
  puts(BARRA);
  Pair *casilla = searchMap(map, nombre);
  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else {
    printf("Nombre: %s\nPuntos: %hu\nItems: %i\n", ((Jugador *)casilla->value)->nombre, ((Jugador *)casilla->value)->puntos, ((Jugador *)casilla->value)->items);
    
    if (((Jugador *)casilla->value)->items == 0) {
      printf("El inventario esta vacío\n");
    } else {
      mostrarItems(casilla);
    }
  }
}

void agregarItem(HashMap *map, char *nombre) {
  char aux[31];
  Pair *casilla = searchMap(map, nombre);
  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else {
    // para crear la copia y ponerla en historial
    Jugador *aCopiar = ((Jugador *)casilla->value); 
    Jugador *historial = (Jugador*)malloc(sizeof(Jugador)); // reserva memoria para historial
    memcpy(historial, aCopiar, sizeof(Jugador)); // copia el objeto Jugador
    ((Jugador *)casilla->value)->historial = historial;
  }
    //
    printf("Ingrese el nombre del item\n");
    scanf(" %[^\n]s", aux);
    //aqui modifique el puntero i en vez de ponerlo dentro de las condiciones lo puse antes para ambos casos
    inventario *i = (inventario *)malloc(sizeof(inventario));
    if (firstList(((Jugador *)casilla->value)->inventario) == NULL) {
      ((Jugador *)casilla->value)->inventario =
          createList(); 
      strcpy(i->item, aux);
      pushBack(((Jugador *)casilla->value)->inventario, i);
      ((Jugador *)casilla->value)->items++;
    } else {
      strcpy(i->item, aux);
      pushBack(((Jugador *)casilla->value)->inventario, i);
      ((Jugador *)casilla->value)->items++;
    }
}
void eliminarItem(HashMap *map, char * nombre) {
  
  Pair *casilla = searchMap(map, nombre);
  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else {
    // para crear la copia y ponerla en historial
    Jugador *aCopiar = ((Jugador *)casilla->value); 
    Jugador *historial = (Jugador*)malloc(sizeof(Jugador)); // reserva memoria para historial
    memcpy(historial, aCopiar, sizeof(Jugador)); // copia el objeto Jugador
    ((Jugador *)casilla->value)->historial = historial;
    //
    char nombre2[31];
    printf("Ingrese el nombre del item a eliminar\n");
    scanf(" %[^\n]s", nombre2);
    for (char *a = firstList(((Jugador *)casilla->value)->inventario);
         a != NULL; a = nextList(((Jugador *)casilla->value)->inventario)) {
      if (strcmp(nombre2, a) == 0) {
        popCurrent(((Jugador *)casilla->value)->inventario);
        ((Jugador *)casilla->value)->items--;
      }
    }
  }
}

void agregarPuntos(HashMap *map, char * nombre) {
  Pair *casilla = searchMap(map, nombre);

  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else {
    // para crear la copia y ponerla en historial
    Jugador *aCopiar = ((Jugador *)casilla->value); 
    Jugador *historial = (Jugador*)malloc(sizeof(Jugador)); // reserva memoria para historial
    memcpy(historial, aCopiar, sizeof(Jugador)); // copia el objeto Jugador
    ((Jugador *)casilla->value)->historial = historial;
    //
    unsigned short puntos;
    printf("Ingrese la cantidad de puntos\n");
    scanf("%hu", &puntos);
    ((Jugador *)casilla->value)->puntos += puntos;
  }
}

void mostrarJugadoresItemEsp(HashMap *map, List *nombres, char *nombre) {
  int cont = 0;

  printf("Jugadores con el item %s: \n", nombre);
  for (char *a = firstList(nombres); a != NULL; a = nextList(nombres)) {
    long key = hash(a, map->capacity);
    for (char *b = firstList(((Jugador *)map->buckets[key]->value)->inventario);
         b != NULL;
         b = nextList(((Jugador *)map->buckets[key]->value)->inventario)) {
      if (strcmp(b, nombre) == 0) {
        // printf("Jugadores con el item %s: \n", nombre);
        printf("[ %s ]\n", a);
        cont++;
        break;
      }
    }
  }
  if (cont == 0)
    printf("No se encontraron jugadores con ese item\n");
}

void deshacer(HashMap *map, char * nombre){
  Pair *casilla = searchMap(map, nombre);
  if (casilla == NULL) {
    printf("El jugador ingresado no existe\n");
    return;
  } else if(((Jugador *)casilla->value)->historial!=NULL){
    // para crear la copia y ponerla en historial
    Jugador *version_anterior =(Jugador*)malloc(sizeof(Jugador)); 
    Jugador *historial = ((Jugador *)casilla->value)->historial; // reserva memoria para historial
    memcpy(version_anterior, historial, sizeof(Jugador)); // copia el objeto Jugador
    casilla->value = version_anterior;
  }
  else{
    printf("no hay nada que deshacer\n");
  }
}


/*
void importar(List *l,FILE *archivo){
  char linea[1300];
  char *aux;
  int i, edadInt;
fgets(linea, 1024, archivo); //Se lee la primera linea por separado ya que no se pretende guardar su valor.
  while(fgets(linea, 1024, archivo) != NULL){ //Se leen todas las lineas en orden
    paciente* p = (paciente*) malloc(sizeof(paciente));
    p->medicos = createList();
    for(i = 0 ; i < 7 ; i++){//Se realizan 7 ciclos para permitir que se realizen las suficientes operaciones(6 valores en el struct)
        aux = get_csv_field(linea, i); //aux se convierte en la linea de caracteres i-esima para rellenar el valor correspondiente.
        if(i == 0){
        strcpy(p->nombre, aux);
        }
        if(i == 1){
        strcpy(p->apellido, aux);
        }
        if(i == 2){ 
        edadInt= atoi(aux);
        p->edad=edadInt;
        }
        if(i == 3){
        strcpy(p->telefono, aux);
        }
        if(i== 4){
        strcpy(p->direccion, aux);
        }
        if(i == 5){
        strcpy(p->numeroSocial, aux);
        Registrar(p, l);
        }  
        if(i == 6){
        asignarMedico(l, p->nombre, p->apellido, aux );
        }
      }
    }
  fclose(archivo);
}

*/
/*
void exportar(List* l,FILE *archivo){
  
  fprintf(archivo, "Nombre,Apellido,Edad,Teléfono,Dirección,Num Seguridad Social,Médicos\n");
  for(paciente *p = firstList(l) ; p != NULL; p = nextList(l)){
    //aqui pone todos los datos del struct menos medicos
    fprintf(archivo, "%s,%s,%d,%s,%s,%s\n", p->nombre,p->apellido,p->edad,p->telefono, p->direccion, p->numeroSocial);
    //aqui pone la lista de medicos
    for(char* a = firstList(p->medicos) ; a != NULL ; a = nextList(p->medicos)){
      fprintf(archivo, "%s", a);
      if(nextList(p->medicos) != NULL){
        fprintf(archivo,"; ");
      }
      else{
        fprintf(archivo, "\n");
      }
    }
  }
  fclose(archivo);
}
*/

int main() {
  HashMap *map = createMap(10000); ///////////////////////////////////////////
  char nombre[31];
  List *nombres = createList(); // ruben: pq esto?
  unsigned short numIngresado;
  int largoName;

  while (true) {
    mostrarMenu();

    scanf("%hu", &numIngresado);
    while (numIngresado > 9 || numIngresado < 0) {
      printf("Ingrese un número válido \n");
      scanf("%hu", &numIngresado);
    }
    if (numIngresado == 0) {
      printf("\n");
      puts(BARRA);
      printf("Fin De La Aventura!!!!!\n");
      puts(BARRA);
      return 0;
    }
    if (numIngresado == 1) {
      Jugador *player = (Jugador *)malloc(sizeof(Jugador));
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]", nombre);
      largoName = strlen(nombre);
      while (largoName > 31 || largoName < 1) {
      printf("Ingrese un nombre válido (hasta 30 caracteres)\n");
      scanf(" %[^\n]", nombre);
      largoName = strlen(nombre);
      }
      crearPerfil(player, map, nombres, nombre);
    }

    if (numIngresado == 2) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      mostrarPerfil(nombre, map);
    }
    if (numIngresado == 3) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      agregarItem(map,nombre);
    }
    if (numIngresado == 4) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      eliminarItem(map,nombre);
    }
    if (numIngresado == 5) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[^\n]s", nombre);
      agregarPuntos(map,nombre);
    }
    if (numIngresado == 6) {
      printf("Ingrese el nombre del item\n");
      scanf(" %[^\n]s", nombre);
      mostrarJugadoresItemEsp(map, nombres, nombre);
    }
    if (numIngresado == 7) {
      printf("Ingrese el nombre del jugador\n");
      scanf(" %[\n]s", nombre);
      deshacer(map,nombre);
    }
    if (numIngresado == 8) {
      /*printf("Escriba el nombre del archivo\n");
      scanf(" %[^\n]", nombreArchivo);
      setbuf(stdout, NULL);

      FILE *archivo = fopen(nombreArchivo,"rt");
      if (archivo == NULL) {
        printf("* Error al abrir el archivo.\n");
      } else{
         importar();
      }*/
    }
    if (numIngresado == 9) {
      /*if(cantPacientes != 0){
        printf("Escriba el nombre del archivo\n");
        scanf(" %[^\n]", nombreArchivo2);
        FILE* archivo = fopen(nombreArchivo2, "a");
        exportar());*/
    } else {
      // printf("* No hay lista exportable\n");
    }
  }
}