// ANEXO V. execdemo1.cpp

#include <iostream> // Necesario para cout
#include <cstdlib>  // Necesario para la funciÃ³n exit
#include <unistd.h> // Proporciona acceso a las llamadas al sistema 
					// a travÃ©s de la API POSIX, como fork

using namespace std;

int main() {
	// Ejemplo de ejecuciÃ³n de un programa especificado 
	// en los argumentos de la funciÃ³n execlp
	// En este caso ejecuta ls -l /home
	
	if (execlp("cd","cd","dirprueba",NULL) == -1) {
		perror("Error al ejecutar la funciÃ³n execlp");
		exit(-1);
	}
	cout << "El programa ha terminado"; //Este mensaje no lo mostrarÃ¡ nunca
	exit(0);

/*
    // Comenta todo el cÃ³digo de las lineas 15 a 20 y descomenta este
	char pid[16];
  sprintf(pid, "%d", getpid());
	// ps -o command muestra los argumentos pasados al progrma en ejecuciÃ³n (array argv en la funciÃ³n main)
	if (execlp("ps", "pepito", "-p", pid, "-o", "command", NULL) == -1) {
		perror("Error al ejecutar la funciÃ³n execlp");
		exit(-1);
	}
*/
}


