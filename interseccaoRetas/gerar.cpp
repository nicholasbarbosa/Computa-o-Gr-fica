
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <array>
#include <cstdlib>
using namespace std;

/*
Ideia: para cada um dos N segmentos a serem sorteados:
-- sorteia o y1,x1 aproximadamente entre 0 e mxDim
-- sorteia y2 = um valor que estará na posicao y1 + numeroAleatorioEntre(-mxLen e mxLen) 
-- o mesmo para x2
*/
int main(int argc, char **argv) {
	if(argc!=4) {
		cerr << "Erro: use ./gerar N maxDim maxLen" << endl;
		cerr << "Onde N é o número de segmentos" << endl;
		cerr << "maxDim é a largura máxima da bounding-box (quadrada) da região onde os vértices iniciais dos segmentos serão sorteados" << endl;
		cerr << "mxLen é o lado máximo do bounding-box dos segmentos sorteados" << endl;
		exit(1);
	}
	int n = atoi(argv[1]);
	int mxDim = atoi(argv[2]);
	int mxLen = atoi(argv[3]);

	cout << "bruta\n";
	cout << n << endl;
	for(int i=0;i<n;i++) {
		double y1 = (rand()%(10*mxDim))/10.0;
		double x1 = (rand()%(10*mxDim))/10.0;

		//y2,x2 poderao ser sorteados tanto maiores quanto menores do que y1,x1...
		double y2 = y1 + (rand()%2==0?-0.1:0.1)*( (rand()%(10*mxLen)) );  
		double x2 = x1 + (rand()%2==0?-0.1:0.1)*( (rand()%(10*mxLen)) );  
		cout << y1 << " " << x1 << " " << y2 << " " << x2 << "\n";
	}
}
