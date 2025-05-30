#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

// implementar:
// salvar alteração

using namespace std;

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

    int tamanhoArquivo = 0;
    char nomeArquivoBin[50];

public:
	
	bool testarNomeArquivoBin(char* nomeArquivoBin);
    void transBinarioEmCsv(string nomeArquivoBin, string nomeArquivoCSV);
    void transCsvEmBinario(string nomeArquivoCSV);
    void imprimirTrecho(int posInicial, int posFinal);
    int alterarEmPosicao(int posicao);
    void espiarPosicao(int posicao);
    Binario() = default;
    ~Binario() = default;
    string getNomeArquivo();
    int getTamanho();
    bool inserePosicao(int posicao);

};

// retorna o nome do arquivo binario
string Binario::getNomeArquivo(){
    return nomeArquivoBin;
}

int Binario::getTamanho(){

    ifstream verTamanho(nomeArquivoBin, ios::binary);

    int tamanhoRegistro = sizeof(athletes);
    verTamanho.seekg(0, ios::end);
    int totalBytes = verTamanho.tellg();
	int totalRegistros = totalBytes / tamanhoRegistro;

	return totalRegistros+1;
}

void Binario::espiarPosicao(int posicao){

    string nomeArquivoBin = getNomeArquivo();
    
    ifstream arquivo(nomeArquivoBin, ios::binary);

    if(!arquivo){
        throw runtime_error("Erro, arquivo inexistente.");
    }

    athletes atleta;
    arquivo.seekg(posicao * sizeof(athletes));
    arquivo.read(reinterpret_cast<char*>(&atleta), sizeof(athletes));

    cout << atleta << endl;

    arquivo.close();
    return;
}

bool testarNomeArquivoCSV(char* nomeDoCSV){
	
	int tamanhoNomeArquivo = 0;
	
	while (nomeDoCSV[tamanhoNomeArquivo] != '\0'){
		tamanhoNomeArquivo++;
	}
	
	if (tamanhoNomeArquivo < 5){
		return false;
	}
	
	else if (nomeDoCSV[tamanhoNomeArquivo - 4] == '.' and
        nomeDoCSV[tamanhoNomeArquivo - 3] == 'c' and
        nomeDoCSV[tamanhoNomeArquivo - 2] == 's' and
        nomeDoCSV[tamanhoNomeArquivo - 1] == 'v') {
            
        return true;
        
	}
	
	return false;
	
}

bool Binario::testarNomeArquivoBin(char* nomeArquivoBin){
	
	int tamanhoNomeArquivo = 0;
	
	while (nomeArquivoBin[tamanhoNomeArquivo] != '\0'){
		tamanhoNomeArquivo++;
	}
	
	if (tamanhoNomeArquivo < 5){
		return false;
	}
	
	else if (nomeArquivoBin[tamanhoNomeArquivo - 4] == '.' and
        nomeArquivoBin[tamanhoNomeArquivo - 3] == 'b' and
        nomeArquivoBin[tamanhoNomeArquivo - 2] == 'i' and
        nomeArquivoBin[tamanhoNomeArquivo - 1] == 'n') {
            
        return true;
	}
	return false;
}

void Binario::transCsvEmBinario(string nomeArquivoCSV){

	bool nomeCorreto = false;
    string arquivochamada = nomeArquivoCSV;
    athletes atleta;
    ifstream entrada(nomeArquivoCSV);

    if (!entrada){
        throw runtime_error("Erro, arquivo inexistente.");
    }
    
	while (!nomeCorreto) {
		
		cout << "Qual o nome do arquivo binário no qual deseja guardar os dados? ";
		cin >> nomeArquivoBin;
		
		if (testarNomeArquivoBin(nomeArquivoBin)) {
			
			nomeCorreto = true;
			ofstream saida(nomeArquivoBin, ios::binary);

			if (!saida.is_open()) {
				throw runtime_error("Erro ao abrir arquivo de saída.");
			}

			cout << "Aguarde enquanto os dados são lidos do arquivo CSV..." << endl;

			// Lê o cabeçalho do arquivo CSV
			string cabecalho;
			getline(entrada, cabecalho);
    
			while(entrada){
				// leitura dos dados
				entrada.getline(atleta.measure, 10, ',');
				entrada.getline(atleta.quantile, 80, ',');
				entrada.getline(atleta.area, 50, ',');
				entrada.getline(atleta.sex, 15, ',');
				entrada.getline(atleta.age, 20, ',');
				entrada.getline(atleta.geography, 90, ',');
				entrada.getline(atleta.ethnic, 60, ',');
				entrada.getline(atleta.value, 15);

                if (entrada){
                    saida.write(reinterpret_cast<char*>(&atleta), sizeof(athletes));
                }
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

            // atribuo o tamanho do arquivo
            int quantidade = getTamanho();
			cout << "Quantidade de dados gravados: " << quantidade << endl;

			if (opcao == 1){
				transBinarioEmCsv(nomeArquivoBin, arquivochamada);
			}else if(opcao == 2){
				return;
			}
		}

		else {
			cout << "--------------------------------------------------------------------" << endl;
			cout << "Por favor, não se esqueça de adicionar o '.bin' no final do nome." << endl; 
			}
		}
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
                   << a.ethnic << ',' << a.value;
    }


    arquivoBin.close();
    arquivoCsv.close();

    cout << "Conversão de binário para csv concluída, verifique abrindo o arquivo." << endl;
}

void Binario::imprimirTrecho(int posInicial, int posFinal){

    fstream entrada(nomeArquivoBin, ios::binary);

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
        entrada.read(reinterpret_cast<char*>(&atleta), tamanhoRegistro);
        cout << atleta << endl;
    }

    entrada.close();
}

int Binario::alterarEmPosicao(int posicao) {

    fstream binario(nomeArquivoBin, ios::in | ios::out | ios::binary);

    if(!binario){
        cout << "Erro ao abrir arquivo binário." << endl;
        return -1;
    }

    int tamanhoAtual = getTamanho();

    if (posicao < 0 || posicao > tamanhoAtual){
        cout << "A posição que você digitou é inválida." << endl;
        return -1;
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
    cout << "9. Esquece, decidi não mudar nada." << endl;


    int opcao;
    cin >> opcao;
    cin.ignore();


    if(opcao == 9){
        cout << "Nenhuma alteração feita." << endl;
        return -1;
    }

    athletes atleta;

    binario.seekg(posicao * sizeof(athletes));
    binario.read(reinterpret_cast<char*>(&atleta), sizeof(athletes));

    switch(opcao){
        case 1:
            cout << "Digite o novo 'Measure' do atleta: " << endl;
            cin.getline(atleta.measure, sizeof(atleta.measure));
        break;
        case 2:
            cout << "Digite o novo 'Quantile' do atleta: " << endl;
            cin.getline(atleta.quantile, sizeof(atleta.quantile));
        break;
        case 3:
            cout << "Digite a nova 'Area' do atleta: " << endl;
            cin.getline(atleta.area, sizeof(atleta.area));                
        break;
        case 4:
            cout << "Digite o novo 'Sex' do atleta: " << endl;
            cin.getline(atleta.sex, sizeof(atleta.sex));              
        break;
        case 5:
            cout << "Digite o novo 'Age' do atleta: " << endl;
            cin.getline(atleta.age, sizeof(atleta.age));                
        break;
        case 6:
            cout << "Digite o novo 'Geography' do atleta: " << endl;
            cin.getline(atleta.geography, sizeof(atleta.geography));
        break;
        case 7:
            cout << "Digite o novo 'Ethnic' do atleta: " << endl;
            cin.getline(atleta.ethnic, sizeof(atleta.ethnic));
        break;
        case 8:
            cout << "Digite o novo 'Value' do atleta: " << endl;
            float novo_value;
            cin >> atleta.value;
            cin.ignore();
        default:
            cout <<"Opção inválida. " << endl;
            return -1;
        }

    binario.seekp(posicao * sizeof(athletes));
    binario.write(reinterpret_cast<char*>(&atleta), sizeof(athletes));
    binario.close();
    cout << "Registro atualizado com sucesso." << endl;
    return posicao;

}

bool Binario::inserePosicao(int posicao){

    ifstream binario(nomeArquivoBin, ios::binary);
    int tamanhoAtual = getTamanho();

    if (posicao < 0 || posicao > tamanhoAtual){
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

   ofstream novoBinario("arquivoTemp.bin", ios::binary);

   if (!novoBinario) {
        cerr << "Erro ao criar o arquivo binário." << endl;
        return false;
    }

    athletes atletasLidos;
    int posicoesLidas = 0;

    while (binario.read(reinterpret_cast<char*>(&atletasLidos), sizeof(athletes))) {
        
        if (posicoesLidas == posicao) {
            novoBinario.write(reinterpret_cast<char*>(&atleta_novo), sizeof(athletes));
        }
        else {
            novoBinario.write(reinterpret_cast<char*>(&atletasLidos), sizeof(athletes));
            posicoesLidas++;
        }
    }
    
    if (posicao == tamanhoAtual) {
		novoBinario.write(reinterpret_cast<char*>(&atletasLidos), sizeof(athletes));
	}
		

    tamanhoArquivo++;
    binario.close();
    novoBinario.close();
    
    remove(nomeArquivoBin);
    rename("arquivoTemp.bin", nomeArquivoBin);

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
    int tamanhoMostrar = binario.getTamanho();

    string nomeArquivoCSV;
    cout << "Qual o nome do arquivo que deseja ler? ";
    cin >> nomeArquivoCSV;
    binario.transCsvEmBinario(nomeArquivoCSV);
    string nomeArquivoBin = binario.getNomeArquivo();

    do{
        cout << endl;
        cout << "Bem-vindo ao sistema de gerenciamento de atletas!" << endl;
        cout << "Escolha uma opção:" << endl;
        cout << "1. Inserir atleta" << endl;
        cout << "2. Alterar atleta" << endl;
        cout << "3. Imprimir trecho" << endl;
        cout << "4. Buscar atleta" << endl;
        cout << "5. Excluir atleta" << endl;
        cout << "6. Transformar em csv." << endl;
        cout << "7. Sair" << endl;
        cout << "Digite sua opção: ";
        cin >> opcao;
    
        switch(opcao){
            case 1:

                int posicaoInserir;
                cout << "Digite a posição na qual você deseja inserir um dado: ";
                cin >> posicaoInserir;
                cout << endl;
                
                if (binario.inserePosicao(posicaoInserir)) {
					cout << "O atleta foi inserido na posição " << posicaoInserir << "." << endl;
				}

                retornoMenu(retorno);
                break;
            case 2:
                int posicao;
                cout << "Digite a posição na qual deseja alterar um dado: ";
                cin >> posicao;
                cout << endl;
                if(binario.alterarEmPosicao(posicao) >= 0){

                    cout << "Você deseja ver como seu registro está atualmente?" << endl;
                    cout << "1. Sim." << endl;
                    cout << "2. Não." << endl;
                    int novaOpcao;
                    cin >> novaOpcao;
                    if(novaOpcao == 1){
                        binario.espiarPosicao(posicao);
                    }
                }
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
                binario.transBinarioEmCsv(nomeArquivoBin, nomeArquivoCSV);
                retornoMenu(retorno);
                break;
            case 7:
                cout << "Saindo..."<< endl;
                return;
                break;
        }
        
    }while(retorno && opcao != 7);
}

int main(){

    menuPrincipal();

    return 0;
}