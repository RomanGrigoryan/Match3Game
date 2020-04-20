#ifndef VERTIALBOOSTER_HPP
#define VERTIALBOOSTER_HPP

#include "IBooster.hpp"


class VerticalBooster : public IBooster
{
public:
	VerticalBooster();
	virtual std::list<Board::Index> getInfluence(const Board::Index& boosterIndx, const Board& board);
};


#endif // !VERTIALBOOSTER_HPP