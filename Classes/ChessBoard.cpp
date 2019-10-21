#include "ChessBoard.h"
#include "ChessAi.h"

USING_NS_CC;

using namespace chess;

chess::ChessBoard::ChessBoard() :
	availableTurn(1),
	selectedCell(NULL)
{
	memset(state.code, 0, sizeof(int) * CHESS_COUNT * CHESS_COUNT);
}

chess::ChessBoard::~ChessBoard()
{
}

ChessBoard* chess::ChessBoard::create(const ChessInterfaceConfig& interfaceConfig)
{
	ChessBoard* sprite = new (std::nothrow) ChessBoard();
	if (sprite && sprite->initWithInterfaceConfig(interfaceConfig))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

 bool chess::ChessBoard::thereIsASelectedPiece()
{
	return selectedCell != NULL;
}

bool chess::ChessBoard::initWithInterfaceConfig(const ChessInterfaceConfig& config)
{
	if (!Node::init())
		return false;

	interfaceConfig = config;

	setContentSize(Size(config.size, config.size));

	float cellSize = config.size / 8;
	Vec2 beginCellPosition = Vec2(-cellSize * 3.5f, -cellSize * 3.5f);

	for (int x = 0; x < CHESS_COUNT; x++)
	{
		for (int y = 0; y < CHESS_COUNT; y++)
		{
			Sprite* sprite = Sprite::create(config.cellImage);
			sprite->setPosition(beginCellPosition.x + cellSize * x, beginCellPosition.y + cellSize * y);
			sprite->setScale(cellSize / sprite->getContentSize().width, cellSize / sprite->getContentSize().height);
			addChild(sprite, 0);

			sprite->setColor(LOCATION_TO_CELLCOLOR3B(x, y));

			arrayOfCells[x][y].cellSprite = sprite;
			arrayOfCells[x][y].pieceSprite = NULL;
			arrayOfCells[x][y].x = x;
			arrayOfCells[x][y].y = y;
		}
	}

	restart();

	return true;
}

void chess::ChessBoard::normalizeCellSpriteColor()
{
	for (size_t x = 0; x < CHESS_COUNT; x++)
	{
		for (size_t y = 0; y < CHESS_COUNT; y++)
		{
			arrayOfCells[x][y].cellSprite->setColor(LOCATION_TO_CELLCOLOR3B(x, y));			
		}
	}
}

ChessLocation chess::ChessBoard::getLocationAtWorldLocation(Vec2 worldLocation)
{
	Vec2 localLocation = this->convertToNodeSpace(worldLocation);

	return getLocationAtLocalLocation(localLocation);
}

ChessLocation chess::ChessBoard::getLocationAtLocalLocation(Vec2 localLocation)
{
	size_t x = (size_t)(localLocation.x + interfaceConfig.size / 2) / (interfaceConfig.size / 8);
	size_t y = (size_t)(localLocation.y + interfaceConfig.size / 2) / (interfaceConfig.size / 8);

	return ChessLocation(x, y);
}

chess::ChessBoard::Cell& chess::ChessBoard::getCellAtLocation(ChessLocation location)
{
	return arrayOfCells[location.x][location.y];
}

bool chess::ChessBoard::movePiece(ChessLocation from, ChessLocation target, float speed)
{
	if (availableTurn == 0)
		return false;
	
	Cell& fromCell = getCellAtLocation(from);
	Cell& targetCell = getCellAtLocation(target);

	ChessDoneMove move = ChessRule::move(from, target, getState());
	
	if (move.valid)
	{
		int tempAvailableTurn = availableTurn;
		availableTurn = 0;
		stackOfChessMoves.push(move);

		// pre-move start

		// pre-move end

		std::function<void()> postMoveFunc = [&, from, target, tempAvailableTurn]()
		{
			if (targetCell.pieceSprite) 
				targetCell.pieceSprite->removeFromParent();

			targetCell.pieceSprite = fromCell.pieceSprite;				
			fromCell.pieceSprite = NULL;
			targetCell.pieceSprite->setPosition(targetCell.cellSprite->getPosition());

			availableTurn = -tempAvailableTurn;
			//CCLOG("turn %2d", availableTurn);
		};

	
		if (speed > 0.00001f)
		{
			auto srcPosition = fromCell.cellSprite->getPosition();
			auto dstPosition = targetCell.cellSprite->getPosition();
			auto moveTo = MoveTo::create(ccpDistance(srcPosition, dstPosition) / speed, dstPosition);
			auto postMove = CallFunc::create(postMoveFunc);
			auto moveAction = Sequence::create(moveTo, postMove, NULL);

			fromCell.pieceSprite->runAction(moveAction);
		}
		else
		{
			postMoveFunc();
		}

		//printState();
	}

	return move.valid;
}

void chess::ChessBoard::undo(float speed)
{
	if (availableTurn == 0)
		return;
	
	int tempAvailableTurn = availableTurn;
	availableTurn = 0;

	if (stackOfChessMoves.empty())
		return;

	ChessDoneMove move = stackOfChessMoves.top();
	stackOfChessMoves.pop();

	Cell& fromCell = getCellAtLocation(move.src);
	Cell& targetCell = getCellAtLocation(move.dst);

	ChessRule::undo(move, state);

	// pre-move start
	fromCell.pieceSprite = targetCell.pieceSprite;
	targetCell.pieceSprite = NULL;
	if (speed < 0.00001f)
		fromCell.pieceSprite->setPosition(fromCell.cellSprite->getPosition());

	if (move.removedPieceCode != 0)
		placePiece(move.dst.x, move.dst.y, move.removedPieceCode);
	// pre-move end

	std::function<void()> postMoveFunc = [&, tempAvailableTurn]()
	{
		availableTurn = -tempAvailableTurn;
	};

	if (speed > 0.00001f)
	{
		auto srcPosition = fromCell.cellSprite->getPosition();
		auto dstPosition = targetCell.cellSprite->getPosition();
		auto moveTo = MoveTo::create(ccpDistance(dstPosition, srcPosition) / speed, srcPosition);
		auto postMove = CallFunc::create(postMoveFunc);
		auto moveAction = Sequence::create(moveTo, postMove, NULL);

		fromCell.pieceSprite->runAction(moveAction);
	}
	else
	{
		postMoveFunc();
	}

	//printState();
}

void chess::ChessBoard::printState()
{
	CCLOG("ChessBoardState: %f", ChessAi::evaluate(state));
	for (int y = CHESS_COUNT-1; y >= 0; y--)
	{
	CCLOG("|%2d |%2d |%2d |%2d |%2d |%2d |%2d |%2d |", state.at(0, y), state.at(1, y), state.at(2, y), state.at(3, y),
	state.at(4, y), state.at(5, y), state.at(6, y), state.at(7, y));
	CCLOG("--------------------------------");
	}
}

ChessState& chess::ChessBoard::getState() 
{
	return state;
}

int chess::ChessBoard::getAvailableTurn()
{
	return availableTurn;
}

void chess::ChessBoard::restart()
{
	for (size_t x = 0; x < CHESS_COUNT; x++)
	{
		for (size_t y = 0; y < CHESS_COUNT; y++)
		{
			chess::ChessBoard::Cell& cell = arrayOfCells[x][y];

			if (cell.pieceSprite)
				cell.pieceSprite->removeFromParent();

			cell.pieceSprite = NULL;
		}	
	}

	placePiece(0, 0, ChessRule::PIECE_ROOK);
	placePiece(1, 0, ChessRule::PIECE_KNIGHT);
	placePiece(2, 0, ChessRule::PIECE_BISHOP);
	placePiece(3, 0, ChessRule::PIECE_QUEEN);
	placePiece(4, 0, ChessRule::PIECE_KING);
	placePiece(5, 0, ChessRule::PIECE_BISHOP);
	placePiece(6, 0, ChessRule::PIECE_KNIGHT);
	placePiece(7, 0, ChessRule::PIECE_ROOK);
	for (int i = 0; i < CHESS_COUNT; i++) 
		placePiece(i, 1, ChessRule::PIECE_PAWN);

	placePiece(0, 7, -ChessRule::PIECE_ROOK);
	placePiece(1, 7, -ChessRule::PIECE_KNIGHT);
	placePiece(2, 7, -ChessRule::PIECE_BISHOP);
	placePiece(3, 7, -ChessRule::PIECE_QUEEN);
	placePiece(4, 7, -ChessRule::PIECE_KING);
	placePiece(5, 7, -ChessRule::PIECE_BISHOP);
	placePiece(6, 7, -ChessRule::PIECE_KNIGHT);
	placePiece(7, 7, -ChessRule::PIECE_ROOK);
	for (int i = 0; i < CHESS_COUNT; i++) 
		placePiece(i, 6, -ChessRule::PIECE_PAWN);
}

ChessPiece * chess::ChessBoard::placePiece(size_t x, size_t y, int pieceCode)
{
	std::string fileName;

	size_t pieceType = PIECE(pieceCode);
	int playerType = PLAYER(pieceCode);

	switch (playerType)
	{
	case 1:
		switch (pieceType)
		{
		case ChessRule::PIECE_KING:
			fileName = interfaceConfig.whiteKingImage;
			break;
		case ChessRule::PIECE_QUEEN:
			fileName = interfaceConfig.whiteQueenImage;
			break;
		case ChessRule::PIECE_BISHOP:
			fileName = interfaceConfig.whiteBishopImage;
			break;
		case ChessRule::PIECE_KNIGHT:
			fileName = interfaceConfig.whiteKnightImage;
			break;
		case ChessRule::PIECE_ROOK:
			fileName = interfaceConfig.whiteRookImage;
			break;
		case ChessRule::PIECE_PAWN:
			fileName = interfaceConfig.whitePawnImage;
			break;
		default:
			break;
		}
		break;
	case -1:
		switch (pieceType)
		{
		case ChessRule::PIECE_KING:
			fileName = interfaceConfig.blackKingImage;
			break;
		case ChessRule::PIECE_QUEEN:
			fileName = interfaceConfig.blackQueenImage;
			break;
		case ChessRule::PIECE_BISHOP:
			fileName = interfaceConfig.blackBishopImage;
			break;
		case ChessRule::PIECE_KNIGHT:
			fileName = interfaceConfig.blackKnightImage;
			break;
		case ChessRule::PIECE_ROOK:
			fileName = interfaceConfig.blackRookImage;
			break;
		case ChessRule::PIECE_PAWN:
			fileName = interfaceConfig.blackPawnImage;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	state.set(x, y, pieceCode);
	Cell& cell = arrayOfCells[x][y];

	auto piece = ChessPiece::createWith(fileName);
	piece->setPosition(cell.cellSprite->getPosition());
	piece->setNormalScale(interfaceConfig.size / 8 / piece->getContentSize().width);
	piece->setPieceCode(pieceCode);
	addChild(piece, 1);
	cell.pieceSprite = piece;

	return piece;
}
