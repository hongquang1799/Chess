
#ifndef __CHESS_TYPE_CONFIG_H__
#define __CHESS_TYPE_CONFIG_H__

#include "cocos2d.h"
#include "ChessAi.h"

using namespace cocos2d;

namespace chess
{
	class ChessPlayer
	{
	public:
		ChessPlayer()
		{
			inTurn = false;
		}

		bool inTurn;
		std::string name;
		std::string type;
	protected:
	};

	class HumanChessPlayer : public ChessPlayer
	{
	public:
	};

	class AIChessPlayer : public ChessPlayer
	{
	public:
		ChessMove getBestMove(ChessState& state, size_t depth, bool maximizePlayer)
		{
			return ai.getBestMove(state, depth, maximizePlayer);
		}
	protected:
		ChessAi ai;
	};
}

#endif 
