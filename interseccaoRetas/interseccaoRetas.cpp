#include<iostream>
#include<vector>
#include<stdio.h>
#include<algorithm>
#include<stdio.h>
#include<unistd.h>
#include<chrono>
#include<iomanip>


using namespace std;
struct pBox{ //representa as coordenadas da box
/*
    p4--------p3
    |         |
    |         |
    p1--------p2
*/
    pair<double,double> ponto1;
    pair<double,double> ponto2;
    pair<double,double> ponto3;
    pair<double,double> ponto4;
    //ponto1 ao ponto3 = diagonal
};

double vetorial(const pair<double,double> AB[], const pair<double,double> Ax[]){
    //sabemos que um segmento AB = vetor(B) - vetor(A)

    pair<double,double> vetorAB = make_pair(AB[1].first - AB[0].first, AB[1].second - AB[0].second);
    pair<double,double> vetorAx = make_pair(Ax[1].first - Ax[0].first, Ax[1].second - Ax[0].second);

    //o produto vetorial entre 2 vetores coplanares resulta em um vetor perpendicular a eles
    //ou seja, o produto vetorial de 2 vetores saindo da origem resulta em um vetor apenas com a coodenada z (0,0,z)
    //portanto, podemos retornar apenas essa operação
    return vetorAB.first*vetorAx.second - vetorAB.second*vetorAx.first;
}

bool intercepta(const pair<double,double> AB[], const pair<double,double> CD[]){
    //cout << AB.size() << endl;
    //AB é um segmento do ponto A = (x1,y1) e B = (x2,y2)
    //o segmento CD tem o mesmo raciocínio
    //para saber se o segmento AB intercepta o segmento CD, basta verificar se (AB)x(AC), (AB)x(AD) tem valores opostos

    pair<double,double> AC[] {AB[0], CD[0]};
    pair<double,double> AD[] {AB[0], CD[1]};
    pair<double,double> CA[] {CD[0], AB[0]};
    pair<double,double> CB[] {CD[0], AB[1]};
   
    double resp1 = vetorial(AB,AC);
    double resp2 = vetorial(AB,AD);

    if(((resp1 > 0 && resp2 > 0) || (resp1 < 0 && resp2 < 0))) return false;

    double resp3 = vetorial(CD,CA);
    double resp4 = vetorial(CD,CB);

    //se AB x AC tiver direção contraria de AB x AD
    //e CD x CA tiver direção contraria de CD x CB
    //as retas se interceptam
    if(((resp3 > 0 && resp4 > 0) || (resp3 < 0 && resp4 < 0))) return false;
    return true;
}

long long int bruta(vector<pair<double,double>> &segments){
    long long int intersecoes = 0;    // quantas interseções
    long long int comparacoes = 0;    // quantas possíveis interseções
    long long int cont = 0;           // quantas comparações
    pair<double,double> AB[2]; //vetores auxiliares que vao repersentar os segmentos
    pair<double,double> CD[2];

    pair<double,double> ponto_1; //representam o par ordenado de um ponto dos segmentos
    pair<double,double> ponto_2; 
    
    // contando tempo de interseções da bruta
    auto inicio = std::chrono::high_resolution_clock::now();
    if(segments.size() >= 4){   //se for menor que 4, nao vai ter pontos para 2 segmentos, e nao vai ser necessário verificar interceções
        for(int i = 0; i < segments.size(); i += 2){            //cada posição do vetor segments guarda um ponto de um segmento. Portanto,
            for(int j = i + 2; j < segments.size(); j += 2){    //para pegar todos os pontos dos 2 segmentos, vai de 4 em 4. 
                                                                //compara 1 segmento com todos os outros
                AB[0] = {segments[i].first,segments[i].second};
                AB[1] = {segments[i + 1].first,segments[i + 1].second};

                CD[0] = {segments[j].first,segments[j].second};  
                CD[1] = {segments[j + 1].first,segments[j + 1].second};
                comparacoes++;
                if(intercepta(AB,CD)) intersecoes ++;
            }
        }
    }
    auto resultado = std::chrono::high_resolution_clock::now() - inicio;
    double s = std::chrono::duration_cast<std::chrono::seconds>(resultado).count();

    cerr << "Número de comparações:             " << cont << endl;
    cerr << "Número de possíveis interseções:   " << comparacoes << endl;
    cerr << "Número de interseções:             " << intersecoes << endl;
    cerr << "Tempo (s) para contar interseções: " << std::fixed << std::setprecision(4) << s << endl;
    return intersecoes;
}

bool compareCordinates(pBox box1, pBox box2){
    //para saber se uma caixa intercepta a outra, basta olhar seus vertices
    //por exemplo, cada caixa vai ter 2 pontos distintos no eixo x representando sua largura e dois 2 pontos distintos no y representando altura
    //se duas caixas se interceptam : um dos pontos da box1 deve estar dentro da box2, ou vice e versa
    //ou uma caixa deve atravessar a outra
    
    //verifica se uma caixa tem um ponto dentro da outra
    if((box1.ponto1.first >= box2.ponto1.first && box1.ponto1.first <= box2.ponto3.first) || (box1.ponto3.first >= box2.ponto1.first && box1.ponto3.first <= box2.ponto3.first)){
        if((box1.ponto1.second >= box2.ponto1.second && box1.ponto1.second <= box2.ponto3.second) || (box1.ponto3.second >= box2.ponto1.second && box1.ponto3.second <= box2.ponto3.second)) return true;
        if((box1.ponto1.second >= box2.ponto3.second && box1.ponto1.second <= box2.ponto1.second) || (box1.ponto3.second >= box2.ponto3.second && box1.ponto3.second <= box2.ponto1.second)) return true;
    }
    //caso o ponto1.first > ponto3.first(passa um ponto "de tras pra frente")
    if((box1.ponto1.first >= box2.ponto3.first && box1.ponto1.first <= box2.ponto1.first) || (box1.ponto3.first >= box2.ponto3.first && box1.ponto3.first <= box2.ponto1.first)){
        if((box1.ponto1.second >= box2.ponto1.second && box1.ponto1.second <= box2.ponto3.second) || (box1.ponto3.second >= box2.ponto1.second && box1.ponto3.second <= box2.ponto3.second)) return true;
        if((box1.ponto1.second >= box2.ponto3.second && box1.ponto1.second <= box2.ponto1.second) || (box1.ponto3.second >= box2.ponto3.second && box1.ponto3.second <= box2.ponto1.second)) return true;
    }   
    //verificar se uma box atravessa a outra
    //atravessa na horizontal
    if((box1.ponto1.second >= box2.ponto1.second && box1.ponto1.second <= box2.ponto3.second) || (box1.ponto3.second >= box2.ponto1.second && box1.ponto3.second <= box2.ponto3.second)){
        if((box1.ponto1.first <= box2.ponto1.first) && (box1.ponto3.first >= box2.ponto2.first)) return true;
        if((box1.ponto3.first <= box2.ponto1.first) && (box1.ponto1.first >= box2.ponto2.first)) return true;   //tras pra frente
    }
    //caso o ponto1.first > ponto3.first
    if((box1.ponto1.second >= box2.ponto3.second && box1.ponto1.second <= box2.ponto1.second) || (box1.ponto3.second >= box2.ponto3.second && box1.ponto3.second <= box2.ponto1.second)){
        if((box1.ponto1.first <= box2.ponto1.first) && (box1.ponto3.first >= box2.ponto2.first)) return true;
        if((box1.ponto3.first <= box2.ponto1.first) && (box1.ponto1.first >= box2.ponto2.first)) return true;   //tras pra frente
    }

    //verificar se uma box atravessa a outra
    //atravessa na vertical
    if((box1.ponto1.first >= box2.ponto1.first && box1.ponto1.first <= box2.ponto3.first) ||(box1.ponto3.first >= box2.ponto1.first && box1.ponto3.first <= box2.ponto3.first) ){
        if((box1.ponto1.second <= box2.ponto1.second && box1.ponto3.second >= box2.ponto3.second)) return true;
        if((box1.ponto3.second <= box2.ponto1.second && box1.ponto1.second >= box2.ponto3.second)) return true;
    }
    //caso o ponto1.first > ponto3.first
    if((box1.ponto1.first >= box2.ponto3.first && box1.ponto1.first <= box2.ponto1.first) ||(box1.ponto3.first >= box2.ponto3.first && box1.ponto3.first <= box2.ponto1.first) ){
        if((box1.ponto1.second <= box2.ponto1.second && box1.ponto3.second >= box2.ponto3.second)) return true;
        if((box1.ponto3.second <= box2.ponto1.second && box1.ponto1.second >= box2.ponto3.second)) return true;
    }
    return false;
}

long long int box(vector<pBox> & boxes){ // bounding boxes -> predicado de orientação
    pair<double,double> AB[2]; //segmentos auxiliares para vereficar se dois segmentos se intreceptam
    pair<double,double> CD[2];
    long long int comparacoes = 0;    // quantas possíveis comparações
    long long int intersecoes = 0;    // quantas interceções foram encontradas
    long long int cont = 0;           // quantas comparações foram feitas

    // contando tempo de interseções da box
    auto inicio = std::chrono::high_resolution_clock::now();

    //compara box por box
    for(int i=0;i<boxes.size();i++){
        for(int j=i+1;j<boxes.size();j++){
            cont ++;
            if(compareCordinates(boxes[i], boxes[j]) || compareCordinates(boxes[j], boxes[i])){    //possiveis interceções 
                AB[0] = boxes[i].ponto1;
                AB[1] = boxes[i].ponto3;
                CD[0] = boxes[j].ponto1;
                CD[1] = boxes[j].ponto3;
                comparacoes++;
                if(intercepta(AB,CD)) intersecoes ++;
            }
        } 
    }   
    auto resultado = std::chrono::high_resolution_clock::now() - inicio;
    double s = std::chrono::duration_cast<std::chrono::seconds>(resultado).count();

    cerr << "Número de comparações:             " << cont << endl;
    cerr << "Número de possíveis interseções:   " << comparacoes << endl;
    cerr << "Número de interseções:             " << intersecoes << endl;
    cerr << "Tempo (s) para contar interseções: " << std::fixed << std::setprecision(4) << s << endl;
    return intersecoes;
} 

void boxGrid(vector<pBox> & boxes, vector<int> indice, vector<pair<int,int>> & resp, long long int & cont){ // bounding boxes -> predicado de orientação
    for(int i = 0; i < indice.size() - 1; i ++){
        for(int j = i + 1; j < indice.size(); j ++){
            cont ++;
            if(compareCordinates(boxes[indice[i]], boxes[indice[j]]) || compareCordinates(boxes[indice[j]], boxes[indice[i]])){    //possiveis interseções 
                //se duas box colidem, guardo no vetor para fazer o calculo de interseção depois
                if(indice[i] < indice[j]) resp.push_back({indice[i],indice[j]});
                else resp.push_back({indice[j],indice[i]}); 
            }
        }
    }
}   

int grid(vector<pair<double,double>> &segments, int linhas, int colunas, vector<pBox> & boxes, int xmaior, int xmenor, int ymaior, int ymenor){ // grid -> bounding boxes -> predicado de orientação
    long long int intersecoes = 0;
    long long int comparacoes = 0;
    long long int cont = 0;
    // tempo para criar grid
    auto inicio = std::chrono::high_resolution_clock::now();

    //uma matriz[linha][coluna] que guarda os indices de cada box
    vector<vector<vector<int>>> matriz_grid(linhas, vector<vector<int>>(colunas));  
    vector<pair<int,int>> resp; //guarda o pair de  indices de boxes que se colidem

 

    double larg_tot = (xmaior - xmenor) + 0.01; //largura total
    double larg_cel = larg_tot/colunas;     //largura de cada celula do grid
    double alt_tot = (ymaior - ymenor) + 0.01;  //altura total
    double alt_cel = alt_tot/linhas;        //altura de cada celula do grid

    pair<int,int> lin;
    pair<int,int> col;

    //para achar as celulas que cada box pretence
    for(int k = 0; k < boxes.size(); k ++){
        lin.first = (boxes[k].ponto1.first - xmenor) / larg_cel;
        lin.second = (boxes[k].ponto3.first - xmenor) / larg_cel;

        col.first = (boxes[k].ponto1.second - ymenor) / alt_cel;
        col.second = (boxes[k].ponto3.second - ymenor) / alt_cel;

        if(lin.first > lin.second) swap(lin.first ,lin.second);
        if(col.first > col.second ) swap(col.first, col.second);

        if(lin.first < 0) lin.first = 0;
        if(lin.second < 0) lin.second = 0;
        if(col.first < 0) col.first  = 0;
        if(col.second < 0) col.second = 0;

        if(lin.first >= linhas) lin.first = linhas - 1;
        if(lin.second >= linhas) lin.second = linhas - 1;
        if(col.first >= colunas) col.first  = colunas - 1;
        if(col.second >= colunas) col.second =  colunas - 1;

        //basta olhar onde o segmento começa e onde ele termina, para saber quais celulas contém a box
        for(int i = lin.first; i <= lin.second; i ++)
            for(int j = col.first; j <= col.second; j ++){
                matriz_grid[i][j].push_back(k);   
            }
    }
    // --- TIME ---
        auto resultado = std::chrono::high_resolution_clock::now() - inicio;
        double s = std::chrono::duration_cast<std::chrono::seconds>(resultado).count(); 
    // ---

    // marcando tempo para contar interseções na grid
    auto inicio2 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < linhas; i++){
        for(int j = 0; j <colunas; j ++){
            if(matriz_grid[i][j].size() > 1)    //so compara se tiver 2 ou mais boxes na celula
                boxGrid(boxes, matriz_grid[i][j], resp, cont);    //guarda em resp os indices das boxes que colidem
        }
    }
     


    //ordena e usa unique para não calcular interseções repetidas 
    vector<pair<int,int>>::iterator it;

    sort(resp.begin(), resp.end());
    it = unique(resp.begin(), resp.end());
    
    resp.resize(distance(resp.begin(), it));


    for(int i = 0; i < resp.size(); i ++)  {
        pair<double,double> aux[] {boxes[resp[i].first].ponto1,boxes[resp[i].first].ponto3};
        pair<double,double> aux2[] {boxes[resp[i].second].ponto1,boxes[resp[i].second].ponto3};

        if(intercepta(aux,aux2)) intersecoes++; //vê interseção
    }
    // --- TIME ---
            auto resultado2 = std::chrono::high_resolution_clock::now() - inicio2;
            double s2 = std::chrono::duration_cast<std::chrono::seconds>(resultado2).count(); 
    // ---

    cerr << "Número de comparações:             " << cont << endl;
    cerr << "Número de possíveis interseções:   " << comparacoes << endl;
    cerr << "Número de interseções:             " << intersecoes << endl;
    cerr << "Tempo (s) para contar interseções: " << std::fixed << std::setprecision(4) << s2 << endl;
    cerr << "Tempo (s) para criar grid: " << std::fixed << std::setprecision(4) << s << endl;
    return intersecoes;
}

int main(){
    auto inicio = std::chrono::high_resolution_clock::now();
    freopen ("stderr","w",stderr);

    double nSegment, x1, y1, x2, y2, nIntersection;
    string option;
    int linhas = 1, colunas = 1;
   
    cin >> option;

    if(option == "bruta"){
        // tempo de leitura da bruta
        auto inicio = std::chrono::high_resolution_clock::now();
        //lê os segmentos e guarda
        cin >> nSegment; 
        vector<pair<double,double>> segments;
        pair<double,double> coordinate, coordinateAux; 

        for(int i = 0; i < 2*nSegment; i ++){
            cin >> y1 >> x1;
        
            coordinate = make_pair(y1,x1);
            segments.push_back(coordinate); // a cada dois pontos -> 1 segmento
        }
        auto resultado = std::chrono::high_resolution_clock::now() - inicio;
        double s = std::chrono::duration_cast<std::chrono::seconds>(resultado).count();  
        int numBruta =  bruta(segments);

        freopen ("stdout","w",stdout);
            cout << numBruta << endl;
        fclose (stdout);

        cerr << "Tempo (s) para ler os dados: " << std::fixed << std::setprecision(4) << s << endl;
        if(option!= "grid")  cerr << "Tempo (s) para criar grid: " << "0.0000" << endl;

    }if (option == "box"){
        // tempo de leitura da box
        auto inicio = std::chrono::high_resolution_clock::now();
        cin >> nSegment;
        
        vector<pBox> boxes(nSegment);

        vector<pair<double,double>> segments; // Segmentos
        pair<double,double> coordinate, coordinateAux; 

        for(int i = 0; i < nSegment; i ++){
            cin >> y1 >> x1 >> y2 >> x2;
            
            coordinate = make_pair(y1,x1);
            segments.push_back(coordinate); 
            
            coordinate = make_pair(y2,x2);
            segments.push_back(coordinate);

            //uma struct representa a box

            boxes[i].ponto1 = make_pair(x1,y1);
            boxes[i].ponto3 = make_pair(x2,y2);
            boxes[i].ponto2 = make_pair(x2,y1);
            boxes[i].ponto4 = make_pair(x1,y2);
        }

        auto resultado = std::chrono::high_resolution_clock::now() - inicio;
        double s = std::chrono::duration_cast<std::chrono::seconds>(resultado).count();    

        int numBox = box(boxes);
        freopen ("stdout","w",stdout);
            cout << numBox << endl;
        fclose (stdout);

        cerr << "Tempo (s) para ler os dados: " << std::fixed << std::setprecision(4) << s << endl;
        if(option!= "grid")  cerr << "Tempo (s) para criar grid: " << "0.0000" << endl;


    }else if (option == "grid"){
        // tempo de leitura da grid
        auto inicio = std::chrono::high_resolution_clock::now();
        cin >> linhas >> colunas;
        cin >> nSegment;
    
        vector<pair<double,double>> segments; // Segmentos
        pair<double,double> coordinate, coordinateAux; 
    
        vector<pBox> boxes(nSegment);
        
        int xmaior = 0, xmenor = 0;
        int ymaior = 0, ymenor = 0;

        for(int i = 0; i < nSegment; i ++){
            cin >> y1 >> x1 >> y2 >> x2;

            //enquanto lê os dados, guarda o tamanho total da grid
            if(xmenor > x1) xmenor = x1;
            if(xmenor > x2) xmenor = x2;

            if(xmaior < x1) xmaior = x1;
            if(xmaior < x2) xmaior = x2;

            if(ymenor > y1) ymenor = y1;
            if(ymenor > y2) ymenor = y2;

            if(ymaior < y2) ymaior = y2;
            if(ymaior < y1) ymaior = y1;

            coordinate = make_pair(y1,x1);
            segments.push_back(coordinate);
            
            coordinate = make_pair(y2,x2);
            segments.push_back(coordinate);

            boxes[i].ponto1 = make_pair(x1,y1);
            boxes[i].ponto3 = make_pair(x2,y2);
            boxes[i].ponto2 = make_pair(x2,y1);
            boxes[i].ponto4 = make_pair(x1,y2);
        }

        auto resultado = std::chrono::high_resolution_clock::now() - inicio;
        double s = std::chrono::duration_cast<std::chrono::seconds>(resultado).count();
        
        int numGrid = grid(segments, linhas, colunas, boxes, xmaior, xmenor, ymaior, ymenor);
        freopen ("stdout","w",stdout);
            cout << numGrid << endl;
        fclose (stdout);

        
        cerr << "Tempo (s) para ler os dados: " << std::fixed << std::setprecision(4) << s << endl;
    }   
    auto resultado2 = std::chrono::high_resolution_clock::now() - inicio;
    double total = std::chrono::duration_cast<std::chrono::seconds>(resultado2).count();
    cerr << "Tempo (s) total: " << std::fixed << std::setprecision(4) << total << endl;
    return 0;
}