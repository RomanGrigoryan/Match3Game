#ifndef HORIZONTALBOOSTER_HPP
#define HORIZONTALBOOSTER_HPP

#include "IBooster.hpp"


class HorizontalBooster : public IBooster
{
public:
	HorizontalBooster();
	virtual std::list<Board::Index> getInfluence(const Board::Index& boosterIndx, const Board& board);
};


#endif // !HORIZONTALBOOSTER_HPP