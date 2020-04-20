#include "HorizontalBooster.hpp"

HorizontalBooster::HorizontalBooster()
	: IBooster()
{
	_objectType = EObjectType::HBOMB;
}

std::list<Board::Index> HorizontalBooster::getInfluence(const Board::Index& boosterIndx, const Board& board)
{
	std::list<Board::Index> result;

	for (int c = 0; c < board.getColumnCount(); ++c)
	{
		Board::Index index(boosterIndx.row, c);
		if (board.getObject(index) != nullptr)
			result.push_back(index);
	}
	return result;
}