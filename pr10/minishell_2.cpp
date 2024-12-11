#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;

int trocear_linea(char *str, const char *delim, char **&parts);
void ejecutar(char comando[]);
void read(char str[], int tam);
void ejecutarVarios(char comandos[]);

const int MAX_CARACTERES = 255;

int main(int argc, char *argv[])
{
    char comando[MAX_CARACTERES + 1];
    switch (argc)
    {
    case 1: // Esta es la versión interactiva

        cout << "$ "; // Mostrar un mensaje (prompt)
        
        read(comando,MAX_CARACTERES); // Leer lí­nea de comandos
        while (strcmp(comando,"salir")!=0)
        {   // Repetir mientras comando distinto de salir
            ejecutarVarios(comando);  // Separa los comandos si hay ";" y los ejecuta todos (Creando procesos hijos)
            cout << "$ ";   // Mostrar un mensaje (prompt)
            read(comando,256);    // Leer nueva lÃ­nea de comando
        }
        break;

    default:{ // Esta es la versión de ficheros de comando
        
        for(int i=1; i<argc; i++){ // lee cada fichero y los ejecuta linea a linea

            ifstream entrada;      // define un flujo de entrada
            entrada.open(argv[i]); // abre el fichero y lo asocia a entrada
            if (entrada.fail())
            { // comprueba que se ha abierto correctamente
                cerr << "No puede abrirse el archivo " << argv[i] << endl;
                exit(2);
            }
            else
            {
                // lee una lí­nea de comando del fichero
                entrada.getline(comando, MAX_CARACTERES, '\n');
                while (!entrada.eof() && strcmp(comando,"salir")!=0)
                { // Mientras no termine el fichero ni el comando sea salir
                    // Ejecutar la lí­nea de comando en otro proceso usando la función ejecutar
                    ejecutar(comando);
                    // lee otra lí­nea de comando del fichero
                    entrada.getline(comando, MAX_CARACTERES, '\n');
                }
                entrada.close(); // cierra el fichero
            }
        }

        exit(0);
    }

    exit(0);
    }
}

void ejecutar(char comando[])
{ // Ejecuta un comando

    // Parte 1: Construcción del comando en trozos

    char **parts;
    trocear_linea(comando, " \t", parts);
    
    // Parte 2: Ejecución del mandato en un nuevo proceso

    if (strcmp(parts[0],"cd")==0)
    {
        chdir(parts[1]);
        return;
    }

    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("Error al crear un proceso\n");
        break;
    case 0: // proceso hijo
        execvp(parts[0], parts);
        perror("Error al ejecutar la función execvp");
        exit(-1);    // llega aquí si hay error
    default:         // proceso padre
        wait(NULL);  // espera a que termine el proceso hijo
        free(parts); // Liberar memoria reservada
    }
}

int trocear_linea(char *str, const char *delim, char **&parts)
{
    parts = (char **)malloc(sizeof(char *));
    parts[0] = NULL;

    if (str == NULL)
        return 0; // caso que no recibamos str

    int numParts = 0;

    int i = 0;
    while (str[i] != '\0')
    {
        int j = i;
        // recorrer str hasta encontrar un delimitador o el final
        while (str[j] != '\0' && strchr(delim, str[j]) == NULL)
        {
            j++;
        }

        if (str[j] != '\0')
        { // Si no se ha llegado al final
            str[j] =
                '\0'; // terminar la subcadena, sustituir el delimitador por un '\0'
            j++;      // Avanzar al siguiente caracter de la cadena str
        }

        if (str[i] != '\0')
        { // Si la subcadena tiene caracteres
            // actualizar trozos
            numParts++;
            parts = (char **)realloc(parts, (numParts + 1) * sizeof(char *));
            parts[numParts - 1] = &str[i];
            parts[numParts] = NULL; // necesario para usar después con execvp
        }

        i = j; // Para continuar con el siguiente trozo
    }

    return numParts;
}

void read(char str[], int tam)
{ // Limpia el bufer y lee
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(256, '\n');
    }
    cin.getline(str, tam);
}

void ejecutarVarios(char comandos[])
{ // Si se encuentra ";" ejecuta individualmente cada comando
    char ** comando;
    int n = trocear_linea(comandos, ";", comando);
    for (int i=0; i<n; i++){
        ejecutar(comando[i]);
    }
}