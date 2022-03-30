#pragma once

#include <vector>
#include <string>
#include <functional>
#include "SDL.h"

#include "GraphicsManager.h"
#include "Transformation.h"
#include "System/GameClock.h"

class pDrawable
{
	public:
		virtual ~pDrawable();
		
		virtual void Update();
		[[nodiscard]] bool InBounds(OOInt x, OOInt y) const;
		void Kill(OOTime time);
		void ClearTransforms();
		
		void Transform(TransformType type, OOTime starttime, OOTime endtime, int32_t startvalue, int32_t endvalue);
		void Scale(OOTime starttime, OOTime endtime, OOFloat start, OOFloat end);
        void Heartbeat(OOTime starttime, OOTime length, OOFloat min_scale, OOFloat max_scale);
		void Move(OOTime starttime, OOTime endtime, OOInt startx, OOInt starty, OOInt endx, OOInt endy);
		void Move(OOTime starttime, OOTime endtime, OOInt endx, OOInt endy);
		void Move(OOInt x, OOInt y);
		void Rotate(OOTime starttime, OOTime endtime, OOInt starta, OOInt enda);
		void Rotate(OOInt angle);
		
		void Show();
		void Show(OOTime time);
		void Show(OOTime starttime, OOTime endtime);
		void Hide();
		void Hide(OOTime time);
		void Hide(OOTime starttime, OOTime endtime);

		[[nodiscard]] bool Alive() const { return mAlive; }

		virtual void Draw() = 0;

        OOInt X, Y;
		OOUInt Width, Height;
		SDL_Color Color;
		OOUShort Alpha;
		OOInt Angle;
		OOFloat Z;
		SDL_Rect* UV;

		std::string Tag;
        OOInt TagInt;
		
		DrawOrigin Origin;
		FieldType Field = FIELD_PLAY;
		TextureType Texture;

		void(*OnClick)(pDrawable* self, OOInt x, OOInt y);
        bool Clickable;
        bool ExtendedClickableArea = false;
	
	protected:
		OOUInt mOrigWidth, mOrigHeight;
		bool mAlive;
		std::vector<Transformation*> mTransformations;
		
		void Transform(Transformation* transform);
};
