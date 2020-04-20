#ifndef TOPBARR_HPP
#define TOPBARR_HPP

#include <SFML/Graphics.hpp>
#include <list>

#include "Object.hpp"

class TopBar : public sf::Drawable 
{
    //top bar
    sf::RectangleShape _background;
    std::map<EObjectType, std::pair<sf::Sprite, sf::Text>> _objectives;
    sf::RectangleShape _moveLabelBackground;
    sf::Text _moveCountLabel;
    sf::Vector2f _size;
    sf::Vector2f _pos;
    float _objeciveSize;
    float _objeciveOffset;

public:
    TopBar(const sf::Vector2f& pos, const sf::Vector2f& size);
    
    void setMoveLabelText(const std::string& text);
    void setObjectiveLabelText(EObjectType type, const std::string& text);
    void insertObjectives(EObjectType type, const std::string& text);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // !TOPBARR_HPP

