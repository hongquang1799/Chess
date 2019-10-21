#include "ChessAi.h"

using namespace chess;

size_t ChessAi::evaluationCount= 0;

const float ChessAi::pawnEval[CHESS_COUNT][CHESS_COUNT] = {
	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
	{5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
	{1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0},
	{0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5},
	{0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0},
	{0.5, -0.5, -1.0, 0.0, 0.0, -1.0, -0.5, 0.5},
	{0.5, 1.0, 1.0, -2.0, -2.0, 1.0, 1.0, 0.5},
	{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
};

const float ChessAi::knightEval[CHESS_COUNT][CHESS_COUNT] = {
	{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0},
	{-4.0, -2.0,  0.0,  0.0,  0.0,  0.0, -2.0, -4.0},
	{-3.0,  0.0,  1.0,  1.5,  1.5,  1.0,  0.0, -3.0},
	{-3.0,  0.5,  1.5,  2.0,  2.0,  1.5,  0.5, -3.0},
	{-3.0,  0.0,  1.5,  2.0,  2.0,  1.5,  0.0, -3.0},
	{-3.0,  0.5,  1.0,  1.5,  1.5,  1.0,  0.5, -3.0},
	{-4.0, -2.0,  0.0,  0.5,  0.5,  0.0, -2.0, -4.0},
	{-5.0, -4.0, -3.0, -3.0, -3.0, -3.0, -4.0, -5.0}
};

const float ChessAi::rookEval[CHESS_COUNT][CHESS_COUNT] = {
	{0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0},
	{0.5,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{-0.5,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -0.5},
	{0.0,   0.0, 0.0,  0.5,  0.5,  0.0,  0.0,  0.0}
};

const float ChessAi::bishopEval[CHESS_COUNT][CHESS_COUNT] = {
	{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0} ,
	{-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
	{-1.0,  0.0,  0.5,  1.0,  1.0,  0.5,  0.0, -1.0},
	{-1.0,  0.5,  0.5,  1.0,  1.0,  0.5,  0.5, -1.0},
	{-1.0,  0.0,  1.0,  1.0,  1.0,  1.0,  0.0, -1.0},
	{-1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0},
	{-1.0,  0.5,  0.0,  0.0,  0.0,  0.0,  0.5, -1.0},
	{-2.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -2.0}
};

const float ChessAi::queenEval[CHESS_COUNT][CHESS_COUNT] = {
	{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0},
	{-1.0,  0.0,  0.0,  0.0,  0.0,  0.0,  0.0, -1.0},
	{-1.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
	{-0.5,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
	{0.0,  0.0,  0.5,  0.5,  0.5,  0.5,  0.0, -0.5},
	{-1.0,  0.5,  0.5,  0.5,  0.5,  0.5,  0.0, -1.0},
	{-1.0,  0.0,  0.5,  0.0,  0.0,  0.0,  0.0, -1.0},
	{-2.0, -1.0, -1.0, -0.5, -0.5, -1.0, -1.0, -2.0}
};

const float ChessAi::kingEval[CHESS_COUNT][CHESS_COUNT] = {
	{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	{-3.0, -4.0, -4.0, -5.0, -5.0, -4.0, -4.0, -3.0},
	{-2.0, -3.0, -3.0, -4.0, -4.0, -3.0, -3.0, -2.0},
	{-1.0, -2.0, -2.0, -2.0, -2.0, -2.0, -2.0, -1.0},
	{2.0,  2.0,  0.0,  0.0,  0.0,  0.0,  2.0,  2.0},
	{2.0,  3.0,  1.0,  0.0,  0.0,  1.0,  3.0,  2.0}
};

float chess::ChessAi::evaluate(const ChessState& state)
{
	evaluationCount++;

	float result = 0.f;
	size_t pieceType;
	int x, y, yt, playerType, pieceCode;
	for (x = 0; x < CHESS_COUNT; x++)
	{
		for (y = 0; y < CHESS_COUNT; y++)
		{
			pieceCode = state.code[x][y];
			if (pieceCode == 0) continue;

			pieceType = PIECE(pieceCode);
			playerType = PLAYER(pieceCode);
			yt = (playerType == 1) ? 7 - y : y;
			switch (pieceType)
			{
			case ChessRule::PIECE_KING:
				result += (900 + kingEval[yt][x]) * playerType;
				break;
			case ChessRule::PIECE_QUEEN:
				result += (90 + queenEval[yt][x]) * playerType;
				break;
			case ChessRule::PIECE_BISHOP:
				result += (30 + bishopEval[yt][x]) * playerType;
				break;
			case ChessRule::PIECE_KNIGHT:
				result += (30 + knightEval[yt][x]) * playerType;
				break;
			case ChessRule::PIECE_ROOK:
				result += (50 + rookEval[yt][x]) * playerType;
				break;
			case ChessRule::PIECE_PAWN:
				result += (10 + pawnEval[yt][x]) * playerType;
				break;
			default:
				break;
			}
		}
	}

	return result;
}
