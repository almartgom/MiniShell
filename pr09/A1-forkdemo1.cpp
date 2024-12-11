// ANEXO I. forkdemo1.cpp

#include <iostream> // Necesario para cout
#include <cstdlib>  // Necesario para la funciÃ³n exit
#include <unistd.h> // Proporciona acceso a las llamadas al sistema
                    // a travÃ©s de la API POSIX, como fork

using namespace std;

int main() {
   pid_t valor;
   cout << "Antes de fork: mi PID es " << getpid() << endl;

   valor = fork(); // Se crea un nuevo proceso. Ahora hay 2 procesos en ejecuciÃ³n

   sleep(5);       // Duerme durante 5 segundos

   // Estos mensajes aparecerÃ¡n 2 veces
   cout << "DespuÃ©s de fork: mi PID es " << getpid();
   cout << ", fork() me ha devuelto " << valor;
   cout << ". Â¿quÃ© proceso soy, el padre o el hijo?\n";

   exit(0);
}
