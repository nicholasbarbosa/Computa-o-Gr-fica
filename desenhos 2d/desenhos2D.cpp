#include<iostream>
#include<cstring>
#include<vector>
#include<ctype.h>
#include<cstdio>
#include<stdio.h>
#include<fstream>
#include<cmath>

using namespace std;

void saida(const int altura, const int largura, const vector<vector<vector<int>>> & tela,int argc, char *argv[]){
    ofstream fout;
    fout.open(argv[1]); // Cria arquivo para gravação

    //grava no arquivo
    fout << "P3" << endl;
    fout << largura << " " << altura << endl;
    fout << 255 << endl;

    for(int i = 0; i < altura; i ++){
        for(int j = 0; j < largura; j ++)
            for(int k = 0; k < 3; k ++)
                fout << tela[i][j][k] << " ";

        fout << endl;
    }

    if(fout.is_open()) fout.close();    //fecha o arquivo
}

void pixel(vector<vector<vector<int>>> & tela, int y, int x, int r, int g, int b, const int altura, const int largura){
    y = (altura - 1) - y; //para "mudar a origem" do plano

    if((y >= 0 && y < altura) && (x >= 0 && x < largura)){   //se tentar setar uma posição inválida nao vai funcionar
        //seta a cor do pixel
        tela[y][x][0] = r;
        tela[y][x][1] = g;
        tela[y][x][2] = b;
    }
}

void segmentosimples(vector<vector<vector<int>>> & tela, int y1, int x1, int y2, int x2, int r, int g, int b, const int altura, const int largura){
    int dx = x2 - x1;   //variação de x
    int dy = y2 - y1;   //variação de y

    //abs() retorna o modulo do argumento
    if(abs(dy) > abs(dx)){ //se a reta tiver um angulo menor ou igual a 45
        double m = (double)dx/dy;   //coeficiente angular da reta
        double c = x1 - y1*m;       //constante da equação da reta

        do{
            pixel(tela, y1, (int)(y1*m + c + 0.5), r, g, b, altura, largura);  //vai pintando cada ponto da reta
            if(dy > 0) y1 ++;   //incrementa a parte variável da equação
            else y1 --;         //se começar de tras pra frente
        }while(y1 != y2);

        pixel(tela, y1, ceil(y1*m + c), r, g, b, altura, largura);  //pinta o ultimo pixel
    }
    else if(dx != 0){
        double m = (double)dy/dx;
        double c = y1 - x1*m;

        do{
            pixel(tela, ceil(x1*m + c), x1, r, g, b, altura,largura);
            if(dx > 0) x1 ++;
            else x1 --;
        }while(x1 != x2);

        pixel(tela, ceil(x1*m + c), x1, r, g, b, altura,largura);
    }
}
void segmento(vector<vector<vector<int>>> & tela, int y1, int x1, int y2, int x2, int r, int g, int b, const int altura, const int largura){
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    if((y1 > y2 && x2 > x1 && dx > dy) || (y2 > y1 && x2 < x1  && dx > dy)){    //decrescente com o angulo baixo ou decrescente com angulo baixo e de tras pra frente
        
        if(y2 > y1 && x2 < x1 && dx > dy){
            swap(x1,x2);
            swap(y1,y2);
        }
        int p = 2*dy - dx;  //o predicado eh referente à distancia da reta ao pixel a ser pintado

        pixel(tela, y1, x1, r, g, b, altura, largura);   //pinta o primeiro pixel

        while(x1 != x2){
            x1 ++;
            if(p < 0){      //se menor que zero, apenas incrementa o predicado  
                p += 2*dy;  //incrementa o predicado
            }else{          //se for maior ou igual a zero, pinta o pixel de baixo, decrementa y 
                y1 --;
                p += 2*dy - 2*dx;
            }

            pixel(tela, y1, x1, r, g, b, altura, largura); //pinta o pixel de acordo com o novo predicado
        }
    }
    else if((y1 > y2 && x2 > x1) || (y2 > y1 && x2 < x1)){    //decrescente ou decrescente de tras pra frente
        if(y2 > y1 && x2 < x1){
            swap(x1,x2);
            swap(y1,y2);
        }
        int p = 2*dx - dy; 

        pixel(tela, y1, x1, r, g, b, altura, largura);   //printa o primeiro pixel

        while(y1 != y2){
            y1 --;
            if(p < 0){      //se for menor que zero, pinta o pixel de baixo, ou seja, so decremento o y
                p += 2*dx;  //incrementa o predicado
            }else{          //se for maior ou igual a zero, pinta o pixel de baixo, incrementa  x
                x1 ++;
                p += 2*dx - 2*dy;
            }
            pixel(tela, y1, x1, r, g, b, altura, largura);
        }
    }

    else if(y1 == y2 && x1 != x2){  //retas horizontais
        int p = 2*dy - dx; 
        if(x1 > x2){    //se começar de tras pra frente
            swap(x1,x2);
            swap(y1,y2);
        }

        pixel(tela, y1, x1, r, g, b, altura, largura);   //printa o primeiro pixel

        while(x1 != x2){
            x1 ++;
            if(p < 0){      //vai incrementar apenas o x
                p += 2*dy;  
            }else{          
                y1 ++;
                p += 2*dy - 2*dx;
            }
            pixel(tela, y1, x1, r, g, b, altura, largura);
        }
    }
    else if(dy < dx){   //quando o angulo eh muito pequeno
        int p = 2*dy - dx; 
        if(x1 > x2){    //se começar de tras pra frente
            swap(x1,x2);
            swap(y1,y2);
        }

        pixel(tela, y1, x1, r, g, b, altura, largura);   //printa o primeiro pixel

        while(x1 != x2){
            x1 ++;
            if(p < 0){      
                p += 2*dy;  
            }else{         
                y1 ++;
                p += 2*dy - 2*dx;
            }
            pixel(tela, y1, x1, r, g, b, altura, largura);
        }
    }
    else{   //retas
        int p = 2*dx - dy; 
        if(y1 > y2){    //se começar de tras pra frente
            swap(x1,x2);
            swap(y1,y2);
        }

        pixel(tela, y1, x1, r, g, b, altura, largura);   //printa o primeiro pixel

        while(y1 != y2){
            y1 ++;
            if(p < 0){      //se for menor que zero, pinta o pixel de baixo, ou seja, so incrementa y
                p += 2*dx;  //incrementa o predicado
            }else{          //se for maior ou igual a zero, pinta o pixel de baixo, incrementa y e x
                x1 ++;
                p += 2*dx - 2*dy;
            }
            pixel(tela, y1, x1, r, g, b, altura, largura);
        }
    }
}

void circlePoints(vector<vector<vector<int>>> & tela, int xc, int yc, int x, int y, int raio, int r, int g, int b, const int altura, const int largura){
    if(y == 0){ //90graus
        pixel(tela, yc, xc + x, r, g, b, altura, largura);
        pixel(tela, yc, xc - x, r, g, b, altura, largura);
        pixel(tela, yc + x, xc, r, g, b, altura, largura);
        pixel(tela, yc - x, xc, r, g, b, altura, largura);
    }
    else{
        if(x == y){ //45 graus
            pixel(tela, yc + y, xc + x, r, g, b, altura, largura);
            pixel(tela, yc - y, xc + x, r, g, b, altura, largura);
            pixel(tela, yc + y, xc - x, r, g, b, altura, largura);
            pixel(tela, yc - y, xc - x, r, g, b, altura, largura);
        }

        if(x > y){  //restantes
            pixel(tela, yc + y, xc + x, r, g, b, altura, largura);
            pixel(tela, yc - y, xc + x, r, g, b, altura, largura);
            pixel(tela, yc + y, xc - x, r, g, b, altura, largura);
            pixel(tela, yc - y, xc - x, r, g, b, altura, largura);
            pixel(tela, yc + x, xc + y, r, g, b, altura, largura);
            pixel(tela, yc - x, xc + y, r, g, b, altura, largura);
            pixel(tela, yc + x, xc - y, r, g, b, altura, largura);
            pixel(tela, yc - x, xc - y, r, g, b, altura, largura);
        }
    }
}

void circulo(vector<vector<vector<int>>> & tela, int yc, int xc, int raio, int r, int g, int b, const int altura, const int largura){
    double p = (5 - raio*4)/4;  //o predicado eh pra ver se o ponto do meio entre os pixels esta fora ou dentro do circulo
    int x = raio;
    int y = 0;

    circlePoints(tela, xc, yc, x, y, raio, r, g, b, altura, largura);   //seta o pixel e seus espelhamentos

    while(x > y){   //vai incrementado o predicado e alterando os pontos até completar percorrer todo o circulo
        y++;
        if (p < 0) {
            p += 2*y + 1;
        } else {
            x--;
            p += 2*(y - x) + 1;
        }
        circlePoints(tela, xc, yc, x, y, raio, r, g, b, altura, largura);
    }
}

void retangulo(vector<vector<vector<int>>> & tela, int y1, int x1, int y2, int x2, int r, int g, int b, const int altura, const int largura){
    //4 retas para formar o retangulo
    segmentosimples(tela, y1, x1, y1, x2, r, g, b, altura, largura);
    segmentosimples(tela, y1, x2, y2, x2, r, g, b, altura, largura);
    segmentosimples(tela, y2, x2, y2, x1, r, g, b, altura, largura);
    segmentosimples(tela, y2, x1, y1, x1, r, g, b, altura, largura);

}

void polyline(vector<vector<vector<int>>> & tela, int N, vector<pair<int,int>> points, int r, int g, int b, const int altura, const int largura){
    //um vetor de pair para guardar todos os N pontos. cada pair é um ponto.
    for(int i = 0; i < N - 1; i ++){
        segmento(tela, points[i].first, points[i].second, points[i + 1].first, points[i + 1].second, r, g, b, altura, largura);
    }
}

void preencher4_2(vector<vector<vector<int>>> & tela, int y, int x, vector<int> & old, int r, int g, int b, const int altura, const int largura){
    if(y < 0 || y >= largura) return;   //para n pintar fora do tamanho da matriz
    if(x < 0 || x >= altura) return;

    if(old == tela[(altura - 1) - y][x]){
        pixel(tela,y,x,r,g,b, altura, largura);
        //chama recursivamente para os vertices adjacentes
        preencher4_2(tela, y + 1, x, old, r, g, b, altura, largura);
        preencher4_2(tela, y - 1, x, old, r, g, b, altura, largura);
        preencher4_2(tela, y, x - 1, old, r, g, b, altura, largura);
        preencher4_2(tela, y, x + 1, old, r, g, b, altura, largura);
    }
}

void preencher4(vector<vector<vector<int>>> & tela, int y, int x, int r, int g, int b, const int altura, const int largura){
    vector<int> old = tela[(altura - 1) - y][x];    //na hora de setar o pixel, faço essa operação para acertar a origem do plano
                                                    //por isso preciso fazer isso para pegar o pixel correto

    preencher4_2(tela,y, x, old,r,g,b,altura,largura);
}

void preencher8_2(vector<vector<vector<int>>> & tela, int y, int x, vector<int> & old, int r, int g, int b, const int altura, const int largura){
    if(y < 0 || y >= largura) return;
    if(x < 0 || x >= altura) return;

    if(old == tela[(altura - 1) - y][x]){
        pixel(tela,y,x,r,g,b, altura, largura);
        preencher8_2(tela, y + 1, x, old, r, g, b, altura, largura);
        preencher8_2(tela, y - 1, x, old, r, g, b, altura, largura);
        preencher8_2(tela, y, x - 1, old, r, g, b, altura, largura);
        preencher8_2(tela, y, x + 1, old, r, g, b, altura, largura);

        preencher8_2(tela, y + 1, x + 1, old, r, g, b, altura, largura);
        preencher8_2(tela, y + 1, x - 1, old, r, g, b, altura, largura);
        preencher8_2(tela, y + 1, x + 1, old, r, g, b, altura, largura);
        preencher8_2(tela, y - 1, x + 1, old, r, g, b, altura, largura);
    }
}

void preencher8(vector<vector<vector<int>>> & tela, int y, int x, int r, int g, int b, const int altura, const int largura){
    vector<int> old = tela[(altura - 1) - y][x];    //na hora de setar o pixel, faço essa operação para acertar a origem do plano
                                                    //por isso preciso fazer isso para pegar o pixel correto

    preencher8_2(tela,y, x, old,r,g,b,altura,largura);
}

int main(int argc, char *argv[]){
    int altura, largura;
    int y1, x1, y2, x2, yx, xc, raio, r, g, b; //auxiliares
    string instrucao;

    cin >> largura >> altura;
   
    vector<vector<vector<int>>> tela(altura, vector<vector<int>>(largura, vector<int>(3)));

    for(int i = 0; i < altura; i ++)
        for(int j = 0; j < largura; j ++)
            for(int k = 0; k < 3; k ++)
                tela[i][j][k] = 255;

    //le o arquivo de entrada
    while(cin >> instrucao){
        //se o que foi lido for algumdas das funções, lê os parametros e chama a função
        if(instrucao == "segmentosimples"){
            cin >> y1 >> x1 >> y2 >> x2 >> r >> g >> b;

            segmentosimples(tela, y1, x1, y2, x2, r, g, b, altura, largura);
        }
        else if(instrucao == "segmento"){
            cin >> y1 >> x1 >> y2 >> x2 >> r >> g >> b;
        
            segmento(tela, y1, x1, y2, x2, r, g, b, altura, largura);
        }
        else if(instrucao == "circulo"){
            cin >> y1 >> x1 >> raio >> r >> g >> b;
            
            circulo(tela, y1, x1, raio, r, g, b, altura, largura);
        }
        else if(instrucao == "retangulo"){
            cin >> y1 >> x1 >> y2 >> x2 >> r >> g >> b;

            retangulo(tela, y1, x1, y2, x2, r, g, b, altura, largura);
        }
        else if(instrucao == "pixel"){
            cin >> y1 >> x1 >> r >> g >> b;

            pixel(tela, y1, x1, r, g, b, altura, largura);
        }
        else if(instrucao == "polyline"){
            int N;
            vector<pair<int,int>> points;
            pair<int,int> coordenada;
            
            cin >> N;

            for(int i = 0; i < N; i ++){
                cin >> y1 >> x1;
                coordenada = make_pair(y1,x1);

                points.push_back(coordenada);
            }
            cin >> r >> g >> b;
            polyline(tela, N, points, r, g, b, altura, largura);
        }
        else if(instrucao == "preencher4"){
            cin >> y1 >> x1 >> r >> g >> b;

            preencher4(tela, y1, x1, r, g, b, altura, largura);
        }
        else if(instrucao == "preencher8"){
            cin >> y1 >> x1 >> r >> g >> b;

            preencher8(tela, y1, x1, r, g, b, altura, largura);
        }
        
    }
    
    
    saida(altura, largura, tela, argc, argv);   

    return 0;
}