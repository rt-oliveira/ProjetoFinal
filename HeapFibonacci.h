#ifndef HeapFibonacci_h
#define HeapFibonacci_h

#include <iostream>
#include <limits>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

typedef struct No{
	long long int  	   chave;
	int  	   ordem;
	bool       marcado;
	struct No* pai;
	struct No* filho;
	struct No* esquerda;
	struct No* direita;
	No(int c){
		chave=c;
		marcado=false;
		pai=NULL;
		filho=NULL;
		esquerda=NULL;
		direita=NULL;
		ordem=0;
	}
} No;

typedef struct HeapFibonacci{
	No* minimo;
	No* primeiro;
	int n;
	HeapFibonacci(){
		minimo=NULL;
		primeiro=NULL;
		n=0;
	}
} HeapFibonacci;

HeapFibonacci CriarHeap(){
	return HeapFibonacci();
}

void AdicionarAntesDoNoMinimo(HeapFibonacci& H,No*& c){
    No* minesq=H.minimo->esquerda;
    c->direita=H.minimo;
    c->esquerda=minesq;
    minesq->direita=c;
    H.minimo->esquerda=c;
}

No* Inserir(HeapFibonacci& H,No*& c,bool NovoNo){
    if(H.minimo==NULL){ // Caso seja o primeiro nó, ele é o primeiro e o mínimo.
        H.minimo=c;
        H.primeiro=c;
        c->esquerda=c;
        c->direita=c;
    }else{ // Caso já tenha algum nó no heap, ele é adicionado imediatamente antes do nó mínimo.
        AdicionarAntesDoNoMinimo(H,c);
        if(H.primeiro==H.minimo)
			H.primeiro=c;
    }
    if (c->chave<H.minimo->chave) // Caso o nó tenha uma chave menor que o mínimo, ele se torna o novo mínimo.
        H.minimo=c;
    if (NovoNo) // Testa se é um novo nó que está sendo adicionado. Caso não seja verdade, é porque o nó ou teve sua chave
               // alterada, e virou raiz, ou então era filho de um nó
               // que era o mínimo, e ao extraí-lo, os filhos viram raízes.
        H.n++;
    return c;
}

No* Inserir(HeapFibonacci& H,int d){
    No* c=new No(d);
    No* ret=Inserir(H,c,true);
    return ret;
}

No* Minimo(HeapFibonacci H){
    return H.minimo;
}

HeapFibonacci Uniao(HeapFibonacci& H1,HeapFibonacci& H2){
    HeapFibonacci H=CriarHeap();
    H.minimo=H1.minimo;
    H.primeiro=H1.primeiro;
    if (H2.minimo!=NULL){ // Caso falso, é porque H2 é um heap vazio, sem nó.
        No* ult1=H.primeiro->esquerda;
        No* ult2=H2.primeiro->esquerda;
        H.primeiro->esquerda=ult2;
        ult2->direita=H.primeiro;
        ult1->direita=H2.primeiro;
        H2.primeiro->esquerda=ult1;
    }
    if (H1.primeiro==NULL)
        H.primeiro=H2.primeiro;
    if (H1.minimo==NULL || (H2.minimo!=NULL && H2.minimo->chave<H1.minimo->chave))
        // Teste para saber quem será, de fato, o nó mínimo da união dos dois heaps.
        H.minimo=H2.minimo;
    H.n=H1.n+H2.n;
    H1.minimo=NULL;
    H2.minimo=NULL;
    return H;
}

void UnirArvoreFibonacci(No*& filho,No*& pai){
    filho->direita->esquerda=filho->esquerda;
    filho->esquerda->direita=filho->direita;
    filho->direita=NULL;
    filho->esquerda=NULL;
    if(pai->filho==NULL){ // Caso o filho seja o primeiro filho do pai.
        pai->filho=filho;
        filho->pai=pai;
        filho->esquerda=filho;
        filho->direita=filho;
    }else{ // Caso o pai já tenha algum filho.
        No* t=pai->filho->direita;
        pai->filho->direita=filho;
        filho->esquerda=pai->filho;
        t->esquerda=filho;
        filho->direita=t;
        filho->pai=pai;
	}
    pai->ordem++;
    filho->marcado=false;
}
    
No* ProximaRaiz(No*& w){
    while(w->pai!=NULL) // O nó pode não ser mais uma raiz, e como itera sobre as raízes, precisa retornar ao nó raiz.
        w=w->pai;
    w=w->direita;
    return w;
}
    
No* DeterminarNovoNoPrimeiro(No*& auxw){
    while(auxw->pai!=NULL) // Loop para saber quem é a primeira raiz do heap, para poder se tornar o H.primeiro.
        auxw=auxw->pai;
    return auxw;
}

void FazerUniaoArvoresFibonacci(No* A[],No*& x,int& d){
    while(A[d]!=NULL){ // Enquanto ele for a 2a ocorrência de nó com aquela ordem já registrada, une com 
                                  // o nó que já foi visto com aquela ordem.
        No* y=A[d];
        if (x->chave>y->chave){
            No* aux=x;
            x=y;
            y=aux;
		}
        UnirArvoreFibonacci(y,x);
        A[d]=NULL;
        d++;
	}
    A[d]=x;
}

void Consolidar(HeapFibonacci& H){
    int p=log2(H.n)+1,d;
    No* A[p];
    for (int i = 0; i < p; ++i)
		A[i]=NULL;	
    No* w=H.primeiro;
    No* ultimo=H.primeiro->esquerda;
    bool continua=true;
    No* auxw=H.primeiro;
    while(continua){ // Enquanto ainda não chegou no último nó
        if (w==ultimo)
            continua=false; // Quando chega no último nó da lista de raízes, para de rodar o laço enquanto.
        No* x=w;
		d=w->ordem;
        FazerUniaoArvoresFibonacci(A,x,d); // Esta função tentará fazer a união de árvores que estão no heap de Fibonacci
        w=ProximaRaiz(w);
	}
    H.primeiro=DeterminarNovoNoPrimeiro(auxw);
    H.minimo=NULL;
    for (int i=0;i<p;i++) // Loop para testar as raízes resultantes, e saber quem é a menor delas.
        if (A[i]!=NULL)
            if (H.minimo==NULL || A[i]->chave<H.minimo->chave)
                H.minimo=A[i];
}
                
void AdicionarFilhosAListaDeRaizes(HeapFibonacci& H,No*& f,int k){
    No* fd=f->direita;
	for(int i=1;i<=k;i++){ // Insere na lista de raízes cada filho do nó mínimo.
        Inserir(H,f,false);
        f->pai=NULL;
        f=fd;
        fd=f->direita;
	}
}
                   
No* ExtrairMinimo(HeapFibonacci& H){
    No* z=H.minimo;
    int k=z->ordem;
    if(z!=NULL){ // Se a árvore tem algum nó.
        No* f=z->filho;
        if(f!=NULL) // É porque ele tem alguma quantidade de filhos, no mínimo 1.
            AdicionarFilhosAListaDeRaizes(H,f,k);
        z->esquerda->direita=z->direita;
        z->direita->esquerda=z->esquerda;
        z->esquerda=NULL;
        if(H.n==1){ 
            H.minimo=NULL;
            H.primeiro=NULL;
        }else{                      // Há mais de uma raiz no heap.
            H.minimo=z->direita;
            if (z==H.primeiro)
                H.primeiro=z->direita;
            Consolidar(H);
		}
        z->direita=NULL;
        H.n--;
	}
    return z;
}

void Cortar(HeapFibonacci& H,No*& filho,No*& pai){
    No* filho_esq=filho->esquerda;
    No* filho_dir=filho->direita;
    if (filho==filho_esq) // Se for filho único.
        pai->filho=NULL;
    else{ // Tem mais filhos, além do que foi removido.
        filho_esq->direita=filho_dir;
        filho_dir->esquerda=filho_esq;
        pai->filho=filho_dir;
	}
    pai->ordem--;
    filho->pai=NULL;
    filho->marcado=false;
    Inserir(H,filho,false);
}

void CorteCascata(HeapFibonacci& H,No*& filho){
    No* pai=filho->pai;
    if (pai!=NULL){ // Nó raiz sempre fica com marcado = falso.
        if (!filho->marcado) // É o primeiro filho que o nó perde, caso verdade.
            filho->marcado=true;
        else{ // Caso verdade, já perdeu 2 filhos, e o nó tem que se tornar
               // raiz, fazendo com que seu pai perca um filho, e isso pode
               // ser recursivo, pois o pai pode perder seu 2o filho, e assim
               // segue.
            Cortar(H,filho,pai);
            CorteCascata(H,pai);
		}
	}
}

void DiminuirChave(HeapFibonacci& H,No*& x,long long int novo){
    if (novo>x->chave){
        cout<<"A nova chave é maior que a chave atual do nó"<<endl;
        return;
	}
    x->chave=novo;
    No* y=x->pai;
    if (y!=NULL && x->chave<y->chave){ // Se o nó não é raiz, e tem chave menor que a de seu pai, ele acaba virando ao diminuir sua chave.
        Cortar(H,x,y);
        CorteCascata(H,y);
	}
    if (x->chave<H.minimo->chave) // Se a chave nova for menor que a mínima, o nó se torna o nó mínimo.
        H.minimo=x;
}
	
No* Remover(HeapFibonacci& H,No*& n){
    DiminuirChave(H,n,std::numeric_limits<long long>::min());
    No* t=ExtrairMinimo(H);
    return t;
}

int AcharNo(vector<No*> t,No* x){
    for(int i=0;i<t.size();i++)
        if(t[i]==x)
            return i;
}

Visualizacao(vector<No*> t,HeapFibonacci H){
	ofstream arq;
	stringstream rankigual;	rankigual.str("");	rankigual.clear();
	map<No*,string> mapa;
	string j,k,r;
	arq.open("VisualizacaoFib.dot");
	arq<<"digraph { rankdir=TB;\n";
	rankigual<<"{rank=same;";
    for(int i=0;i<t.size();i++){
		mapa[t[i]]=to_string(i);
		if(t[i]->marcado)
			arq<<i<<"[label="<<t[i]->chave<<",style=filled,fillcolor=\"cyan\"];\n";
		else
			arq<<i<<"[label="<<t[i]->chave<<"];\n";
	}
	for(int i=0;i<t.size();i++){
		j=mapa[t[i]];
        k=mapa[t[i]->direita];
        arq<<j<<"-> "<<k<<"[color=\"blue\"];{rank=same; "<<j<<";"<<k<<"};\n";
		j=mapa[t[i]];
        k=mapa[t[i]->esquerda];
        arq<<j<<"-> "<<k<<"[color=\"red\"];{rank=same; "<<j<<";"<<k<<"};\n";
        if(t[i]->pai!=NULL){
            j=mapa[t[i]];
            k=mapa[t[i]->pai];
            arq<<j<<"-> "<<k<<"[color=\"orange\",constraint=false];\n";
		}else
            rankigual<<" "<<i<<";";
        if(t[i]->filho!=NULL){
            j=mapa[t[i]];
            k=mapa[t[i]->filho];
            arq<<j<<"-> "<<k<<"[color=\"green\"];\n";
		}
	}
	j=mapa[H.minimo];
    arq<<"Mínimo->"<<j<<"[color=\"turquoise4\"];\n";
    j=mapa[H.primeiro];
    arq<<"Primeiro->"<<j<<"[color=\"gold\"];\n";
    rankigual<<"}";
    arq<<rankigual.str()<<"}";
    arq.close();
    system("dot -Tjpg -Gsize=7,7! VisualizacaoFib.dot -o VisualizacaoFib.jpg");
}

void VisualizacaoMinimo(No* minimo){
	ofstream arqmin;
	arqmin.open("VisuFibMin.dot");
	arqmin<<"digraph{ rankdir=LR;\"Nó mínimo\" -> "<<minimo->chave<<"}";
	arqmin.close();
	system("dot -Tjpg -Gsize=7,7! VisuFibMin.dot -o VisuFibMin.jpg");
}
#endif
