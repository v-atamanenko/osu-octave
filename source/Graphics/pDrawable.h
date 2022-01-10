#include <nds.h>
#include <vector>
#include <string>

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
		bool InBounds(s32 x, s32 y);
		void Kill(s32 time);
		void ClearTransforms();
		
		void Transform(TransformType type, s32 starttime, s32 endtime, s32 startvalue, s32 endvalue);
		void Scale(s32 starttime, s32 endtime, float start, float end);
		void Move(s32 starttime, s32 endtime, s32 startx, s32 starty, s32 endx, s32 endy);
		void Move(s32 starttime, s32 endtime, s32 endx, s32 endy);
		void Move(s32 x, s32 y);
		void Rotate(s32 starttime, s32 endtime, s32 starta, s32 enda);
		void Rotate(s32 angle);
		
		void Show();
		void Show(s32 time);
		void Show(s32 starttime, s32 endtime);
		void Hide();
		void Hide(s32 time);
		void Hide(s32 starttime, s32 endtime);
		
		/*
		s32 x() 		{ return mX; }
		s32 y() 		{ return mY; }
		u32 width()		{ return mWidth; }
		u32 height()	{ return mHeight; }
		rgb color()		{ return mColor; }
		u32 alpha()		{ return mAlpha; }
		s32 angle()		{ return mAngle; }
		float deltaz()	{ return mDeltaZ; }
		u32* uv()		{ return mUV; }
		
		void SetTexture(Texture tex) { mTexture = tex; }
		
		//for spinners... a bit hacky but meh -.-
		s32& Angle() { return mAngle; }
		void SetCustomBounds(u32* uv) { mUV = uv; }
		void SetHeight(s32 height) { mHeight = height; }
		void SetWidth(s32 width) { mWidth = width; }
		
		//for score sprites (more hax)
		void SetDeltaZ(float z) { mDeltaZ = z; }
		u32& Alpha() { return mAlpha; }
		
		DrawOrigin origin()		{ return mOrigin; }
		FieldType fieldType()	{ return mFieldType; }
		Texture texture()		{ return mTexture; }
		*/
		bool Alive() { return mAlive; }

		virtual void Draw() = 0;
		
		s32 X, Y;
		u32 Width, Height;
		rgb Color;
		u32 Alpha;
		s32 Angle;
		float Z;
		u32* UV;

		void* Tag;
		
		DrawOrigin Origin;
		FieldType Field;
		TextureType Texture;

		void(*OnClick)(pDrawable* self, u16 x, u16 y);
        bool Clickable;
	
	protected:
		u32 mOrigWidth, mOrigHeight;
		bool mAlive;
		/*
		s32 mX, mY;
		u32 mWidth, mHeight;
		rgb mColor;
		u32 mAlpha;
		s32 mAngle;
		float mDeltaZ;
		u32* mUV;
		
		DrawOrigin mOrigin;
		FieldType mFieldType;
		Texture mTexture;
		*/
		std::vector<Transformation*> mTransformations;
		
		void Transform(Transformation* transform);
};

#endif

