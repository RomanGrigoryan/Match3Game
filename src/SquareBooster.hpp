#ifndef SQUAREBOOSTER_HPP
#define SQUAREBOOSTER_HPP

#include "IBooster.hpp"


class SquareBooster : public IBooster
{
public:
	SquareBooster();
	virtual std::list<Board::Index> getInfluence(const Board::Index& boosterIndx, const Board& board);
};


#endif // !SQUAREBOOSTER_HPP