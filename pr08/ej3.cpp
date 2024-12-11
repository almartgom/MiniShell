#include <iostream>
#include <cstring>

using namespace std;

void read(char str[], int tam){
    if(cin.fail()){
        cin.clear();
        cin.ignore(256,'\n');
    }
    cin.getline(str,tam);
}

int main(){
    char cad1[11],cad2[11];
    cout << "Introduzca la primera cadena" << endl;
    read(cad1,11);
    cout << "Introduzca la segundo cadena" << endl;
    read(cad2,11);
    cout << "Cadena 1 (" << strlen(cad1) << "): " << cad1 << endl;
    cout << "Cadena 2 (" << strlen(cad2) << "): " << cad2 << endl;
    return 0;
}