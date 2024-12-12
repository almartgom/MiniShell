// Importación de librerias de utilidad
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <fcntl.h> 

// Definición de colores en ANSI
#define green string("\033[32m")
#define orange string("\033[38;5;202m")
#define blue string("\033[34m")
#define blue2 string("\033[38;5;45m")
#define reset string("\033[0m")

using namespace std;

// Trocea un array de caracteres conforme a un o varios caracteres delimitadores
int trocear_linea(char *str, const char *delim, char **&parts);

// Ejecuta un comando
void ejecutar(char comando[]);

// Comprueba el estado del buffer y lee una linea por la entrada estándar
void read(char str[], int tam);

// Si encuentra ';' va llamando a la función ejecutar()
void ejecutarVarios(char comandos[]);

// Quita los espacios y tabulaciones del principio y final de una cadena
char *strtrim(char *str);

// Dada una cadena encuentra '<', '>', '>>' y '2>' para gestionar las redirecciones de salidas (de error y estándar) y entrada estándar
bool getredir(char *str, char *&in, char *&out, char *&err);

// Dado un comando redirrecciona sus salidas (de error y estándar) y entrada estándar y lo ejecuta
void RedireccionarYEjecutar(char* comando, bool sinPipe);

// Construye el prompt que muestra: [<usuario>@<host> <cwd>] $
string prompt();

// Muestra un pequeño ASCII art
void inicio();

const int MAX_CARACTERES = 255; // Tamaño máximo a leer

int main(int argc, char *argv[])
{
    char comando[MAX_CARACTERES + 1];
    switch (argc)
    {
    case 1: // Esta es la versión interactiva

        inicio(); // Mostramos el ASCII art

        cout << prompt(); // Mostrar un mensaje (prompt)

        read(comando, MAX_CARACTERES); // Leer lí­nea de comandos
        
        while (strcmp(comando, "salir") != 0)
        { // Repetir mientras comando distinto de salir
            
            ejecutarVarios(comando); // Separa los comandos si hay ";" y los ejecuta todos (Creando procesos hijos)
            cout << prompt();            // Mostrar un mensaje (prompt)
            read(comando, 256);      // Leer nueva lí­nea de comando
        }
        break;

    default:
    { // Esta es la versión de ficheros de comando

        for (int i = 1; i < argc; i++)
        { // lee cada fichero y los ejecuta linea a linea

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
                while (!entrada.eof() && strcmp(comando, "salir") != 0)
                { // Mientras no termine el fichero ni el comando sea salir
                    // Ejecutar la lí­nea de comando en otro proceso usando la función ejecutarVarios (Por si hay ; en la línea)
                    ejecutarVarios(comando);
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
    for(int i=0; comando[i]!='\0'; i++) // Busca si hay comentarios y los quita
    {
        if(comando[i] == '#') comando[i] = '\0';
    }
    char** comandoPipeado;
    int nTuberias = trocear_linea(comando,"|",comandoPipeado);
    nTuberias --;
    if(nTuberias>1){
        cerr << "Más de una tubería! (Solo se ejecutará el primer comando)" << endl;
    }
    if(nTuberias==1)
    {
        int pp[2];
        if(pipe(pp)==-1) // Crear la tubería
        {
            perror("Error al crear la tubería");
            exit(1);
        }

        int pid1=fork(); // Crear hijo 1 (Ejecuta primer comando)

        switch(pid1)
        {
            case -1: // Error al crear el hijo
                perror("ERROR: no he podido crear un proceso con fork");
                exit(2);
            
            case 0: // Primer hijo (Escribirá en la tubería)
                close(pp[0]);
                if(dup2(pp[1],1)==-1){
                    perror("No se puede redireccionar stdout");
                    exit(2);
                }
                close(pp[1]);
                RedireccionarYEjecutar(comandoPipeado[0],false);
                exit(0);
        }

        int pid2=fork(); // Crear hijo 2 (Ejecuta segundo comando)

        switch(pid2){
            case -1: // Error al crear el hijo
                perror("ERROR: no he podido crear un proceso con fork");
                exit(2);

            case 0: // Segundo hijo (Leerá de la tubería)
                close(pp[1]);
                if(dup2(pp[0],0)==-1){
                    perror("No se puede redireccionar stdin");
                    exit(2);
                }
                close(pp[0]);
                RedireccionarYEjecutar(comandoPipeado[1],false);
                exit(0);
        }

        close(pp[0]);close(pp[1]); // Cierra ambos extremos de la tubería del padre
        wait(NULL);wait(NULL); // Espera a los hijos
    }
    else
    {
        RedireccionarYEjecutar(comando,true);
    }

}

void RedireccionarYEjecutar(char* comando, bool sinPipe)
{

    // Parte 1: Construcción del comando en trozos

    char * f_in, * f_out, * f_err; // Declaración de los flujos de E/S 
    bool noVaciarOut = getredir(comando,f_in,f_out,f_err); // Separación del comando y los flujos de E/S y 
                                                        // comprobación de si hay que modificar la salida

    char **parts; 
    trocear_linea(comando, " \t", parts); // Troceamos el comando

    // Parte 2: Ejecución del mandato en un nuevo proceso
    if(sinPipe){
        if (parts[0]!=nullptr && strcmp(parts[0], "cd") == 0) // Comprueba si es cd
        {
            string home = "/home/" + string(getenv("USER")); // Guarda la ruta de la carpeta personal
            if(parts[1] == nullptr)
            {
                chdir(home.c_str()); // Si no pasamos ningún argumento se va a la carpeta personal
                return;
            }
            string nDir = string(parts[1]); // Transforma el directorio al que nos vamos a cambiar a string
            if(nDir.rfind("~", 0) == 0) // Busca que la ruta empiece por ~
            { 
                nDir.erase(0,1); // Elimina el primer caracter (~)
                nDir = home + nDir;
            }
            chdir(nDir.c_str());
            return;
        }
    }

    pid_t pid = fork(); // Creamos proceso hijo para ver que ejecute el comando
    switch (pid)
    {
    case -1:
        perror("Error al crear un proceso\n");
        break;
    case 0: // proceso hijo
        

        if(f_out!=NULL){
            close(1); // cierra la salida estándar
            int fd;          
            if(noVaciarOut){
                fd = open(f_out, O_WRONLY|O_CREAT|O_APPEND, 0644); // Abre el fichero que se usara como salida sin modificar su contenido
            }
            else
            {
                fd = open(f_out, O_WRONLY|O_CREAT|O_TRUNC, 0644); // Abre el fichero que se usara como salida vaciando su contenido
            }
            if ( fd == -1 ){
                perror("No puedo crear el archivo \n");
            exit(2);
            }
        }

        if(f_in!=NULL){
            close(0); // cierra la entrada estándar
            int fd = open(f_in, O_RDONLY); // abre el archivo como sólo lectura
            if ( fd == -1 ){
                cerr << "No puedo abrir el archivo para la lectura" << endl;
                exit(-1);
            }
        }

        if(f_err!=NULL){
            close(2); // cierra la salida de error estándar
            int fd = open(f_err, O_WRONLY|O_CREAT|O_TRUNC, 0644); // abre el archivo como sólo lectura
            if ( fd == -1 ){
                cerr << "No puedo abrir el archivo para la lectura" << endl;
                exit(-1);
            }
        }

        execvp(parts[0], parts); // Ejecuta el comando
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
{ // Limpia el buffer y lee
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(256, '\n');
    }
    cin.getline(str, tam);
}

void ejecutarVarios(char comandos[])
{ // Si se encuentra ";" ejecuta individualmente cada comando
    char **comando;
    int n = trocear_linea(comandos, ";", comando);
    for (int i = 0; i < n; i++)
    {
        ejecutar(comando[i]);
    }
}

char *strtrim(char *str)
{ // Quita espacios al comienzo al final
    if (str == NULL)
        return NULL;

    // Avanza por la izquierda
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t')
    {
        i++;
    }

    // Avanza por la derecha
    int f = strlen(str) - 1;
    while (f > i && (str[f] == ' ' || str[f] == '\t'))
    {
        f--;
    }

    str[f + 1] = '\0'; // Poner el carácter de fin de cadena

    // Devuelve la dirección del nuevo string
    return &str[i];
}

bool getredir(char *str, char *&in, char *&out, char *&err)
{ // Encuentra "<" y ">" y pone lo correspondiente en in, out y err
    bool noVaciarOut=false;
    int i = 0;
    in = NULL;
    out = NULL;
    err = NULL;
    if (str == NULL)
        return false;
    while (*(str + i) != '\0')
    {
        if (*(str + i) == '<')
        { // Hemos encontrado la parte de in
            in = str + i + 1;
            *(str + i) = '\0';
        }
        if (*(str + i) == '>' && *(str + i + 1) == '>')
        { // Hemos encontrado la parte de out (Teniendo en cuenta que no hay que vaciar el out)
            noVaciarOut=true;
            out = str + i + 2;
            *(str + i) = '\0';
            *(str + i + 1) = '\0';
        }
        if (*(str + i) == '>' && *(str + i + 1) != '>')
        { // Hemos encontrado la parte de out (Teniendo en cuenta que hay que vaciar el out)
            noVaciarOut=false;
            out = str + i + 1;
            *(str + i) = '\0';
        }
        if (*(str + i) == '2' && *(str + i + 1) == '>')
        { // Hemos encontrado la parte de error
            err = str + i + 2;
            *(str + i) = '\0';
            *(str + i + 1) = '\0';
        }
        i++;
    }
    in = strtrim(in); // Nos quitamos los espacios
    out = strtrim(out);
    err = strtrim(err);
    if (in != NULL && *in == '\0')
        in = NULL; // comprobamos si solo hay espacios
    if (out != NULL && *out == '\0')
        out = NULL;
    if (err != NULL && *err == '\0'){
        err = NULL;
    }
    
    return noVaciarOut; 
}

string prompt()
{
    const char* username = getenv("USER"); // Array de char que contiene el nombre del usuario
    char cwd[MAX_CARACTERES]; // Array que usaremos para poner la ruta del directorio actual
    char host[MAX_CARACTERES]; // Array que usaremos para poner el nombre del host

    // Si no podemos encontrar los datos mostraremos un prompt básico seguido de un error
    if (username == nullptr)
    {
        perror("Error al obtener el nombre de usuario");
        return "$ ";
    }

    if (getcwd(cwd, sizeof(cwd)) == nullptr)
    {
        perror("Error al obtener el directorio de trabajo");
        return "$ ";
    }

    if(gethostname(host, sizeof(host)) == -1)
    {
        perror("Error al obtener el host del sistema");
        return "$ ";
    }

    // Nos guardamos un string que contenga /home/<usuario
    string home = "/home/" + string(username);
    string currentDir = string(cwd);

    // Reemplazamos el directorio home por ~ si coincide
    if (currentDir.rfind(home, 0) == 0) { // rfind con 0 verifica si comienza con "home"
        currentDir = "~" + currentDir.substr(home.length());
    }

    return "[" + green + string(username) + "@" + string(host) + " " + orange + string(currentDir) + reset + "]$ ";
}

void inicio()
{
    const char *img = "                                ▄                   \n"      
                      "                               ▟█▙                  \n"
                      "                              ▟███▙                 \n"
                      "                             ▟█████▙                \n"
                      "                            ▟███████▙               \n"
                      "                           ▂▔▀▜██████▙              \n"   
                      "                          ▟██▅▂▝▜█████▙             \n"
                      "                         ▟█████████████▙            \n"
                      "                        ▟███████████████▙           \n"
                      "                       ▟█████████████████▙          \n"
                      "                      ▟███████████████████▙         \n"
                      "                     ▟█████████▛▀▀▜████████▙        \n"
                      "                    ▟████████▛      ▜███████▙       \n" 
                      "                   ▟█████████        ████████▙      \n"
                      "                  ▟██████████        █████▆▅▄▃▂     \n"
                      "                 ▟██████████▛        ▜█████████▙    \n"
                      "                ▟██████▀▀▀              ▀▀██████▙   \n"
                      "               ▟███▀▘                       ▝▀███▙  \n"
                      "              ▟▛▀                               ▀▜▙ \n";

    const char *txt = 
        
        " __      __       .__                                  __            \n"
        "/  \\    /  \\ ____ |  |   ____  ____   _____   ____   _/  |_  ____    \n"
        "\\   \\/\\/   // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\  \\   __\\/  _ \\   \n"
        " \\        /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/   |  | (  <_> )  \n"
        "  \\__/\\  /  \\___  >____/\\___  >____/|__|_|  /\\___  >  |__|  \\____/   \n"
        "       \\/       \\/          \\/            \\/     \\/                  \n"
        "       .__                         __                      /\\        \n"
        "_____  |  |   _____ _____ ________/  |_  ____   ____   ____\\(  ______\n"
        "\\__  \\ |  |  /     \\\\__  \\\\_  __ \\   __\\/ ___\\ /  _ \\ /     \\ /  ___/ \n"
        " / __ \\|  |_|  Y Y  \\/ __ \\|  | \\/|  | / /_/  >  <_> )  Y Y  \\\\___ \\  \n"
        "(____  /____/__|_|  (____  /__|   |__| \\___  / \\____/|__|_|  /____  > \n"
        "     \\/           \\/     \\/           /_____/              \\/     \\/ \n"
        "             .__       .__  _________.__           .__  .__          \n"
        "       _____ |__| ____ |__|/   _____/|  |__   ____ |  | |  |         \n"
        "      /     \\|  |/    \\|  |\\_____  \\ |  |  \\_/ __ \\|  | |  |         \n"
        "     |  Y Y  \\  |   |  \\  |/        \\|   Y  \\  ___/|  |_|  |__       \n"
        "     |__|_|  /__|___|  /__/_______  /|___|  /\\___  >____/____/       \n"
        "           \\/        \\/           \\/      \\/     \\/                  \n";                                                                                                                                                                                                                                                                                                                                                                      

    cout << blue2 << img << reset << endl;
    cout << blue2 << txt << reset << endl;
}