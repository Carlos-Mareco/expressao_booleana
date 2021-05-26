/*
 * @file expressao_booleana.cpp
 * @author Carlos Henrique Barreto Mareco
 * @date 19 Mai 2021
 * @brief Imprime a expressão booleana de acordo com a saída da tabela verdade em "input.txt"
 */
#include <iostream>
#include <cmath> //Necessário para log2(n) e pow(2, n)
#include <fstream> //Necessário para ler o arquivo de texto

using namespace std;

string lerSaida(string nomeArquivo);
string expressaoBooleana(string saida, int n_entradas);
string convDecToBin(int valor, int tam);

int main(){
    string saida = lerSaida("input.txt"); //ler o arquivo de nome "input.txt" e retorna uma string
    int n_entradas = log2(saida.length()); // n = log2(x), onde x é a quant. de saídas e n é a quant. de entradas
    string expressao = expressaoBooleana(saida, n_entradas); //Guarda a expressão booleana
    
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
//Gerador de expressão booleana
string expressaoBooleana(string saida, int n_entradas){
    int linhas = pow(2, n_entradas); //Gera 2^n linhas válidas, onde n é a quantidade de entradas
    string cadeia, expressao;
    string variaveis = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //Variáveis disponíveis para entradas
    bool sinalOr = false; //Sinalizador para adicionar o sinal +
    
    for(int i = 0; i < linhas; i++){
        if(saida[i] == '1'){
            if(sinalOr) //Não adiciona o sinal + no primeiro termo
                expressao += " + ";
            sinalOr = true; //Adiciona o sinal antes do próximo termo
            // O valor de i é um número que convertido em binário fornece os valores de cada entrada.
            // Exemplo: Se a saída for '1' quando i = 3 e a quant. de entradas for 3, então as entradas são 0, 1 e 1
            cadeia = convDecToBin(i, n_entradas); 
            //Ler as entradas de cada variável de trás para frente, visto que os bits estão invertidos em 'cadeia'
            //Exemplo: Quando i = 4, n_entradas = 3, então cadeia será 001 ao invés de 100
            for(int j = n_entradas - 1; j >= 0; j--){
                expressao += variaveis[n_entradas - j -1]; //Imprime uma variável de cada posição de 'variáveis'
                if(cadeia[j] == '0') // Se a entrada for 0, então ele nega a variável da entrada
                    expressao += "'";
            }
        }
    }
    return expressao;
}
//Converte decimal para binário e retorna uma string INVERTIDA com uma cadeia de bits com tamanho 'tam'
string convDecToBin(int valor, int tam){
    string bin;
    for(int i = 0; i < tam; i++){
        bin += to_string(valor%2);
        valor >>= 1;
    }
    return bin;
}
