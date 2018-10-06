#pragma once

#include "Vec2.h"
#include "Rect.h"
#include "Graphics.h"
#include "Surface.h"
#include "Codex.h"

class Key
{
public:
	Key( const Vei2& pos );

	void Draw( Graphics& gfx ) const;

	void SpawnAt( const Vei2& pos );
	void Despawn();

	const RectI& GetRect() const;
private:
	Vei2 pos;
	static constexpr Vei2 size = { 32,32 };
	RectI hitbox;
	bool active = true;
	const Surface* const spr = Codex<Surface>::RetrieveSurf( "Images/Key.bmp",{ 4,4 } );
};