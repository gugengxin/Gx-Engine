﻿//
//  MGraphics.c
//  GXGameTest
//
//  Created by Gu Gengxin on 2017/6/1.
//  Copyright © 2017年 Gengxin Gu. All rights reserved.
//

#include "MGraphics.h"

GX_OBJECT_IMPLEMENT(MGraphics, Module);

void MGraphics::init()
{
    Module::init();
    m_Data = GDataBuffer::alloc();
    typedef struct {
        GVector3 pos;
        GColor4 clr;
    } MD;
    MD md[4];
    md[0].pos.set(-100.0f, -100.0f, 0.0f);
    md[1].pos.set(100.0f, -100.0f, 0.0f);
    md[2].pos.set(-100.0f, 100.0f, 0.0f);
    md[3].pos.set(100.0f, 100.0f, 0.0f);
    md[0].clr.set(0xFF, 0, 0, 0xFF);
    md[1].clr.set(0, 0xFF, 0, 0xFF);
    md[2].clr.set(0, 0, 0xFF, 0xFF);
    md[3].clr.set(0xFF, 0xFF, 0xFF, 0xFF);
    m_Data->changeBytes(sizeof(md));
    void* p = m_Data->map();
    memcpy(p, &md[0], sizeof(md));
    m_Data->unmap();
    m_Data->setOffset(0);
    m_Data->setStride(sizeof(md[0]));
}

void MGraphics::uninit()
{
    GO::release(m_Data);
    Module::uninit();
}

void MGraphics::idle()
{
    
}
void MGraphics::render3D(GCanvas* canvas,GContext& context)
{
    GSRGraphics* graph = context.getSRGraphics(GSRGraphics::ID_CAndCM);
    canvas->setColorMul(1, 1, 1, 1.0f);
    graph->draw(canvas, m_Data, GSRGraphics::IT_Float, GX_TRIANGLE_STRIP, 0, 4);
}
void MGraphics::render2D(GCanvas* canvas,GContext& context)
{
    
}
