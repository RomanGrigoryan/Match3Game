#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <cstdint>
#include <cassert>
#include <string>
#include <SFML/Graphics/Sprite.hpp>

enum class EObjectMask : uint8_t
{
	FIGURE = 0x0F,
	BOOSTER = 0xF0,
};

enum class EObjectType : uint8_t
{
	RED    = 0x01,
	GREEN  = 0x02,
	BLUE   = 0x03,
	ORANGE = 0x04,
	VIOLET = 0x05,
	HBOMB  = 0xF1,
	VBOMB  = 0xF2,
	SBOMB  = 0xF3,
};

class Object : public sf::Drawable
{
private:
	EObjectType _type;
	sf::Sprite _sprite;
	bool _selected;

public:
	Object(EObjectType type);

	EObjectType type() const;
	
	void setSelected(bool selected);

	bool isOfType(EObjectMask objectType) const;
	void resize(float size);

	void setPos(const sf::Vector2f& pos);
	const sf::Vector2f& getPos() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

typedef Object* ObjectPtr;

#endif // !OBJECT_HPP