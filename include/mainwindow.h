#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <SDL.h>

// Singleton representing mainwindow
class MainWindow final {
    static int width;
    static int height;
    static std::string title;

    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;

    MainWindow();
    ~MainWindow() noexcept;
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

public:
    static MainWindow& instance(); // Get the singleton instance

    void clear();
    void update();

    SDL_Renderer* getRenderer() const noexcept {return gRenderer;}
    SDL_Window*     getWindow() const noexcept {return gWindow;  }
    int              getWidth() const noexcept {return MainWindow::width;    }
    int             getHeight() const noexcept {return MainWindow::height;   }
    static void  setWidth(int w) noexcept {MainWindow::width = w;}
    static void setHeight(int h) noexcept {MainWindow::height = h;}
    static void setSize(int w, int h) noexcept {setWidth(w); setHeight(h);}
    static void setTitle(const std::string& t) noexcept {MainWindow::title = t;}
};

#endif // MAINWINDOW_H
