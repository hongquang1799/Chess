
#ifndef __CHESS_AI_H__
#define __CHESS_AI_H__

#include "ChessRule.h"

namespace chess
{
	class ChessAi
	{
	public:
		ChessAi()
		{

		}

		ChessMove getBestMove(ChessState& state, size_t depth, bool maximizePlayer)
		{
			evaluationCount = 0;
			this->depth = depth;

			minimax(state, depth, -1000000, 1000000, maximizePlayer);

			return bestMove;
		}

		float minimax(ChessState& state, size_t depth, float alpha, float beta, bool maximizePlayer)
		{
			if (depth == 0)
				return evaluate(state);

			if (maximizePlayer == true)
			{
				float maxEval = -99999999;
				for (size_t x = 0; x < 8; x++)
				{
					for (size_t y = 0; y < 8; y++)
					{
						if (PLAYER(state.at(x, y)) == 1)
						{
							ChessLocation from = ChessLocation(x, y);
							auto& targets = ChessRule::availableLocationsFrom(from, state);
							for (const ChessLocation& target : targets)
							{
								ChessDoneMove move = ChessRule::moveFast(from, target, state);
								float tempEval = minimax(state, depth - 1, alpha, beta, !maximizePlayer);
								if (maxEval < tempEval)
								{
									maxEval = tempEval;
									if (this->depth == depth)
									{
										bestMove.src = move.src;
										bestMove.dst = move.dst;
									}									
								}
								ChessRule::undo(move, state);
								alpha = MAX_OF(alpha, maxEval);
								if (beta <= alpha) {
									return maxEval;
								}
							}

						}
					}
				}
				return maxEval;			
			}
			else
			{
				float minEval = 99999999;
				for (size_t x = 0; x < 8; x++)
				{
					for (size_t y = 0; y < 8; y++)
					{
						if (PLAYER(state.at(x, y)) == -1)
						{
							ChessLocation from = ChessLocation(x, y);
							auto& targets = ChessRule::availableLocationsFrom(from, state);
							for (const ChessLocation& target : targets)
							{
								ChessDoneMove move = ChessRule::moveFast(from, target, state);
								float tempEval = minimax(state, depth - 1, alpha, beta, !maximizePlayer);
								if (minEval > tempEval)
								{
									minEval = tempEval;
									if (this->depth == depth)
									{
										bestMove.src = move.src;
										bestMove.dst = move.dst;
									}
								}
								ChessRule::undo(move, state);
								beta = MIN_OF(beta, minEval);
								if (beta <= alpha) {
									return minEval;
								}
							}

						}
					}
				}
				return minEval;
			}
		}

		static float evaluate(const ChessState& state);

		static size_t evaluationCount;
	
		static const float pawnEval[CHESS_COUNT][CHESS_COUNT];

		static const float knightEval[CHESS_COUNT][CHESS_COUNT];

		static const float rookEval[CHESS_COUNT][CHESS_COUNT];

		static const float bishopEval[CHESS_COUNT][CHESS_COUNT];

		static const float queenEval[CHESS_COUNT][CHESS_COUNT];

		static const float kingEval[CHESS_COUNT][CHESS_COUNT];

	private:
		size_t depth;
		ChessMove bestMove;

	};
}

#endif
