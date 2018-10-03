#pragma once

#include "Vec2.h"
#include "Graphics.h"
#include "Codex.h"
#include "Surface.h"
#include "Anim.h"
#include "Rect.h"
#include "Player.h"

class SpikeWall
{
public:
	enum class Dir
	{
		Left = true,
		Right = false
	};
public:
	SpikeWall( const Vei2& pos,Dir facingDir );

	void Update( const Rect& guyRect,float dt );
	void Draw( Graphics& gfx ) const;

	Vec2 GetPos() const;
	const Rect& GetRect() const;
private:
	static constexpr Vei2 size = { 80,40 };
	Vei2 pos;
	const Surface* const sprSheet = Codex<Surface>::RetrieveSurf( "Images/SpikeWallAnim.bmp",{ size.x * 6,size.y } );
	Anim shootSpikes;
	bool spiking = false;
	bool doneSpiking = false;
	Dir myDir;
	Rect hitbox;
};