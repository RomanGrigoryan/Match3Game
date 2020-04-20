#include "TopBar.hpp"
#include "ResourceManager.hpp"

TopBar::TopBar(const sf::Vector2f& pos, const sf::Vector2f& size) 
	: _pos(pos)
	, _size(size)
{
	_background.setPosition(pos);
	_background.setSize(_size);
	_background.setFillColor(sf::Color(150, 150, 150, 255));

	_moveCountLabel.setFont(ResourceManager::instance().getFont());

	_moveLabelBackground.setFillColor(sf::Color(100, 100, 100, 255));
	_moveLabelBackground.setSize(sf::Vector2f(_size.x / 5, size.y));
	_moveLabelBackground.setPosition(sf::Vector2f(_pos.x + (_size.x/2 - _moveLabelBackground.getSize().x/2), _pos.y));

	_moveCountLabel.setCharacterSize(_moveLabelBackground.getSize().x* 0.4);
	
	auto bSize = _moveLabelBackground.getSize();
	auto bPos = _moveLabelBackground.getPosition();
	auto localBounds = _moveCountLabel.getLocalBounds();
	
	_objeciveSize = ((bPos.x - _pos.x) / 3) *0.5f;
	_objeciveOffset = ((bPos.x - _pos.x) / 3) * 0.2f;
}

void TopBar::setMoveLabelText(const std::string& text)
{
	_moveCountLabel.setString(text);
	auto bSize = _moveLabelBackground.getSize();
	auto bPos = _moveLabelBackground.getPosition();
	auto localBounds = _moveCountLabel.getLocalBounds();
	_moveCountLabel.setOrigin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
	_moveCountLabel.setPosition(bPos.x + bSize.x / 2.0f, bPos.y + bSize.y / 2.0f);
}

void TopBar::setObjectiveLabelText(EObjectType type, const std::string& text)
{
	_objectives[type].second.setString(text);
}

void TopBar::insertObjectives(EObjectType type, const std::string& text)
{
	sf::Sprite sprite;
	auto sPos = sf::Vector2f(_pos.x + _objectives.size() * (_objeciveSize + _objeciveOffset), _pos.y );
	auto tPos = sf::Vector2f(_pos.x + _objectives.size() * (_objeciveSize + _objeciveOffset), _pos.y + _objeciveSize);

	sprite.setTexture(ResourceManager::instance().textureByType(type));
	auto objectivWScalingFactor = _objeciveSize / float(sprite.getTextureRect().width);
	auto objectivHScalingFactor = _objeciveSize / float(sprite.getTextureRect().height);
	sprite.setScale(sf::Vector2f(objectivWScalingFactor, objectivHScalingFactor));
	sprite.setPosition(sPos);
	
	sf::Text textLable;
	textLable.setFont(ResourceManager::instance().getFont());
	textLable.setString(text);
	textLable.setCharacterSize(_objeciveSize * 0.6);
	auto localBounds = textLable.getLocalBounds();
	textLable.setOrigin(localBounds.left + localBounds.width / 2.0f, localBounds.top + localBounds.height / 2.0f);
	textLable.setPosition(tPos.x + _objeciveSize / 2.0f, tPos.y + _objeciveSize / 2.0f);

	_objectives[type] = std::make_pair(sprite, textLable);
}

void TopBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_background, states);

	for (auto objective : _objectives)
	{
		target.draw(objective.second.first, states);
		target.draw(objective.second.second, states);
	}

	target.draw(_moveLabelBackground, states);
	target.draw(_moveCountLabel, states);
}