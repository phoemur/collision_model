#include "mainwindow.h"
#include "ball.h"

#include <cstdlib>
#include <vector>
#include <memory>
#include <string>

int main(int argc, char** argv)
{
    int num = argc == 2 ? std::stoi(argv[1]): 200;

    MainWindow::setSize(640, 480);
    MainWindow::setTitle("Ball Collision test");
    auto& win = MainWindow::instance();

    // Create balls
    std::vector<std::unique_ptr<Ball>> vec;
    for (int i=0; i<num; ++i) {
        vec.emplace_back(new Ball (std::rand() % win.getWidth(), std::rand() % win.getHeight(), 3, 3 ));
    }

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if( e.type == SDL_QUIT ) {
                quit = true;
            }
        }

        win.clear();

        Ball::move_all();
        Ball::check_collisions();
        Ball::render_all();

        win.update();
    }

    return 0;
}
