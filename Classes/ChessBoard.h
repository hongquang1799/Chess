
#ifndef __CHESS_BOARD_H__
#define __CHESS_BOARD_H__

#include "cocos2d.h"
#include "ChessPiece.h"
#include "ChessRule.h"

using namespace cocos2d;

#define LOCATION_TO_CELLCOLOR3B(x, y) ((x + CHESS_COUNT * y + y + 1) % 2 == 0) ? Color3B(interfaceConfig.colorA) : Color3B(interfaceConfig.colorB)

namespace chess
{
	struct ChessInterfaceConfig
	{
		float size;

		std::string whiteKingImage;
		std::string whitePawnImage;
		std::string whiteQueenImage;
		std::string whiteRookImage;
		std::string whiteBishopImage;
		std::string whiteKnightImage;

		std::string blackKingImage;
		std::string blackPawnImage;
		std::string blackQueenImage;
		std::string blackRookImage;
		std::string blackBishopImage;
		std::string blackKnightImage;

		std::string cellImage;

		Color4B colorA;
		Color4B colorB;
	};

	class ChessBoard : public cocos2d::Node
	{
	public:
		struct Cell
		{
			size_t		x, y;
			Sprite*		cellSprite;
			ChessPiece* pieceSprite;
		};

		static ChessBoard* create(const ChessInterfaceConfig& interfaceConfig);

		ChessLocation getLocationAtWorldLocation(Vec2 worldLocation);

		ChessLocation getLocationAtLocalLocation(Vec2 localLocation);

		Cell& getCellAtLocation(ChessLocation location);

		bool onPlayerTouchBegin(const Vec2& worldLocation, int playerCode)
		{
			auto location = getLocationAtWorldLocation(worldLocation);

			if (playerCode != 0 && PLAYER(state.at(location.x, location.y)) == playerCode)
			{
				selectedCell = &getCellAtLocation(location);

				auto availableLocations = ChessRule::availableLocationsFrom(location, state);
				for (ChessLocation& location : availableLocations)
				{
					highlightLocation(location);
				}
				return true;
			}
			else
			{
				selectedCell = null;
				return false;
			}
		}

		void onPlayerTouchMove(const Vec2& worldLocation)
		{
			if (selectedCell != null)
			{
				auto node = selectedCell->pieceSprite;
				node->setPosition(node->getParent()->convertToNodeSpace(worldLocation));
			}
		}

		void onPlayerTouchEnd(const Vec2& worldLocation)
		{
			if (selectedCell != null)
			{
				ChessLocation from = ChessLocation(selectedCell->x, selectedCell->y);
				ChessLocation to = getLocationAtWorldLocation(worldLocation);
				normalizeCellSpriteColor();
				bool result = movePiece(from, to, 0.f);
				if (result)
				{

				}
				else
				{
					selectedCell->pieceSprite->setPosition(selectedCell->cellSprite->getPosition());
				}
			}
		}

		bool movePiece(ChessLocation from, ChessLocation target, float speed);

		bool thereIsASelectedPiece();

		ChessState& getState();

		int getAvailableTurn();

		void restart();

		void undo(float speed = 0.f);

		void printState();
	protected:
		ChessBoard();

		virtual ~ChessBoard();

		virtual bool initWithInterfaceConfig(const ChessInterfaceConfig& interfaceConfig);

		void highlightLocation(const ChessLocation& location)
		{
			auto sprite = getCellAtLocation(location).cellSprite;
			Color3B color = sprite->getColor();
			float division = 3.0f;
			color.r = (GLubyte)(color.r / division);
			color.g = (GLubyte)(color.g / division);
			color.b = (GLubyte)(color.b / division);
			sprite->setColor(color);
		}

		void normalizeCellSpriteColor();

		ChessPiece * placePiece(size_t x, size_t y, int pieceCode);

		int availableTurn;
	
		Cell* selectedCell;

		ChessInterfaceConfig	interfaceConfig;
		
		std::stack<ChessDoneMove>	stackOfChessMoves;

		ChessState state;
		Cell					arrayOfCells[CHESS_COUNT][CHESS_COUNT];
		
	};
}

#endif // __HELLOWORLD_SCENE_H__
