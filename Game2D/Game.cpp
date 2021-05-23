#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() {
	vidas = 3;
    score = 0;
}

void Game::newGame() {  // Reseta o jogo
	Game();
	asteroids.clear();
    tiros.clear();
    score = 0;
}

bool Game::alive() {    // Verifica se um asteroide atingiu a nave
    for(int i=0; i<asteroids.size(); i++) {
        if(asteroids[i].y - (asteroids[i].raio*scale_y) <= 15*scale_y) {
            // Se o asteroide é maior que a nave
            if(asteroids[i].x + (asteroids[i].raio*scale_x) >= getSpaceship() + (7*scale_x) 
            	&& asteroids[i].x - (asteroids[i].raio*scale_x) <= getSpaceship() - (7*scale_x)) 
            {
            	vidas--; removeAsteroid(i);
            }
            // Se o asteroide é menor que a nave           
            else if(asteroids[i].x + (asteroids[i].raio*scale_x) <= getSpaceship() + (7*scale_x) 
            		&& asteroids[i].x - (asteroids[i].raio*scale_x) >= getSpaceship() - (7*scale_x)) 
            { 
            	vidas--; removeAsteroid(i);
            }
            // Se a metade esquerda do asteroide atingiu a nave
            else if(asteroids[i].x - (asteroids[i].raio*scale_x) <= getSpaceship() - (7*scale_x) 
            		&& (asteroids[i].x + (asteroids[i].raio*scale_x) <= getSpaceship() + (7*scale_x) 
            		&& asteroids[i].x + (asteroids[i].raio*scale_x) >= getSpaceship() + (7*scale_x))) 
        	{
        		vidas--; removeAsteroid(i);
        	}
            // Se a metade direita do asteroide atingiu a nave
            else if((asteroids[i].x - (asteroids[i].raio*scale_x) <= getSpaceship() + (7*scale_x) 
            		&& asteroids[i].x - (asteroids[i].raio*scale_x) >= getSpaceship() - (7*scale_x) ) 
            		&& asteroids[i].x + (asteroids[i].raio*scale_x) >= getSpaceship() + (7*scale_x) ) 
        	{
        		vidas--; removeAsteroid(i);
        	}
        }
    }

    if(vidas == 0) {
    	vidas = 3;
    	return false;
    }

    return true;
}

// Se o tiro sair da tela, remove do vetor de tiros
void Game::existTiro() {    
    for(int i=0; i<tiros.size(); i++)
        if(tiros[i].second > 150) 
        	tiros.erase(tiros.begin() + i);
}

// Incrementa a posição em y de todos os tiros
void Game::incrementaTiro(float y) {  
    for(int i=0; i<tiros.size(); i++) 
    	tiros[i].second += y; 
}

// Cria um novo tiro
void Game::createTiro(int x, int y) {
    tiros.push_back({x,y});
}

// Se o asteroide sair da tela, remove do vetor de asteroides
void Game::existAsteroid() {   
    for(int i=0; i<asteroids.size(); i++)
        if(asteroids[i].y < 0)
        	removeAsteroid(i);
}

// Remove um asteroide da tela
void Game::removeAsteroid(int i) {
    asteroids.erase(asteroids.begin() + i);

    // Atualiza os ids
    while(i<asteroids.size()) {
        asteroids[i].id = i;
        i++;
    }
}

// Verifica se o tiro acertou um asteroide
void Game::explodeAsteroid() {  
    int i;
    // Se o tiro estiver entre as dimesões do asteroide, aumenta o score e deleta o asteroide e o tiro
    for(int j=0; j<tiros.size(); j++) {
        for(i=0; i<asteroids.size(); i++) {
            if(asteroids[i].x + asteroids[i].raio*scale_x >= getTiroX(j) &&  asteroids[i].x - asteroids[i].raio*scale_x <= getTiroX(j)) {
                if (getTiroY(j) >= (asteroids[i].y - asteroids[i].raio*scale_x) && getTiroY(j) <= asteroids[i].y + asteroids[i].raio*scale_x) {
                    removeAsteroid(i);  // Deleta o asteroide
                    tiros.erase(tiros.begin() + j);
                    score += 10;
                    break;
                }
            }
        } 
    }
}

// Cria um asteroide na posição (x,y) com raio aleatório de 1 a 4 e número de pontos aleatório de 4 a 8
const Asteroid& Game::createAsteroid(double x, double y) {  
    Asteroid a(x, y, 1+rand()%4, 4+rand()%4, asteroids.size()-1); // Cada asteroide guarda um id que indica sua posição no vetor de asteroides
    asteroids.push_back(a);
    return asteroids[asteroids.size()-1];
}

// Retorna um asteroide
const Asteroid& Game::getAsteroid(int i) const {   
    return asteroids[i];
}

// Retorna todos os asteroides
const std::vector<Asteroid>& Game::getAsteroids() const {   
	return asteroids;
}

// Retorna a posição do centro da nave
float Game::getSpaceship() const {  
    return spaceship;
}

// Retorna a posição em x de um tiro
int Game::getTiroX(int i) const {   
    return tiros[i].first;
}

// Retorna a posição em y de um tiro
int Game::getTiroY(int i) const {   
    return tiros[i].second;
}

// Quantidade de tiros na tela
int Game::getNumTiros() const {    
    return tiros.size();
}

// Número de vidas
int Game::getLife() const {  
    return vidas;
}

// Retorna a pontuação atual do game
int Game::getScore() const {
    return score;
}

// Atualiza a posição em y de um asteroide
void Game::setAsteroid(int i, double y) {  
    asteroids[i].y = y;
}

// Atualiza a posição do centro da nave e as novas escalas
void Game::setSpaceship(float x, float scale_x, float scale_y) { 
    spaceship = x;
    this->scale_x = scale_x;
    this->scale_y = scale_y;
}