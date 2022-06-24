//Created by: WellingtonBRRS - 06/2022

/*
	Na utilização do programa defina primeiramente o tamanho da memória total do sistema.
	
	Posteriormente escolha o tipo do seu gerenciamento (First, Best, Best, Worst).
	
	Opções do programa:
		1 - Alocar Processos.
		2 - Desalocar Processos.
		3 - Ver a situação geral da memória.

*/

#include <stdio.h>
#include <stdlib.h>

void compactar(int* part,int tamPart){//a logica de compactacao utilizada é: trazes todos os valores para o mais perto possivel do começo, deixando o final limpo
	int cont=0;
	for(int i=0;i<tamPart;i++){//testa se o vetor esta vazio
		if(part[i]!=0){
			cont++;
			break;
		}
	}
	if(cont>0){//se nao estiver vazio entao ele executa um metodo que compacta o vetor (se o vetor estivesse vazio, ficaria em loop infinito)
		for(int i=0;i<tamPart-1;i++){
			if(part[i]==0 && part[i+1]!=0){
				for(int j=i;j<tamPart-1;j++){
					part[j]=part[j+1];
				}
				part[tamPart-1]=0;
			}
		}
	}
}

void alocarMemoria(int* part,int tamPart, int x,int* processo,int tpPos,int* contNext,bool firstORrecurs){//o firstORrecurs é usado para usar a funcao como recursiva, porem, manter ela sem entrar em loop
	int aux=0,pos=0,cont=0,alrAloc=0,alocou=false;
	int posBest,posAux,tamPos=tamPart+1;
	printf("\nAlocando %d bytes na Memoria...\n",x);
	
	
	switch(tpPos){
		case 1:
			for(int i=0;i<tamPart;i++){
				if(part[i]==0 && aux==0){
					aux=1;
					pos=i;
					cont++;
				}else if(part[i]==0 && aux==1){
					cont++;
				}else if(part[i]!=0){
					aux=0;
					pos=0;
					cont=0;
				}
				if(cont==x){
					*processo+=1;
					alocou=true;
					for(int j=0;j<x;j++){
						part[pos]=*processo;
						pos++;
					}
					break;
				}
			}
			if(alocou==false && firstORrecurs==false){
				compactar(part,tamPart);
				alocarMemoria(part, tamPart, x, processo,tpPos,contNext,true);
			}
			break;
		case 2:
			for(int i=*contNext;i<tamPart;i++){//Primeiro é testado se ha lugar livre a partir da ultima alocacao ate o final do vetor
				if(part[i]==0 && aux==0){//se a memoria estiver livre e nao teve memoria anteriormente encontrada no do modo livre
					aux=1;//o auxiliar opera informando que ele ja encontrou uma casa livre, e agora esta em busca das posteriores
					pos=i;//o pos recebe a primeira posicao livre encontrada
					cont++;//e o cont é incrementado para comparar o tamanho necessario com o tamanho livre necessario
				}else if(part[i]==0 && aux==1){//caso esteja livre mas havia memoria livre anteriormente
					cont++;//o cont é incrementado para comparar o tamanho necessario com o tamanho livre necessario
				}else if(part[i]!=0){//se a particao nao estiver livre as variaveis auxiliares sao zeradas
					aux=0;
					pos=0;
					cont=0;
				}
				if(cont==x){//se o cont atingiu o tamanho requerido pela alocacao
					*processo+=1;//o numero do processo é incrementado
					alrAloc=1;
					alocou=true;
					for(int j=0;j<x;j++){//é alocado todos os valores com seu respectivo processo
						part[pos]=*processo;
						pos++;
					}
					*contNext=pos;
					break;
				}
			}
			if(alrAloc==0 && *contNext!=0){//Em segundo é testado se ha lugar livre no vetor a partir do comeco ate a ultima alocacao
				for(int i=0;i<*contNext+1;i++){
					if(part[i]==0 && aux==0){
						aux=1;
						pos=i;
						cont++;
					}else if(part[i]==0 && aux==1){
						cont++;
					}else if(part[i]!=0){
						aux=0;
						pos=0;
						cont=0;
					}
					if(cont==x){
						*processo+=1;
						alrAloc=1;
						alocou=true;
						for(int j=0;j<x;j++){
							part[pos]=*processo;
							pos++;
						}
						*contNext=pos;
						break;
					}
				}
			}
			if(alocou==false && firstORrecurs==false){
				compactar(part,tamPart);
				alocarMemoria(part, tamPart, x, processo,tpPos,contNext,true);
			}
			break;
		case 3:
			for(int i=0;i<tamPart;i++){//Loop durante todo o vetor para encontrar as posicoes validas e guardar respectivas posicoes e valores
				if(part[i]==0 && aux==0){//se a posicao estiver livre e anteriormente estava ocupada
					aux=1;
					pos=i;
					cont++;
				}else if(part[i]==0 && aux==1){//se a posicao esta livre e anteriormente estava livre
					cont++;
				}
				if(i+1>=tamPart && aux==1){//se a proxima posicao é o final do vetor e anteriormente estava livre
					if(cont<tamPos && cont>=x){
						tamPos=cont;
						posBest=pos;
					}	
					aux=0;
					pos=0;
					cont=0;
				}else if(part[i]!=0 && aux==1){//se a posicao esta ocupada e a anterior estava livre
					if(cont<tamPos && cont>=x){
						tamPos=cont;
						posBest=pos;
					}
					aux=0;
					pos=0;
					cont=0;
				}
			}
			//printf("\n\ntamPos: %d, posBest: %d",tamPos,posBest);
			
			if(part[posBest]==0){
				*processo+=1;
				alocou=true;
				for(int j=0;j<x;j++){
					part[posBest]=*processo;//é inserido os processos no vetor seguindo a posicao mais eficiente
					posBest++;
				}
			}else{//se o posBest continuar como 0 (assim como foi instanciado) e a primeira posicao nao for nula, entao significa que nao foi encontrado nenhuma posicao livre
				printf("Nao foi encontrado uma posicao valida");
			}
			if(alocou==false && firstORrecurs==false){
				compactar(part,tamPart);
				alocarMemoria(part, tamPart, x, processo,tpPos,contNext,true);
			}
			break;
		case 4:
			tamPos=0;
			for(int i=0;i<tamPart;i++){//Loop durante todo o vetor para encontrar as posicoes validas e guardar respectivas posicoes e valores
				if(part[i]==0 && aux==0){//se a posicao estiver livre e anteriormente estava ocupada
					aux=1;
					pos=i;
					cont++;
				}else if(part[i]==0 && aux==1){//se a posicao esta livre e anteriormente estava livre
					cont++;
				}
				if(i+1>=tamPart && aux==1){//se a proxima posicao é o final do vetor e anteriormente estava livre
					if(cont>tamPos && cont>=x){
						tamPos=cont;
						posBest=pos;
					}	
					aux=0;
					pos=0;
					cont=0;
				}else if(part[i]!=0 && aux==1){//se a posicao esta ocupada e a anterior estava livre
					if(cont>tamPos && cont>=x){
						tamPos=cont;
						posBest=pos;
					}
					aux=0;
					pos=0;
					cont=0;
				}
			}
			//printf("\n\ntamPos: %d, posBest: %d",tamPos,posBest);
			if(part[posBest]==0){
				*processo+=1;
				alocou=true;
				for(int j=0;j<x;j++){
					part[posBest]=*processo;//é inserido os processos no vetor seguindo a posicao mais eficiente
					posBest++;
				}
			}else{//se o posBest continuar como 0 (assim como foi instanciado) e a primeira posicao nao for nula, entao significa que nao foi encontrado nenhuma posicao livre
				printf("Nao foi encontrado uma posicao valida");
			}
			if(alocou==false && firstORrecurs==false){
				compactar(part,tamPart);
				alocarMemoria(part, tamPart, x, processo,tpPos,contNext,true);
			}
			break;
	}
	
}

void opcao1(int* part,int tamPart,int* proc,int tpPos,int* contNext){
	int x;
	printf("\nInforme o tamanho a ser alocado: ");
	scanf("%d",&x);
	alocarMemoria(part, tamPart, x, proc,tpPos,contNext,false);
}

void desalocarProcesso(int* part,int tamPart, int x){
		printf("\nDesalocando Processo %d...\n",x);
		for(int i=0;i<tamPart;i++){
			if(part[i]==x){
				part[i]=0;
			}
		}
}

void opcao2(int* part,int tamPart){
	int x;
	printf("\nQual processo deseja desalocar? ");
	scanf("%d",&x);
	desalocarProcesso(part,tamPart, x);
}

int tamLivre(int* part,int tamPart){
	int cont=0;
	for(int i=0;i<tamPart;i++){
		if(part[i]==0){
			cont++;
		}
	}
	return cont;
}

int maPartPos(int* part,int tamPart){//Maior particao possivel
	int aux=0,cont=0,maPosEnc=0;
	for(int i=0;i<tamPart;i++){
		if(part[i]==0 && aux==0){
			aux=1;
			cont++;
		}else
		if(part[i]==0 && aux==1){
			cont++;
		}
		if(part[i]!=0 || i+1>=tamPart){
			if(cont>maPosEnc){
				maPosEnc=cont;
			}
			aux=0;
			cont=0;
		}
	}
	return maPosEnc;
}

void situacaoMemoria(int* part,int tamPart){	
	int cont=0,contPos=0,aux=0,contPart=1;
	bool end=false;
	
	printf("\nMemoria:\n");
	printf("Particao - Tam - Situacao\n");
	
	while(end==false){
		cont=0;
		aux=part[contPos];
		for(int i=contPos;i<tamPart;i++){
			if(part[i]!=aux){
				break;
			}else{
				contPos++;
				cont++;
			}
		}
		if(aux==0){
			printf("\n%d          %d     Livre",contPart,cont);
		}else{
			printf("\n%d          %d     Processo: %d",contPart,cont,aux);
		}
		if(contPos>=tamPart){
			end=true;
		}else{
			contPart++;
		}
	}
	
//	for(int i=0;i<tamPart;i++){
//		printf("%d         --       %d\n",i,part[i]);
//	}
	
	printf("\n\n\nMemoria Alocada: %d",tamPart);
	printf("\nMemoria Livre: %d",tamLivre(part,tamPart));
	printf("\n%% memoria Livre: %.2f%%",((tamLivre(part,tamPart)*100.0)/tamPart));
	printf("\nMaior Particao Possivel: %d",maPartPos(part,tamPart));
}

void opcao3(int* part,int tamPart){
	situacaoMemoria(part,tamPart);	
}

int main(){
	bool inExec = 1;
	int tamanhoParticao=0;
	int tipoPosicionamento;
	int opcao;
	int *particao;
	int processo=0;
	int contNext=0;
	
	printf("Digite o tamanho da particao do sistema: ");
	scanf("%d",&tamanhoParticao);
	particao = (int*) malloc(tamanhoParticao * sizeof(int));
	int memoria[tamanhoParticao][2];
	for(int i=0;i<tamanhoParticao;i++){
		for(int j=0;j<2;j++){
			memoria[i][j]=0;
		}
	}	
	for(int i=0;i<tamanhoParticao;i++)
		particao[i]=0;
	
	printf("Digite o tipo de posicionamento: (1- First, 2- Next, 3- Best, 4- Worst) ");
	scanf("%d",&tipoPosicionamento);
	
	while(inExec==1){
		printf("\n\nOpcoes:");
		printf("\n1 - Alocar Processo");
		printf("\n2 - Desalocar Processo");
		printf("\n3 - Situacao da Memoria");
		printf("\n0 - Sair\n");
		scanf("%d",&opcao);
		
		switch(opcao){
			case 1:
				opcao1(particao,tamanhoParticao,&processo,tipoPosicionamento,&contNext);
				break;
			case 2:
				opcao2(particao,tamanhoParticao);
				break;
			case 3:
				opcao3(particao,tamanhoParticao);
				break;
			case 0:
				inExec = 0;
				break;
		}
	}
	
	
	free(particao);
}

