# MiniShell de Alejandro Martínez Gómiz

## Instrucciones de uso
* Compilar el archivo `minishell.cpp` (Por ejemplo con el comando `g++`) para generar un ejecutable
```
g++ minishell.cpp -o minishell
```

* Si se quiere entrar en **modo interactivo** basta con ejecutar el programa
```
./minishell
```
* Para salir del modo interactivo hay que ejecutar el comando `salir` dentro de la minishell
```
[<user>@<host> ~/minishell]$ salir
```

* Si se quiere entrar en **modo ejecución de guiones** basta con ejecutar el programa seguido por los guiones que se quieran ejecutar (Separados por espacios)

```
./minishell guion1.sh guion2.sh
```

## Especficaciones
* **Prompt**: En su versión interactiva incluye un prompt indicando el nombre de usuario, host y directorio de trabajo. El prompt muestra además el directorio /home/\<user> como `~`.

  [![Captura-de-pantalla-2024-12-13-164307.png](https://i.postimg.cc/4x3K8hh7/Captura-de-pantalla-2024-12-13-164307.png)](https://postimg.cc/4mrNyyPs)

* **Comandos**: Admite la ejecución de comandos separados por espacios (El comando `cd` funciona correctamente como funciona en bash, dejarlo sin argumentos ejecutará `cd /home/<user>`).

* **Guiones**: Admite la ejecución de guiones.

* **Redirección**: La entrada y salida de un comando se puede redirigir de la siguiente forma:
    * sdfsd 

