/**
 * ANEXO III. Archivo pipedemo3.cpp
 *
 * Este programa demuestra c�mo ejecutar dos mandatos unidos por una tuber�a
 * El resultado del programa es la ejecuci�n de la orden 'ls /etc | wc -w'
 * El programa crea primero una tuber�a 
 * Luego crea dos procesos hijos (pid1 y pid2 respectivamente) 
 * Los dos procesos hijos han heredado la tuber�a creada por el padre
 * El proceso pid1 redirecciona su salida est�ndar al extremo de escritura de la tuber�a
 * y ejecuta el primer mandato, 'ls /etc'.
 * El proceso pid2 redirecciona su entrada est�ndar al extremo de lectura de la tuber�a
 * y ejecuta el segundo mandato 'wc -w'
 * El programa principal (proceso padre) tendr� que cerrar los dos extremos de la tuber�a
 * y esperar la terminaci�n de los dos procesos hijos hijos
 * 
 * @author  Eloy J. Mata
 * @author  Francisco J. Garc�a
 */

#include <iostream>   // Necesario para cout
#include <unistd.h>   // Proporciona acceso a las llamadas al sistema 
                      // a trav�s de la API POSIX, como fork y pipe
#include <cstdlib>    // Necesario para la funci�n exit
#include <sys/wait.h> // Neceasario para la funci�n wait
#include <fcntl.h>    // Necesario para llamadas sistema archivos: creat, open

using namespace std;

int main() {
  // el PADRE crea una tuber�a para compartir con sus HIJOS
  int  tuberia[2];
  if ( pipe(tuberia) == -1 )  { 
    perror("ERROR: No se ha podido crear la tuber�a");
    exit(1);
  }  

  cout << "El resultado de ejecutar la orden 'ls /etc | wc -w' es:" << endl;

  // Se crea el primer proceso hijo
  pid_t pid1=fork();
  
  switch ( pid1 ) {
    case -1: // error al crear el proceso    
      perror("ERROR: no he podido crear un proceso con fork");
      exit(1);
      
    case 0: // PRIMER HIJO: ejecutar� el primer mandato y escribir� en la tuber�a
      // Como va a escribir por la tuber�a...
      close(tuberia[0]);  // ... cierro extremo de lectura
      // redirecciona extremo de escritura de la tuber�a a stdout
      if ( dup2(tuberia[1], 1) == -1 ) { 
        perror("No se puede redireccionar stdout");
        exit(2);
      }
      // stdout ya redireccionado, cierra descriptor out tuber�a
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
      
    case 0: // SEGUNDO HIJO: ejecutar� el segundo mandato y leer� de la tuber�a    
      // Como quiere leer de la tuber�a... 
      close(tuberia[1]);  // ... cierra extremo de escritura de la tuber�a
      // redirecciona stdin al extremo de lectura de la tuber�a
      if ( dup2(tuberia[0], 0) == -1 ) { 
        perror("No se puede redireccionar stdin");
        exit(5);
      }
      // stdin ya redireccionado, cierra descriptor in tuber�a
      close(tuberia[0]);  
      
      execlp("wc","wc","-w", NULL); // El SEGUNDO HIJO ejecuta el segundo mandato
      perror("Error al ejecutar el segundo mandato");
      exit(6);
  }
  
  // el proceso padre cierra extremos tuber�a ya que no los usa
  close(tuberia[0]);
  close(tuberia[1]);
  // y espera a que terminen los dos hijos
  wait(NULL);
  wait(NULL);
  
}
