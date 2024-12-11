// Anexo I. Fichero stdoutredir.cpp
// Redirige la salida estándar del proceso al fichero salida.txt
// Autores: Eloy J. Mata
//          Francisco J. García Izquierdo

#include <iostream> // Necesario para cout
#include <cstdlib>  // Necesario para la función exit
#include <fcntl.h>  // Necesario para llamadas sistema archivos: open, close...
#include <unistd.h> // Proporciona acceso a las llamadas al sistema a través del API POSIX

using namespace  std;
int main() {
  // El ejemplo ilustra cómo redirigir la salida estándar de un proceso.
  // Un programa C++ escribe en su salida estándar usando cout, que por
  // defecto está conectada a la terminal (todo lo que se escribe en la
  // salida estándar desde el programa con cout se ve en la terminal)
  // El ejercicio hace varias cosas
  // - Primero se muestra un mensaje con cout en la salida estándar sin haber 
  //   modificado esta (se verá en la terminal).
  // - Después se redirecciona la salida estándar del proceso al archivo 
  //   salida.txt. A partir de este momento, todo lo que se escriba en la
  //   salida estándar con cout irá a parar al fichero salida.txt
  // - Por último, se muestra nuevamente un mensaje con cout en la salida 
  //   estándar. El código es exactamente el mismo que el anterior, pero 
  //   en esta ocasión el mensaje se envía al archivo salida.txt

  // escribe un mensaje en la salida estándar
  cout << "En un lugar de La Mancha, de cuyo nombre ..." << endl;
  
  cout << "Voy a redirigir la salida estandar al fichero salida.txt" << endl;

  cout << "Después, el mensaje se escribirá en el archivo salida.txt (mira el fichero)" << endl;
  
  // Se redirige la salida al archivo salida.txt
  close(1);                                                     // cierra la salida estándar
  int fd = open("salida.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);  // crea el fichero resultado.txt
  if ( fd == -1 ){
    perror("No puedo crear el archivo \n");
    exit(2);
  }
  
  // Ahora la salida estándar (stdout) está redireccionada al archivo salida.txt
  // Se repiten las instrucciones para escribir un mensaje en la salida estándar
  cout << "En un lugar de La Mancha, de cuyo nombre ..." << endl;
  // Pero ahora el mensaje se ha escrito en el archivo salida.txt (compruébalo)
}
