#include <vector>
#include <string>
#include "SDL.h"

#include "GraphicsManager.h"
#include "Transformation.h"
#include "System/GameClock.h"

#ifndef __PDRAWABLE_H__
#define __PDRAWABLE_H__

#include <functional>

typedef std::vector<Transformation*>::iterator transformIterator;

class pDrawable
{
	public:
		virtual ~pDrawable();
		
		virtual void Update();
		bool InBounds(int32_t x, int32_t y);
		void Kill(int32_t time);
		void ClearTransforms();
		
		void Transform(TransformType type, int32_t starttime, int32_t endtime, int32_t startvalue, int32_t endvalue);
		void Scale(int32_t starttime, int32_t endtime, float start, float end);
		void Move(int32_t starttime, int32_t endtime, int32_t startx, int32_t starty, int32_t endx, int32_t endy);
		void Move(int32_t starttime, int32_t endtime, int32_t endx, int32_t endy);
		void Move(int32_t x, int32_t y);
		void Rotate(int32_t starttime, int32_t endtime, int32_t starta, int32_t enda);
		void Rotate(int32_t angle);
		
		void Show();
		void Show(int32_t time);
		void Show(int32_t starttime, int32_t endtime);
		void Hide();
		void Hide(int32_t time);
		void Hide(int32_t starttime, int32_t endtime);
		
		/*
		int32_t x() 		{ return mX; }
		int32_t y() 		{ return mY; }
		uint32_t width()		{ return mWidth; }
		uint32_t height()	{ return mHeight; }
		rgb color()		{ return mColor; }
		uint32_t alpha()		{ return mAlpha; }
		int32_t angle()		{ return mAngle; }
		float deltaz()	{ return mDeltaZ; }
		uint32_t* uv()		{ return mUV; }
		
		void SetTexture(Texture tex) { mTexture = tex; }
		
		//for spinners... a bit hacky but meh -.-
		int32_t& Angle() { return mAngle; }
		void SetCustomBounds(uint32_t* uv) { mUV = uv; }
		void SetHeight(int32_t height) { mHeight = height; }
		void SetWidth(int32_t width) { mWidth = width; }
		
		//for score sprites (more hax)
		void SetDeltaZ(float z) { mDeltaZ = z; }
		uint32_t& Alpha() { return mAlpha; }
		
		DrawOrigin origin()		{ return mOrigin; }
		FieldType fieldType()	{ return mFieldType; }
		Texture texture()		{ return mTexture; }
		*/
		bool Alive() { return mAlive; }

		virtual void Draw() = 0;
		
		int32_t X, Y;
		uint32_t Width, Height;
		SDL_Color Color;
		uint32_t Alpha;
		int32_t Angle;
		float Z;
		uint32_t* UV;

		void* Tag;
		
		DrawOrigin Origin;
		FieldType Field;
		TextureType Texture;

		void(*OnClick)(pDrawable* self, uint16_t x, uint16_t y);
        bool Clickable;
	
	protected:
		uint32_t mOrigWidth, mOrigHeight;
		bool mAlive;
		/*
		int32_t mX, mY;
		uint32_t mWidth, mHeight;
		rgb mColor;
		uint32_t mAlpha;
		int32_t mAngle;
		float mDeltaZ;
		uint32_t* mUV;
		
		DrawOrigin mOrigin;
		FieldType mFieldType;
		Texture mTexture;
		*/
		std::vector<Transformation*> mTransformations;
		
		void Transform(Transformation* transform);
};

#endif

