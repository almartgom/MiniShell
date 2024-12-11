#include <cstring>
#include <iostream>

using namespace std;

/**
 * Divide una cadena de caracteres en un array de subcadenas.
 * El último elemento del array de subcadenas tendrá el valor NULL (esto es
 * necesario para usar después con execvp). Para determinar las subcadenas se
 * utiliza cualquiera de los caracteres contenidos en otra cadena recibida como
 * segundo parámetro.
 *
 * La función reserva memoria dinámicamente para almacenar el array de
 * subcadenas. La memoria reservada debería liberarse cuando no se vaya a
 * necesitar más.
 *
 * ATENCION !! La cadena original se destruye. Si es necesario conservarla, hay
 * que hacer una copia antes de invocar a la función.
 *
 * @param str   (in)  Cadena de caracteres a dividir (se destruye)
 * @param delim (in)  Cadena que especifica los carácteres a usar para la
 * división de la cadena
 * @param parts (out) Array de cadenas con las partes (subcdenas) en la que se
 * divide la cadena
 * @return Número de trozos en los que se descompone la cadena original
 *
 * Ejemplo: El siguiente código trocea la cadena "Hola, que tal estás" en cuatro
 * trozos, ya que usamos el espacio en blanco o la coma como delimitadores ("
 * ,"). Los trozos son "Hola", "que", "tal" y "estás". Notar que tras la
 * ejecución de la función la cadena inicial ya no será nunca más "Una línea de
 * texto\t cualquiera", sino que ha queda reducida a "Una".
 *
 *   char cad [] = "Hola, que tal estas";
 *   char ** trozos;
 *   int n = trocear_linea(cad, " ,", trozos);
 *   cout << "Cadena dividida en " << n << " trozos" << endl;
 *   // Mostrar resultado con notación vectores
 *   int i=0;
 *   while (trozos[i] != NULL) { // El último trozo del array es siempre NULL
 *     cout << trozos[i] << endl;
 *     i++;
 *   }
 *   ...
 *   // Cuando la variable trozos no se necesite
 *   free(trozos);
 */
int trocear_linea(char *str, const char *delim, char **&parts) {
  parts = (char **)malloc(sizeof(char *));
  parts[0] = NULL;

  if (str == NULL)
    return 0; // caso que no recibamos str

  int numParts = 0;

  int i = 0;
  while (str[i] != '\0') {
    int j = i;
    // recorrer str hasta encontrar un delimitador o el final
    while (str[j] != '\0' && strchr(delim, str[j]) == NULL) {
      j++;
    }

    if (str[j] != '\0') { // Si no se ha llegado al final
      str[j] =
          '\0'; // terminar la subcadena, sustituir el delimitador por un '\0'
      j++;      // Avanzar al siguiente caracter de la cadena str
    }

    if (str[i] != '\0') { // Si la subcadena tiene caracteres
      // actualizar trozos
      numParts++;
      parts = (char **)realloc(parts, (numParts + 1) * sizeof(char *));
      parts[numParts - 1] = &str[i];
      parts[numParts] = NULL; // necesario para usar después con execvp
    }

    i = j; // Para continuar con el siguiente trozo
  }

  return numParts;
}

void read(char str[], int tam){
    if(cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
    }
    cin.getline(str,tam);
}

void mostrar_trozos(char ** trozos){
  for(int i=0; trozos[i]!=NULL; i++){
    cout << trozos[i] << endl;
  }
}

int main() {
    char cad[256];
    read(cad,256);
    char **trozos;
    int n = trocear_linea(cad, " \t", trozos);
    cout << "Cadena dividida en " << n << " trozos: " << endl;
    mostrar_trozos(trozos);

    return 0;
}