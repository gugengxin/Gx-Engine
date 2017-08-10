﻿//
//  GTexture2D.cpp
//  GX
//
//  Created by Gengxin Gu on 16/6/17.
//  Copyright © 2016年 Gengxin Gu. All rights reserved.
//

#include "GTexture2D.h"


#include "GXGObject.h"

GX_GOBJECT_IMPLEMENT(GTexture2D, GTexture);


GTexture2D::GTexture2D()
{
    m_Width=0;
    m_Height=0;
}

GTexture2D::~GTexture2D()
{

}

GTexture2D * GTexture2D::autoCreate(GReader * reader, GDib::FileType suggestFT, GTexture2D::Parameter * param)
{
	GTexture2D* res = GTexture2D::alloc();
	if (res->create(reader, suggestFT, param)) {
		GO::autorelease(res);
	}
	else {
		GO::release(res);
		res = NULL;
	}
	return res;
}

GTexture2D * GTexture2D::autoCreate(GDib * dib, Parameter * param)
{
	GTexture2D* res = GTexture2D::alloc();
	if (res->create(dib, param)) {
		GO::autorelease(res);
	}
	else {
		GO::release(res);
		res = NULL;
	}
	return res;
}

GTexture2D * GTexture2D::autoCreate(GX::PixelFormat pixelFormat, gint32 width, gint32 height, Parameter * param)
{
	GTexture2D* res = GTexture2D::alloc();
	if (res->create(pixelFormat, width, height, param)) {
		GO::autorelease(res);
	}
	else {
		GO::release(res);
		res = NULL;
	}
	return res;
}

bool GTexture2D::create(GReader* reader, GDib::FileType suggestFT, GTexture2D::Parameter* param)
{
	GDib* dib = GDib::load(reader, suggestFT);
	if (dib) {
		return create(dib, param);
	}
	return false;
}

bool GTexture2D::create(GDib* dib, Parameter* param)
{
	dib = prepareDib(dib);
	if (!dib) {
		return false;
	}
	return create(dib->getDataPtr(), dib->getPixelFormat(), dib->getWidth(), dib->getHeight(), dib->getStride(), param);
}
bool GTexture2D::create(GX::PixelFormat pixelFormat, gint32 width, gint32 height, Parameter* param)
{
	return create(NULL, pixelFormat, width, height, 0, param);
}
void GTexture2D::destroy()
{

	GTexture::destroy();
}

GDib* GTexture2D::prepareDib(GDib* dib)
{
	if (dib) {
#if defined(GX_OPENGL)
#error
#elif defined(GX_DIRECTX)
		switch (dib->getPixelFormat()) {
		case GX::PixelFormatA8:
		case GX::PixelFormatBGR565:
		case GX::PixelFormatBGRA4444:
		case GX::PixelFormatBGRA5551:
		case GX::PixelFormatRGBA8888:
		case GX::PixelFormatBGRA8888:
			return dib;
		case GX::PixelFormatRGB565:
			return GDib::convert(dib, GX::PixelFormatBGR565);
		case GX::PixelFormatRGBA4444:
			return GDib::convert(dib, GX::PixelFormatBGRA4444);
		case GX::PixelFormatRGBA5551:
			return GDib::convert(dib, GX::PixelFormatBGRA5551);
		case GX::PixelFormatRGB888:
			return GDib::convert(dib, GX::PixelFormatRGBA8888);
		default:
			break;
		}
#elif defined(GX_METAL)
#if defined(GX_OS_IPHONE)
        switch (dib->getPixelFormat()) {
            case GX::PixelFormatA8:
            case GX::PixelFormatRGB565:
            case GX::PixelFormatRGBA4444:
            case GX::PixelFormatRGBA5551:
            case GX::PixelFormatRGBA8888:
            case GX::PixelFormatBGRA8888:
                return dib;
            case GX::PixelFormatBGR565:
                return GDib::convert(dib, GX::PixelFormatRGB565);
            case GX::PixelFormatBGRA4444:
                return GDib::convert(dib, GX::PixelFormatRGBA4444);
            case GX::PixelFormatBGRA5551:
                return GDib::convert(dib, GX::PixelFormatRGBA5551);
            case GX::PixelFormatRGB888:
                return GDib::convert(dib, GX::PixelFormatRGBA8888);
            default:
                break;
        }
#elif defined(GX_OS_MACOSX)
        switch (dib->getPixelFormat()) {
            case GX::PixelFormatA8:
            case GX::PixelFormatRGBA8888:
            case GX::PixelFormatBGRA8888:
                return dib;
            case GX::PixelFormatRGB565:
            case GX::PixelFormatBGR565:
            case GX::PixelFormatRGBA4444:
            case GX::PixelFormatBGRA4444:
            case GX::PixelFormatRGBA5551:
            case GX::PixelFormatBGRA5551:
            case GX::PixelFormatRGB888:
                return GDib::convert(dib, GX::PixelFormatRGBA8888);
            default:
                break;
        }
#endif
#endif
	}
	return NULL;
}

bool GTexture2D::create(const void * dibData, GX::PixelFormat pf, gint32 w, gint32 h, gint32 s, Parameter * param)
{
#if defined(GX_OPENGL)

#elif defined(GX_DIRECTX)
	ID3D10Device* device = GX::direct3DDevice();

	D3D10_TEXTURE2D_DESC desc = { 0 };
	switch (pf)
	{
	case GX::PixelFormatA8:
		desc.Format = DXGI_FORMAT_A8_UNORM;
		break;
	case GX::PixelFormatBGR565:
		desc.Format = DXGI_FORMAT_B5G6R5_UNORM;
		break;
	case GX::PixelFormatBGRA4444:
		desc.Format = DXGI_FORMAT_B4G4R4A4_UNORM;
		break;
	case GX::PixelFormatBGRA5551:
		desc.Format = DXGI_FORMAT_B5G5R5A1_UNORM;
		break;
	case GX::PixelFormatRGBA8888:
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case GX::PixelFormatBGRA8888:
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		break;
	default:
		break;
	}
	if (desc.Format != 0) {
		desc.Width = w;
		desc.Height = h;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		if (dibData) {
			desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
			desc.Usage = D3D10_USAGE_IMMUTABLE;
		}
		else {//没有Dib数据，代表是为FrameBuffer创建
			desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
			desc.Usage = D3D10_USAGE_DEFAULT;
		}
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		ID3D10Texture2D* pTex2D = NULL;
		HRESULT hr;
		if (dibData) {
			D3D10_SUBRESOURCE_DATA initialData;
			initialData.pSysMem = dibData;
			initialData.SysMemPitch = s;
			initialData.SysMemSlicePitch = 0;
			hr = device->CreateTexture2D(&desc, &initialData, &pTex2D);
		}
		else {
			hr = device->CreateTexture2D(&desc, NULL, &pTex2D);
		}
		if (SUCCEEDED(hr)) {
			ID3D10ShaderResourceView* resSRV = NULL;
			hr = device->CreateShaderResourceView(pTex2D, NULL, &resSRV);
			pTex2D->Release();

			if (SUCCEEDED(hr)) {
				ID3D10SamplerState* resSS=NULL;
				D3D10_SAMPLER_DESC samplerDesc;
				if (param) {
					samplerDesc.Filter = (D3D10_FILTER)param->filter;
					samplerDesc.AddressU = (D3D10_TEXTURE_ADDRESS_MODE)param->wrapU;
					samplerDesc.AddressV = (D3D10_TEXTURE_ADDRESS_MODE)param->wrapV;
				}
				else {
					samplerDesc.Filter = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
					samplerDesc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
					samplerDesc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
				}
				samplerDesc.AddressW = samplerDesc.AddressV;
				samplerDesc.MipLODBias = 0.0f;
				samplerDesc.MaxAnisotropy = 1;
				samplerDesc.ComparisonFunc = D3D10_COMPARISON_ALWAYS;
				samplerDesc.BorderColor[0] = 0;
				samplerDesc.BorderColor[1] = 0;
				samplerDesc.BorderColor[2] = 0;
				samplerDesc.BorderColor[3] = 0;
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D10_FLOAT32_MAX;
				hr = device->CreateSamplerState(&samplerDesc, &resSS);
				if (SUCCEEDED(hr)) {
					GTexture::create(resSRV, resSS);
					resSRV->Release();
					resSS->Release();
					m_Width = w;
					m_Height = h;
					return true;
				}
				else {
					resSRV->Release();
				}
			}
		}
	}

	return false;

#elif defined(GX_METAL)
    
    MTLPixelFormat mpf=MTLPixelFormatInvalid;
    
    switch (pf)
    {
        case GX::PixelFormatA8:
            mpf=MTLPixelFormatA8Unorm;
            break;
#if defined(GX_OS_IPHONE)
        case GX::PixelFormatRGB565:
            mpf=MTLPixelFormatB5G6R5Unorm;
            break;
        case GX::PixelFormatRGBA4444:
            mpf=MTLPixelFormatABGR4Unorm;
            break;
        case GX::PixelFormatRGBA5551:
            mpf=MTLPixelFormatA1BGR5Unorm;
            break;
#endif
        case GX::PixelFormatRGBA8888:
            mpf=MTLPixelFormatRGBA8Unorm;
            break;
        case GX::PixelFormatBGRA8888:
            mpf=MTLPixelFormatBGRA8Unorm;
            break;
        default:
            break;
    }
    
    if (mpf!=MTLPixelFormatInvalid) {
        id <MTLDevice> device=GX::metalDevice();
        
        MTLTextureDescriptor* texDesc=[MTLTextureDescriptor texture2DDescriptorWithPixelFormat:mpf width:w height:h mipmapped:NO];
        
        if (!dibData && [texDesc respondsToSelector:@selector(setUsage:)]) {
            texDesc.usage=MTLTextureUsageShaderRead|MTLTextureUsageRenderTarget;
            texDesc.storageMode = MTLStorageModePrivate;
        }
        
        id<MTLTexture> tex=[device newTextureWithDescriptor:texDesc];
        if (tex) {
            if (dibData) {
                MTLRegion region = MTLRegionMake2D(0, 0, w, h);
                [tex replaceRegion:region mipmapLevel:0 withBytes:dibData bytesPerRow:s];
            }
            
            MTLSamplerDescriptor* sprDesc=[[MTLSamplerDescriptor alloc] init];
            if (param) {
                switch (param->filter) {
                    case GX_FILTER_MIN_MAG_POINT:
                    {
                        sprDesc.minFilter=MTLSamplerMinMagFilterNearest;
                        sprDesc.magFilter=MTLSamplerMinMagFilterNearest;
                    }
                        break;
                    case GX_FILTER_MIN_POINT_MAG_LINEAR:
                    {
                        sprDesc.minFilter=MTLSamplerMinMagFilterNearest;
                        sprDesc.magFilter=MTLSamplerMinMagFilterLinear;
                    }
                        break;
                    case GX_FILTER_MIN_LINEAR_MAG_POINT:
                    {
                        sprDesc.minFilter=MTLSamplerMinMagFilterLinear;
                        sprDesc.magFilter=MTLSamplerMinMagFilterNearest;
                    }
                        break;
                    default:
                    case GX_FILTER_MIN_MAG_LINEAR:
                    {
                        sprDesc.minFilter=MTLSamplerMinMagFilterLinear;
                        sprDesc.magFilter=MTLSamplerMinMagFilterLinear;
                    }
                        break;
                }
                sprDesc.sAddressMode=(MTLSamplerAddressMode)param->wrapU;
                sprDesc.tAddressMode=(MTLSamplerAddressMode)param->wrapV;
            }
            else {
                sprDesc.minFilter=MTLSamplerMinMagFilterLinear;
                sprDesc.magFilter=MTLSamplerMinMagFilterLinear;
                sprDesc.sAddressMode=MTLSamplerAddressModeClampToEdge;
                sprDesc.tAddressMode=MTLSamplerAddressModeClampToEdge;
            }
            id <MTLSamplerState> spr=[device newSamplerStateWithDescriptor:sprDesc];
            [sprDesc release];
            
            if (spr) {
                GTexture::create(tex, spr);
                m_Width = w;
                m_Height = h;
                return true;
            }
        }
    }
    
    return false;
    
#endif
}
