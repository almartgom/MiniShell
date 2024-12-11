// ANEXO II. forkdemo2.cpp

#include <iostream> // Necesario para cout
#include <cstdlib>  // Necesario para la funciÃ³n exit
#include <unistd.h> // Proporciona acceso a las llamadas al sistema
                    // a travÃ©s de la API POSIX, como fork

using namespace std;

int main() {
   cout << "Soy el proceso padre: mi PID es " << getpid() << endl;

   fork();
   fork();
   fork();

   sleep(20); // EL proceso se suspende durante 20 segundos

   cout << endl << "He terminado: Soy el proceso con PID: " << getpid() 
        << " y el PID de mi padre es " << getppid() << endl;

   exit(0);
}
