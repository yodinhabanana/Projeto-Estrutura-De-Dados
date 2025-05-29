#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sys/stat.h>

// implementar:
// altera em posicao
// imprimir trecho
// salvar alteração
// escrita e alterar mudar coisas

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

    int tamanhoArquivo;
    string nomeArquivoBin;
    bool inserePosicao(int posicao);


public:

    void transBinarioEmCsv();
    void transCsvEmBinario(string nomeArquivoCSV);
    void imprimirTrecho(int posInicial, int posFinal);
    bool alterarEmPosicao(int posicao);
    Binario() = default;
    // destrutor
    ~Binario() = default;
    string getNomeArquivo();
    int getTamanho();

};

// retorna o nome do arquivo binario
string Binario::getNomeArquivo(){
    return nomeArquivoBin;
}

int Binario::getTamanho(){
    return tamanhoArquivo;
}

void Binario::transCsvEmBinario(string nomeArquivoCSV){

    string arquivochamada = nomeArquivoCSV;
    athletes atleta;
    ifstream entrada(nomeArquivoCSV);

    if (!entrada){
        throw runtime_error("Erro, arquivo inexistente.");
    }

    cout << "Qual o nome do arquivo binário no qual deseja guardar os dados? " << endl;
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
//teste
    entrada.close();
    cout << "Dados gravados em binario com sucesso." << endl;
    saida.close();

    int opcao;
    cout << "Você deseja verificar se a leitura foi correta com a criação de outro CSV?" << endl;
    cout << "1. Sim" << endl;
    cout << "2. Não" << endl;
    cin >> opcao;


    ifstream verTamanho(nomeArquivoBin, ios::binary);

    int tamanhoRegistro = sizeof(athletes);
    verTamanho.seekg(0, ios::end);
    int totalBytes = verTamanho.tellg();
    int totalRegistros = totalBytes / tamanhoRegistro;
    tamanhoArquivo = totalRegistros;

    cout << "Quantidade de dados gravados: " << tamanhoArquivo << endl;

    if(opcao == 1){
        transBinarioEmCsv();
    }else if(opcao == 2){
        return;
    }
    
}

void Binario::transBinarioEmCsv(){

    string nomeArquivoBin = getNomeArquivo();
    
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

void Binario::imprimirTrecho(int posInicial, int posFinal){

    ifstream entrada(nomeArquivoBin, ios::binary);

    if (!entrada) {
        cerr << "Erro ao abrir o arquivo binário." << endl;
        return;
    }

    athletes atleta;

    int tamanhoRegistro = sizeof(athletes);
    entrada.seekg(0, ios::end);
    int totalBytes = entrada.tellg();
    int totalRegistros = totalBytes / tamanhoRegistro;

    if (posInicial < 0 || posFinal > totalRegistros || posInicial > posFinal) {
        cerr << "Intervalo inválido." << endl;
        return;
    }

    for (int i = posInicial; i <= posFinal; i++) {
        entrada.seekg(i * tamanhoRegistro);
        entrada.read(reinterpret_cast<char*>(&atleta), sizeof(athletes));
        cout << atleta << endl;
    }

    entrada.close();
}

bool Binario::alterarEmPosicao(int posicao) {

    fstream binario(nomeArquivoBin, ios::in | ios::out | ios::binary);

    int tamanhoAtual = getTamanho();

    cout << "Total registros: " << tamanhoAtual << endl;

    // chegando na posicao
    athletes alterar;
    binario.seekg(posicao * sizeof(athletes));
    binario.read(reinterpret_cast<char*>(&alterar), sizeof(athletes));

    if (posicao < 0 || posicao > tamanhoAtual){
        cout << "A posição que você digitou é inválida." << endl;
        return false;
    }

    cout << "O que você deseja alterar? " << endl;
    cout << "1. Measure." << endl;
    cout << "2. Quantile." << endl;
    cout << "3. Area." << endl;
    cout << "4. Sex." << endl;
    cout << "5. Age." << endl;
    cout << "6. Geography." << endl;
    cout << "7. Ethnic." << endl;
    cout << "8. Value." << endl;

    int opcao;
    cin >> opcao;
    cin.ignore();

    switch(opcao){

        case 1:
            char novo_measure[10];
            cin.getline(alterar.measure, 10);
        break;
        case 2:
            char novo_quantile[100];
            cin.getline(alterar.quantile, 100);
        break;
        case 3:
            char novo_area[100];
            cin.getline(alterar.area, 50);
        break;
        case 4:
            char novo_sex[20];
            cin.getline(alterar.sex, 20);
        break;
        case 5:
            char novo_age[20];
            cin.getline(alterar.age, 20);
        break;
        case 6:
            char novo_geography[100];
            cin.getline(alterar.geography, 100);
        break;
        case 7:
            char novo_ethnic[100];
            cin.getline(alterar.ethnic, 100);
        break;
        case 8:
            float novo_value;
            cin >> alterar.value;
            cin.ignore();
        break;
    }


    binario.seekp(posicao * sizeof(athletes));
    binario.write(reinterpret_cast<char*>(&alterar), sizeof(athletes));

    return true;

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

void retornoMenu(bool& retorno){
    
	cout << "Deseja retornar para o menu? Por favor responda com 'S' ou 's' para sim e 'N' ou 'n' para não." << endl;
		
	string resposta;
	bool continuar = true;
		
	while (continuar){
		cin >> resposta;

		if (resposta == "S" or resposta == "s"){
			continuar = false;
			retorno = true;
		}
		else if (resposta == "N" or resposta == "n"){
			continuar = false;
			retorno = false;
		}
				
		else {
			continuar = false;
			cout << "Por favor, digite uma resposta válida." << endl;
		}
	}
}

//implementacao do Menu para interação com o usuário
void menuPrincipal(){

    int opcao = 0;
    Binario binario;
    bool retorno = false;

    string nomeArquivoCSV;
    cout << "Qual o nome do arquivo que deseja ler? ";
    cin >> nomeArquivoCSV;
    binario.transCsvEmBinario(nomeArquivoCSV);

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
    cout << "8. Salvar em CSV" << endl;
    cout << "Digite sua opção: ";
    cin >> opcao;
    
        switch(opcao){
            case 1:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                retornoMenu(retorno);
                break;
            case 2:
                int posicao;
                cout << "Fale a posição na qual deseja alterar um dado: ";
                cin >> posicao;
                cout << endl;
                binario.alterarEmPosicao(posicao);
                retornoMenu(retorno);
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
                retornoMenu(retorno);

                break;
            case 4:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                retornoMenu(retorno);
                break;
            case 5:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                retornoMenu(retorno);
                break;
            case 6:
                cout << "Sendo implementado, volte mais tarde."<< endl;
                retornoMenu(retorno);
                break;
            case 7:
                cout << "Saindo..."<< endl;
                return;
                break;
            case 8:
                binario.transBinarioEmCsv();
                break;
        }
        
    }while(opcao != 7);

}

int main(){

    menuPrincipal();

    return 0;
}