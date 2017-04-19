﻿//
// Created by Gengxin Gu on 16/5/9.
//

#ifndef AS_GXDIRECTX_H
#define AS_GXDIRECTX_H

#include "GXPrefix.h"
#include "GX3DAPIType.h"
#if defined(GX_DIRECTX)

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace GX {
	ID3D10Device* d3dDevice();
}

#define GX_POINTS			D3D10_PRIMITIVE_TOPOLOGY_POINTLIST
#define GX_LINES			D3D10_PRIMITIVE_TOPOLOGY_LINELIST
#define GX_LINE_STRIP		D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP
#define GX_TRIANGLES		D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
#define GX_TRIANGLE_STRIP	D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP

#define GX_INDEX16			DXGI_FORMAT_R16_UINT
#define GX_INDEX32			DXGI_FORMAT_R32_UINT

#define GX_FILTER_MIN_MAG_POINT			D3D10_FILTER_MIN_MAG_MIP_POINT
#define GX_FILTER_MIN_POINT_MAG_LINEAR	D3D10_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT
#define GX_FILTER_MIN_LINEAR_MAG_POINT	D3D10_FILTER_MIN_LINEAR_MAG_MIP_POINT
#define GX_FILTER_MIN_MAG_LINEAR		D3D10_FILTER_MIN_MAG_LINEAR_MIP_POINT

#define GX_WARP_REPEAT		D3D10_TEXTURE_ADDRESS_WRAP
#define GX_WARP_MIRRORED	D3D10_TEXTURE_ADDRESS_MIRROR
#define GX_WARP_CLAMP		D3D10_TEXTURE_ADDRESS_CLAMP

#endif
#endif //AS_GXDIRECTX_H
