#ifndef GAME_H
#define GAME_H

#include <vector>

struct Asteroid {
    double x, y, raio;
    int numPontos, id;

    Asteroid(double x0, double y0, double r, int n, int i) : x(x0), y(y0), raio(r), numPontos(n), id(i) {}
};

class Game {
    public:
        Game();

        void newGame();
        bool alive();

        void existTiro();
        void incrementaTiro(float y);
        void createTiro(int x, int y);

        void existAsteroid();
        void removeAsteroid(int i);
        void explodeAsteroid();        
        const Asteroid& createAsteroid(double x, double y);

        const Asteroid& getAsteroid(int i) const;
        const std::vector<Asteroid>& getAsteroids() const;
        float getSpaceship() const;
        int getTiroX(int i) const;
        int getTiroY(int i) const;
        int getNumTiros() const;
        int getLife() const;
        int getScore() const;

        void setAsteroid(int i, double y);
        void setSpaceship(float x, float scale_x, float scale_y);
        
    private: 
        int vidas;
        std::vector<Asteroid> asteroids;            // Posições dos asteroids
        std::vector<std::pair<int,float>> tiros;    // Posições dos tiros
        float spaceship;                            // Posição do centro da nave no eixo x
        float scale_x = 1;                          // Variavel da escala em x
        float scale_y = 1;                          // Variável da escala em y
        int score;                                  // Pontuação do game, cada asteroide vale 10 pontos
};

#endif