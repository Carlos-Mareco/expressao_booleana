#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

string lerSaida(string nomeArquivo);
string expressaoBooleana(string saida, int n_entradas);
string convDecToBin(int valor, int tam);

int main(){
    string saida = lerSaida("input.txt");
    int n_entradas = log2(saida.length());
    string expressao = expressaoBooleana(saida, n_entradas);
    
    cout << expressao << endl;
    return 0;
}
//Ler o arquivo de texto e retorna uma cadeia do tipo string
string lerSaida(string nomeArquivo){
    string cadeiaSaida, aux;
    ifstream arquivo;

    arquivo.open(nomeArquivo);
    if(arquivo.is_open()){
        while (!arquivo.eof()){
            arquivo >> aux;
            cadeiaSaida += aux;
        }
        arquivo.close();
    } else {
        cout << "ERRO: Não foi possível abrir o arquivo \"" << nomeArquivo << "\".";
        getchar();
    }
    return cadeiaSaida;
}
string expressaoBooleana(string saida, int n_entradas){
    int linhas = pow(2, n_entradas);
    string cadeia, expressao;
    string variaveis = "ABCDEF"; //Variáveis disponíveis para cada entrada
    bool sinalAnd = false;
    
    for(int i = 0; i < linhas; i++){
        if(saida[i] == '1'){
            if(sinalAnd)
                expressao += " + ";
            sinalAnd = true;
            cadeia = convDecToBin(i, n_entradas);
            for(int j = n_entradas - 1; j >= 0; j--){
                expressao += variaveis[n_entradas - j -1];
                if(cadeia[j] == '0')
                    expressao += "'";
            }
        }
    }
    return expressao;
}
//COnverte decimal para binário
string convDecToBin(int valor, int tam){
    string bin;
    for(int i = 0; i < tam; i++){
        bin += to_string(valor%2);
        valor >>= 1;
    }
    return bin;
}
