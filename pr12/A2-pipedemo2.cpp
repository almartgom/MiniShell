/** 
 * ANEXO II. Archivo pipedemo2.cpp          
 *
 * PROBLEMA PRODUCTOR-CONSUMIDOR
 * 
 * REQUISITO PREVIO: necesitas un fichero llamado texto.txt con algún contenido
 * 
 * El programa crea dos procesos que actúan de productor y de consumidor respectivamente.
 * Los dos proceso se comunican mediante una tubería creada por el proceso padre.
 * El productor lee del archivo texto.txt y escribe en la tubería (en bloques de 512 bytes).
 * El consumidor lee de la tubería y cuenta el número de caracteres.
 * 
 * @author  Eloy J. Mata
 * @author  Francisco J. García
 */

#include <iostream>   // Necesario para cout
#include <cstdlib>    // Necesario para la función exit
#include <cstring>    // Necesario para la función strcpy
#include <sys/wait.h> // Neceasario para la función wait
#include <fcntl.h>    // Necesario para llamadas sistema archivos: creat, open,...
#include <unistd.h>   // Proporciona acceso a las llamadas al sistema
                      // a través de la API POSIX, como fork y pipe

using namespace std;

const int MAXBUFFER = 512; // tamaño mínimo garantizado para una tubería

void Productor(int f, const char[]);
void Consumidor(int f);

int main() {
  int tuberia[2];

  // CREAR LA TUBERÍA
  if (pipe(tuberia) == -1) {
    perror("Aviso: error al crear la tubería");
    exit(1);
  }

  // CREAR UN PROCESO PARA EL PRODUCTOR
  pid_t pid1 = fork();
  switch (pid1) {
    case -1: // error
      perror("Aviso: error al crear el proceso");
      exit(2);

    case 0:              // proceso hijo: Productor
      close(tuberia[0]); // QUIERE ESCRIBIR: cierra el descriptor de lectura
      Productor(tuberia[1], "texto.txt");
      close(tuberia[1]); // fin de escritura
      exit(0);
  }

  // CREAR OTRO PROCESO PARA EL CONSUMIDOR
  pid_t pid2 = fork();
  switch (pid2) {
    case -1: // error
      perror("Aviso: error al crear el proceso");
      exit(2);

    case 0:              // proceso hijo: Consumidor
      close(tuberia[1]); // QUIERE LEER: cierra el descriptor de escritura
      Consumidor(tuberia[0]);
      close(tuberia[0]); // fin de lectura
      exit(0);
  }
  
  // Esto lo ejecuta sólo el PADRE
  // El padre no usa la tubería, por lo que cierra sus descriptores de fichero
  close(tuberia[0]); // NO QUIERE LEER
  close(tuberia[1]); // NO QUIERE ESCRIBIR
  
  // El PADRE espera a que terminen los dos hijos
  wait(NULL);
  wait(NULL);
  exit(0);
}

void Productor(int out_tuberia, const char fichero[]) {
  int n;
  char buffer[MAXBUFFER];

  // ABRIR FICHERO DE DATOS PARA LECTURA
  int fd_fichero = open(fichero, O_RDONLY);
  if (fd_fichero == -1) { // error
    perror("ERROR al abrir el fichero. Crea antes el fichero texto.txt");
  } else {
    while ((n = read(fd_fichero, buffer, MAXBUFFER)) != 0) // lee bloques de 512 bytes
      write(out_tuberia, buffer, n);                        // escribe en la tubería

    if (n == -1)
      perror("Aviso: error al escribir en la tubería");

    close(fd_fichero);
  }
  return;
}

void Consumidor(int in_tuberia) {
  int n, nbytes = 0;
  char buffer[MAXBUFFER];

  while ((n = read(in_tuberia, buffer, MAXBUFFER)) != 0)   { // lee de la tubería
    nbytes = nbytes + n; // cuenta bytes
    write(1, buffer, n); // escribe en la salida estándar
  }
  if (n == -1)
    perror("Aviso: Error al leer de la tubería");
  else
    cout << "El archivo tiene " << nbytes << " bytes" << endl;
  return;
}
