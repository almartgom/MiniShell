// ANEXO VI. execdemo2.cpp
// Ejemplo de ejecuciÃ³n de un programa especificado 
// en los argumentos de la funciÃ³n execvp
// En este caso ejecuta: "ls -l /home"

#include <iostream>	// Necesario para cout
#include <cstdlib>	// Necesario para la funciÃ³n exit
#include <unistd.h>	// Proporciona acceso a las llamadas al sistema 
					// a travÃ©s de la API POSIX, como fork
#include <cstring>	// Necesario para la funciÃ³n strcpy

using namespace std;

int main() {	
	char ** trozos_cmd;          // Puntero doble a char, es decir, array de cadenas
  
  // Inicializamos las cadenas apuntadas por los punteros de trozos_cmd
  // Se reserva memoria para 4 punteros a char;
	trozos_cmd = new char * [4]; 
  
	// La funciÃ³n strdup reserva memoria para el string que recibe como argumento
	// y devuelve un puntero a la zona de memoria reservada
	trozos_cmd[0] = strdup("ls");
	trozos_cmd[1] = strdup("-l");
	trozos_cmd[2] = strdup("/home");
	trozos_cmd[3] = NULL; // El Ãºltimo argumento tiene que ser NULL (lo obliga execvp)
	
	if (execvp(trozos_cmd[0], trozos_cmd) == -1) {
 		perror("Error al ejecutar la funciÃ³n execvp\n");
		exit(-1);
	}
	cout << "El programa ha terminado"; // Este mensaje no lo mostrarÃ¡ nunca
	exit(0);
}

