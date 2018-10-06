#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Graphics.h"
#include "Surface.h"
#include "Codex.h"

class Stairs
{
public:
	Stairs( const Vec2& pos );

	void Draw( Graphics& gfx ) const;

	void Spawn( const Vec2& whereTo );
	void Despawn();
	void Activate();
	void Deactivate();

	const Rect& GetRect() const;
	bool IsActive() const;
private:
	static constexpr Vei2 size = { 32,32 };
	Vec2 pos;
	Rect hitbox;
	bool activated = false;
	const Surface* const mySpr = Codex<Surface>::RetrieveSurf( "Images/Stairs.bmp",{ 4,4 } );
};