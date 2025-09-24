/*
** EPITECH PROJECT, 2025
** Zappy [WSL: Ubuntu-24.04]
** File description:
** Window
*/

#ifndef WINDOW_HPP_
    #define WINDOW_HPP_
    #include <string>
    #include "raylib.h"
    #include <atomic>

enum Scene {
    MENU,
    GAME,
    PAUSE,
    OPTION
};

class RayWindow {
    public:
        RayWindow(int, int, const std::string &);
    
        void start();
        void setup_camera(Vector3, Vector3, Vector3, int);
        Camera camera;

    private:

};

#endif /* !WINDOW_HPP_ */
