#include "HeapFibonacci.h"
#include <vector>
#include <iostream>
using namespace std;

int main(){
		vector<No*> listadenos1;
		vector<No*> listadenos2;
		HeapFibonacci h1=CriarHeap();
		HeapFibonacci h2=CriarHeap();
		vector<No*> listadenos3;
		HeapFibonacci h3=Uniao(h1,h2);
		cout<<h3.n<<endl;
		h1=CriarHeap();
		listadenos1.push_back(Inserir(h1,13));
		listadenos1.push_back(Inserir(h1,10));
		listadenos1.push_back(Inserir(h1,8));
		listadenos1.push_back(Inserir(h1,17));
		listadenos1.push_back(Inserir(h1,12));
		Visualizacao(listadenos1,h1);
		No* minimo=Minimo(h1);
		VisualizacaoMinimo(minimo);
		listadenos3.push_back(Inserir(h3,8));
		listadenos3.push_back(Inserir(h3,24));
		listadenos3.push_back(Inserir(h3,17));
		listadenos3.push_back(Inserir(h3,22));
		listadenos3.push_back(Inserir(h3,21));
		Visualizacao(listadenos3,h3);
		h3=Uniao(h1,h3);
		listadenos3.insert(begin(listadenos3),begin(listadenos1),end(listadenos1));
		Visualizacao(listadenos3,h3);
		No* temp=ExtrairMinimo(h3);
		cout<<temp->chave<<endl;
		int j=AcharNo(listadenos3,temp);
		vector<No*>::iterator it=listadenos3.begin();
		advance(it,j);
		listadenos3.erase(it);
		Visualizacao(listadenos3,h3);
		DiminuirChave(h3,listadenos3[2],12);
		Visualizacao(listadenos3,h3);
		DiminuirChave(h3,listadenos3[8],9);
		Visualizacao(listadenos3,h3);
		temp=Remover(h3,listadenos3[1]);
		j=AcharNo(listadenos3,temp);
		it=listadenos3.begin();
		advance(it,j);
		listadenos3.erase(it);
		Visualizacao(listadenos3,h3);
}
