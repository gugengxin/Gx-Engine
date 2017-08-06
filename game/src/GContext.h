﻿//
// Created by Gengxin Gu on 16/5/9.
//

#ifndef AS_GCONTEXT_H
#define AS_GCONTEXT_H

#include "GXPrefix.h"
#include "GOGLContext.h"
#include "GD3DContext.h"
#include "GMTLContext.h"
#include "GXContext.h"
#include "GSRGraphics.h"
#include "GSRTexture2D.h"
#include "GReader.h"
#include "GTexture2D.h"
#include "GFrameBuffer.h"
#include "GResourceManager.h"
#include "GString.h"
#include "GTex2DFont.h"

#include "GXGObject.h"

class GContext : public GX_CONTEXT_BASE, public GResourceManager
{
    friend class GX_CONTEXT_BASE;
    friend class GWindow;
    friend class GTexture;
    friend class GFrameBuffer;
private:
	enum ShaderID {
		SRIDGraphics = 0,
		SRIDGraphicsEnd = SRIDGraphics + GSRGraphics::IDCount - 1,

        SRIDTexture2D,
        SRIDTexture2DEnd= SRIDTexture2D+ GSRTexture2D::MMCount*2*2 - 1,

		_SRIDCount,
	};
    
    class NodeLoadObj : public GObject {
        GX_GOBJECT(NodeLoadObj);
    public:
        GContext* context;
    };
    class NodeUnloadObj : public GObject {
        GX_GOBJECT(NodeUnloadObj);
    public:
        GContext* context;
    };

    class T2DNodeLoadObjBase : public NodeLoadObj {
        GX_GOBJECT(T2DNodeLoadObjBase);
    public:
        GTexture2D::Parameter* param;
        GTexture2D::Node* nodeOut;
    };
    class T2DNodeLoadObj : public T2DNodeLoadObjBase {
        GX_GOBJECT(T2DNodeLoadObj);
    public:
        GDib* dib;
    };
    class T2DNodeLoadCreateObj : public T2DNodeLoadObjBase {
		GX_GOBJECT(T2DNodeLoadCreateObj);
    public:
		GX::PixelFormat pixelFormat;
		gint32          width;
		gint32          height;
    };
    class T2DNodeUnloadObj : public NodeUnloadObj {
        GX_GOBJECT(T2DNodeUnloadObj);
    public:
        GTexture2D::Node* nodeOut;
    };

    class FBNodeLoadObj : public NodeLoadObj {
        GX_GOBJECT(FBNodeLoadObj);
    public:
        GFrameBuffer::Node* nodeOut;

        GFrameBuffer::Use use;
        GTexture* texTarget;
    };
    class FBNodeUnloadObj : public NodeUnloadObj {
        GX_GOBJECT(FBNodeUnloadObj);
    public:
        GFrameBuffer::Node* nodeOut;
    };

private:
    static void initialize();
private:
    GContext();
	virtual ~GContext();
private:
    virtual bool create(GWindow* win);
    virtual void destroy();
#if defined(GX_OS_ANDROID)
private:
	virtual void androidDestroy();
	virtual void androidRecreate(GWindow* win);
#endif
//Resource Manager
private:
    typedef enum _Map {
        MapTex2D,
        MapTex2DFont,
        MapCount,
    } Map;
    virtual gint getMapCount() {
        return MapCount;
    }
    virtual GMap<GString, GObject>* getMap(gint index);
protected:
    virtual void didReceivedMemoryWarning();
private:
    GMap<GString,GObject> m_Maps[MapCount];
//Shader
public:
	GSRGraphics* getSRGraphics(GSRGraphics::ID srID);
    GSRTexture2D* getSRTexture2D(bool alphaOnly,bool colorMul,GSRTexture2D::MaskMode mm);
//Texture
public:
    GTexture2D* loadTexture2D(GReader* reader,GDib::FileType suggestFT,GTexture2D::Parameter* param);
    GTexture2D* loadTexture2D(GDib* dib,GTexture2D::Parameter* param);
    GTexture2D* loadTexture2D(GString* name,GDib::FileType suggestFT,GTexture2D::Parameter* param);
private:
	void addTextureNodeInMT(GTexture::Node* node);
	void removeTextureNodeInMT(GTexture::Node* node);
	bool loadTexture2DNode(GTexture::Node* node, GDib* dib, GTexture2D::Parameter* param);
	bool loadTexture2DNode(GTexture::Node* node, GX::PixelFormat pixelFormat, gint32 width, gint32 height, GTexture2D::Parameter* param);
	void unloadTextureNode(GTexture::Node* node);
	static void unloadTextureNodeInMT(GObject* obj);
//FrameBuffer
public:
    GFrameBuffer* loadFrameBuffer(gint32 width, gint32 height, GTexture2D::Parameter* param, GFrameBuffer::Use use);
private:
    void addFrameBufferNodeInMT(GFrameBuffer::Node* node);
    void removeFrameBufferNodeInMT(GFrameBuffer::Node* node);
	bool loadFrameBufferNode(GFrameBuffer::Node* node, GTexture* texTarget, GFrameBuffer::Use use);
    void unloadFrameBufferNode(GFrameBuffer::Node* node);
	static void unloadFrameBufferNodeInMT(GObject* obj);
//Tex2DFont
public:
    GTex2DFont* loadTex2DFont(GString* name, gint32 size, gint32 outlineSize);
private:
	GShader*                        m_Shaders[_SRIDCount];
	GDataList<GTexture::Handle>     m_Textures;
    GDataList<GFrameBuffer::Handle> m_FrameBuffers;
};

#include "GXGObjectUD.h"

#endif //AS_GCONTEXT_H
