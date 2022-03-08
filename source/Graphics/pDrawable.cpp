#include "pDrawable.h"

pDrawable::~pDrawable() {
	ClearTransforms();
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
		x -= (int32_t)GraphicsManager::PlayXOffset;
		y -= (int32_t)GraphicsManager::PlayYOffset;
	}

	uint32_t s_w = Width;
	uint32_t s_h = Height;
	uint32_t s_x = X;
	uint32_t s_y = Y;

    if (ExtendedClickableArea) {
        // We use this flag for small-height buttons that are hard to tap with big fingers
        // Virtually extending height here a little.
        s_h += 30;
        s_y -= 15;
    }


	switch (Origin)
	{
		case ORIGIN_TOPLEFT:
		{
			return x >= s_x && x <= s_x+(int32_t)s_w
				&& y >= s_y && y <= s_y+(int32_t)s_h;
		}
		
		case ORIGIN_CENTER:
		{
			int32_t halfWidth = (int32_t)s_w>>1;
			int32_t halfHeight = (int32_t)s_h>>1;
			
			return x >= s_x-halfWidth && x <= s_x+halfWidth
				&& y >= s_y-halfHeight && y <= s_y+halfHeight;
		}
		
		case ORIGIN_BOTTOMLEFT:
		{
			return x >= s_x && x <= s_x+(int32_t)s_w
				&& y >= s_y-(int32_t)s_h && y <= s_y;
		}
		
		default:
			return false;
	}
}

void pDrawable::Kill(long time)
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

void pDrawable::Transform(TransformType type, long starttime, long endtime, int32_t startvalue, int32_t endvalue)
{
	Transform(new Transformation(type, starttime, endtime, startvalue, endvalue));
}

void pDrawable::Scale(long starttime, long endtime, float start, float end)
{
	Transform(TR_SCALEX, starttime, endtime, mOrigWidth*start, mOrigWidth*end);
	Transform(TR_SCALEY, starttime, endtime, mOrigHeight*start, mOrigHeight*end);
}

void pDrawable::Move(long starttime, long endtime, int32_t startx, int32_t starty, int32_t endx, int32_t endy)
{
	Transform(TR_MOVEX, starttime, endtime, startx, endx);
	Transform(TR_MOVEY, starttime, endtime, starty, endy);
}

void pDrawable::Move(long starttime, long endtime, int32_t endx, int32_t endy)
{
	Move(starttime, endtime, X, Y, endx, endy);
}

void pDrawable::Move(int32_t endx, int32_t endy)
{
	X = endx;
	Y = endy;
}

void pDrawable::Rotate(long starttime, long endtime, int32_t starta, int32_t enda)
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

void pDrawable::Show(long time)
{
	Transform(TR_FADE, time, time, 255, 255);
}

void pDrawable::Show(long starttime, long endtime)
{
	Transform(TR_FADE, starttime, endtime, 0, 255);
}

void pDrawable::Hide()
{
	Hide(GameClock::Clock().Time());
}

void pDrawable::Hide(long time)
{
	Transform(TR_FADE, time, time, 0, 0);
}

void pDrawable::Hide(long starttime, long endtime)
{
	Transform(TR_FADE, starttime, endtime, 255, 0);
}

