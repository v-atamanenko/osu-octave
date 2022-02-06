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

		bool Alive() const { return mAlive; }

		virtual void Draw() = 0;
		
		int32_t X, Y;
		uint32_t Width, Height;
		SDL_Color Color;
		uint32_t Alpha;
		int32_t Angle;
		float Z;
		SDL_Rect* UV;

		void* Tag;
		
		DrawOrigin Origin;
		FieldType Field;
		TextureType Texture;

		void(*OnClick)(pDrawable* self, uint16_t x, uint16_t y);
        bool Clickable;
	
	protected:
		uint32_t mOrigWidth, mOrigHeight;
		bool mAlive;
		std::vector<Transformation*> mTransformations;
		
		void Transform(Transformation* transform);
};

#endif // __PDRAWABLE_H__
