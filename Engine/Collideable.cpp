#include "Collideable.h"
#include <vector>

Collideable::Collideable( const TileMap& tileMapRef,
	Graphics& gfx )
	:
	map( tileMapRef ),
	gfx( gfx )
{}

bool Collideable::CheckCollision( Vec2& posToTest,
	const Vec2& moveDir ) const
{
	bool hit = false;

	if( map.GetTileAt( posToTest + Vec2{ moveDir.x,0.0f } ) ==
		TileMap::TileType::Empty )
	{
		posToTest.x += moveDir.x;
	}
	else hit = true;

	if( map.GetTileAt( posToTest + Vec2{ 0.0f,moveDir.y } ) ==
		TileMap::TileType::Empty )
	{
		posToTest.y += moveDir.y;
	}
	else hit = true;

	return( hit );
}

bool Collideable::CheckCollisionRect( Vec2& posRef,
	const Rect& rectToTest,const Vec2& moveDir ) const
{
	bool hit = false;

	// We gotta test every point on the rect.
	std::vector<Vec2> positions;
	positions.emplace_back( Vec2{ rectToTest.left,rectToTest.top } );
	positions.emplace_back( Vec2{ rectToTest.right,rectToTest.top } );
	positions.emplace_back( Vec2{ rectToTest.left,rectToTest.bottom } );
	positions.emplace_back( Vec2{ rectToTest.right,rectToTest.bottom } );

	// Start true, if any point cannot move then none can.
	bool isGoodX = true;
	bool isGoodY = true;
	
	// typedef-y stuff.
	const auto xMove = Vec2{ moveDir.x,0.0f };
	const auto yMove = Vec2{ 0.0f,moveDir.y };
	const auto empty = TileMap::TileType::Empty;

	for( const auto& pos : positions )
	{
		// If tile at one corner is not empty, don't go that way.
		if( map.GetTileAt( pos + xMove ) != empty )
		{
			isGoodX = false;
			hit = true;
		}

		if( map.GetTileAt( pos + yMove ) != empty )
		{
			isGoodY = false;
			hit = true;
		}
	}

	if( isGoodX ) posRef.x += xMove.x;
	if( isGoodY ) posRef.y += yMove.y;

	return( hit );
}
