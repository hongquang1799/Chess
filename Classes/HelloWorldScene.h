
#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ChessBoard.h"
#include "ChessTypeConfig.h"

enum class ChessMatchType
{
	HUMAN_VS_HUMAN,
	HUMAN_VS_AI,
	AI_VS_AI
};

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
  
    CREATE_FUNC(HelloWorld);

	bool onTouchBegan(cocos2d::Touch* touch, Event* unused_event) override;
	void onTouchMoved(cocos2d::Touch* touch, Event* unused_event) override;
	void onTouchEnded(cocos2d::Touch* touch, Event* unused_event) override;

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

	void update(float dt) override;
protected:
	HelloWorld() {}
	virtual ~HelloWorld() 
	{
		delete whitePlayer;
		delete blackPlayer;
	}

	chess::ChessBoard* chessBoard;

	//// HUMAN VS HUMAN
	//chess::ChessPlayer* whiteHuman;
	//chess::ChessPlayer* blackHuman;

	//// HUMAN VS AI
	//chess::ChessPlayer* human;
	//chess::ChessPlayer* ai;

	//// AI VS AI
	//chess::ChessPlayer* whiteAi;
	//chess::ChessPlayer* blackAi;

	chess::ChessPlayer* whitePlayer;
	chess::ChessPlayer* blackPlayer;

	ChessMatchType matchType;

	std::queue<chess::ChessMove> whiteStepQueue;
	std::queue<chess::ChessMove> blackStepQueue;
};

#endif // __HELLOWORLD_SCENE_H__
