#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <unistd.h>
#include "Game.h"

#define PI 3.1415926535898

using namespace std;

Game game;

/*
 * MODO M - estado da tela mostra o menu
 * MODO G - estado da tela mostra o jogo
 * MODO E - estado da tela mostra o fim de jogo
*/
char MODO = 'M';
bool bebado = false;
bool gameInit = false;

int h = 500, w = 700;

int w_ortho = 150; int start_ortho_x = 0;
int h_ortho = 150; int start_ortho_y = 0;

float posCenter = w_ortho/2;
bool increment = false;

int vw = 500; int vh = 500;
int viewh = (h-vw)/2;
int vieww = (w-vh)/2;

// Velocidade do jogo
float velocidade_tiro = 1;
float velocidade_asteroid = 1;
float velocidade_nave = 2;
// Variáveis para alterar a escala
float scale_x = 1;
float scale_y = 1;

/*
 * Opções de cores
 * 1 - preto e branco
 * 2 - tons cyberpunk
 * 3 - tons pastéis
*/
int c=0; int m = 0; int t=1; int v=1; // variaveis selecionadas

static GLfloat spin = 0.0;

struct coord {
    GLfloat y1,x1,y2,x2;
    coord() : x1(0.0), y1(0.0), x2(0.0), y2(0.0) {}

    coord(GLfloat x0, GLfloat y0, GLfloat x, GLfloat y) {
        y1 = y0;
        x1 = x0;
        y2 = y;
        x2 = x;
    }
};

struct color {
    GLfloat r,g,b;
    color() : r(0.0), g(0.0), b(0.0) {}

    color(GLfloat r1, GLfloat g1, GLfloat b1) {
        r = r1;
        g = g1;
        b = b1;
    }
};

color a(1.0,1.0,1.0);// cor padrao 
vector<vector<color>> cores(3, vector<color>(4,a));

GLint id=0;
vector<coord> menuOption(13);
vector<color> cor(13);

/******************************************************************************************/
/* MENU */

void atribuiCor(){
    cores[0][0].r = 0.827; cores[0][0].g = 0.827; cores[0][0].b = 0.827;

    cores[1][0].r = 1.0; cores[1][0].g = 0.164; cores[1][0].b = 0.427; 
    cores[1][1].r = 0.019; cores[1][1].g = 0.850; cores[1][1].b = 0.909; 
    cores[1][2].r = 1.0; cores[1][2].g = 0.070; cores[1][2].b = 0.254; 
    cores[1][3].r = 0.0; cores[1][3].g = 1.0; cores[1][3].b = 1.0; 

    cores[2][0].r = 0.94; cores[2][0].g = 0.50; cores[2][0].b = 0.50; 
    cores[2][1].r = 0.87; cores[2][1].g = 0.898; cores[2][1].b = 0.88; 
    cores[2][2].r = 0.804; cores[2][2].g = 0.498; cores[2][2].b = 0.196; 
    cores[2][3].r = 0.81; cores[2][3].g = 0.97; cores[2][3].b = 0.48; 
}
void preencheCor() {  
    for(int i=0;i<13;i++) {
        cor[i] = cores[c][1];
    }
}

void texto(const string &texto, int x, int y, double tamanhoX, double tamanhoY) { //função que escreve algo na tela
    glPushMatrix();

    glTranslatef(x, y, 0);
    glScalef(0.05, 0.05, 1.0);
    
    for(int i=0; i<texto.length(); i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, texto[i]);
    }

    glPopMatrix();
}

void quadrado(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) { //desenha quadrados de acordo com os parâmetros
    glBegin (GL_QUADS);
    glVertex2f (x1 , y1);
    glVertex2f (x2 , y1);
    glVertex2f (x2 , y2);
    glVertex2f (x1 , y2);
    glEnd();
}

void seleciona(coord &a) {
    glPolygonMode (GL_FRONT, GL_LINE);
    glPolygonMode (GL_BACK, GL_LINE);

    glBegin (GL_POLYGON);
    glColor3f (cores[c][0].r, cores[c][0].g, cores[c][0].b);
    glVertex2f (a.x1-1 , a.y1-1);
    glVertex2f (a.x2+1 , a.y1-1);
    glVertex2f (a.x2+1 , a.y2+1);
    glVertex2f (a.x1-1 , a.y2+1);
    glEnd();

    glPolygonMode (GL_FRONT, GL_FILL);
    glPolygonMode (GL_BACK, GL_FILL);
}

void selecionado() {
    if(id==12) {
        exit(0);
    }

    if(id==0) {
        MODO = 'G';
        return;
    }
    
    if(id == 11){ bebado = true; m = 1;}
    else if(id==10){ bebado = false; m = 0;}

    if(id>=1 && id<=3) {
        // Menu sempre tem cor padrão
        for(int i=1; i<=3; i++) {
            cor[i] = cores[c][1];
        }

        // Seleciona a velocidade do jogo
        if(id == 1) {    // Lento
            v=0;
            velocidade_tiro = 0.3;
            velocidade_asteroid = 0.3;
            velocidade_nave = 0.6;
        }
        if(id == 2) {    // Normal
            v=1;
            velocidade_tiro = 1;
            velocidade_asteroid = 1;
            velocidade_nave = 2;
        }
        if(id == 3) {    // Rápido
            v=2;
            velocidade_tiro = 3;
            velocidade_asteroid = 3;
            velocidade_nave = 8;
        }

    } else if(id >=4 && id <= 6) {
        for(int i=4; i<=6; i++) {
            cor[i] = cores[c][1];
        }

        // Seleciona o tamanho do jogo
        if(id == 4) {     // Pequeno
            t=0;
            scale_x = 0.5;
            scale_y = 0.5;
        }
        if(id == 5) {     // Normal
            t=1;
            scale_x = 1;
            scale_y = 1;
        }
        if(id == 6) {     // Grande
            t=2;
            scale_x = 2;
            scale_y = 2;
        }

    } else if(id>=7 && id <=9) {
        for(int i=7; i<=9; i++) {
            cor[i] = cores[c][1];
        }

        // Seleciona cor
        if(id == 7) {     // padrão
            c = 0;
            preencheCor();
        }
        if(id == 8) {     // tons cyberpunk
            c = 1;
            preencheCor();
        }
        if(id == 9) {     // tons pastéis
            c = 2;
            preencheCor();
        }

    } else if(id>=10 && id <=11) {
        for(int i=10; i<=11; i++) {
            cor[i] = cores[c][1];
        }
    }

    //cor[id] = cores[c][0];
}

// Desenha o menu na tela
void menu() {   
    cor[v+1] = cores[c][0];
    cor[t+4] = cores[c][0];
    cor[c+7] = cores[c][0];
    cor[m+10] = cores[c][0];

    glColor3f (1.0, 1.0, 1.0);
    texto("Asteroids", 61.5,140,0.05,0.05);

    /* Inciar */
    glColor3f (cor[0].r, cor[0].g, cor[0].b);   // Cor padrão do menu
    quadrado(60.0,125.0,90.0,135.0);    // Posição do quadrado em volta do texto 'Iniciar'
    menuOption[0].x1 = 60; menuOption[0].y1 = 125; menuOption[0].x2 = 90; menuOption[0].y2 = 135;
    
    glColor3f(0,0,0);
    texto("iniciar", 67,127,0.05,0.05); // Texto que está no quadrado

    /***************************************/
    /* Velocidade */
    
    glColor3f(cores[c][3].r,cores[c][3].g,cores[c][3].b);
    texto("Velocidades", 58,107,0.04,0.04);

    glColor3f (cor[1].r, cor[1].g, cor[1].b);
    quadrado(20.0,95.0,50.0,105.0);
    glColor3f(0,0,0);
    texto("baixa", 27,97,0.05,0.05);

    glColor3f (cor[2].r, cor[2].g, cor[2].b);
    quadrado(60.0,95,90,105);
    glColor3f(0,0,0);
    texto("normal", 65,97,0.05,0.05);

    glColor3f (cor[3].r, cor[3].g, cor[3].b);
    quadrado(100,95,130,105);
    glColor3f(0,0,0);
    texto("alta", 110,97,0.05,0.05);
    
    // Guarda a posição de cada opção
    GLfloat x=20;
    for(int i=1;i<4;i++) {
        menuOption[i].x1 = x;
        menuOption[i].y1 = 95;
        x += 30;
        menuOption[i].x2 = x;
        menuOption[i].y2 = 105;
        x += 10;
    }
   
    /***************************************/
    /* Tamanho */
    
    glColor3f(cores[c][3].r,cores[c][3].g,cores[c][3].b);
    texto("Tamanhos", 60,87,0.04,0.04);
    
    glColor3f (cor[4].r, cor[4].g, cor[4].b);
    quadrado(20,75,50,85);
    
    glColor3f(0,0,0);
    texto("pequeno", 22,77,0.05,0.05);

    glColor3f (cor[5].r, cor[5].g, cor[5].b);
    quadrado(60.0,75,90,85);
    
    glColor3f(0,0,0);
    texto("normal", 65,77,0.05,0.05);

    glColor3f (cor[6].r, cor[6].g, cor[6].b);
    quadrado(100,75,130,85);
    
    glColor3f(0,0,0);
    texto("grande", 105,77,0.05,0.05);

    x=20;
    for(int i=4;i<7;i++) {
        menuOption[i].x1 = x;
        menuOption[i].y1 = 75;
        x += 30;
        menuOption[i].x2 = x;
        menuOption[i].y2 = 85;
        x += 10;
    }

    /***************************************/
    /* Cores */
    
    glColor3f(cores[c][3].r,cores[c][3].g,cores[c][3].b);
    texto("Cores", 66,67,0.04,0.04);

    glColor3f (cor[7].r, cor[7].g, cor[7].b);
    quadrado(20,55,50,65);
    
    glColor3f(0,0,0);
    texto("classico", 24,57,0.05,0.05);

    glColor3f (cor[8].r, cor[8].g, cor[8].b);
    quadrado(60.0,55,90,65);
    
    glColor3f(0,0,0);
    texto("cyberpunk", 60,57,0.05,0.05);

    glColor3f (cor[9].r, cor[9].g, cor[9].b);
    quadrado(100,55,130,65);
    
    glColor3f(0,0,0);
    texto("soft", 109,57,0.05,0.05);

    x=20;
    for(int i=7;i<10;i++) {
        menuOption[i].x1 = x;
        menuOption[i].y1 = 55;
        x += 30;
        menuOption[i].x2 = x;
        menuOption[i].y2 = 65;
        x += 10;
    }

    /***************************************/
    /* Modo */

    glColor3f(cores[c][3].r,cores[c][3].g,cores[c][3].b);
    texto("Modos", 66,47,0.04,0.04);
    
    glColor3f (cor[10].r, cor[10].g, cor[10].b);
    quadrado(40,35,70,45);
    
    glColor3f(0,0,0);
    texto("normal", 45,37,0.05,0.05);
    
    glColor3f (cor[11].r, cor[11].g, cor[11].b);
    quadrado(80,35,110,45);
    
    glColor3f(0,0,0);
    texto("bebado", 85,37,0.05,0.05);

    x=40;
    for(int i=10;i<12;i++) {
        menuOption[i].x1 = x;
        menuOption[i].y1 = 35;
        x += 30;
        menuOption[i].x2 = x;
        menuOption[i].y2 = 45;
        x += 10;
    }

    /***************************************/
    /* Sair */

    glColor3f (cor[12].r, cor[12].g, cor[12].b);
    quadrado(60,15,90,25);
    glColor3f(0,0,0);
    texto("sair", 70,17,0.05,0.05);
    menuOption[12].x1 = 60; menuOption[12].y1 = 15; menuOption[12].x2 = 90; menuOption[12].y2 = 25;

    seleciona(menuOption[id]);
}

// Tela que aparece após o jogador perder
void endGame() {    
    // Usa a cor selecionada
    glColor3f(cores[c][rand()%4].r,cores[c][rand()%4].g,cores[c][rand()%4].b);
    texto("Vc perdeu",55,75,1.5,1.5);
    texto("Pressione Enter para voltar",30,55,1,1);
}

/******************************************************************************************/
/* JOGO */

// Desenha o score na tela do jogo
void scoreText(int score, int x, int y, double tamanhoX, double tamanhoY) {
    string texto = to_string(score);    // Transforma a pontuação int em string, para poder escrever
    string text = "score: ";

    double x2 = (texto.length() + text.length())*3.5; 
    glPushMatrix();

    // Caixa de texto do score
    glPolygonMode (GL_FRONT, GL_FILL);
    glBegin (GL_QUADS);

    glVertex2f (x-1, y-1);
    glVertex2f (x2 , y-1);
    glVertex2f (x2, y+4);
    glVertex2f (x-1 , y+4);

    glEnd();
    glColor3f(0,0,0);
    glTranslatef(x, y, 0);  // Posição do texto na tela
    glScalef(tamanhoX, tamanhoY, 1.0);  // Redimensiona o tamanho
    
    for(int i=0; i<text.length(); i++) {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text[i]);
    }

    for(int i=0; i<texto.length(); i++) {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, texto[i]);
    }

    glPopMatrix();
}

// Altera a posição da nave na classe
void move(float x) {    
    game.setSpaceship(x, scale_x,scale_y);
}

// Incrementa a posição dos tiros
void incrementa(int value) {    
    game.incrementaTiro(velocidade_tiro);
    game.explodeAsteroid(); // Verifica se o tiro atingiu o asteroid, se sim, o asteroid some e incrementa a pontuação

    // Incrementa se estiver no modo game
        glutTimerFunc(10,incrementa,1); 
    
}

// Movimentar os asteroids
void decrementa(int value) {    
    int n = game.getAsteroids().size();

    for(int i=0;i<n;i++) {
        Asteroid a = game.getAsteroid(i);
        double y = a.y;
        game.setAsteroid(i,y-velocidade_asteroid);  // A cada 10ms, decrementa a posição do asteroid
    }

    glutPostRedisplay();

    if (MODO == 'G') {  // Decrementa se estiver no modo game
        glutTimerFunc(10,decrementa,1);
    }
}

// Desenha todos os asteroides na tela
void drawAsteroids() {  
    vector<Asteroid> asteroids = game.getAsteroids();
    
    for(Asteroid &a : asteroids) {
        glPushMatrix();
        a.raio *= scale_y;  //o tamanho do asteroid vai ser definido pelo raio, assim basta multiplicar
                            //a escala pelo tamanho do raio, para alterar o tamanho dos asteroids
        //cout << a.id%4<< endl;
        glBegin(GL_POLYGON);
        for(int i=0; i<a.numPontos; i++) {
            GLfloat angle = 2*PI*i/a.numPontos;
            
            glColor3f(cores[c][a.id%4].r,cores[c][a.id%4].g,cores[c][a.id%4].b);
            glVertex2f(a.x+a.raio * cos(angle), a.y+a.raio * sin(angle)); // Desenha os asteroides
        }
       
        glEnd();
        glPopMatrix();
    }
    glutPostRedisplay();
}

// Cria um novo asteroide por segundo
void createAsteroids(int value) {
    // Asteroide deve ser desenhado inicialmente em um x aletório e y máximo da janela
    Asteroid a = game.createAsteroid(rand()%151,150.0);
    glutTimerFunc(1000,createAsteroids,1);
}

// Desenha um tiro na tela
void shot(double x, double y) {   
    glPushMatrix();
    glBegin (GL_POINTS);

    glColor3f(cores[c][rand()%4].r,cores[c][rand()%4].g,cores[c][rand()%4].b);
   
    glVertex2i(x, y);
    glEnd();
    glutPostRedisplay();
    glPopMatrix();
}

// Desenha a nave na tela
void spaceship(double center) { 
 
    glPushMatrix();
    glTranslatef(-center,0,0);  //muda a escala da nave
    glScalef(1,scale_y,0);
    glTranslatef(center,0,0);

    
    glPolygonMode (GL_FRONT, GL_LINE);
    glBegin (GL_POLYGON);
    int i=0;
    
    glColor3f(cores[c][i].r,cores[c][i].g,cores[c][i].b);
    glVertex2f (center, 15);    //o centro da nave sempre vai ser em função da ortho

    if(c==1) i++;
    glColor3f(cores[c][i].r,cores[c][i].g,cores[c][i].b);
    glVertex2f (center-(7*scale_x) , 1);//para define o tamanho da largura da nave

    if(c==1) i++;
    glColor3f(cores[c][i].r,cores[c][i].g,cores[c][i].b);
    glVertex2f (center, 4);

    if(c==1) i++;
    glColor3f(cores[c][i].r,cores[c][i].g,cores[c][i].b);
    glVertex2f (center +(7*scale_x), 1);
    glEnd();

    glPopMatrix();
}

// Desenha as nnaves que representam as vidas no topo da tela
void vidas() {  
    glPushMatrix();

    
    for(int i=0;i<game.getLife();i++){
        int j=0;
        glBegin (GL_POLYGON);
        glColor3f(cores[c][j].r,cores[c][j].g,cores[c][j].b);
        glVertex2f (149-5*(i+1), 149);
        
        if(c==1) j++;
        glColor3f(cores[c][j].r,cores[c][j].g,cores[c][j].b);
        glVertex2f (149-5*(i+1) -2, 145);
        
        if(c==1) j++;
        glColor3f(cores[c][j].r,cores[c][j].g,cores[c][j].b);
        glVertex2f (149-5*(i+1), 146.15);
        
        if(c==1) j++;
        glColor3f(cores[c][j].r,cores[c][j].g,cores[c][j].b);
        glVertex2f (149-5*(i+1) +2, 145);
        glEnd();
    }
    glPopMatrix();

}
/******************************************************************************************/
/* FUNÇÕES GERAIS */

void reshape(int w1, int h1) {
    w = w1; h = h1;
    viewh = (h1-vw)/2;
    vieww = (w1-vh)/2;
    glViewport (vieww, viewh, (GLsizei) vw, (GLsizei) vh);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D(start_ortho_x, w_ortho, start_ortho_y, h_ortho);    
}

void SpecialKeys(int key, int x, int y) {
    if (MODO == 'M') { // Modo Menu
        if(key == GLUT_KEY_UP) {
            // Se estiver selecionando o primeiro quadrado da coluna, quando apertar pra cima, vai para o último
            if(id == 0) id += 12;
            else if(id == 3 | id == 4 || id == 5 || id == 6 || id == 7 || id == 8 || id == 9 || id == 11) id -= 3;
            else if(id ==2 || id == 10 || id == 12) id -= 2;
            else if(id ==1 ) id--;
        }

        if(key == GLUT_KEY_DOWN){
            // Se estiver selecionando o último quadrado da coluna, quando apertar pra baixo, vai para o primeiro
            if(id >= 1 && id <= 7) id += 3;
            else if(id == 0 || id == 8 || id == 9 || id == 10) id += 2;
            else if(id == 11 ) id++;
            else if(id == 12 ) id -= 12;
        }

        if(key == GLUT_KEY_LEFT) {
            // Quando tiver selecionado o primeiro quadrado da linha, vai para o último
            if(id == 2 || id == 3 || id == 5 || id == 6 || id == 8 || id == 9 || id == 11) id--;
            else if(id == 1 || id == 4 || id == 7) id += 2;
            else if(id == 10) id++;
        }

        if(key == GLUT_KEY_RIGHT){
            // Quando tiver selecionando o último quadrado da linha, vai para o primeiro
            if(id == 1 || id == 2 || id == 4 || id == 5 || id == 7 || id == 8 || id == 10) id++;
            else if(id == 3 || id == 6 || id == 9) id -= 2;
            else if(id == 11) id--;
        }

        seleciona(menuOption[id]);

    } else if (MODO == 'G') {       // Modo game
        if(key == GLUT_KEY_LEFT) {  // Move a nave
            if(posCenter-2 > 0) 
                posCenter -= velocidade_nave;
            move(posCenter);
        } else if(key == GLUT_KEY_RIGHT) {
            if(posCenter+2 < w_ortho) 
                posCenter += velocidade_nave;
            move(posCenter);
        }
    }

    glutPostRedisplay();
}

void HandleKeyboard(unsigned char key, int x, int y) {

    if (MODO == 'M') {
        switch (key) {
            case 13: // enter
                selecionado();
                break;
        }

    } else if (MODO == 'G') {
        switch (key) {
            case 32: // space
                // A nave atira
                game.createTiro(game.getSpaceship(), 15*scale_y);
                if(!increment) {
                    incrementa(velocidade_tiro);
                    increment = true;
                }
                break;

            case 27: // esc
                // Volta para o menu
                spin = 0;
                gameInit = false;
                MODO = 'M';
                break;

            case 'Z':
            case 'z':   
                // Da zoom
                if(w_ortho - start_ortho_x > 10 && h_ortho > 10) {
                    start_ortho_x +=5;
                    h_ortho-=10;
                    w_ortho-=5;
                   
                    reshape(w,h); 
                }
                break;

            case 'X':
            case 'x':
                // Tira o zoom
                if(w_ortho - start_ortho_x < 150 && h_ortho < 150) {
                    start_ortho_x-=5;
                    h_ortho+=10;
                    w_ortho+=5;
                    
                    reshape(w,h); 
                }
                break;

            case 'A':
            case 'a':
                // Move a tela, no jogo
                if(start_ortho_x >= 0) {
                    start_ortho_x -= 5;
                    w_ortho -= 5;
                    reshape(w,h); 
                }
                break;
            
            case 'D':
            case 'd':
                if(w_ortho <= 150) {
                    start_ortho_x += 5;
                    w_ortho += 5;
                    reshape(w,h); 
                }
                break;

            case 'W':
            case 'w':
                if(h_ortho <= 150) {
                    start_ortho_y += 5;
                    h_ortho += 5;
                    reshape(w,h); 
                }
                break;

            case 'S':
            case 's':
                if(start_ortho_y >= 0) {
                    start_ortho_y -= 5;
                    h_ortho -= 5;
                    reshape(w,h); 
                }
                break;
        }

    } else if (MODO == 'E') {
        switch (key) {
            case 13: // enter
                // Volta para a tela de Menu
                MODO = 'M';
                break;
        }
    }
	
    glutPostRedisplay();
}

void HandleMouse(int button, int state, int x, int y) {
    // Ao clicar em algum quadrado, seleciona ele
    if (MODO == 'M') {
        int x0 = (w_ortho * (x-vieww)) / (vw);
        int y0 = abs((h_ortho * (y-viewh)) / (vh) - h_ortho);

    	if (button == GLUT_LEFT_BUTTON) {
    		if (state == GLUT_UP) {
                for(int i=0; i<13; i++) {
                    coord c = menuOption[i];
                    if (x0 >= c.x1 && x0 <= c.x2 && y0 >= c.y1 && y0 <= c.y2) {
                        id = i;
                    }
                }
                selecionado();
            } 
        }
    	glutPostRedisplay();
    }
}

void MoveMouse(int x, int y) {     
    // Quando passa o mouse em cima de algum quadrado, desenha uma caixinha
    if (MODO == 'M') {
        int x0 = (w_ortho * (x-vieww)) / (vw);
        int y0 = abs((h_ortho * (y-viewh)) / (vh) - h_ortho);
        
        for(int i=0; i<13; i++) {
            coord c = menuOption[i];
            if (x0 >= c.x1 && x0 <= c.x2 && y0 >= c.y1 && y0 <= c.y2) {
                id = i;
            }
        }
        glutPostRedisplay();
    }
}

void display(void) {
    glClear (GL_COLOR_BUFFER_BIT);

    if (MODO == 'M') {  // Se voltar para o menu reseta os padrões da tela (para retirar o zoom, caso tenha)
        w_ortho = 150;  start_ortho_x = 0;
        h_ortho = 150;  start_ortho_y = 0;

        posCenter = w_ortho/2;

        reshape(w,h);
        menu();

    } else if (MODO == 'G') {
        if(bebado) { // Faz a tela girar, se estiver no modo bebado
            glPushMatrix();
            glTranslatef(75,75,0);
            glRotatef(spin, 0.0, 0.0, 1.0);
            glViewport (vieww, viewh, (GLsizei) vw, (GLsizei) vh);
            glTranslatef(-75,-75,0);
        }
               
        if (!gameInit) {
            viewh = (h-vw)/2;
            vieww = (w-vh)/2;
            posCenter = w_ortho/2;
            glViewport (vieww, viewh, (GLsizei) vw, (GLsizei) vh);
            glMatrixMode (GL_PROJECTION);
            glLoadIdentity ();
            gluOrtho2D(0,150,0,150);
            game.newGame();
            createAsteroids(1);
            glutTimerFunc(100,decrementa,1);
            game.setSpaceship(posCenter, scale_x, scale_y);
            gameInit = true;
        }

        glMatrixMode (GL_PROJECTION);
        if(game.alive()) {
            game.existAsteroid();
            game.existTiro();
            
            spaceship(game.getSpaceship());

            for(int i=0; i<game.getNumTiros(); i++){
                shot(game.getTiroX(i),game.getTiroY(i));
            }

            drawAsteroids();
            glColor3f(cores[c][3].r,cores[c][3].g,cores[c][3].b);
            scoreText(game.getScore(), 2,145, 0.03, 0.03);
            vidas();
            
        } else {
            spin = 0;
            gameInit = false;
            MODO = 'E';
        }
        
       if(bebado) glPopMatrix();

    } else if (MODO == 'E') {
        // Quando perder reseta os padrões da tela (para retirar o zoom, caso tenha)
        w_ortho = 150;  start_ortho_x = 0;
        h_ortho = 150;  start_ortho_y = 0;

        posCenter = w_ortho/2;

        reshape(w,h);
        endGame();
    }

    glFlush();
    glutSwapBuffers();
}

void spinDisplay(void) {
   spin += 0.2;
   if (spin > 360.0)
      spin -= 360.0;
   glutPostRedisplay();
}

void init(void) {   
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
}

int main(int argc, char** argv) {  
    srand(0);
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (w, h); 
    glutInitWindowPosition (250, 100);
    glutCreateWindow (argv[0]);
    preencheCor();
    atribuiCor();
    init ();
    glutDisplayFunc(display); 
    glutIdleFunc(spinDisplay);
    glutReshapeFunc(reshape); 
    glutKeyboardFunc(HandleKeyboard);
    glutSpecialFunc(SpecialKeys);
    glutMouseFunc(HandleMouse);
    glutPassiveMotionFunc(MoveMouse);
    glutMainLoop();
    return 0;
}