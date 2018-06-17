#include "mainwindow.h"
#include "exception.h"


#include <SDL_image.h>
#include <iostream>

// Static data
int MainWindow::height = 0;
int MainWindow::width = 0;
std::string MainWindow::title = " ";

MainWindow::MainWindow()
    : gWindow{nullptr}, gRenderer{nullptr}
{
    int res = SDL_Init(SDL_INIT_VIDEO);
    if (res < 0) {throw SDLException(std::string("SDL could not initialize! SDL Error: ") + SDL_GetError());}

    res = SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
    if (!res) {std::cerr << "Warning: Linear texture filtering not enabled!" << std::endl;}

    gWindow = SDL_CreateWindow( title.c_str(),
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                width,
                                height,
                                SDL_WINDOW_SHOWN );

    if (gWindow == nullptr) {
        throw SDLException(std::string("Could not create Window! SDL Error: ") + SDL_GetError());
    }

    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    if (gRenderer == nullptr) {
        throw SDLException(std::string("Renderer could not be created! SDL Error: ") + SDL_GetError());
    }

    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

    res = IMG_INIT_JPG | IMG_INIT_PNG;
    if( !( IMG_Init( res ) & res ) ) {
        throw SDLException(std::string("SDL_image could not initialize! SDL_image Error: ") + IMG_GetError());
    }
}

MainWindow::~MainWindow() noexcept
{
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );

    IMG_Quit();
    SDL_Quit();
}

MainWindow& MainWindow::instance()
{
    static MainWindow the_instance {};

    return the_instance;
}

void MainWindow::clear()
{
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );
}

void MainWindow::update()
{
    SDL_RenderPresent( gRenderer );
}
