#include "pSprite.h"

pSprite::pSprite(TextureType texture, s32 x, s32 y, u32 width, u32 height, DrawOrigin origin, FieldType fieldtype, rgb color, u32 alpha, float z)
{
	Texture = texture;
	X = x;
	Y = y;
	Width = mOrigWidth = width;
	Height = mOrigHeight = height;
	Origin = origin;
	Field = fieldtype;
	Color = color;
	Alpha = alpha;
	mDraw = true;
	Angle = 0;
	UV = NULL;
	Z = z;
    Clickable = false;
}

pSprite::~pSprite()
{
	for (transformIterator it = mTransformations.begin(); it != mTransformations.end(); ++it)
	{
		if (*it != NULL)
			delete *it;
	}
	
	if (UV != NULL)
		delete UV;
}

void pSprite::Update()
{
	for (transformIterator it = mTransformations.begin(); it != mTransformations.end(); ++it)
	{
		Transformation* tr = (*it);
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
					mDraw = false;
					break;
			}
		}
	}
}

bool pSprite::InBounds(s32 x, s32 y)
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
			s32 halfWidth = Width>>1;
			s32 halfHeight = Height>>1;
			
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

void pSprite::Kill(s32 time)
{
	Transform(TR_KILL, time, time, 0, 0);
}

void pSprite::ClearTransforms()
{
	for (transformIterator it = mTransformations.begin(); it != mTransformations.end(); ++it)
	{
		if (*it != NULL)
			delete *it;
	}
	
	mTransformations.clear();
}

void pSprite::Transform(Transformation* transform)
{
	mTransformations.push_back(transform);
}

void pSprite::Transform(TransformType type, s32 starttime, s32 endtime, s32 startvalue, s32 endvalue)
{
	Transform(new Transformation(type, starttime, endtime, startvalue, endvalue));
}

void pSprite::Scale(s32 starttime, s32 endtime, float start, float end)
{
	Transform(TR_SCALEX, starttime, endtime, mOrigWidth*start, mOrigWidth*end);
	Transform(TR_SCALEY, starttime, endtime, mOrigHeight*start, mOrigHeight*end);
}

void pSprite::Move(s32 starttime, s32 endtime, s32 startx, s32 starty, s32 endx, s32 endy)
{
	Transform(TR_MOVEX, starttime, endtime, startx, endx);
	Transform(TR_MOVEY, starttime, endtime, starty, endy);
}

void pSprite::Move(s32 starttime, s32 endtime, s32 endx, s32 endy)
{
	Move(starttime, endtime, X, Y, endx, endy);
}

void pSprite::Move(s32 endx, s32 endy)
{
	X = endx;
	Y = endy;
}

void pSprite::Rotate(s32 starttime, s32 endtime, s32 starta, s32 enda)
{
	Transform(TR_ROTATE, starttime, endtime, starta, enda);
}

void pSprite::Rotate(s32 angle)
{
	Transform(TR_ROTATE, GameClock::Clock().Time(), GameClock::Clock().Time(), angle, angle);
}

void pSprite::Show()
{
	Show(GameClock::Clock().Time());
}

void pSprite::Show(s32 time)
{
	Transform(TR_FADE, time, time, 31, 31);
}

void pSprite::Show(s32 starttime, s32 endtime)
{
	Transform(TR_FADE, starttime, endtime, 0, 31);
}

void pSprite::Hide()
{
	Hide(GameClock::Clock().Time());
}

void pSprite::Hide(s32 time)
{
	Transform(TR_FADE, time, time, 0, 0);
}

void pSprite::Hide(s32 starttime, s32 endtime)
{
	Transform(TR_FADE, starttime, endtime, 31, 0);
}

