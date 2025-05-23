#include <iostream>
#include <fstream>
#include <string>


// implementar:
// altera em posicao
// imprimir trecho
// salvar alteracao

using namespace std;

struct athletes{

    unsigned measure;
    unsigned quantile;
    unsigned area;
    unsigned sex;
    unsigned age;
    unsigned geography;
    unsigned ethnic;
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

class noh {
    friend class lista;
private:
    athletes data;
    noh* proximo;
    noh* anterior;
public:
    noh(athletes d);
};

noh::noh(athletes d){
    data = d;
    proximo = nullptr;
    anterior = nullptr;
}

// lista dinamicamente encadeada
class lista {
private:

    noh* primeiro;
    noh* ultimo;
    int tamanho;
    void removeTodos();

public:

    //construtor e destrutor da lista
    lista();
    ~lista();
    // inserção, remoção e procura 
    bool insereOrdenado(athletes data);
    void insereNoFim(athletes data);
    void insereNoInicio(athletes data);
    bool inserePosicao(int posicao);
    bool alterarEmPosicao(int posicao);
    //metodos adicionais
    bool Vazia();
    void imprime();
    void imprimirTrecho(int posInicial, int posFinal);
    int getTamanho();
    // metodos que envolvem arquivos
    void lerArquivoCSV(string nomeArquivoCSV);
    void gravarEmBin();
};

// construtor e destrutor da classe
lista::lista(){

    ultimo = nullptr;
    primeiro = nullptr;
    tamanho = 0;
}

lista::~lista(){
    removeTodos();
}

// remove todos os elementos da lista
void lista::removeTodos(){
	
	noh* aux = primeiro;
	
	while (aux != NULL) {
		noh* temp = aux;
		aux = aux->proximo;
		delete temp;
	}
	
	primeiro = NULL;
	ultimo = NULL;
	tamanho = 0;
	
}

// verificacao se esta vazia
bool lista::Vazia(){
    return tamanho == 0; 
}

void lista::insereNoInicio(athletes data) {

    noh* novo = new noh(data);

    if (Vazia()) {
        primeiro = novo;
        ultimo = novo;
    } else {
        novo->proximo = primeiro;
        primeiro->anterior = novo;
        primeiro = novo;
    }

    tamanho++;
}

void lista::insereNoFim(athletes data) {

    noh* novo = new noh(data);

    if (Vazia()){
        primeiro = novo;
        ultimo = novo;
    } else{
        ultimo->proximo = novo;
        novo->anterior = ultimo;
        ultimo = novo;
    }

    tamanho++;
}

bool lista::insereOrdenado(athletes data){
    
    noh* novo = new noh(data);

   if(Vazia()) {

	    primeiro = novo;
	    ultimo = novo;
	    tamanho = 1;
        return true;
	}

    if(novo->data.value < primeiro->data.value){
        insereNoInicio(data);
        return true;
    }else if(novo-> data.value > ultimo->data.value){
        insereNoFim(data);
        return true;
    }
    else {
        noh* aux = primeiro;

        while (aux != nullptr) {

            if(novo->data.value < aux->data.value) {
                novo->proximo = aux;
                novo->anterior = aux->anterior;

                if (aux->anterior != nullptr){
                    aux->anterior->proximo = novo;
                }
                else{
                    primeiro = novo; // novo o primeiro da lista
                }
                aux->anterior = novo;
                tamanho++;
                return true;
            }
            aux = aux->proximo;
        }
    }

    return false;
}

void lista::lerArquivoCSV(string nomeArquivoCSV){

    athletes atleta;

    ifstream entrada(nomeArquivoCSV);

    if (!entrada){
        throw runtime_error("Erro, arquivo inexistente.");
    }

    string cabecalho;
    getline(entrada, cabecalho);
    char lixo;
    
    while(entrada >> atleta.measure){
        // leitura dos dados
        entrada >> lixo >> atleta.quantile >> lixo >> atleta.area >> lixo >> atleta.sex >> lixo >> atleta.age >> lixo >> atleta.geography >> lixo
                >> atleta.ethnic >> lixo >> atleta.value;

        insereOrdenado(atleta);

    }

    entrada.close();

    cout << "Arquivo lido com sucesso." << endl;
    gravarEmBin();
    cout << "Dados gravados em binario com sucesso." << endl;
}

void lista::imprime() {
    noh* aux = primeiro;
    while (aux != nullptr) {
        cout << aux->data << endl;
        aux = aux->proximo;
    }
}

bool lista::inserePosicao(int posicao){

    if(posicao > tamanho or posicao < 0){
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


    if (posicao == 0) {
        insereNoInicio(atleta_novo);
        return true;
    }

    
    if (posicao == tamanho) {
        insereNoFim(atleta_novo);
        return true;
    }

    noh* novo = new noh(atleta_novo);
    noh* aux = primeiro;
    int posAux = 0;

    while(posAux < posicao){
        aux = aux->proximo;
        posAux++;
    }

    novo->proximo = aux;
    novo->anterior = aux->anterior;
    aux->anterior->proximo = novo;
    aux->anterior = novo;

    tamanho++;
    return true;

}

void lista::gravarEmBin(){

    cout << "Qual o nome do arquivo que deseja gravar os dados?" << endl;
    string nomeArquivoBin;
    cin >> nomeArquivoBin;

    ofstream saida(nomeArquivoBin, ios::binary);

    if(!saida.is_open()){
        throw runtime_error("Erro ao abrir arquivo de saída.");
    }


    noh* atual = primeiro;
    saida.write(reinterpret_cast<char*>(&tamanho), sizeof(tamanho));

    while (atual != nullptr) {
        saida.write(reinterpret_cast<char*>(&(atual->data)), sizeof(athletes));
        atual = atual->proximo;
    }
    
    saida.close();
    cout << "Gravado em " << nomeArquivoBin << " com sucesso." << endl;

}

bool lista::alterarEmPosicao(int posicao) {
 
    if ( posicao < 0 and posicao > tamanho) {
        cout << "A posição que você digitou é inválida." << endl;
        return false;
    }

    
    noh* auxAtletas = primeiro;

    if (posicao == 0) {
        auxAtletas = primeiro;
    }


    if (posicao == tamanho){

        auxAtletas = ultimo;
    } 
    else{

        int posAux = 0;

        while(posAux < posicao){
            auxAtletas = auxAtletas->proximo;
            posAux++;
        }

    }

    cout << "Qual dado você quer alterar: " << endl;
    cout << "Measure: ";
    cout << auxAtletas->data.measure;
    cout << " Quantile: ";
    cout << auxAtletas->data.quantile;
    cout << " Area: ";
    cout << auxAtletas->data.area;
    cout << " Sex: ";
    cout << auxAtletas->data.sex;
    cout << " Age: ";
    cout << auxAtletas->data.age;
    cout << " Geography: ";
    cout << auxAtletas->data.geography;
    cout << " Ethnic: ";
    cout << auxAtletas->data.ethnic;
    cout << " Value: ";
    cout << auxAtletas->data.value;
    cout << endl;
    cout << "1. Measure " << "2. Quantile " << "3. Area " << "4. Sex " << "5. Age " << "6. Geography " << "7. Ethnic " << "8. Value " << "0. Não Altera" << endl;

    int opcao = 0;
        
    do {

        cin >> opcao;

        switch(opcao) {
            case 1: 
                    
                break;
            case 2: 
                    
                break;
            case 3: 
                    
                break;
            case 4:
                    
                break;
            case 5:
                    
                break;
            case 6:
                    
                break;
            case 7:
                    
                break;
            case 8:
                    
                break;
            case 0:
                cout << "Saindo..." << endl;
                return false;
                break;
            default:
                cout << "Opção inválida." << endl;
        }

        cout << "Mudar mais algum dado? 1. Sim 2. Não" << endl;

        cin >> opcao;

    }while(opcao != 2);

    return true;

}

int main() {

    lista listaaux;
    string nomeArquivoCSV;

    cout << "Qual o nome do arquivo que deseja ler? ";
    cin >> nomeArquivoCSV;
    listaaux.lerArquivoCSV(nomeArquivoCSV);

    int opcao;
    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Imprimir lista\n";
        cout << "2. Inserir em posição\n";
        cout << "3. Alterar em posição\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        switch(opcao) {
            case 1:
                listaaux.imprime();
                break;
            case 2: {
                int pos;
                cout << "Digite a posição para inserir: ";
                cin >> pos;
                listaaux.inserePosicao(pos);
                break;
            }
            case 3: {
                // implementar
                break;
            }
            case 0:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opção inválida." << endl;
        }

    } while(opcao != 0);

    return 0;
}