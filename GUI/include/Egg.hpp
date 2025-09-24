/*
** EPITECH PROJECT, 2025
** GUI [WSL: Ubuntu-24.04]
** File description:
** Egg
*/

#ifndef EGG_HPP_
    #define EGG_HPP_
    #include <iostream>
    #include <vector>
    #include <memory>
    #include "Resource.hpp"
    #include "raylib.h"

class Egg {
    public:
        Egg(int id, Vector2 tile);
        ~Egg();

        void Draw();

        int getId() const;
        Vector3 getPosition() const;
        
    private:
        int _id;
        Type _type;
        Vector2 _tile;
        Vector3 _position;
        Model _model;
        float _scale;

};

#endif /* !EGG_HPP_ */
