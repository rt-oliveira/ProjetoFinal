#ifndef HeapBinomial_h
#define HeapBinomial_h

#include <limits>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

typedef struct No{
	long long int chave;
	int ordem;
	struct No* pai;
	struct No* filho_esq;
	struct No* irmao;
	No(int c){
		chave=c;
		pai=NULL;
		filho_esq=NULL;
		irmao=NULL;
		ordem=0;
	}
} No;

typedef struct HeapBinomial{
	No* cabeca;
	HeapBinomial(){
		cabeca=NULL;
	}
} HeapBinomial;

HeapBinomial CriarHeap(){
	return HeapBinomial();
}

No* Minimo(HeapBinomial& H){
    No* y=NULL;
    No* x=H.cabeca;
    long long int nmin=std::numeric_limits<long long>::max();
    while(x!=NULL){
        if (x->chave<nmin){
            nmin=x->chave;
            y=x;
		}
        x=x->irmao;
	}
    return y;
}

void Anexar(No*& c, No*& b,bool primeiro){
    if (primeiro){
        c=b;
        b=b->irmao;
    }else{
        c->irmao=b;
        b=b->irmao;
	}
}

No* Juncao(HeapBinomial& H1,HeapBinomial& H2){
	No* a=H1.cabeca;
	No* b=H2.cabeca;
	No* c=NULL;
	bool primeiro=true;
	No* d=c;
	while(a!=NULL || b!=NULL){
		if (a==NULL)
			Anexar(c,b,primeiro);
		else if (b==NULL)
			Anexar(c,a,primeiro);
		else
			if (a->ordem<b->ordem)
				Anexar(c,a,primeiro);
			else
				Anexar(c,b,primeiro);
		if (primeiro){
			primeiro=!primeiro;
			d=c;
		}
		else
			c=c->irmao;
	}
	return d;
}

void UnirArvoresBinomiais(No* a, No* b){
    a->pai=b;
    a->irmao=b->filho_esq;
    b->filho_esq=a;
    b->ordem++;
}

HeapBinomial Uniao(HeapBinomial& H1,HeapBinomial& H2){
    HeapBinomial H=CriarHeap();
    H.cabeca=Juncao(H1,H2);
    if (H.cabeca==NULL){ // Se verdade, é porque não havia Nós nos dois heaps
        H1.cabeca=NULL;
		H2.cabeca=NULL;
        return H;
	}
    No* prev=NULL;
    No* x=H.cabeca;
    No* next=H.cabeca->irmao;
    while (next!=NULL){ // Enquanto ainda não chegou na última raiz
        if ((x->ordem!=next->ordem) || (next->irmao!=NULL && next->irmao->ordem==x->ordem)){
            prev=x;
            x=next;
        }else{
            if (x->chave<=next->chave){
                x->irmao=next->irmao;
                UnirArvoresBinomiais(next,x);
            }else{
                if (!prev)
                    H.cabeca=next;
                else
                    prev->irmao=next;
                UnirArvoresBinomiais(x,next);
                x=next;
			}
		}
        next=x->irmao;
	}
    H1.cabeca=NULL;
	H2.cabeca=NULL;
    return H;
}

No* Inserir(HeapBinomial& H,int x){
    HeapBinomial H2=CriarHeap();
    No* n=new No(x);
    H2.cabeca=n;
    H=Uniao(H,H2);
    return n;
}

No* PosicaoAnterior(HeapBinomial& H,No*& x){
    No* prev=NULL;
    No* atual=H.cabeca;
    while(atual!=x){
        prev=atual;
        atual=atual->irmao;
	}
    return prev;
}

void ReverterIrmaos(HeapBinomial& H,No*& n){
    No* prev=NULL;
    No* atual=n;
    No* prox=NULL;
    while(atual!=NULL){
        prox=atual->irmao;
        atual->irmao=prev;
        prev=atual;
        atual=prox;
	} 
    H.cabeca=prev;
}

void EliminarPai(No* n){
    n->pai=NULL;
    No* a=n->irmao;
    while(a!=NULL){
        a->pai=NULL;
        a=a->irmao;
	}
}

No* ExtrairMinimo(HeapBinomial& H){
    No* x=Minimo(H);
    No* prev=PosicaoAnterior(H,x);
    if (prev==NULL)
        H.cabeca=x->irmao;
    else
        prev->irmao=x->irmao;
    HeapBinomial H2=CriarHeap();
    EliminarPai(x->filho_esq);
    ReverterIrmaos(H2,x->filho_esq);
    H=Uniao(H,H2);
    x->filho_esq=NULL;
    return x;
}

void DiminuirChave(HeapBinomial& H,No*& x,long long int novo){
    if(novo>x->chave){
        cout<<"A nova chave é maior que a chave atual do nó"<<endl;
		return;
	}
    x->chave=novo;
    No* y=x;
    No* z=x->pai;
    while(z!=NULL && y->chave<z->chave){
        int aux=z->chave;
        z->chave=y->chave;
        y->chave=aux;
        y=z;
        z=y->pai;
	}
}
        
No* Remover(HeapBinomial& H,No*& x){
    DiminuirChave(H,x,std::numeric_limits<long long>::min());
    No* t=ExtrairMinimo(H);
    return t;
}

int AcharNo(vector<No*> t,No* x){
    for(int i=0;i<t.size();i++)
        if(t[i]==x)
            return i;
}

Visualizacao (vector<No*> t,HeapBinomial& H){
	ofstream arq;
	stringstream rankigual;	rankigual.str("");	rankigual.clear();
	string j,k;
	map<No*,string> mapa;
	mapa.clear();
	arq.open("VisualizacaoBin.dot");
	arq<<"digraph { rankdir=TB;\n";
	arq<<"charset=\"Latin1\";\n";
    rankigual<<"{rank=same;";
    for(int i=0;i<t.size();i++){
		mapa[t[i]]=to_string(i);
        arq<<i<<"[label="<<t[i]->chave<<"];\n";
	}
    for(int i=0;i<t.size();i++){
        if(t[i]->filho_esq!=NULL)
            arq<<mapa[t[i]]<<"-> "<<mapa[t[i]->filho_esq]<<"[color=\"blue\"];\n";
		if(t[i]->pai!=NULL)
            arq<<mapa[t[i]]<<"-> "<<mapa[t[i]->pai]<<"[color=\"red\",constraint=false];\n";
        else
            rankigual<<" "<<i<<";";
        if(t[i]->irmao!=NULL){
            j=mapa[t[i]];
			k=mapa[t[i]->irmao];
            arq<<j<<"-> "<<k<<"[color=\"darkgreen\"];";
            arq<<"{rank=same; "<<j<<";"<<k<<"};\n";
		}
	}
    arq<<"Cabeça->"<<mapa[H.cabeca]<<"[color=\"gold\"];\n";
    rankigual<<"}";
    arq<<rankigual.str()<<"\n}";
	arq.close();
	system("dot -Tjpg -Gsize=7,7! VisualizacaoBin.dot -o VisualizacaoBin.jpg");
}

void VisualizacaoMinimo(No* minimo){
	ofstream arqmin;
	arqmin.open("VisuBinMin.dot");
	arqmin<<"digraph{ rankdir=LR; charset=\"Latin1\";\"Nó mínimo\" -> "<<minimo->chave<<"}";
	arqmin.close();
	system("dot -Tjpg -Gsize=7,7! VisuBinMin.dot -o VisuBinMin.jpg");
}

#endif
