#ifndef BOARD_HPP
#define BOARD_HPP

#include "Object.hpp"
#include <SFML/Graphics.hpp>
#include <utility>
#include <list>

class Board : public sf::Drawable
{
public:
	struct Index
	{
		Index() : row(-1), column(-1) {};
		Index(int r, int c) : row(r), column(c) {};
		bool isValid() const { return row != -1 && column != -1; };
		bool operator<(const Index& a) const { return (row < a.row) || ((row == a.row) && (column < a.column)); };
		bool operator==(const Index& a) const { return (row == a.row) && (column == a.column); };
		int row;
		int column;
	};

private:
	ObjectPtr* _objectMatrix;
	unsigned _rowCount;
	unsigned _columnCount;

	std::vector<EObjectType> _availableTypes;

	sf::Texture _brightBackgroundTexture;
	float _brightXScaleFactor;
	float _brightYScaleFactor;

	sf::Texture _darkBackgroundTexture;
	float _darkXScaleFactor;
	float _darkYScaleFactor;

	sf::IntRect _boardRect;
	float _cellSize;

private:
	void drawBackgroundCell(unsigned i, sf::RenderTarget& target, sf::RenderStates states) const;
	void initBackgroundTextures();

	void moveToEmpty(const Index& from, const Index& to);

	sf::Vector2f posByIndex(const Index& index) const;
	bool checkForMatch(const Index& index, EObjectType type)  const;

public:
	Board(const std::vector<EObjectType>& availableTypes,
		unsigned rowCount,
		unsigned columnColumn,
		const sf::IntRect& boardRect);

	unsigned getRowCount() const;
	unsigned getColumnCount() const;
	void setObject(const Index& index, Object* object);
	Object* getObject(const Index& index) const;
	void deleteObject(const Index& index);
	void swapObjects(const Index& a, const Index& b, bool instant = false);
	std::list<Index> refill();
	void insertObject(EObjectType type, const Index& index, bool instant = false);

	Index indexByCordinates(int x, int y) const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void fill();
	void clean();
	void shuffle();
};

#endif // !BOARD_HPP