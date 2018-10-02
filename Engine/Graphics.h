/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Surface.h"
#include "Rect.h"
#include <cassert>

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	Color& GetPixel( int x,int y ) const;
	void PutPixel( int x,int y,Color c );
	void PutPixelAlpha( int x,int y,Color c,float alpha );
	void DrawRect( int x,int y,int width,int height,Color c );
	void DrawRectAlpha( int x,int y,int width,int height,Color c,float alpha );
	void DrawRectDim( int x1,int y1,int x2,int y2,Color c );
	void DrawCircle( int x,int y,int radius,Color c );
	void DrawLine( Vec2 p0,Vec2 p1,Color c );
	template<typename R>
	void DrawHitbox( const Rect_<R>& hitbox,Color c = { 255,160,0 } )
	{
		DrawLine( { float( hitbox.left ),float( hitbox.top ) },
			{ float( hitbox.right ),float( hitbox.top ) },c );
		DrawLine( { float( hitbox.right ),float( hitbox.top ) },
			{ float( hitbox.right ),float( hitbox.bottom ) },c );
		DrawLine( { float( hitbox.right ),float( hitbox.bottom ) },
			{ float( hitbox.left ),float( hitbox.bottom ) },c );
		DrawLine( { float( hitbox.left ),float( hitbox.bottom ) },
			{ float( hitbox.left ),float( hitbox.top ) },c );
	}
	template<typename E>
	void DrawSprite( int x,int y,const Surface& s,E effect,bool reversed = false )
	{
		DrawSprite( x,y,s.GetRect(),s,effect,reversed );
	}
	template<typename E>
	void DrawSprite( int x,int y,const RectI& srcRect,const Surface& s,E effect,bool reversed = false )
	{
		DrawSprite( x,y,srcRect,GetScreenRect(),s,effect,reversed );
	}
	template<typename E>
	void DrawSprite( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,E effect,bool reversed = false )
	{
		assert( srcRect.left >= 0 );
		assert( srcRect.right <= s.GetWidth() );
		assert( srcRect.top >= 0 );
		assert( srcRect.bottom <= s.GetHeight() );

		// Mirror in x depending on reversed bool switch.
		if( !reversed )
		{
			// Clipping is different depending on mirroring status.
			if( x < clip.left )
			{
				srcRect.left += clip.left - x;
				x = clip.left;
			}
			if( y < clip.top )
			{
				srcRect.top += clip.top - y;
				y = clip.top;
			}
			if( x + srcRect.GetWidth() > clip.right )
			{
				srcRect.right -= x + srcRect.GetWidth() - clip.right;
			}
			if( y + srcRect.GetHeight() > clip.bottom )
			{
				srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
			}
			for( int sy = srcRect.top; sy < srcRect.bottom; sy++ )
			{
				for( int sx = srcRect.left; sx < srcRect.right; sx++ )
				{
					effect(
						// No mirroring!
						s.GetPixel( sx,sy ),
						x + sx - srcRect.left,
						y + sy - srcRect.top,
						*this
					);
				}
			}
		}
		else
		{
			if( x < clip.left )
			{
				srcRect.right -= clip.left - x;
				x = clip.left;
			}
			if( y < clip.top )
			{
				srcRect.top += clip.top - y;
				y = clip.top;
			}
			if( x + srcRect.GetWidth() > clip.right )
			{
				srcRect.left += x + srcRect.GetWidth() - clip.right;
			}
			if( y + srcRect.GetHeight() > clip.bottom )
			{
				srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
			}
			const int xOffset = srcRect.left + srcRect.right - 1;
			for( int sy = srcRect.top; sy < srcRect.bottom; sy++ )
			{
				for( int sx = srcRect.left; sx < srcRect.right; sx++ )
				{
					effect(
						// Mirror in x.
						s.GetPixel( xOffset - sx,sy ),
						x + sx - srcRect.left,
						y + sy - srcRect.top,
						*this
					);
				}
			}
		}
	}
	
	void JSDrawImage( const Surface& image,int dx,int dy )
	{
		JSDrawImage( image,dx,dy,image.GetWidth(),image.GetHeight() );
	}
	void JSDrawImage( const Surface& image,int dx,int dy,int dWidth,int dHeight )
	{
		JSDrawImage( image,dx,dy,dWidth,dHeight,
			dx,dy,dWidth,dHeight );
	}
	void JSDrawImage( const Surface& image,int sx,int sy,int sWidth,int sHeight,int dx,int dy,int dWidth,int dHeight );
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 1000;
	static constexpr int ScreenHeight = 600;
	static constexpr int GameScreenWidth = 800;
	static constexpr int GameScreenHeight = 600;
	static RectI GetScreenRect();
};