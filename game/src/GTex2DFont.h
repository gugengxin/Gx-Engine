﻿//
//  GTex2DFont.h
//  GX
//
//  Created by GuGengxin.
//  Copyright by gxengine.com
//

#ifndef GTex2DFont_h
#define GTex2DFont_h

#include "GXPrefix.h"
#include "GFont.h"
#include "GFTFont.h"
#include "GTexture2D.h"
#include "GBuffer.h"


#include "GXGObject.h"
// Down can't include other h file

class GTex2DFont : public GFont
{
    GX_GOBJECT(GTex2DFont);
public:
    class Glyph : public GFont::Glyph {
        friend class GTex2DFont;
        GX_PRIVATE_GOBJECT(Glyph);
    public:
        virtual gint32 getWidth();
        virtual gint32 getHeight();
        virtual gint32 getHoriBearingX();
        virtual gint32 getHoriBearingY();
        virtual gint32 getVertBearingX();
        virtual gint32 getVertBearingY();
        virtual gint32 getOutlineSize();
        virtual guint32 getOutlinePointCount();
        virtual gint32 getOutlinePointX(guint32 index);
        virtual gint32 getOutlinePointY(guint32 index);
    private:
        void load(GTex2DFont* font,GFTFont::Glyph* ftGlyph);
        inline gint getUseNumber() {
            return m_UseNumber;
        }
        inline void addUseNumber(gint step) {
            m_UseNumber+=step;
        }
        void render();
    public:
        GBuffer* getBuffer();
        GTexture2D* getTexture();
        GTexture2D* getOLTexture();
    private:
        gint   m_UseNumber;
    private:
        GFTFont::Glyph* m_FTGlyph;
        GBuffer* m_Buffer;
        GTexture2D* m_Tex2D;
        GTexture2D* m_OLTex2D;
    };
public:
    void create(GFTFont* ftFont);
public:
    virtual gint32 getScaleX();
    virtual gint32 getScaleY();
    virtual gint32 getHeight();
    virtual gint32 getAscender();
    virtual gint32 getDescender();
    virtual gint32 getAdvanceH(guint32 index);
    virtual gint32 getAdvanceV(guint32 index);
    virtual bool hasKerning();
    virtual gint32 getKerningX(guint32 index,guint32 next);
    virtual bool hasOutline();
    virtual gint32 getOutlineSize();
public:
    virtual guint32 getIndex(guint32 code);
    virtual guint32 getVariantIndex(guint32 code,guint32 variation_selector);
    virtual guint32 getGlyphCount();
    virtual GFont::Glyph* getGlyph(guint32 index);
    virtual bool getGlyphName(guint32 index, char* name, guint32 size);
    virtual bool getGlyphNameIndex(guint32* idxOut, const char *name, gint32 len);
private:
    GFTFont*  m_FTFont;
    GArray<Glyph> m_GlyphCache;
};

// Up can't include other h file
#include "GXGObjectUD.h"

#endif /* GTex2DFont_h */
