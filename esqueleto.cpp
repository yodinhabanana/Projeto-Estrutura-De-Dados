#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sys/stat.h>

// implementar:
// altera em posicao
// imprimir trecho
// salvar alteração

using namespace std;

struct athletes{

    char measure[10];
    char quantile[100];
    char area[100];
    char sex[20];
    char age[20];
    char geography[100];
    char ethnic[100];
    float value;

};

// sobrecarga de operador de saida;
ostream& operator<<(ostream& os, const athletes& a) {
    os << "Measure: " << a.measure
       << ", Quantile: " << a.quantile
       << ", Area: " << a.area
       << ", Sex: " << a.sex
       << ", Age: " << a.age
       << ", Geography: " << a.geography
       << ", Ethnic: " << a.ethnic
       << ", Value: " << a.value;
    return os;
}

// classe do arquivo binario e suas operacoes
class Binario{


private:

     // inserção, remoção e procura 
    bool alterarEmPosicao(int posicao);
    //metodos adicionais
    // metodos que envolvem arquivos
    bool inserePosicao(int posicao);


public:

    void transBinarioEmCsv(string nomeArquivoBin, string nomeArquivoCSV);
    void transCsvEmBinario(string nomeArquivoCSV);
    void imprimirTrecho(int posInicial, int posFinal);
    // construtor
    Binario() = default;
    // destrutor
    ~Binario() = default;

};

void Binario::transCsvEmBinario(string nomeArquivoCSV){

    string arquivochamada = nomeArquivoCSV;
    athletes atleta;
    ifstream entrada(nomeArquivoCSV);

    if (!entrada){
        throw runtime_error("Erro, arquivo inexistente.");
    }

    cout << "Qual o nome do arquivo binário no qual deseja guardar os dados? " << endl;
    string nomeArquivoBin;
    cin >> nomeArquivoBin;
    ofstream saida(nomeArquivoBin, ios::binary);

    if (!saida.is_open()) {
        throw runtime_error("Erro ao abrir arquivo de saída.");
    }

    cout << "Aguarde enquanto os dados são lidos do arquivo CSV..." << endl;

    // Lê o cabeçalho do arquivo CSV
    string cabecalho;
    getline(entrada, cabecalho);
    
    while(entrada.good()){
        // leitura dos dados
        entrada.getline(atleta.measure, 50, ',');
        entrada.getline(atleta.quantile, 100., ',');
        entrada.getline(atleta.area, 50, ',');
        entrada.getline(atleta.sex, 10, ',');
        entrada.getline(atleta.age, 20, ',');
        entrada.getline(atleta.geography, 100, ',');
		entrada.getline(atleta.ethnic, 50, ',');
		entrada >> atleta.value;
		entrada.ignore();
		
        saida.write(reinterpret_cast<char*>(&atleta), sizeof(atleta));

    }

    entrada.close();
    cout << "Dados gravados em binario com sucesso." << endl;
    saida.close();

    transBinarioEmCsv(nomeArquivoBin, arquivochamada);
}

bool Binario::inserePosicao(int posicao){

    if(posicao > 1 or posicao < 0){ // implementar tamanho do arquivo
        cout << "A posição que você digitou é inválida." << endl;
        return false;
    }

    athletes atleta_novo;

    cout << "Digite os dados que você quer inserir: " << endl;
    cout << "Measure: " << endl;
    cin >> atleta_novo.measure;
    cout << "Quantile: " << endl;
    cin >> atleta_novo.quantile;
    cout << "Area: " << endl;
    cin >> atleta_novo.area;
    cout << "Sex: " << endl;
    cin >> atleta_novo.sex;
    cout << "Age: " << endl;
    cin >> atleta_novo.age;
    cout << "Geography: " << endl;
    cin >> atleta_novo.geography;
    cout << "Ethnic: " << endl;
    cin >> atleta_novo.ethnic;
    cout << "Value: " << endl;
    cin >> atleta_novo.value;

    // terminar implementacao 


    return true;

}

void Binario::imprimirTrecho(int posInicial, int posFinal) {

    ifstream entrada("nomeArquivoBinario", ios::binary);


    if (!entrada) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        return;
    }

    athletes atleta;

    int tamanhoRegistro = sizeof(athletes);
    entrada.seekg(0, ios::end);
    int totalBytes = entrada.tellg();
    int totalRegistros = totalBytes / tamanhoRegistro;

    if (posInicial < 0 || posFinal >= totalRegistros || posInicial > posFinal) {
        cerr << "Intervalo inválido." << endl;
        return;
    }

    for (int i = posInicial; i <= posFinal; ++i) {
        entrada.seekg(i * tamanhoRegistro);
        entrada.read(reinterpret_cast<char*>(&atleta), tamanhoRegistro);
        cout << atleta << endl;
    }

    entrada.close();
}

bool Binario::alterarEmPosicao(int posicao) {
 
    if ( posicao < 0 || posicao > 255000) {
        cout << "A posição que você digitou é inválida." << endl;
        return false;
    }

    return true;

}

void Binario::transBinarioEmCsv(string nomeArquivoBin, string nomeArquivoCSV){
    
    ifstream arquivoBin(nomeArquivoBin, ios::binary);
    if(!arquivoBin){
        cerr << "Erro ao abrir o arquivo." << endl;
        return;
    }

    cout << "Digite o nome do arquivo no qual deseja gravar: ";
    string nomeArquivoCSVnovo;
    cin >> nomeArquivoCSVnovo;
    ofstream arquivoCsv(nomeArquivoCSVnovo);
    if(!arquivoCsv){
        cerr << "Erro ao criar o arquivo CSV." << endl;
        return;
    }

    athletes a;
    arquivoCsv << "measure,quantile,area,sex,age,geography,ethnic,value" << endl;

    while(arquivoBin.read(reinterpret_cast<char*>(&a), sizeof(athletes))){
        arquivoCsv << a.measure << ',' << a.quantile << ',' << a.area  << ','
                   << a.sex << ',' << a.age << ',' << a.geography << ','
                   << a.ethnic << ',' << a.value << endl;
    }

    arquivoBin.close();
    arquivoCsv.close();

    cout << "Conversão de binário para csv concluída, verifique abrindo o arquivo." << endl;
}


//implementacao do Menu para interação com o usuário
void menuPrincipal(){

    int opcao = 0;
    Binario binario;
    
    do{
    cout << endl;
    cout << "Bem-vindo ao sistema de gerenciamento de atletas!" << endl;
    cout << "Escolha uma opção:" << endl;
    cout << "1. Inserir atleta" << endl;
    cout << "2. Alterar atleta" << endl;
    cout << "3. Imprimir trecho" << endl;
    cout << "4. Salvar alterações" << endl;
    cout << "5. Buscar atleta" << endl;
    cout << "6. Excluir atleta" << endl;
    cout << "7. Sair" << endl;
    cout << "Digite sua opção: ";
    cin >> opcao;
    
        switch(opcao){
            case 1:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                break;
            case 2:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                break;
            case 3:
                int posInicial, posFinal;
                cout << "Digite a posição inicial da impressão: ";
                cin >> posInicial;
                cout << endl;
                cout << "Digite a posição final da impressão: ";
                cin >> posFinal;
                cout << endl;
                binario.imprimirTrecho(posInicial, posFinal);

                break;
            case 4:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                break;
            case 5:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                break;
            case 6:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                break;
            case 7:
                cout << "Saindo..."<< endl;
                return;
                break;
        }
        
    }while(opcao != 7);

}

int main(){

    Binario binario;
    string nomeArquivoCSV;

    cout << "Qual o nome do arquivo que deseja ler? ";
    cin >> nomeArquivoCSV;
    binario.transCsvEmBinario(nomeArquivoCSV);

    //chamada do menu principal loools gayr
    menuPrincipal();

    return 0;
}