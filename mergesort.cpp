//arquivo para teste do mergesort multiway, no qual dividimos os arquivos em k arquivos com aproximadamente o
// mesmo numero de entradas e depois os juntamos ao final, ja ordenados

#include "base.h"

struct athletes{

    char measure[10];
    char quantile[80];
    char area[50];
    char sex[15];
    char age[20];
    char geography[90];
    char ethnic[60];
    char value[15];

};

void dividirArquivo(ifstream& arquivoBinario, string nomeArquivo, int registrosPorBloco){

    athletes saida[10000];
    int registrosLidos = 0;

    for (int i = 0; i < registrosPorBloco; i++){
        if (arquivoBinario.read(reinterpret_cast<char*>(&saida[i]), sizeof(athletes))) {
            registrosLidos++;
        }
    }

    int i = 0;
    while(i <= registrosPorBloco){
        "base.h".read(reinterpret_cast<char*>(&buffer[registrosLidos]), sizeof(athletes));
        i++;
    }

    if(registrosLidos > 0){
        ofstream saida(nomeArquivo, ios::binary);
        saida.write(reinterpret_cast<char*>(buffer), registrosLidos * sizeof(athletes));
        saida.close();
    }

    cout << "Arquivo " << nomeArquivo << " criado com " << registrosLidos << " registros." << endl;

}

int main(){

    ifstream arquivoBinario("base.h", ios::binary);

    if (!arquivoBinario) {
        cout << "Erro ao abrir arquivo binario" << endl;
        return 1;
    }

    const int registrosPorBloco = 10000;
    int i = 0;

    while(arquivoBinario){
        string nomeArquivo = "temp" + to_string(i) + ".bin"; 
        dividirArquivo(arquivoBinario, nomeArquivo, registrosPorBloco);
        i++;
    }


    arquivoBinario.close();

    return 0;
}















