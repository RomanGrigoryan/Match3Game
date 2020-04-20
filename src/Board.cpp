#include "Board.hpp"

#include <SFML/Graphics.hpp>
#include <chrono>
#include <random>
#include "AnimationManager.hpp"

Board::Board(const std::vector<EObjectType>& availableTypes
	, unsigned rowCount
    , unsigned columnCount
	, const sf::IntRect& boardRect)
	: _availableTypes(availableTypes)
	, _rowCount(rowCount)
	, _columnCount(columnCount)
	, _boardRect(boardRect)
{
	_cellSize = _boardRect.width / (float)std::max(_rowCount, columnCount);

	initBackgroundTextures();

	_objectMatrix = new ObjectPtr[_rowCount * _columnCount];
}

void Board::initBackgroundTextures()
{
	_brightBackgroundTexture.loadFromFile("../Resources/tile_2.png");
	_brightXScaleFactor = _cellSize / (_brightBackgroundTexture.getSize().x);
	_brightYScaleFactor = _cellSize / (_brightBackgroundTexture.getSize().y);

	_darkBackgroundTexture.loadFromFile("../Resources/tile_1.png");
	_darkXScaleFactor = _cellSize / (_darkBackgroundTexture.getSize().x);
	_darkYScaleFactor = _cellSize / (_darkBackgroundTexture.getSize().y);
}

unsigned Board::getRowCount() const
{
	return _rowCount;
}

unsigned Board::getColumnCount() const
{
	return _columnCount;
}

void Board::setObject(const Board::Index& index, Object* object)
{
	_objectMatrix[(index.row * _columnCount) + index.column] = object;
}

Object* Board::getObject(const Board::Index& index) const
{
	return _objectMatrix[(index.row * _columnCount) + index.column];
}

void Board::deleteObject(const Board::Index& index)
{
	delete _objectMatrix[(index.row * _columnCount) + index.column] ;
	_objectMatrix[(index.row * _columnCount) + index.column] = nullptr;
}

void Board::swapObjects(const Board::Index& a, const Board::Index& b, bool instant)
{
	ObjectPtr pAObject = _objectMatrix[(a.row * _columnCount) + a.column];
	ObjectPtr pBObject = _objectMatrix[(b.row * _columnCount) + b.column];
	sf::Vector2f aPos =  pAObject->getPos();
	sf::Vector2f bPos = pBObject->getPos();
	if (instant)
	{
		pAObject->setPos(bPos);
		pBObject->setPos(aPos);
	}
	else
	{
		AnimationManager::instance().pushBackAnimationTusk(AnimationTask(pAObject, bPos, 0.5));
		AnimationManager::instance().pushBackAnimationTusk(AnimationTask(pBObject, aPos, 0.5));
	}
	std::swap(_objectMatrix[(a.row * _columnCount) + a.column], _objectMatrix[(b.row * _columnCount) + b.column]);
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (unsigned i = 0; i < _rowCount * _columnCount; ++i)
	{
		drawBackgroundCell(i, target, states);
		if (_objectMatrix[i] != nullptr)
			_objectMatrix[i]->draw(target, states);
	}
}
sf::Vector2f Board::posByIndex(const Index& index) const
{
	return sf::Vector2f(_boardRect.left + (index.column * _cellSize), _boardRect.top + (index.row * _cellSize));
}
void Board::fill()
{
	for (unsigned row = 0; row < _rowCount; ++row)
	{
		for (unsigned column = 0; column < _columnCount; ++column)
		{
			Index index(row, column);
			EObjectType type(_availableTypes[(rand() % _availableTypes.size())]);
			while (checkForMatch(index, type))
				type = _availableTypes[(rand() % _availableTypes.size())];

			insertObject(type, index);
		}
	}
}
bool Board::checkForMatch(const Index& index, EObjectType type)  const
{
	std::vector<std::vector<Board::Index>> indexes =
	{
		{Index(index.row, index.column - 1), Index(index.row, index.column - 2)},
		{Index(index.row - 1, index.column), Index(index.row - 2, index.column)},
		{Index(index.row, index.column - 1), Index(index.row - 1, index.column - 1),  Index(index.row - 1, index.column)},

	};

	for (auto& matchVector : indexes){
		int counter = 0;
		for (auto& matchIndex : matchVector){
			if (matchIndex.isValid() && (getObject(matchIndex)->type() == type))
				counter++;
			else
				break;
		}
		if (counter == matchVector.size())
			return true;
	}
	

	return false;
}

void Board::insertObject(EObjectType type, const Index& index, bool instant)
{
	ObjectPtr obj = new Object(type);
	auto tempPos = posByIndex(index);

	tempPos.y -= (float)_boardRect.width;
	obj->setPos(tempPos);
	obj->resize(_cellSize);
	if (instant)
	{
		obj->setPos(posByIndex(index));
	} else {
		AnimationManager::instance().pushBackAnimationTusk(
			AnimationTask(
				obj,
				posByIndex(index),
				0.30f
			)
		);
	}
	
	_objectMatrix[(index.row * _columnCount) + index.column] = obj;
}

void Board::moveToEmpty(const Index& from, const Index& to)
{
	AnimationManager::instance().pushBackAnimationTusk(
		AnimationTask(
			_objectMatrix[(from.row * _columnCount) + from.column],
			posByIndex(to),
			0.30f
			)
	);
	_objectMatrix[(to.row * _columnCount) + to.column] = _objectMatrix[(from.row * _columnCount) + from.column];
	_objectMatrix[(from.row * _columnCount) + from.column] = nullptr;
}

void Board::drawBackgroundCell(unsigned i, sf::RenderTarget& target, sf::RenderStates states) const
{
	unsigned r = (i / _columnCount);
	unsigned c = (i % _columnCount);
	sf::Sprite sp;
	if ((r % 2) ^ (c % 2)) {
		sp.setTexture(_darkBackgroundTexture);
		sp.setScale(_darkXScaleFactor, _darkYScaleFactor);
	}
	else {
		sp.setTexture(_brightBackgroundTexture);
		sp.setScale(_brightXScaleFactor, _brightYScaleFactor);
	}
	sp.setPosition(sf::Vector2f(_boardRect.left + c * _cellSize, _boardRect.top + r * _cellSize));
	
	target.draw(sp, states);
}
std::list<Board::Index> Board::refill()
{
	std::list<Board::Index> newItems;
	for (int c = 0; c < _columnCount; c++)
	{
		int er = -1;
		for (int r = _rowCount - 1; r >= 0; r--)
		{
			int i = (r * _columnCount) + c;
			if (_objectMatrix[i] != nullptr) continue;
			er = r;
			while (r--)
			{
				if (_objectMatrix[(r * _columnCount) + c] != nullptr)
				{
					moveToEmpty(Index(r, c), Index(er, c));
					newItems.push_back(Index(er, c));
					er--;
					r = er;
				}
			}
		}
		while(er != -1)
		{
			unsigned typeIndex = (rand() % _availableTypes.size());
			insertObject(EObjectType(_availableTypes[typeIndex]), Index(er, c));
			newItems.push_back(Index(er, c));
			er--;
		}
	}
	return newItems;
}
void Board::clean()
{
	for (unsigned i = 0; i < _rowCount * _columnCount; ++i){
		if (_objectMatrix[i] != nullptr) {
			delete _objectMatrix[i];
			_objectMatrix[i] = nullptr;
		}
	}
}

Board::Index Board::indexByCordinates(int x, int y) const
{
	Index result;
	result.row = (y - _boardRect.top) / _cellSize;
	result.column  = (x - _boardRect.left) / _cellSize;
	return result;
}

void Board::shuffle()
{
	auto seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::shuffle(_objectMatrix, _objectMatrix + (_rowCount * _columnCount), std::default_random_engine(seed));

	for (unsigned i = 0; i < _rowCount * _columnCount; ++i)
	{
		unsigned r = (i / _columnCount);
		unsigned c = (i % _columnCount);

		AnimationManager::instance().pushBackAnimationTusk(
			AnimationTask(_objectMatrix[i]
			, posByIndex(Index(r, c))
			, 1.0f
			)
		);
	}
}