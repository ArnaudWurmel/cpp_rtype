//
// Created by Arnaud WURMEL on 15/01/2018.
//

#ifndef SERVERGAME_COLLIDERECT_HH
#define SERVERGAME_COLLIDERECT_HH

# include "Vector2.hh"

namespace rtp {
    class CollideRect {
    public:
        CollideRect(int, int, int, int);
        ~CollideRect();

    public:
        void    translate(Vector2<int> const&);
        int getX() const;
        int getY() const;
        int getWidth() const;
        int getHeight() const;

    private:
        int _x;
        int _y;
        int _width;
        int _height;
    };
}

#endif //SERVERGAME_COLLIDERECT_HH
