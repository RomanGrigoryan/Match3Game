#include "Object.hpp"
#include "ResourceManager.hpp"
#include "GameController.hpp"

Object::Object(EObjectType type) 
	: _type(type)
	, _selected(false)
{
	_sprite.setTexture(ResourceManager::instance().textureByType(_type));
}

EObjectType Object::type() const
{ 
	return _type; 
}

bool Object::isOfType(EObjectMask objectType) const
{ 
	return (uint8_t)_type & (uint8_t)objectType; 
}

void Object::resize(float size)
{
	float xScaleFactor = size / (_sprite.getTextureRect().width);
	float yScaleFactor = size / (_sprite.getTextureRect().height);
	_sprite.setScale(xScaleFactor, yScaleFactor);
}

void Object::setPos(const sf::Vector2f& p)
{
	_sprite.setPosition(p);
}

const sf::Vector2f& Object::getPos() const
{
	return _sprite.getPosition();
}

void Object::setSelected(bool selected)
{
	_selected = selected;
	if (_selected)
		_sprite.setColor(sf::Color(255,255,255,150));
	else
		_sprite.setColor(sf::Color(255, 255, 255, 255));
}

void Object::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite, states);
}