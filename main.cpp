/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "game.h"
#include "game.cpp"
#include "SDL_init.cpp"

extern const bool DEBUG = 1;
extern const bool SPEED_CHECK = 0;

const int GAME_WIDTH = 100;
const int GAME_HEIGHT = 100;

extern bool PAUSE = true;

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
    {
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n");
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

            //Init game
            Game *game = new Game(GAME_WIDTH, GAME_HEIGHT);
            game->set_population(GAME_WIDTH / 2, GAME_HEIGHT / 2, 1000);
            //game->set_population(0, 0, 1000);

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					} else if ( e.type == SDL_KEYDOWN ) {
                        switch( e.key.keysym.sym ) {
                            case SDLK_SPACE:
                            if (PAUSE) {
                                PAUSE = false;
                            } else {
                                PAUSE = true;
                            }
                            break;
                        }
                    }
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

                clock_t start = clock();
                game->update();
                game->render();
                //std::cout << "sum : " << clock() - start << "ms" << std::endl;

				/*
                //Render green outlined quad
				SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );		
				SDL_RenderDrawRect( gRenderer, &outlineRect );
				
				//Draw blue horizontal line
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
				SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

				//Draw vertical line of yellow dots
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
				for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
				{
					SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
				}
                */

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
            delete game;
		}
	}

	//Free resources and close SDL
	close();
    
	return 0;
}
