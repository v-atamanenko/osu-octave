#include "pDrawable.h"

pDrawable::~pDrawable() {
	ClearTransforms();
    delete UV;
}

void pDrawable::Update() {
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

bool pDrawable::InBounds(OOInt x, OOInt y) const {
	if (Field == FIELD_PLAY)
	{
		x -= (OOInt)GraphicsManager::PlayXOffset;
		y -= (OOInt)GraphicsManager::PlayYOffset;
	}

	auto s_w = (OOInt)Width;
	auto s_h = (OOInt)Height;
	OOInt s_x = X;
	OOInt s_y = Y;

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
			return x >= s_x && x <= s_x+s_w
				&& y >= s_y && y <= s_y+s_h;
		}
		
		case ORIGIN_CENTER:
		{
			OOInt halfWidth = s_w>>1;
			OOInt halfHeight = s_h>>1;
			
			return x >= s_x-halfWidth && x <= s_x+halfWidth
				&& y >= s_y-halfHeight && y <= s_y+halfHeight;
		}
		
		case ORIGIN_BOTTOMLEFT:
		{
			return x >= s_x && x <= s_x+s_w
				&& y >= s_y-s_h && y <= s_y;
		}
		
		default:
			return false;
	}
}

void pDrawable::Kill(OOTime time) {
	Transform(TR_KILL, time, time, 0, 0);
}

void pDrawable::ClearTransforms() {
	for (auto & mTransformation : mTransformations) {
		delete mTransformation;
	}
	
	mTransformations.clear();
}

void pDrawable::Transform(Transformation* transform) {
	mTransformations.push_back(transform);
}

void pDrawable::Transform(TransformType type, OOTime starttime, OOTime endtime, OOInt startvalue, OOInt endvalue) {
	Transform(new Transformation(type, starttime, endtime, startvalue, endvalue));
}

void pDrawable::Scale(OOTime starttime, OOTime endtime, OOFloat start, OOFloat end) {
	Transform(TR_SCALEX, starttime, endtime, (OOInt)round((OOFloat)mOrigWidth*start), (OOInt)round((OOFloat)mOrigWidth*end));
	Transform(TR_SCALEY, starttime, endtime, (OOInt)round((OOFloat)mOrigHeight*start), (OOInt)round((OOFloat)mOrigHeight*end));
}

void pDrawable::Move(OOTime starttime, OOTime endtime, OOInt startx, OOInt starty, OOInt endx, OOInt endy) {
	Transform(TR_MOVEX, starttime, endtime, startx, endx);
	Transform(TR_MOVEY, starttime, endtime, starty, endy);
}

void pDrawable::Move(OOTime starttime, OOTime endtime, OOInt endx, OOInt endy) {
	Move(starttime, endtime, X, Y, endx, endy);
}

void pDrawable::Move(OOInt endx, OOInt endy) {
	X = endx;
	Y = endy;
}

void pDrawable::Rotate(OOTime starttime, OOTime endtime, OOInt starta, OOInt enda) {
	Transform(TR_ROTATE, starttime, endtime, starta, enda);
}

void pDrawable::Rotate(OOInt angle) {
	Transform(TR_ROTATE, GameClock::Clock().Time(), GameClock::Clock().Time(), angle, angle);
}

void pDrawable::Show() {
	Show(GameClock::Clock().Time());
}

void pDrawable::Show(OOTime time) {
	Transform(TR_FADE, time, time, 255, 255);
}

void pDrawable::Show(OOTime starttime, OOTime endtime) {
	Transform(TR_FADE, starttime, endtime, 0, 255);
}

void pDrawable::Hide() {
	Hide(GameClock::Clock().Time());
}

void pDrawable::Hide(OOTime time) {
	Transform(TR_FADE, time, time, 0, 0);
}

void pDrawable::Hide(OOTime starttime, OOTime endtime) {
	Transform(TR_FADE, starttime, endtime, 255, 0);
}

void pDrawable::Heartbeat(OOTime starttime, OOTime length, OOFloat min_scale, OOFloat max_scale) {
    auto time_a = (OOTime)round((OOFloat)starttime+((OOFloat)length/5));
    auto time_b = (OOTime)round((OOFloat)starttime+((OOFloat)length/2.5));
    auto time_c = (OOTime)round((OOFloat)starttime+((OOFloat)length/1.667));
    auto time_d = (OOTime)round((OOFloat)starttime+((OOFloat)length/1.25));

    Scale(starttime, time_a, min_scale, max_scale);
    Scale(time_a, time_b, max_scale, min_scale);
    Scale(time_b, time_c, min_scale, max_scale);
    Scale(time_c, time_d, max_scale, min_scale);
    Scale(time_d, starttime+length, min_scale, min_scale);
}
