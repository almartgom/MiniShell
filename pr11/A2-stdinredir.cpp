// Anexo II. Fichero stdinredir.cpp
// Redirige la entrada estándar del proceso al fichero /etc/passwd
// Autores: Eloy J. Mata
//          Francisco J. García Izquierdo

#include <iostream> // Necesario para cout
#include <cstdlib>  // Necesario para la función exit
#include <fcntl.h>  // Necesario para llamadas sistema archivos: open, creat...
#include <unistd.h> // Proporciona acceso a las llamadas al sistema a través de la API POSIX

using namespace  std;

int main() {  
  // El ejemplo ilustra como redirigir la entrada estándar de un proceso.
  // Un programa C++ lee de su entrada estándar usando cin, que por
  // defecto está conectada a la terminal (todo lo que se teclea en la 
  // terminal usando el teclado, es leído por el programa)
  // El ejercicio hace varias cosas
  // - Primero se utiliza la función cin.getline para leer una línea desde 
  //   la entrada estándar (stdin) sin haber modificado esta (se leerá lo 
  //   tecleado en el teclado).
  // - Después se redirecciona la entrada estándar del proceso al archivo 
  //   /etc/passwd. A partir de este momento, todo lo que se lea de la
  //   entrada estándar con cin se tomará de ese fichero.
  // - Por último, se utiliza nuevamente cin.getline para leer desde stdin. 
  //   El código es exactamente el mismo que el anterior, pero 
  //   en esta ocasión se lee una línea del archivo /etc/passwd

  const int TAM = 100;
  char linea[TAM+1];

  cout << "Escribe una línea:" << endl;
    
  cin.getline(linea, TAM+1);         // lee una línea
  cout << "La línea leída es:" << endl;
  cout << linea << endl;             // escribe la línea
    
  // Se redirige la entrada estándar al archivo /etc/passwd
  close(0);                               // cierra la entrada estándar
  int fd = open("/etc/passwd", O_RDONLY); // abre el archivo como sólo lectura
  if ( fd == -1 ){
    cerr << "No puedo abrir el archivo para la lectura" << endl;
    exit(-1);
  }
  
  // Ahora la entrada estándar (stdin) está redireccionada al archivo /etc/passwd
  // La línea se lee ahora del archivo /etc/passwd
  cout << "Ahora se va a leer la línea del archivo /etc/passwd:" << endl;

  // Se repiten las instrucciones para leer una línea de la entrada estándar
  cin.getline(linea, TAM+1);         // lee una línea
  // Pero ahora la línea leída se ha tomado del fichero, y no del teclado
  cout << "La línea leída es:" << endl;
  cout << linea << endl;             // escribe la línea
}