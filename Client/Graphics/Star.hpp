#ifndef STAR_HPP
#define STAR_HPP

#include <SFML/Graphics.hpp>

class Star {
public:
    Star();
    Star(sf::Uint16 const&, sf::Uint16 const&, sf::Uint16 const&);
    ~Star();

public:
    sf::Uint16 const& getX() const;
    sf::Uint16 const& getY() const;
    sf::Uint16 const& getSize() const;
    sf::Image const& getImage() const;
    sf::Image&  getImage();
    void setX(sf::Uint16 const&);
    void setY(sf::Uint16 const&);
    void moveY(sf::Uint16 const&);

private:
    sf::Uint16 _x;
    sf::Uint16 _y;
    sf::Uint16 _size;
    sf::Image _starImg;

};

#endif
