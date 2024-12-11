/**
 * ANEXO I. Archivo pipedemo1.cpp
 * 
 * Comunicaci�n entre dos procesos, padre e hijo, mediante una tuber�a
 * El proceso padre crea una tuber�a y luego crea un proceso hijo, que
 * hereda la tuber�a.
 * El proceso hijo env�a un mensaje al proceso padre a trav�s de la tuber�a.
 * El proceso padre muestra el mensaje recibido en la salida est�ndar.
 * 
 * @author  Eloy J. Mata
 * @author  Francisco J. Garc�a
 */

#include <iostream> // Necesario para cout
#include <cstdlib>  // Necesario para la funci�n exit
#include <cstring>  // Necesario para la funci�n strcpy
#include <unistd.h> // Proporciona acceso a las llamadas al sistema
                    // a trav�s de la API POSIX, como fork y pipe

using namespace std;

const int MAXBUFFER = 4096; // tama�o m�nimo garantizado para una tuber�a

int main() {

  int tuberia[2];

  // crea un tuber�a
  if (pipe(tuberia) == -1)   { 
    perror("Aviso: Error al crear la tuber�a");
    exit(1);
  }
  
  // crea un nuevo proceso. El proceso hijo hereda la tuber�a
  pid_t pid = fork(); 

  switch (pid) {
    case -1: // comprueba si hay error
      perror("Aviso: Error al ejecutar fork");
      exit(2);

    case 0: { // proceso hijo
      // Se crea un mensaje
      char mensaje[] = "Hola papi, estoy bien";
      // el hijo muestra el mensaje en la salida est�ndar
      cout << "Proceso HIJO. este ser� el,mensaje que env�e mi padre: " << mensaje << endl;
      
      // El hijo usa solo el descriptor de escritura de la tuber�a
      // por lo que cierra el descriptor de lectura 
      close(tuberia[0]); 
      
      // El hijo env�a el mensaje al padre a trav�s de la tuber�a
      int len = strlen(mensaje);
      // se escribe el mensaje en la tuber�a
      if (write(tuberia[1], mensaje, len) != len) { 
        perror("Aviso: Error al escribir en la tuber�a");
        exit(3);
      }
      
      // Se cierra el descriptor de escritura en el hijo
      close(tuberia[1]); 
      exit(0);
    }

    default: // proceso padre    
      // El padre usa solo el descriptor de lectura de la tuber�a
      // por lo que cierra el descriptor de escritura 
      close(tuberia[1]);  
      
      // se recoge el mensaje de la tuber�a
      char buffer[MAXBUFFER];
      int len = read(tuberia[0], buffer, MAXBUFFER); 
      if (len == -1) {
        perror("Aviso: Error al leer de la tuber�a");
        exit(4);
      } else {
        // Se cierra el descriptor de lectura en el padre
        close(tuberia[0]); 
        
        // el padre muestra el mensaje recibido en la salida est�ndar
        cout << "Mensaje recibido por el PADRE: " << buffer << endl;
      }
      exit(0);
  }
}
