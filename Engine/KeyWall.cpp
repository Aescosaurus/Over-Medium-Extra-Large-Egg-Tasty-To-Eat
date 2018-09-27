#include "KeyWall.h"
#include "SpriteEffect.h"

const Surface KeyWall::wallSpr = Surface( "Images/KeyWall.bmp",40,40 );

KeyWall::KeyWall( const Vei2& pos,TileMap& map )
	:
	pos( pos ),
	myTileMap( map )
{
	const auto setPos = map.GetTilePos( this->pos );
	myTileMap.SetTile( setPos.x,setPos.y,TileMap::TileType::Wall );
}

void KeyWall::Draw( Graphics& gfx ) const
{
	if( !unlocked )
	{
		// gfx.DrawRect( pos.x,pos.y,
		// 	size.x,size.y,
		// 	Colors::Yellow );
		gfx.DrawSprite( pos.x,pos.y,wallSpr,
			SpriteEffect::Chroma( Colors::Magenta ) );
	}
}

void KeyWall::Unlock()
{
	unlocked = true;

	const auto setPos = myTileMap.GetTilePos( pos );
	myTileMap.SetTile( setPos.x,setPos.y,TileMap::TileType::Empty );
}
