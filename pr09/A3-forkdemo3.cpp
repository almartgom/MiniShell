// ANEXO III. forkdemo3.cpp

#include <iostream> // Necesario para cout
#include <cstdlib>  // Necesario para la funciÃ³n exit
#include <unistd.h> // Proporciona acceso a las llamadas al sistema
                    // a travÃ©s de la API POSIX, como fork

int const RETARDO1 = 0;
int const RETARDO2 = 3;

using namespace std;
int main() {
   cout << "Antes de llamar a fork(): Mi PID es " << getpid() << endl;

   pid_t valor = fork(); // crea un nuevo proceso

   switch (valor) {
   case -1: // comprueba si hay error
      perror("Error al ejecutar fork");
      break;
   case 0:
      cout << "Yo soy el proceso hijo. Mi PID es " << getpid();
      cout << " y el de mi padre es " << getppid() << endl << endl;
      sleep(RETARDO1);
      cout << "Fin del proceso hijo\n";
      break;
   default:
      cout << "Yo soy el proceso padre. Mi PID es " << getpid();
      cout << " y el de mi hijo es " << valor << endl << endl;
      sleep(RETARDO2);
      cout << "Fin del proceso padre\n";
   }
   exit(0);
}
