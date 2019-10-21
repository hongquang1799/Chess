
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ChessBoard.h"
#include "ChessAi.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();

	auto layer = HelloWorld::create();
	
	scene->addChild(layer);

	return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	Director::getInstance()->setDisplayStats(false);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);


	chess::ChessInterfaceConfig config;
	config.size = visibleSize.height;
	config.cellImage = "cell_white_64.png";
	config.colorA = Color4B(237, 236, 210, 255);
	config.colorB = Color4B(185, 122, 87, 255);

	config.blackBishopImage = "black_bishop.png";
	config.blackKingImage = "black_king.png";
	config.blackKnightImage = "black_knight.png";
	config.blackPawnImage = "black_pawn.png";
	config.blackQueenImage = "black_queen.png";
	config.blackRookImage = "black_rook.png";

	config.whiteBishopImage = "white_bishop.png";
	config.whiteKingImage = "white_king.png";
	config.whiteKnightImage = "white_knight.png";
	config.whitePawnImage = "white_pawn.png";
	config.whiteQueenImage = "white_queen.png";
	config.whiteRookImage = "white_rook.png";

	matchType = ChessMatchType::HUMAN_VS_HUMAN;

	chessBoard = chess::ChessBoard::create(config);
	chessBoard->setPosition(visibleSize / 2);
	addChild(chessBoard);

	whitePlayer = new chess::HumanChessPlayer();
	whitePlayer->inTurn = true;
	whitePlayer->name = "WHITE";
	whitePlayer->type = "hm";

	/*whitePlayer = new chess::AIChessPlayer();
	whitePlayer->inTurn = true;
	whitePlayer->name = "WHITE";
	whitePlayer->type = "ai";*/

	blackPlayer = new chess::AIChessPlayer();
	blackPlayer->inTurn = false;
	blackPlayer->name = "BLACK";
	blackPlayer->type = "ai";

	chess::ChessLocation loc {6, 2};

	MAX(5, 3);

	float a = chess::ChessAi::pawnEval[loc.y][loc.x];

	scheduleUpdate();

    return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, Event* unused_event)
{
	int turn = chessBoard->getAvailableTurn();
	if (turn == 1 && whitePlayer->type == "hm")
	{
		return chessBoard->onPlayerTouchBegin(touch->getLocation(), 1);
	}
	else if (turn == -1 && blackPlayer->type == "ai")
	{
		return chessBoard->onPlayerTouchBegin(touch->getLocation(), -1);
	}

	return false;
}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, Event* unused_event)
{
	int turn = chessBoard->getAvailableTurn();
	if (turn == 1 && whitePlayer->type == "hm")
	{
		chessBoard->onPlayerTouchMove(touch->getLocation());
	}
	else if (turn == -1 && blackPlayer->type == "ai")
	{
		chessBoard->onPlayerTouchMove(touch->getLocation());
	}
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, Event* unused_event)
{
	int turn = chessBoard->getAvailableTurn();
	if (turn == 1 && whitePlayer->type == "hm")
	{
		chessBoard->onPlayerTouchEnd(touch->getLocation());
	}
	else if (turn == -1 && blackPlayer->type == "ai")
	{
		chessBoard->onPlayerTouchEnd(touch->getLocation());
	}
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_Z)
		chessBoard->undo(600);
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void HelloWorld::update(float dt)
{
	int turn = chessBoard->getAvailableTurn();
	if (turn == 1 && whitePlayer->type == "ai")
	{
		auto aiPlayer = static_cast<chess::AIChessPlayer*>(whitePlayer);
		auto move = aiPlayer->getBestMove(chessBoard->getState(), 2, true);
		chessBoard->movePiece(move.src, move.dst, 600);

		CCLOG("Turn: %d\nEvaluation Count: %d\nSrc:%d-%d\nDst:%d-%d\n",
			turn, chess::ChessAi::evaluationCount, move.src.x, move.src.y, move.dst.x, move.dst.y);
	}
	else if (turn == -1 && blackPlayer->type == "ai")
	{
		auto aiPlayer = static_cast<chess::AIChessPlayer*>(blackPlayer);
		auto move = aiPlayer->getBestMove(chessBoard->getState(), 3, false);
		chessBoard->movePiece(move.src, move.dst, 600);

		CCLOG("Turn: %d\nEvaluation Count: %d\nSrc:%d-%d\nDst:%d-%d",
			turn, chess::ChessAi::evaluationCount, move.src.x, move.src.y, move.dst.x, move.dst.y);
	}
}



