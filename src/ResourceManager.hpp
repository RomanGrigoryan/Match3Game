#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>

#include "Object.hpp"

class ResourceManager
{
public:
	enum class BackgroundTextur
	{
		BRIGHT,
		DARK,
	};

private:
	std::map<EObjectType, sf::Texture> _textureMap;
	std::map<BackgroundTextur, sf::Texture> _backgroundTextureMap;
	sf::Font _font;

private:
	ResourceManager();
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager & operator=(ResourceManager &&) = delete;

	std::string fileNameByType(EObjectType type) const;

public:
	static ResourceManager& instance();
	const sf::Texture& textureByType(EObjectType type);
	const sf::Texture& backgroundTexturByType(BackgroundTextur type);
	const sf::Font& getFont() const;
};

#endif // !RESOURCEMANAGER_HPP