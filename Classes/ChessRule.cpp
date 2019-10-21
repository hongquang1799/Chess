#include "ChessRule.h"

using namespace chess;

std::vector<ChessLocation> chess::ChessRule::availableLocationsFrom(const ChessLocation& location, const ChessState& state)
{
	std::vector<ChessLocation> result;
	size_t x = location.x;
	size_t y = location.y;

	int pc = state.at(location.x, location.y);
	size_t pieceType = PIECE(pc);
	int playerType = PLAYER(pc);

	switch (pieceType)
	{
	case ChessRule::PIECE_KING:
	{
		pushIfLocationEmptyOrContainEnemy(pc, x - 1, y - 1, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x, y - 1, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x + 1, y - 1, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x - 1, y, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x + 1, y, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x - 1, y + 1, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x, y + 1, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x + 1, y + 1, result, state);
	}
		break;
	case ChessRule::PIECE_QUEEN:
	{
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x + i, y + i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x - i, y - i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x - i, y + i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x + i, y - i, result, state))
				break;

		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x, y + i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x, y - i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x + i, y, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x - i, y, result, state))
				break;
	}
		break;
	case ChessRule::PIECE_BISHOP:
	{
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x + i, y + i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x - i, y - i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x - i, y + i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x + i, y - i, result, state))
				break;
	}
		break;
	case ChessRule::PIECE_KNIGHT:
	{
		pushIfLocationEmptyOrContainEnemy(pc, x - 2, y - 1, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x - 2, y + 1, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x + 2, y - 1, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x + 2, y + 1, result, state);

		pushIfLocationEmptyOrContainEnemy(pc, x - 1, y - 2, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x + 1, y - 2, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x - 1, y + 2, result, state);
		pushIfLocationEmptyOrContainEnemy(pc, x + 1, y + 2, result, state);
	}
		break;
	case ChessRule::PIECE_ROOK:
	{
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x, y + i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x, y - i, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x + i, y, result, state))
				break;
		for (int i = 1; i < 8; i++)
			if (!pushIfLocationEmptyOrContainEnemy(pc, x - i, y, result, state))
				break;
	}
		break;
	case ChessRule::PIECE_PAWN:
	{
		if (playerType == WHITE_PLAYER)
		{
			pushIfLocationEmpty(pc, x, y + 1, result, state);
			if (y == 1)
				pushIfLocationEmpty(pc, x, y + 2, result, state);
			pushIfLocationContainEnemy(pc, x - 1, y + 1, result, state);
			pushIfLocationContainEnemy(pc, x + 1, y + 1, result, state);
		}
		else if (playerType == BLACK_PLAYER)
		{
			pushIfLocationEmpty(pc, x, y - 1, result, state);
			if (y == 6)
				pushIfLocationEmpty(pc, x, y - 2, result, state);
			pushIfLocationContainEnemy(pc, x - 1, y - 1, result, state);
			pushIfLocationContainEnemy(pc, x + 1, y - 1, result, state);
		}
	}
		break;
	default:
		break;
	}


	return result;
}

chess::ChessDoneMove chess::ChessRule::move(const ChessLocation& from, const ChessLocation& to, ChessState& state)
{
	ChessDoneMove doneMove;
	doneMove.src = from;
	doneMove.dst = to;

	if (state.at(from.x, from.y) == 0)
		return doneMove;

	auto availableSteps = ChessRule::availableLocationsFrom(from, state);
	
	for (const ChessLocation& location : availableSteps)
	{
		if (to == location)
		{
			doneMove.valid = true;
			break;
		}
	}

	if (doneMove.valid)
	{
		doneMove.removedPieceCode = state.at(to.x, to.y);
		state.set(to.x, to.y, state.at(from.x, from.y));
		state.set(from.x, from.y, 0);
	}

	return doneMove;
}

chess::ChessDoneMove chess::ChessRule::moveFast(const ChessLocation& from, const ChessLocation& to, ChessState& state)
{
	ChessDoneMove doneMove;
	doneMove.src = from;
	doneMove.dst = to;
	doneMove.valid = true;
	doneMove.removedPieceCode = state.at(to.x, to.y);
	state.set(to.x, to.y, state.at(from.x, from.y));
	state.set(from.x, from.y, 0);

	return doneMove;
}

void chess::ChessRule::undo(const ChessDoneMove& move, ChessState& state)
{
	state.set(move.src.x, move.src.y, state.at(move.dst.x, move.dst.y));
	state.set(move.dst.x, move.dst.y, move.removedPieceCode);
}

bool chess::ChessRule::pushIfLocationEmptyOrContainEnemy(int pc, size_t x, size_t y,
	std::vector<ChessLocation>& result, const ChessState& state)
{
	if (x >= 0 && x < CHESS_COUNT && y >= 0 && y < CHESS_COUNT)
	{
		int pct = state.at(x, y);
		if (pct == 0)
		{
			result.push_back(ChessLocation(x, y));
			return true;
		}
		else if (PLAYER(pct) != PLAYER(pc))
		{
			result.push_back(ChessLocation(x, y));
			return false;
		}
	}
	return false;
}

bool chess::ChessRule::pushIfLocationEmpty(int pc, size_t x, size_t y,
	std::vector<ChessLocation>& result, const ChessState& state)
{
	if (x >= 0 && x < CHESS_COUNT && y >= 0 && y < CHESS_COUNT)
	{
		int pct = state.at(x, y);
		if (pct == 0)
		{
			result.push_back(ChessLocation(x, y));
			return true;
		}
	}
	return false;
}

bool chess::ChessRule::pushIfLocationContainEnemy(int pc, size_t x, size_t y,
	std::vector<ChessLocation>& result, const ChessState& state)
{
	if (x >= 0 && x < CHESS_COUNT && y >= 0 && y < CHESS_COUNT)
	{
		int pct = state.at(x, y);
		if (pct != 0 && PLAYER(pct) != PLAYER(pc))
		{
			result.push_back(ChessLocation(x, y));
			return false;
		}
	}
	return false;
}
