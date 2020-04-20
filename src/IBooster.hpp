#ifndef BOOSTER_HPP
#define BOOSTER_HPP

#include "Object.hpp"
#include "Board.hpp"
#include <list>

class IBooster
{
protected:
	EObjectType _objectType;

public:
	IBooster();
	EObjectType getType() const;
	virtual std::list<Board::Index> getInfluence(const Board::Index& boosterIndx, const Board& board) = 0;
};



#endif // !BOOSTER_HPP