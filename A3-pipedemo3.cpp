/**
 * ANEXO III. Archivo pipedemo3.cpp
 *
 * Este programa demuestra cómo ejecutar dos mandatos unidos por una tubería
 * El resultado del programa es la ejecución de la orden 'ls /etc | wc -w'
 * El programa crea primero una tubería 
 * Luego crea dos procesos hijos (pid1 y pid2 respectivamente) 
 * Los dos procesos hijos han heredado la tubería creada por el padre
 * El proceso pid1 redirecciona su salida estándar al extremo de escritura de la tubería
 * y ejecuta el primer mandato, 'ls /etc'.
 * El proceso pid2 redirecciona su entrada estándar al extremo de lectura de la tubería
 * y ejecuta el segundo mandato 'wc -w'
 * El programa principal (proceso padre) tendrá que cerrar los dos extremos de la tubería
 * y esperar la terminación de los dos procesos hijos hijos
 * 
 * @author  Eloy J. Mata
 * @author  Francisco J. García
 */

#include <iostream>   // Necesario para cout
#include <unistd.h>   // Proporciona acceso a las llamadas al sistema 
                      // a través de la API POSIX, como fork y pipe
#include <cstdlib>    // Necesario para la función exit
#include <sys/wait.h> // Neceasario para la función wait
#include <fcntl.h>    // Necesario para llamadas sistema archivos: creat, open

using namespace std;

int main() {
  // el PADRE crea una tubería para compartir con sus HIJOS
  int  tuberia[2];
  if ( pipe(tuberia) == -1 )  { 
    perror("ERROR: No se ha podido crear la tubería");
    exit(1);
  }  

  cout << "El resultado de ejecutar la orden 'ls /etc | wc -w' es:" << endl;

  // Se crea el primer proceso hijo
  pid_t pid1=fork();
  
  switch ( pid1 ) {
    case -1: // error al crear el proceso    
      perror("ERROR: no he podido crear un proceso con fork");
      exit(1);
      
    case 0: // PRIMER HIJO: ejecutará el primer mandato y escribirá en la tubería
      // Como va a escribir por la tubería...
      close(tuberia[0]);  // ... cierro extremo de lectura
      // redirecciona extremo de escritura de la tubería a stdout
      if ( dup2(tuberia[1], 1) == -1 ) { 
        perror("No se puede redireccionar stdout");
        exit(2);
      }
      // stdout ya redireccionado, cierra descriptor out tubería
      close(tuberia[1]);  
      
      execlp("ls","ls","/etc", NULL); // el PRIMER HIJO ejecuta el primer mandato 
      perror("Error al ejecutar el primer mandato");
      exit(3);
  }
  
  // Se crea el segundo proceso hijo
  pid_t pid2=fork();
  switch ( pid2 ) {
    case -1: // error al crear el proceso    
      perror("ERROR: no he podido crear un proceso con fork");
      exit(4);
      
    case 0: // SEGUNDO HIJO: ejecutará el segundo mandato y leerá de la tubería    
      // Como quiere leer de la tubería... 
      close(tuberia[1]);  // ... cierra extremo de escritura de la tubería
      // redirecciona stdin al extremo de lectura de la tubería
      if ( dup2(tuberia[0], 0) == -1 ) { 
        perror("No se puede redireccionar stdin");
        exit(5);
      }
      // stdin ya redireccionado, cierra descriptor in tubería
      close(tuberia[0]);  
      
      execlp("wc","wc","-w", NULL); // El SEGUNDO HIJO ejecuta el segundo mandato
      perror("Error al ejecutar el segundo mandato");
      exit(6);
  }
  
  // el proceso padre cierra extremos tubería ya que no los usa
  close(tuberia[0]);
  close(tuberia[1]);
  // y espera a que terminen los dos hijos
  wait(NULL);
  wait(NULL);
  
}
