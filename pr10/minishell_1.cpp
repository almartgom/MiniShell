#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;

int trocear_linea(char *str, const char *delim, char **&parts);
void ejecutar(char comando[]);
void read(char str[], int tam);

int main()
{

    char cmd[256];
    cout << "$ ";
    read(cmd,256);
    while (strcmp(cmd,"salir")!=0){
        ejecutar(cmd);
        cout << "$ ";
        read(cmd,256);
    }

    return 0;
}

void ejecutar(char comando[])
{

    // Parte 1: Construcción del comando en trozos

    char **parts;
    trocear_linea(comando, " \t", parts);

    // Parte 2: Ejecución del mandato en un nuevo proceso

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

void read(char str[], int tam){
	if(cin.fail()){
		cin.clear();
		cin.ignore(256,'\n');
	}
	cin.getline(str,tam);
}

