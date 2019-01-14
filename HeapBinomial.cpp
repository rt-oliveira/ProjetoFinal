#include "HeapBinomial.h"
#include <iostream>
#include <vector>
using namespace std;

int main(){
	vector<No*> listadenos;
	HeapBinomial h1=CriarHeap();
	listadenos.push_back(Inserir(h1,21));
	listadenos.push_back(Inserir(h1,7));
	listadenos.push_back(Inserir(h1,18));
	listadenos.push_back(Inserir(h1,4));
	listadenos.push_back(Inserir(h1,17));
	Visualizacao(listadenos,h1);
	No* mini=Minimo(h1);
	VisualizacaoMinimo(mini);
	vector<No*> listadenos2;
	HeapBinomial h2=CriarHeap();
	listadenos2.push_back(Inserir(h2,20));
	listadenos2.push_back(Inserir(h2,19));
	listadenos2.push_back(Inserir(h2,24));
	listadenos2.push_back(Inserir(h2,10));
	Visualizacao(listadenos2,h2);
	HeapBinomial h3=Uniao(h1,h2);
	vector<No*> listadenos3;
	listadenos3.insert(end(listadenos3),begin(listadenos),end(listadenos));
	listadenos3.insert(end(listadenos3),begin(listadenos2),end(listadenos2));
	Visualizacao(listadenos3,h3);
	mini=ExtrairMinimo(h3);
	cout<<mini->chave<<endl;
	int t=AcharNo(listadenos3,mini);
	vector<No*>::iterator it=listadenos3.begin();
	advance(it,t);
	listadenos3.erase(it);
	Visualizacao(listadenos3,h3);
	DiminuirChave(h3,listadenos3[4],5);
	Visualizacao(listadenos3,h3);
	No* temp=Remover(h3,listadenos3[5]);
	t=AcharNo(listadenos3,temp);
	vector<No*>::iterator it2=listadenos3.begin();
	advance(it2,t);
	listadenos3.erase(it2);
	Visualizacao(listadenos3,h3);
}
