#pragma once

#include "SimpleMath.h"
#include <vector>

//#include "SpriteBatch.h"

struct DrawData2D;
struct GameData;

using namespace DirectX;
using namespace DirectX::SimpleMath;

class RenderTarget
{
public:

	RenderTarget(ID3D11Device* _device, int _width, int _height);
	~RenderTarget();

	ID3D11ShaderResourceView* GetShaderResourceView() { return m_textureSRV; }
	IDXGISurface* GetDXGISurface() { 
		IDXGISurface* ptr; 
		if(m_renderTargetTexture)
		{
			return SUCCEEDED(m_renderTargetTexture->QueryInterface(&ptr)) ? ptr : nullptr;
		}
		else { return nullptr; }
	}
	Vector2 GetTargetSize() { D3D11_TEXTURE2D_DESC* texture_desc;  m_renderTargetTexture->GetDesc(texture_desc); return { (float)texture_desc->Width, (float)texture_desc->Height }; }
	//clear render target
	void ClearRenderTarget(ID3D11DeviceContext* _pd3dImmediateContext, float red, float green, float blue, float alpha);

	// Begin/End a rendering to the target
	void Begin(ID3D11DeviceContext* _pd3dImmediateContext, bool _deletePass = false);
	void End(ID3D11DeviceContext* _pd3dImmediateContext);

	//map and unmap the texture
	//map th texture to an area on the CPU to allow its pixels to be accessed vis GetPixel() for collisions
	//and then unmap to allow it to be used as a RenderTarget again.
	void Map(ID3D11DeviceContext* _pd3dImmediateContext);
	void Unmap(ID3D11DeviceContext* _pd3dImmediateContext);
	UINT GetPitch();
	Color* GetPixel(int x, int y, GameData* _GD);

protected:

	int m_width, m_height;
	bool m_drawing = true;
	bool m_mapped = false;

	//stuff needed to use the staging texture
	UINT m_pitch;
	BYTE* m_byte = nullptr;
	D3D11_MAP eMapType = D3D11_MAP_READ;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	//the actual texture
	ID3D11Texture2D* m_renderTargetTexture = nullptr;

	//the staging texture for the collisions
	ID3D11Texture2D* m_stageTexture = nullptr;

	//pointer to this texture as a shader resource view
	ID3D11ShaderResourceView* m_textureSRV = nullptr;

	//pointer to this object as a render target view
	ID3D11RenderTargetView* m_renderTargetView = nullptr;

	//pointers to the old render target and depth stencil views
	ID3D11RenderTargetView* m_oldRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_oldDepthStencilView = nullptr;
};