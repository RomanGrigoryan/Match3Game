#include "VerticalBooster.hpp"

VerticalBooster::VerticalBooster()
	: IBooster()
{
	_objectType = EObjectType::VBOMB;
}

std::list<Board::Index> VerticalBooster::getInfluence(const Board::Index& boosterIndx, const Board& board)
{
	std::list<Board::Index> result;

	for (int r = 0; r < board.getRowCount(); ++r)
	{
		Board::Index index(r, boosterIndx.column);
		if (board.getObject(index) != nullptr)
			result.push_back(index);
	}
	return result;
}