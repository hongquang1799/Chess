
#ifndef __CHESS_RULE_H__
#define __CHESS_RULE_H__

#include "stdio.h"
#include "stdlib.h"
#include "vector"

#define null NULL

#define CHESS_COUNT 8

#define PIECE(x) (x > 0 ? x : -x)
#define PLAYER(x) (x > 0 ? 1 : (x < 0 ? -1 : 0))

#define MAX_OF(x, y) (((x) < (y)) ? (y) : (x))
#define MIN_OF(x, y) (((x) > (y)) ? (y) : (x))

namespace chess
{
	struct ChessLocation
	{
		size_t x;
		size_t y;

		ChessLocation()
		{
			x = y = 0;
		}

		ChessLocation(size_t x, size_t y)
		{
			this->x = x;
			this->y = y;
		}

		const bool operator==(const ChessLocation& target) const
		{
			return x == target.x && y == target.y;
		}

		const bool operator!=(const ChessLocation& target) const
		{
			return x != target.x || y != target.y;
		}
	};

	struct ChessMove
	{
		ChessLocation	src;
		ChessLocation	dst;

		ChessMove()
		{

		}

		ChessMove(const ChessLocation& src, const ChessLocation& dst)
		{
			this->src = src;
			this->dst = dst;
		}
	};

	struct ChessDoneMove
	{
		ChessDoneMove()
		{
			valid = false;
			removedPieceCode = 0;
		}

		bool			valid;
		int				removedPieceCode;
		ChessLocation	src;
		ChessLocation	dst;
		
	};

	struct ChessState
	{
		inline void set(size_t x, size_t y, int pieceCode)
		{
			code[x][y] = pieceCode;
		}

		inline int at(size_t x, size_t y) const 
		{
			return code[x][y];
		}

		int	code[CHESS_COUNT][CHESS_COUNT];
	};

	class ChessRule
	{
	public:
		static const int WHITE_PLAYER = 1;
		static const int BLACK_PLAYER = -1;

		static const size_t PIECE_KING = 1;
		static const size_t PIECE_QUEEN = 2;
		static const size_t PIECE_BISHOP = 3;
		static const size_t PIECE_KNIGHT = 4;
		static const size_t PIECE_ROOK = 5;
		static const size_t PIECE_PAWN = 6;

		static std::vector<ChessLocation> availableLocationsFrom(const ChessLocation& location, const ChessState& state);

		static ChessDoneMove move(const ChessLocation& from, const ChessLocation& to, ChessState& state);

		static ChessDoneMove moveFast(const ChessLocation& from, const ChessLocation& to, ChessState& state);

		static void undo(const ChessDoneMove& move, ChessState& state);
	private:
		static bool pushIfLocationEmptyOrContainEnemy(int pc, size_t x, size_t y,
			std::vector<ChessLocation>& result, const ChessState& state);;

		static bool pushIfLocationEmpty(int pc, size_t x, size_t y,
			std::vector<ChessLocation>& result, const ChessState& state);;

		static bool pushIfLocationContainEnemy(int pc, size_t x, size_t y,
			std::vector<ChessLocation>& result, const ChessState& state);;
	};
}

#endif
