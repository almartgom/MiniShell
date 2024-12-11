// ANEXO VII. completo.cpp
// Programa que crea un nuevo proceso para que ejecute el mandato "ls -l /home" 
// invocando a la llamada al sistema: execvp  

#include <iostream>   // Necesario para cout
#include <cstdlib>    // Necesario para la funcion exit
#include <unistd.h>   // Proporciona acceso a las llamadas al sistema 
                      // a travÃ©s de la API POSIX, como fork
#include <cstring>    // Necesario para la funciÃ³n strcpy
#include <sys/wait.h> // Necesario para la funciÃ³n wait

using namespace std;

int main() {
  
  // PARTE 1: CONSTRUCCIÃ“N del mandato en trozos_cmd
  // La funciÃ³n strdup reserva memoria para duplicar el string que recibe como 
  // argumento y devuelve un puntero a la zona de memoria reservada

  char ** trozos_cmd; // Puntero doble a char, es decir, array de cadenas
  
  // Se reserva memoria para 4 punteros a char;
  trozos_cmd = new char * [4]; 

  trozos_cmd[0] = strdup("ls");
  trozos_cmd[1] = strdup("-l");
  trozos_cmd[2] = strdup("/home");
  trozos_cmd[3] = NULL; // El Ãºltimo argumento tiene que ser NULL
  
  //PARTE 2: EJECUCIÃ“N del mandato en un nuevo proceso 
  pid_t pid = fork(); //Se crea un nuevo proceso
  switch(pid) {
  case -1:
    perror("Error al crear un proceso\n");
    break;
  case 0:     // proceso hijo
    execvp(trozos_cmd[0], trozos_cmd);
    perror("Error al ejecutar la funciÃ³n execvp");
    exit(-1);        // llega aquÃ­ si hay error
  default:    // proceso padre
    wait(NULL);      // espera a que termine el proceso hijo
    free(trozos_cmd); // Una vez terminado, liberar memoria reservada
  }
}
