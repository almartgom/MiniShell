/**
 * ANEXO I. Archivo pipedemo1.cpp
 * 
 * Comunicación entre dos procesos, padre e hijo, mediante una tubería
 * El proceso padre crea una tubería y luego crea un proceso hijo, que
 * hereda la tubería.
 * El proceso hijo envía un mensaje al proceso padre a través de la tubería.
 * El proceso padre muestra el mensaje recibido en la salida estándar.
 * 
 * @author  Eloy J. Mata
 * @author  Francisco J. García
 */

#include <iostream> // Necesario para cout
#include <cstdlib>  // Necesario para la función exit
#include <cstring>  // Necesario para la función strcpy
#include <unistd.h> // Proporciona acceso a las llamadas al sistema
                    // a través de la API POSIX, como fork y pipe

using namespace std;

const int MAXBUFFER = 4096; // tamaño mínimo garantizado para una tubería

int main() {

  int tuberia[2];

  // crea un tubería
  if (pipe(tuberia) == -1)   { 
    perror("Aviso: Error al crear la tubería");
    exit(1);
  }
  
  // crea un nuevo proceso. El proceso hijo hereda la tubería
  pid_t pid = fork(); 

  switch (pid) {
    case -1: // comprueba si hay error
      perror("Aviso: Error al ejecutar fork");
      exit(2);

    case 0: { // proceso hijo
      // Se crea un mensaje
      char mensaje[] = "Hola papi, estoy bien";
      // el hijo muestra el mensaje en la salida estándar
      cout << "Proceso HIJO. este será el,mensaje que envíe mi padre: " << mensaje << endl;
      
      // El hijo usa solo el descriptor de escritura de la tubería
      // por lo que cierra el descriptor de lectura 
      close(tuberia[0]); 
      
      // El hijo envía el mensaje al padre a través de la tubería
      int len = strlen(mensaje);
      // se escribe el mensaje en la tubería
      if (write(tuberia[1], mensaje, len) != len) { 
        perror("Aviso: Error al escribir en la tubería");
        exit(3);
      }
      
      // Se cierra el descriptor de escritura en el hijo
      close(tuberia[1]); 
      exit(0);
    }

    default: // proceso padre    
      // El padre usa solo el descriptor de lectura de la tubería
      // por lo que cierra el descriptor de escritura 
      close(tuberia[1]);  
      
      // se recoge el mensaje de la tubería
      char buffer[MAXBUFFER];
      int len = read(tuberia[0], buffer, MAXBUFFER); 
      if (len == -1) {
        perror("Aviso: Error al leer de la tubería");
        exit(4);
      } else {
        // Se cierra el descriptor de lectura en el padre
        close(tuberia[0]); 
        
        // el padre muestra el mensaje recibido en la salida estándar
        cout << "Mensaje recibido por el PADRE: " << buffer << endl;
      }
      exit(0);
  }
}
