#pragma once
#include <iostream>

struct Cell
{
    bool is_land;
    int elevation;
    int population[2];
    int land_neighbours;
    Cell *neighbours[8];
    Cell *land_nighbours;
};

class Game {
    private:
        Cell **Field;
        int HEIGHT, WIDTH;
        int year;
        int global_population;
        int max_population;
        int min_population_to_migrate;
        float birth_rate, migration_rate;
        
        int population_growth();
        int population_migration();
        int check_max_population();
    public:
        //Конструктор
        Game(int width, int height);
        //Деструктор
        ~Game();
        //game loop
        bool update();
        //game render
        bool render();
        void set_population(int x, int y, int population);
        void add_population(int x, int y, int population);
};
