﻿//
//  GFrameBuffer.cpp
//  GX
//
//  Created by GuGengxin.
// Copyright by gxengine.com
//

#include "GFrameBuffer.h"
//Down include other h file
#include "GContext.h"
#include "GTexture.h"

//Up include other h file
#include "GXGObject.h"

GX_GOBJECT_IMPLEMENT(GFrameBuffer, GCanvas);


GFrameBuffer::Handle::Handle()
{
#if defined(GX_OPENGL)
    m_Name = 0;
    m_DepthName=0;
#elif defined(GX_DIRECTX)
    m_Name=NULL;
    m_DepthName=NULL;
	m_DepthStencilState = NULL;
#elif defined(GX_METAL)
    m_Name=NULL;
    m_DepthName=NULL;
    m_StencilTex=NULL;
    m_DepthStencilState=NULL;
#endif
}

GFrameBuffer::Handle::~Handle()
{
    
}

bool GFrameBuffer::Handle::isValid()
{
#if defined(GX_OPENGL)
    return m_Name != 0;
#elif defined(GX_DIRECTX) || defined(GX_METAL)
    return m_Name != NULL;
#endif
}




GFrameBuffer::Node::Node()
{
    m_Context = NULL;
	m_TexTarget = NULL;
    m_BgdColor.set(0.0f, 0.0f, 0.0f, 1.0f);
}

GFrameBuffer::Node::~Node()
{

}

GFrameBuffer::Name GFrameBuffer::getName()
{
    if (m_Node) {
        return m_Node->getData().getName();
    }
#if defined(GX_OPENGL)
    return 0;
#else
    return NULL;
#endif
}
GTexture* GFrameBuffer::getTexture()
{
    if (m_Node) {
        return m_Node->getTexture();
    }
    return NULL;
}

float GFrameBuffer::Node::getWidth()
{
    return GX_CAST_S(float,m_TexTarget->getWidth());
}
float GFrameBuffer::Node::getHeight()
{
	return GX_CAST_S(float, m_TexTarget->getHeight());
}



GFrameBuffer::GFrameBuffer()
{
    m_Node=NULL;
    m_Density=1.0f;
#if defined(GX_OPENGL)
    m_PreBindName=0;
    m_PreViewport[0]=0;
    m_PreViewport[1]=0;
    m_PreViewport[2]=0;
    m_PreViewport[3]=0;
    m_PreCullFace=GX::DCullFaceNone;
    m_PreBlend=GX::DBlendNone;
#elif defined(GX_DIRECTX)
	m_PreName=NULL;
	m_PreDepthName=NULL;
	m_PreDepthStencilState=NULL;
	m_PreRasterState = NULL;
	m_PreBlendState=NULL;
	m_PreBlendFacotr[0] = 0.0f;
	m_PreBlendFacotr[1] = 0.0f;
	m_PreBlendFacotr[2] = 0.0f;
	m_PreBlendFacotr[3] = 0.0f;
	m_PreBlendMask=0xFFFFFFFF;
	//m_PreViewport;
#elif defined(GX_METAL)
    m_CommandBuffer=NULL;
#endif
}

GFrameBuffer::~GFrameBuffer()
{
    if (m_Node && m_Node->getContext()) {
        m_Node->getContext()->unloadFrameBufferNode(m_Node);
    }
}

void GFrameBuffer::enable2D(float width, float height)
{
	GCanvas::enable2D(width, height);
#if defined(GX_OPENGL)
    GMatrix4 m(1.0f,    0.0f,   0.0f,   0.0f,
               0.0f,    -1.0f,  0.0f,   0.0f,
               0.0f,    0.0f,   1.0f,   0.0f,
               0.0f,    height, 0.0f,   1.0f);
    getMatrix(MatrixProjection).multiply(m);
    /*
    getMatrix(MatrixProjection).translate(0, height, 0.0f);
	getMatrix(MatrixProjection).scale(1.0f, -1.0f, 1.0f);
    //*/
    
#endif
}
void GFrameBuffer::enable3D(float width, float height, float fovy, float zNear, float zFar)
{
	GCanvas::enable3D(width, height, fovy, zNear, zFar);
#if defined(GX_OPENGL)
	getMatrix(MatrixProjection).scale(1.0f, -1.0f, 1.0f);
#endif
}

float GFrameBuffer::getWidth()
{
    return m_Node->getWidth()/m_Density;
}
float GFrameBuffer::getHeight()
{
    return m_Node->getHeight()/m_Density;
}
float GFrameBuffer::getDensity()
{
    return m_Density;
}

const GColor4F& GFrameBuffer::getBackgroundColor() const
{
    return m_Node->getBackgroundColor();
}
void GFrameBuffer::setBackgroundColor(float r,float g,float b,float a)
{
    m_Node->setBackgroundColor(r, g, b, a);
}

void GFrameBuffer::config(Node* node)
{
    setNode(node);
}

#if defined(GX_OPENGL)

bool GFrameBuffer::openGLCFNeedReverse()
{
    return true;
}

#elif defined(GX_DIRECTX)

bool GFrameBuffer::direct3DCFNeedMultisampleEnabled()
{
	return false;
}

#elif defined(GX_METAL)

void* GFrameBuffer::metalNewRCE()
{
    id<MTLRenderCommandEncoder> rce = [GX_CAST_R(id<MTLCommandBuffer>, m_CommandBuffer) renderCommandEncoderWithDescriptor:GX_CAST_R(MTLRenderPassDescriptor*, m_Node->getData().getName())];
    if (m_Node->getData().isEnableDepth()) {
        [rce setDepthStencilState:GX_CAST_R(id<MTLDepthStencilState>, m_Node->getData().getDepthStencilState())];
    }
    [rce setViewport:*(MTLViewport*)&m_Viewport];
    applyToRCE(rce);
    return rce;
}

gint GFrameBuffer::metalBlendIndex()
{
    return GX_CAST_S(gint, getBlend());
}

#endif

bool GFrameBuffer::renderCheck()
{
    if (!m_Node) {
        return false;
    }
    return true;
}

void GFrameBuffer::renderBegin()
{
    GContext* context=m_Node->getContext();
    context->makeCurrent();
    
#if defined(GX_OPENGL)
    
    GX_glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&m_PreBindName);
    GX_glGetIntegerv(GL_VIEWPORT, m_PreViewport);
    m_PreCullFace=context->getCullFace();
    m_PreBlend=context->getBlend();
    
    GX_glBindFramebuffer(GL_FRAMEBUFFER, m_Node->getData().getName());
    
    const GColor4F& bgdClr=getBackgroundColor();
    GX_glClearColor(bgdClr.r, bgdClr.g, bgdClr.b, bgdClr.a);
    GX_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    openGLCFUpdate();
    openGLBDUpdate();
    
#elif defined(GX_DIRECTX)
	ID3D10Device* device = GX::direct3DDevice();

	device->OMGetRenderTargets(1, &m_PreName, &m_PreDepthName);
	device->OMGetDepthStencilState(&m_PreDepthStencilState,NULL);
	device->RSGetState(&m_PreRasterState);
	device->OMGetBlendState(&m_PreBlendState, m_PreBlendFacotr, &m_PreBlendMask);
	UINT numVP = 1;
	device->RSGetViewports(&numVP, &m_PreViewport);

	ID3D10RenderTargetView* rtv = m_Node->getData().getName();
	ID3D10DepthStencilView* dsv = m_Node->getData().getDepthName();
	// 设置深度模版状态，使其生效
	device->OMSetDepthStencilState(m_Node->getData().getDepthStencilState(), 1);
	// 绑定渲染目标视图和深度缓冲到渲染管线.
	
	device->OMSetRenderTargets(1, &rtv, dsv);
	//设置光栅化状态，使其生效
	direct3DCFUpdate();
	direct3DBDUpdate();

	const GColor4F& bgdClr = getBackgroundColor();
	const FLOAT color[] = { bgdClr.r, bgdClr.g, bgdClr.b, bgdClr.a };
	device->ClearRenderTargetView(rtv, color);
	device->ClearDepthStencilView(dsv, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
#elif defined(GX_METAL)
    
#define M_COMMAND_BUFFER()      GX_CAST_R(id<MTLCommandBuffer>, m_CommandBuffer)
#define M_RENDER_ENCODER()      GX_CAST_R(id<MTLRenderCommandEncoder>, m_RenderEncoder)
#define M_DEPTH_STENCIL_STATE() GX_CAST_R(id<MTLDepthStencilState>, m_DepthStencilState)
    
    GX::MetalBufferCache::shared()->pushBuffer();
    
    MTLRenderPassDescriptor* rpd=GX_CAST_R(MTLRenderPassDescriptor*, m_Node->getData().getName());
    MTLRenderPassColorAttachmentDescriptor *colorAttachment = rpd.colorAttachments[0];
    const GColor4F& bgdClr=m_Node->getBackgroundColor();
    colorAttachment.clearColor = MTLClearColorMake(bgdClr.r, bgdClr.g, bgdClr.b, bgdClr.a);
    
    MTLLoadAction cla=rpd.colorAttachments[0].loadAction;
    MTLLoadAction dla=rpd.depthAttachment.loadAction;
    MTLLoadAction sla=rpd.stencilAttachment.loadAction;
    
    m_CommandBuffer=[[GX_CAST_R(id<MTLCommandQueue>, m_Node->getContext()->getCommandQueue()) commandBuffer] retain];
    //clear
    id<MTLRenderCommandEncoder> rce=[M_COMMAND_BUFFER() renderCommandEncoderWithDescriptor:rpd];
    [rce endEncoding];
    
    rpd.colorAttachments[0].loadAction=cla;
    rpd.depthAttachment.loadAction=dla;
    rpd.stencilAttachment.loadAction=sla;
#endif
}

void GFrameBuffer::setViewport(float x, float y, float w, float h, float scale)
{
#if defined(GX_OPENGL)
    GX_glViewport((GLint)(x*scale), (GLint)(y*scale), (GLint)(w*scale), (GLint)(h*scale));
#elif defined(GX_DIRECTX)
	D3D10_VIEWPORT viewport;
	viewport.TopLeftX = (INT)(x*scale);
	viewport.TopLeftY = (INT)(y*scale);
	viewport.Width = (UINT)(w*scale);
	viewport.Height = (UINT)(h*scale);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	GX::direct3DDevice()->RSSetViewports(1, &viewport);
#elif defined(GX_METAL)
    m_Viewport.originX=x*scale;
    m_Viewport.originY=y*scale;
    m_Viewport.width=w*scale;
    m_Viewport.height=h*scale;
    m_Viewport.znear=0;
    m_Viewport.zfar=1;
#endif
}

void GFrameBuffer::renderEnd()
{
    GContext* context=m_Node->getContext();
#if defined(GX_OPENGL)
    context->setCullFace(m_PreCullFace);
    context->setBlend(m_PreBlend);
    GX_glBindFramebuffer(GL_FRAMEBUFFER, m_PreBindName);
    GX_glViewport(m_PreViewport[0], m_PreViewport[1], m_PreViewport[2], m_PreViewport[3]);
#elif defined(GX_DIRECTX)
	ID3D10Device* device = GX::direct3DDevice();
	device->OMSetRenderTargets(1, &m_PreName, m_PreDepthName);
	if (m_PreName) {
		m_PreName->Release();
	}
	if (m_PreDepthName) {
		m_PreDepthName->Release();
	}
	device->OMSetDepthStencilState(m_PreDepthStencilState, 1);
	if (m_PreDepthStencilState) {
		m_PreDepthStencilState->Release();
	}
	device->RSSetState(m_PreRasterState);
	if (m_PreRasterState) {
		m_PreRasterState->Release();
	}
	device->OMSetBlendState(m_PreBlendState, m_PreBlendFacotr, m_PreBlendMask);
	if (m_PreBlendState) {
		m_PreBlendState->Release();
	}
	device->RSSetViewports(1, &m_PreViewport);
#elif defined(GX_METAL)
    [GX_CAST_R(id<MTLCommandBuffer>, m_CommandBuffer) commit];
    [GX_CAST_R(id<MTLCommandBuffer>, m_CommandBuffer) waitUntilCompleted];
    
    [GX_CAST_R(id, m_CommandBuffer) release];
    m_CommandBuffer=NULL;
    
    GX::MetalBufferCache::shared()->popBuffer();
#endif
    context->makeClear();
}

