#include "ResourceManager.hpp"

#include <cassert>

ResourceManager::ResourceManager()
{
	_font.loadFromFile("../Resources/arialbd.ttf");
}

ResourceManager& ResourceManager::instance()
{
	static ResourceManager instance;
	return instance;
}

std::string ResourceManager::fileNameByType(EObjectType type) const
{
	switch (type)
	{
	case EObjectType::RED:    return std::string("../Resources/red.png");
	case EObjectType::GREEN:  return std::string("../Resources/green.png");
	case EObjectType::BLUE:   return std::string("../Resources/blue.png");
	case EObjectType::ORANGE: return std::string("../Resources/orange.png");
	case EObjectType::VIOLET: return std::string("../Resources/violet.png");
	case EObjectType::HBOMB:  return std::string("../Resources/h_bomb.png");
	case EObjectType::VBOMB:  return std::string("../Resources/v_bomb.png");
	case EObjectType::SBOMB:  return std::string("../Resources/bomb.png");
	default:
		assert(0);
		return std::string();
	}
}

const sf::Texture& ResourceManager::textureByType(EObjectType type)
{
	if (_textureMap.end() == _textureMap.find(type))
	{	
		_textureMap[type].loadFromFile(fileNameByType(type));
	}

	return _textureMap[type];
}

const sf::Texture& ResourceManager::backgroundTexturByType(ResourceManager::BackgroundTextur type)
{
	if (_backgroundTextureMap.end() == _backgroundTextureMap.find(type))
	{
		switch (type)
		{
		case BackgroundTextur::BRIGHT:
			_backgroundTextureMap[type].loadFromFile(std::string("../Resources/tile_2.png"));
			break;
		case BackgroundTextur::DARK:
			_backgroundTextureMap[type].loadFromFile(std::string("../Resources/tile_1.png"));
			break;
		default:
			break;
		}
	}
	return _backgroundTextureMap[type];
}

const sf::Font& ResourceManager::getFont() const
{
	return _font;
}