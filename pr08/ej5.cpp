#include <cstring>
#include <iostream>

using namespace std;

/**
 * VERSIÓN notación de vectores de caracteres
 * La función recibe una cadena de caracteres str, que puede estar precedida o
 * terminada por uno o varios espacios en blanco o tabuladores ('\t'). Devuelve
 * otra cadena que no tiene esos espacios ni tabuladores iniciales ni finales.
 *
 * ATENCION !! La cadena original se modifica por el final (todos los espacios o
 * tabuladores finales se pierden). Si es necesario conservarla, hay que hacer
 * una copia antes de invocar a la función.
 *
 * @param str (in)  Cadena de caracteres a procesar (es modificada)
 * @return puntero a char apuntando al primer caracter de la cadena str que no
 * es un espacio o tabulador
 */
char *strtrim(char *str) {
  if (str == NULL)
	return NULL;

  // Avanza por la izquierda
  int i = 0;
  while (str[i] == ' ' || str[i] == '\t') {
	i++;
  }

  // Avanza por la derecha
  int f = strlen(str) - 1;
  while (f > i && (str[f] == ' ' || str[f] == '\t')) {
	f--;
  }

  str[f + 1] = '\0'; // Poner el carácter de fin de cadena

  // Devuelve la dirección del nuevo string
  return &str[i];
}

void read(char str[], int tam){
	if(cin.fail()){
		cin.clear();
		cin.ignore(256,'\n');
	}
	cin.getline(str,tam);
}

int main(){
	char cad[256];
	read(cad,256);
	char *cad2 = strtrim(cad);
	cout << cad2 << endl;

	return 0;
}