#include "pDrawable.h"

pDrawable::~pDrawable()
{
	for (auto & mTransformation : mTransformations)
	{
		delete mTransformation;
	}
	
	if (UV != nullptr)
		delete UV;
}

void pDrawable::Update()
{
	for (auto tr : mTransformations)
	{
        tr->Update();
		
		if (tr->Active())
		{
			switch (tr->Type())
			{
				case TR_FADE:
					Alpha = tr->Value();
					break;
				case TR_MOVEX:
					X = tr->Value();
					break;
				case TR_MOVEY:
					Y = tr->Value();
					break;
				case TR_SCALEX:
					Width = tr->Value();
					break;
				case TR_SCALEY:
					Height = tr->Value();
					break;
				case TR_ROTATE:
					Angle = tr->Value();
					break;
				case TR_KILL:
					mAlive = false;
					break;
			}
		}
	}
}

bool pDrawable::InBounds(int32_t x, int32_t y)
{
	if (Field == FIELD_PLAY)
	{
		x -= GraphicsManager::PlayXOffset;
		y -= GraphicsManager::PlayYOffset;
	}
	
	switch (Origin)
	{
		case ORIGIN_TOPLEFT:
		{
			return x >= X && x <= X+Width
				&& y >= Y && y <= Y+Height;
		}
		
		case ORIGIN_CENTER:
		{
			int32_t halfWidth = Width>>1;
			int32_t halfHeight = Height>>1;
			
			return x >= X-halfWidth && x <= X+halfWidth
				&& y >= Y-halfHeight && y <= Y+halfHeight;
		}
		
		case ORIGIN_BOTTOMLEFT:
		{
			return x >= X && x <= X+Width
				&& y >= Y-Height && y <= Y;
		}
		
		default:
			return false;
	}
}

void pDrawable::Kill(int32_t time)
{
	Transform(TR_KILL, time, time, 0, 0);
}

void pDrawable::ClearTransforms()
{
	for (auto & mTransformation : mTransformations)
	{
		delete mTransformation;
	}
	
	mTransformations.clear();
}

void pDrawable::Transform(Transformation* transform)
{
	mTransformations.push_back(transform);
}

void pDrawable::Transform(TransformType type, int32_t starttime, int32_t endtime, int32_t startvalue, int32_t endvalue)
{
	Transform(new Transformation(type, starttime, endtime, startvalue, endvalue));
}

void pDrawable::Scale(int32_t starttime, int32_t endtime, float start, float end)
{
	Transform(TR_SCALEX, starttime, endtime, mOrigWidth*start, mOrigWidth*end);
	Transform(TR_SCALEY, starttime, endtime, mOrigHeight*start, mOrigHeight*end);
}

void pDrawable::Move(int32_t starttime, int32_t endtime, int32_t startx, int32_t starty, int32_t endx, int32_t endy)
{
	Transform(TR_MOVEX, starttime, endtime, startx, endx);
	Transform(TR_MOVEY, starttime, endtime, starty, endy);
}

void pDrawable::Move(int32_t starttime, int32_t endtime, int32_t endx, int32_t endy)
{
	Move(starttime, endtime, X, Y, endx, endy);
}

void pDrawable::Move(int32_t endx, int32_t endy)
{
	X = endx;
	Y = endy;
}

void pDrawable::Rotate(int32_t starttime, int32_t endtime, int32_t starta, int32_t enda)
{
	Transform(TR_ROTATE, starttime, endtime, starta, enda);
}

void pDrawable::Rotate(int32_t angle)
{
	Transform(TR_ROTATE, GameClock::Clock().Time(), GameClock::Clock().Time(), angle, angle);
}

void pDrawable::Show()
{
	Show(GameClock::Clock().Time());
}

void pDrawable::Show(int32_t time)
{
	Transform(TR_FADE, time, time, 31, 31);
}

void pDrawable::Show(int32_t starttime, int32_t endtime)
{
	Transform(TR_FADE, starttime, endtime, 0, 31);
}

void pDrawable::Hide()
{
	Hide(GameClock::Clock().Time());
}

void pDrawable::Hide(int32_t time)
{
	Transform(TR_FADE, time, time, 0, 0);
}

void pDrawable::Hide(int32_t starttime, int32_t endtime)
{
	Transform(TR_FADE, starttime, endtime, 31, 0);
}

