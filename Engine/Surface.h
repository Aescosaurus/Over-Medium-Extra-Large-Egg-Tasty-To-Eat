#pragma once

#include "Colors.h"
#include <string>
#include "Rect.h"
#include <vector>

class Surface
{
public:
	Surface( const std::string& filename );
	Surface( const std::wstring& fileName );
	Surface( const Surface& other,const RectI& clip );
	Surface( const std::string& fileName,const RectI& clip );
	Surface( int width,int height );
	// Gives a surface loaded from filename expanded to xAmount and yAmount.
	Surface( const std::string& filename,int xAmount,int yAmount );
	Surface( const Surface& other,int width,int height );

	Surface( const Surface& ) = default;
	Surface& operator=( const Surface& ) = default;
	
	Surface( Surface&& donor );
	Surface& operator=( Surface&& rhs );

	template<typename E>
	void Draw( const Vei2& pos,class Graphics& gfx,E effect,bool reversed = false ) const
	{
		Draw( pos,gfx,effect,reversed );
	}
	template<typename E>
	void Draw( const Vei2& pos,class Graphics& gfx,const RectI& clip,E effect,bool reversed = false ) const
	{
		gfx.DrawSprite( pos.x,pos.y,GetRect(),Graphics::GetScreenRect(),
			*this,E,reversed );
	}

	void PutPixel( int x,int y,Color c );
	void DrawRect( int x,int y,int width,int height,Color c );

	Color GetPixel( int x,int y ) const;
	int GetWidth() const;
	int GetHeight() const;
	Vei2 GetSize() const;
	RectI GetRect() const;
	// Expand to width and height.  Must be multiples of original.
	Surface GetExpanded( int width,int height ) const;
	// Expand by xFactor and yFactor.
	Surface GetExpandedBy( int xFactor,int yFactor ) const;
	Surface GetInterpolated( int width,int height ) const;
private:
	std::vector<Color> pixels;
	int width;
	int height;
};