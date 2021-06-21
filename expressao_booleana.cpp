/*
 * @file expressao_booleana.cpp
 * @author Carlos Henrique Barreto Mareco
 * @author Kauê de Magalhães Brandão
 * @date 12 Jun 2021
 * @brief Imprime a expressão booleana simplificada de acordo com a saída da tabela verdade em "input.txt"
 */
#include <iostream>
#include <cmath>
#include <fstream>
#include <bitset>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

/*
	Util: uma classe que contém todas as ferramentas necessárias para executar o programa.
*/
class Util{
public:
	//Ler o arquivo de texto e retorna uma cadeia do tipo string
	string lerSaida(string nomeArquivo){
		string cadeiaSaida, aux;
		ifstream arquivo;

		arquivo.open(nomeArquivo);
		if(arquivo.is_open()){
			while (!arquivo.eof()){
				arquivo >> aux;
				if(aux == "0" || aux == "1")
					cadeiaSaida += aux;
			}
			arquivo.close();
		} else {
			cout << "ERRO: Não foi possível abrir o arquivo \"" << nomeArquivo << "\".";
			getchar();
		}
		return cadeiaSaida;
	}
	//Converte decimal para binário e retorna uma string com uma cadeia de bits com tamanho 'tam'
	string convDecToBin(int tam, int valor){
		string bin;
		string aux = "";
		for(int i = 0; i < tam; i++){
			bin += to_string(valor%2);
			valor >>= 1;
		}
		for(int i = tam-1; i >= 0; i--){
			aux += bin[i];
		}
    return aux;
	}
	//Retorna a quantidade de uns em um binário
	int get1s(string x) {
		int count = 0;
		for(int i = 0; i < x.size(); ++i){
			if(x[i] == '1')
				count++;
		}
		return count;
	}
	//Verifica se duas strings diferenciam em apenas um local ou não
	bool compare(string a, string b) {
		int count = 0;
		for(int i = 0; i < a.length(); i++){
			if(a[i]!=b[i])
				count++;
		}
		return (count == 1);
	}
	//Retorna uma string que no local da diferença entre dois binários tem '-'
	string getDiff(string a, string b) {
		for(int i = 0; i < a.length(); i++){
			if (a[i]!=b[i])
				a[i]='-';
		}
		return a;
	}
	bool checkEmpty(vector< vector< string> > table){
		for (int i = 0; i < table.size(); ++i){
			if (table[i].size()!=0) {
				return false;
			}
		}
		return true;
	}
	//Converte cadeira binária para expressão booleana
	string binToExpressao(string x){ 
		string out = "";
		for (int i = 0; i < x.size(); ++i){
			if (x[i]=='1') {
				char x = 65 + i;
				out += x;
			}
			else if (x[i]=='0') {
				char x = 65 + i;
				out += x;
				out += "'";
			}
		}
		return out;
	}
	//check is a prime implicant satisfies a min term or not
	bool primeIncludes(string imp, string minTerm){
		for (int i = 0; i < imp.size(); ++i){
			if (imp[i]!='-'){
				if(imp[i]!=minTerm[i]){
					return false;
				}
			}
		}
		return true;
	}
	//Retorna o número de variáveis em uma cominação no método de petrick
	int getVar(set<int> comb, vector<string> primeImp){
		int count = 0;
		set<int> :: iterator itr;
		for (itr = comb.begin(); itr != comb.end(); ++itr){
			int imp = *itr;
			for (int i = 0; i < primeImp[imp].size(); ++i){
				if (primeImp[imp][i]!='-')
					count ++;
			}
		}
		return count;
	}
};
class Simplificador{
private:
    string saida;
    Util util;
	vector<int> minInt; // mintermos em decimal
 	vector<string> minBin; // mintermos em binário
 	int nVariaveis; // quantidade de variáveis
 	int nMin;  // quantidade de mintermos
 	vector < vector<string> > table;
 	vector< string > primeImp;
 	vector< set<int> > functions;

public:
	void iniciar() {
		saida = util.lerSaida("input.txt"); //ler o arquivo de nome "input.txt" e retorna uma string
		nVariaveis = log2(saida.length()); // n = log2(x), onde x é a quant. de saídas e n é a quant. de entradas
        int linhas = pow(2, nVariaveis); //Gera 2^n linhas válidas, onde n é a quantidade de entradas;
		nMin = 0;

        for(int i = 0; i < linhas; i++){
            if(saida[i] == '1') {
                minInt.push_back(i);
                minBin.push_back(util.convDecToBin(nVariaveis, i));
				nMin++;
            }
        }
		table = vector< vector<string> >(nVariaveis+1);
	}
	void setPrimeImp() {
		set< string > primeImpTemp;
		createTable();

		while (!util.checkEmpty(table)){
			table = combinePairs(table, primeImpTemp);
		}
		set<string > :: iterator itr;  // Converte set para vector
		 for (itr = primeImpTemp.begin(); itr != primeImpTemp.end(); ++itr){
		 	string x = *itr;
        	primeImp.push_back(x);
    	}
	}
	//Petrick: minimizar as expressões restantes
	void minimizar() {
		bool primeImpChart[primeImp.size()][nMin];

		for (int i = 0; i < primeImp.size(); ++i){
			for (int j = 0; j < nMin; ++j){
				primeImpChart[i][j] = false;
			}
		}
		for (int i = 0; i < primeImp.size(); ++i){
			for (int j = 0; j < nMin; ++j){
				primeImpChart[i][j] = util.primeIncludes(primeImp[i], minBin[j]);
			}
		}
		//Lógica de Petrick
		vector< set<int> > patLogic;
		for (int j = 0; j < nMin; ++j){ // iteração de colunas
			set<int> x;
			for (int i = 0; i < primeImp.size(); ++i){ // iteração de linhas
				if (primeImpChart[i][j] == true) {
					x.insert(i);
				}
			}
			patLogic.push_back(x);
		}
		//Pegar todas as possíveis combinações
		set< set<int> > posComb;
		set<int> prod;
		getPosComb(patLogic, 0, prod, posComb);
		int min = 9999;

		set< set<int> > :: iterator itr1;
		for (itr1 = posComb.begin(); itr1 != posComb.end(); ++itr1){
			set<int> comb = *itr1;
			if (comb.size() < min){
				min = comb.size();
			}
			set<int > :: iterator itr;  
			for (itr = comb.begin(); itr != comb.end(); ++itr){
				int x = *itr;
	    	}
		}
		//Combinações com termos mínimos
		vector< set<int> > minComb;
		set< set<int> > :: iterator itr;
		for (itr = posComb.begin(); itr != posComb.end(); ++itr){
			set<int> comb = *itr;
			if (comb.size() == min) {
				minComb.push_back(comb);
			}
		}

		//Combinações com variáveis mínimas
		min = 9999;
		for (int i = 0; i < minComb.size(); ++i){
			if(util.getVar(minComb[i], primeImp) < min){
				min = util.getVar(minComb[i], primeImp);
			}
		}

		for (int i = 0; i < minComb.size(); ++i){
			if(util.getVar(minComb[i], primeImp) == min){
				functions.push_back(minComb[i]);
			}
		}
	}
	//Mostrar na tela as combinações
	void exibirExpressoes() {
		cout << "1 - Possiveis expressões:" << endl;
		for (int i = 0; i < functions.size(); ++i){
			set<int> function = functions[i]; 
			set<int> :: iterator itr;
			cout << "Expressão " << i+1 << ": ";
			for (itr = function.begin(); itr != function.end(); ++itr){
				int x = *itr;
				cout << util.binToExpressao(primeImp[x]) << " + ";
			}
			cout << "\b\b  \n";
		}
	}
	void getPosComb(vector< set<int> > &patLogic, int level, set<int> prod, set< set<int> > &posComb) {
		if (level == patLogic.size()){
			set<int> x = prod;
			posComb.insert(x);
			return;
		} else{
			set<int > :: iterator itr;  
			for (itr = patLogic[level].begin(); itr != patLogic[level].end(); ++itr){

				int x = *itr;
	        	bool inserted = prod.insert(x).second;
	        	getPosComb(patLogic, level+1, prod, posComb);
	        	if (inserted){
	        		prod.erase(x);
	        	}
	    	}
		}
	}
	vector< vector< string> > combinePairs(vector< vector< string> > table, set<string>& primeImpTemp) {
		bool checked[table.size()][nMin];
		vector< vector< string> > newTable(table.size()-1);

		for (int i = 0; i < table.size() -1; ++i){
			for (int j = 0; j < table[i].size(); ++j){
				checked[i][j] = false;
			}
		}
		for (int i = 0; i < table.size() -1; ++i){
			for (int j = 0; j < table[i].size(); ++j){
				for (int k = 0; k < table[i+1].size(); k++){
					if (util.compare(table[i][j], table[i+1][k])){
						newTable[i].push_back(util.getDiff(table[i][j], table[i+1][k]));
						checked[i][j] = true;
						checked[i+1][k] = true;
					}
				}
			}
		}
		for (int i = 0; i < table.size(); ++i){
			for (int j = 0; j < table[i].size(); ++j){
				if (!checked[i][j]) {
					primeImpTemp.insert(table[i][j]);
				}
			}
		}
		return newTable;
	}
	void createTable() {
		for (int i = 0; i < nMin; ++i){
			int num1s = util.get1s(minBin[i]);
			table[num1s].push_back(minBin[i]);
		}
	}
	void exibirKmap() {
		cout << "2 - Mapa de Karnaugh:" << endl;
		string dados = util.lerSaida("input.txt");
		int count = 0;
		if(nVariaveis == 2) {
			string kmap[2][2];
			for(int i = 0; i < nVariaveis; i++){
				for(int j = 0; j < nVariaveis; j++){
					kmap[i][j] = dados[count];
					count++;
				}
			}
			cout << kmap[0][0]+"\t"+kmap[0][1]+"\n"+kmap[1][0]+"\t"+kmap[1][1] << endl;
		}
		if(nVariaveis == 3){
			string kmap[nVariaveis-1][nVariaveis+1];
			for(int i = 0; i < nVariaveis-1; i++){
				for(int j = 0; j < nVariaveis+1; j++){
					kmap[i][j] = dados[count];
					count++;
				}
			}
			cout << kmap[0][0]+"\t"+kmap[0][1]+"\t"+kmap[0][3]+"\t"+kmap[0][2]+"\n"+kmap[1][0]+"\t"+kmap[1][1]+"\t"+kmap[1][3]+"\t"+kmap[1][2] << endl;
		}
		if(nVariaveis == 4){
			string kmap[nVariaveis][nVariaveis];
			for(int i = 0; i < nVariaveis; i++){
				for(int j = 0; j < nVariaveis; j++){
					kmap[i][j] = dados[count];
					count++;
				}
			}
			cout << kmap[0][0]+"\t"+kmap[0][1]+"\t"+kmap[0][3]+"\t"+kmap[0][2]+"\n"+kmap[1][0]+"\t"+kmap[1][1]+"\t"+kmap[1][3]+"\t"+kmap[1][2]+"\n"+kmap[3][0]+"\t"+kmap[3][1]+"\t"+kmap[3][3]+"\t"+kmap[3][2]+"\n"+kmap[2][0]+"\t"+kmap[2][1]+"\t"+kmap[2][3]+"\t"+kmap[2][2] << endl;
		}
	}
};
int main(){
	Simplificador expressao;

    expressao.iniciar();
	expressao.setPrimeImp();
	expressao.minimizar();
	expressao.exibirExpressoes();
	expressao.exibirKmap();
    return 0;
}