// Anexo III. Fichero ps_a_fichero.cpp
// Ejecuta el mandato "ps -l" en otro proceso, redireccionando su salida a resultado.txt
// Autores: Eloy J. Mata
//          Francisco J. García Izquierdo

#include <iostream>   // Necesario para cout
#include <cstdlib>    // Necesario para la función exit
#include <sys/wait.h> // Neceasario para la función wait
#include <fcntl.h>    // Necesario para llamadas sistema archivos: open, creat...
#include <unistd.h>   // Proporciona acceso a las llamadas al sistema a través de la API POSIX

using namespace  std;

int main() {
  // Crea un nuevo proceso en el que ejecuta el mandato "ps -l"
  // Previamente a la ejecución de ps, el proceso hijo, redirecciona
  // su salida estándar al fichero resultado.txt
  
  int pid = fork(); // creamos un nuevo proceso
  switch(pid) {
    case -1: // error
      perror("Error en fork");
      exit(-1);
      
    case 0: // proceso hijo
    {  // La llave es necesaria porque se declaran variables dentro del bloque
      cout << "El hijo va a redireccionar su salida estándar al archivo resultado.txt" << endl << endl;
      // El proceso hijo redirige su salida estándar (stdout)
      close(1);     // cierra la salida estándar
      int fd = open("resultado.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);  // crea el fichero resultado.txt
      if ( fd == -1 ){
        perror("No puedo crear el archivo \n");
        exit(2);
      }
      // Ahora el proceso hijo tiene su stdout redireccionada
      // Todo lo que escriba irá a parar al fichero resultado.txt
      // Por tanto, el comando "ps -l" que se va a ejecutar lo 
      // hará escribiendo su resultado en el fichero resultado.txt
      execlp("ps", "ps","-l", NULL); // ejecuta ps

      perror("Error al ejecutar execlp");
      exit(-1);
    }
    default:
      wait(NULL);
      cout << "Terminó la ejecución de ps.\nEl resultado está en el archivo resultado.txt" << endl;
  }
}

