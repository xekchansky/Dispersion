#include "game.h"
#include <iostream>
#include "SDL_init.cpp"

extern const bool DEBUG;
extern const bool SPEED_CHECK;

extern bool PAUSE;

Game::Game(int width, int height) {
    HEIGHT = height;
    WIDTH = width;
    //terrain generaion
    Field = new Cell * [HEIGHT];
    for (int i = 0; i < HEIGHT; ++i) {
        Field[i] = new Cell [WIDTH];
        for (int j = 0; j < WIDTH; ++j) {
            Field[i][j].elevation = 100;
            Field[i][j].is_land = (Field[i][j].elevation > 0);
            Field[i][j].population[0] = 0;
            Field[i][j].population[1] = 0;
        }
    }
    //calculating neighbours
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            //BRUHMOMENTHERE
            std::cout << "creating neighbours for: " << i << ' ' << j << std::endl;
            *Field[i][j].neighbours[0] = Field[i + 1][j + 1];    //NW
            std::cout << "bruh";
            *Field[i][j].neighbours[1] = Field[i][j];    //N
            *Field[i][j].neighbours[2] = Field[i][j];    //NE
            *Field[i][j].neighbours[3] = Field[i][(j + 1) % WIDTH];    //E
            *Field[i][j].neighbours[4] = Field[i][j];    //SE
            *Field[i][j].neighbours[5] = Field[i][j];    //S
            *Field[i][j].neighbours[6] = Field[i][j];    //SW
            *Field[i][j].neighbours[7] = Field[i][(j + WIDTH - 1) % WIDTH];    //W
            //if (Field[i][j].is_land) {
            //    ++Field[i][j].land_neighbours;
            //}
        }
    }
    year = 0;
    global_population = 0;
    birth_rate = 0.01;
    migration_rate = 0.01;
    max_population = 1000;
    min_population_to_migrate = 100;
    std::cout << "Created game: " << WIDTH << ' ' << HEIGHT << std::endl;
}

Game::~Game() {
    for (int i = 0; i < HEIGHT; ++i) {
        delete [] Field[i];
    }
    delete [] Field;
    std::cout << "Game destroyed" << std::endl;
}

bool Game::update() {
    clock_t start = clock();

    std::cout << year % 2 << ' ' << year << ' ' << global_population << std::endl;
    if (!PAUSE) {
        global_population += population_growth();
        population_migration();
        global_population -= check_max_population();
        ++year;
    }

    if (DEBUG && SPEED_CHECK) {
        std::cout << "update: " << clock() - start << "ms" << std::endl;
    }
}

bool Game::render() {
    clock_t start = clock();
    int cell_width = SCREEN_WIDTH / WIDTH;
    int cell_height = SCREEN_HEIGHT / HEIGHT;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
                SDL_Rect fillRect = { cell_width * j, cell_height * i, cell_width, cell_height };
                float density = float(Field[i][j].population[year % 2]) / float(max_population);
                int green = 255 * density;
                SDL_SetRenderDrawColor( gRenderer, 0, green, 0, 0 );
                SDL_RenderFillRect( gRenderer, &fillRect );
        }
    }
    if (DEBUG && SPEED_CHECK) {
        std::cout << "render: " << clock() - start << "ms" << std::endl;
    }
}

void Game::set_population(int x, int y, int population) {
    global_population += population - Field[y][x].population[0];
    //возможна ошибка подсчета
    Field[y][x].population[0] = population;
    Field[y][x].population[1] = population;
}

void Game::add_population(int x, int y, int population) {
    Field[y][x].population[0] += population;
    Field[y][x].population[1] += population;

}

int Game::population_growth() {
    bool this_year = year % 2;
    bool next_year = (year + 1) % 2;
    int new_population = 0;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            int newborn = int(birth_rate * Field[i][j].population[this_year]);
            new_population += newborn;
            Field[i][j].population[next_year] = Field[i][j].population[this_year] += newborn;
        }
    }
    //std::cout << "newborn: " << new_population << std::endl;
    return new_population;
}

int Game::population_migration() {
    bool this_year = year % 2;
    bool next_year = (year + 1) % 2;
    int migrated = 0;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (Field[i][j].population[this_year] > min_population_to_migrate) {
                int migrants = int(migration_rate * Field[i][j].population[this_year]);
                migrated += migrants;
                Field[i][j].population[next_year] -= migrants;
                Field[i][(j + 1) % WIDTH].population[next_year] += migrants;
            }
        }
    }
    //std::cout << "migrated: " << migrated << std::endl;
    return migrated;
}

int Game::check_max_population() {
    int starved = 0;
    bool this_year = year % 2;
    bool next_year = (year + 1) % 2;
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (Field[i][j].population[next_year] > max_population) {
                starved += Field[i][j].population[next_year] - max_population;
                Field[i][j].population[next_year] = max_population;
            }
        }
    }
    return starved;
}
