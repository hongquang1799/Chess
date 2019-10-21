#include "ChessPiece.h"

USING_NS_CC;

using namespace chess;

ChessPiece* chess::ChessPiece::createWith(const std::string& fileName)
{
	ChessPiece* sprite = new (std::nothrow) ChessPiece();
	if (sprite && sprite->initWithFile(fileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool chess::ChessPiece::initWithFile(const std::string& fileName)
{
	if (!Sprite::initWithFile(fileName))
		return false;

	scheduleUpdate();

	return true;
}

void chess::ChessPiece::update(float dt)
{
	if (highLightEnabled)
	{
		setScale(normalScale + 0.15f * sinf(highLightTime * 5.f));

		highLightTime += dt;
	}
}

chess::ChessPiece::ChessPiece() : pieceCode(0)
{
	highLightEnabled = false;
}

chess::ChessPiece::~ChessPiece()
{
}
