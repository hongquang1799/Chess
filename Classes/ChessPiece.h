
#ifndef __CHESS_PIECE_H__
#define __CHESS_PIECE_H__

#include "cocos2d.h"

using namespace cocos2d;

namespace chess
{
	class ChessPiece : public cocos2d::Sprite
	{
	public:
		static ChessPiece* createWith(const std::string& fileName);

		virtual bool initWithFile(const std::string& fileName) override;

		virtual void update(float dt) override;

		virtual void setNormalScale(float scale)
		{
			normalScale = scale;
			setScale(scale);
		}

		virtual void enableHighLight(bool flag)
		{
			highLightEnabled = flag;

			if (highLightEnabled)
			{
				highLightTime = 0.f;
			}
			else
			{
				setScale(normalScale);
			}
		}

		void setPieceCode(int pieceCode)
		{
			this->pieceCode = pieceCode;
		}

		int getPieceCode() const
		{
			return pieceCode;
		}
	protected:
		ChessPiece();

		virtual ~ChessPiece();

		bool highLightEnabled;
		float highLightTime;
		float normalScale;

		int pieceCode;
	};
}

#endif // __HELLOWORLD_SCENE_H__
