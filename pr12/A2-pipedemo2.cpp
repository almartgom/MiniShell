/** 
 * ANEXO II. Archivo pipedemo2.cpp          
 *
 * PROBLEMA PRODUCTOR-CONSUMIDOR
 * 
 * REQUISITO PREVIO: necesitas un fichero llamado texto.txt con alg�n contenido
 * 
 * El programa crea dos procesos que act�an de productor y de consumidor respectivamente.
 * Los dos proceso se comunican mediante una tuber�a creada por el proceso padre.
 * El productor lee del archivo texto.txt y escribe en la tuber�a (en bloques de 512 bytes).
 * El consumidor lee de la tuber�a y cuenta el n�mero de caracteres.
 * 
 * @author  Eloy J. Mata
 * @author  Francisco J. Garc�a
 */

#include <iostream>   // Necesario para cout
#include <cstdlib>    // Necesario para la funci�n exit
#include <cstring>    // Necesario para la funci�n strcpy
#include <sys/wait.h> // Neceasario para la funci�n wait
#include <fcntl.h>    // Necesario para llamadas sistema archivos: creat, open,...
#include <unistd.h>   // Proporciona acceso a las llamadas al sistema
                      // a trav�s de la API POSIX, como fork y pipe

using namespace std;

const int MAXBUFFER = 512; // tama�o m�nimo garantizado para una tuber�a

void Productor(int f, const char[]);
void Consumidor(int f);

int main() {
  int tuberia[2];

  // CREAR LA TUBER�A
  if (pipe(tuberia) == -1) {
    perror("Aviso: error al crear la tuber�a");
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
  
  // Esto lo ejecuta s�lo el PADRE
  // El padre no usa la tuber�a, por lo que cierra sus descriptores de fichero
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
      write(out_tuberia, buffer, n);                        // escribe en la tuber�a

    if (n == -1)
      perror("Aviso: error al escribir en la tuber�a");

    close(fd_fichero);
  }
  return;
}

void Consumidor(int in_tuberia) {
  int n, nbytes = 0;
  char buffer[MAXBUFFER];

  while ((n = read(in_tuberia, buffer, MAXBUFFER)) != 0)   { // lee de la tuber�a
    nbytes = nbytes + n; // cuenta bytes
    write(1, buffer, n); // escribe en la salida est�ndar
  }
  if (n == -1)
    perror("Aviso: Error al leer de la tuber�a");
  else
    cout << "El archivo tiene " << nbytes << " bytes" << endl;
  return;
}
