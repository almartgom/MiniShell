// ANEXO IV. waitdemo.cpp

#include <iostream>   // Necesario para cout
#include <cstdlib>    // Necesario para la funciÃ³n exit
#include <unistd.h>   // Proporciona acceso a las llamadas al sistema 
                      // a travÃ©s de la API POSIX, como fork
#include <sys/wait.h> // Neceasario para la funciÃ³n wait

int const RETARDO = 5;

using namespace std;

int main() {
    cout << "[Padre] pid " << getpid() << ". Antes de llamar a fork()" << endl;
    pid_t valor = fork(); // crea un nuevo proceso
    switch(valor) {
    case -1: // comprueba si hay error
        perror("Error al ejecutar fork");
        break;
    case 0:
        cout << "  [Hijo] pid " << getpid() << ". Voy a dormir durante " << RETARDO << " segundos" << endl;
        sleep(RETARDO);
        cout << "  [Hijo] He terminado" << endl;
        break;
    default:
        cout << "[Padre] pid " << getpid() << ". Estoy esperando a que termine mi proceso hijo, de pid " << valor << endl;
        wait(NULL);
        cout << "[Padre] He terminado" << endl;
    }
    exit(0);
}

