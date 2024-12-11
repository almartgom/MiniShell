#include <iostream>
#include <cstring>

using namespace std;

char *strtrim(char *str) {
  if (str == NULL)
	return NULL;

  // Avanza por la izquierda
  int i = 0;
  while (str[i] == ' ' || str[i] == '\t') {
	i++;
  }

  // Avanza por la derecha
  int f = strlen(str) - 1;
  while (f > i && (str[f] == ' ' || str[f] == '\t')) {
	f--;
  }

  str[f + 1] = '\0'; // Poner el carácter de fin de cadena

  // Devuelve la dirección del nuevo string
  return &str[i];
}

void read(char str[], int tam){
	if(cin.fail()){
		cin.clear();
		cin.ignore(256,'\n');
	}
	cin.getline(str,tam);
}

void getredir(char* str, char* & in, char* & out){
    int i=0;
    in=NULL;
    out=NULL;
    if(str==NULL) return;
    while(*(str+i)!='\0'){
        if(*(str+i)=='<'){ //Hemos encontrado la parte de in
            in=str+i+1;
            *(str+i)='\0';
        }
        if(*(str+i)=='>'){ //Hemos encontrado la parte de out
            out=str+i+1;
            *(str+i)='\0';
        }
        i++;
    }
    in=strtrim(in); //Nos quitamos los espacios
    out=strtrim(out);
    if(in!=NULL && *in=='\0')in=NULL; //comprobamos si solo hay espacios
    if(out!=NULL && *out=='\0')out=NULL;
}

int main(){
    char comando[256];
    read(comando,256);
    char * f_in, * f_out;
    getredir(comando, f_in, f_out);
    cout << "|" << (comando != NULL ? comando : "NULL") << "|";
    cout << (f_in != NULL ? f_in : "NULL") << "|";
    cout << (f_out != NULL ? f_out : "NULL") << "|" << endl;

    return 0;
}