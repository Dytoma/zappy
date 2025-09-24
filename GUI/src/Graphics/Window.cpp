/*
** EPITECH PROJECT, 2025
** Zappy [WSL: Ubuntu-24.04]
** File description:
** Window
*/

#include "Window.hpp"

RayWindow::RayWindow(int width, int height, const std::string &name)
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(width, height, name.c_str());
    SetTargetFPS(60);
    camera = Camera();
    setup_camera(
        {10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45
    );
}

void RayWindow::setup_camera(Vector3 pos, Vector3 target, Vector3 up, int fov)
{
    camera.position = pos;
    camera.target = target;
    camera.up = up;
    camera.fovy = fov;
    camera.projection = CAMERA_PERSPECTIVE;
}