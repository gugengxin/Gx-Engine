﻿//
//  GObject.cpp
//  GX
//
//  Created by Gengxin Gu on 16/4/19.
//  Copyright © 2016年 Gengxin Gu. All rights reserved.
//

#include "GObject.h"
#include "GThread.h"
#include <pthread.h>


static pthread_mutex_t g_Mutex;
static pthread_once_t key_once=PTHREAD_ONCE_INIT;
static void keyCreate()
{
    pthread_mutex_init(&g_Mutex, NULL);
}

typedef struct __ObjExData {
	gint32 refCount;
	//pthread_mutex_t mutex;
} _ObjExData;

static inline void _ObjExDataInit(_ObjExData* om)
{
	om->refCount = 1;
}

static inline void _ObjExDataFina(_ObjExData* om)
{
}



void GObject::retain(GObject* obj)
{
	if (obj) {
		__ObjExData* p = GX_CAST_R(__ObjExData*, obj) - 1;
        
        pthread_once(&key_once,keyCreate);
        pthread_mutex_lock(&g_Mutex);
        
		++p->refCount;
        
        pthread_mutex_unlock(&g_Mutex);
	}
}
void GObject::release(GObject* obj)
{
	if (obj) {
		__ObjExData* p = GX_CAST_R(__ObjExData*, obj) - 1;
        
        pthread_once(&key_once,keyCreate);
        pthread_mutex_lock(&g_Mutex);
		
        --p->refCount;
		
        pthread_mutex_unlock(&g_Mutex);
        
        if (p->refCount <= 0) {
			delete obj;
		}
	}
}
void GObject::autorelease(GObject* obj)
{
    if (obj) {
        GThread::current()->pushARObj(obj);
    }
}

gint32 GObject::refCount(GObject* obj)
{
	if (obj) {
		__ObjExData* p = GX_CAST_R(__ObjExData*, obj) - 1;
		return p->refCount;
	}
	return 0;
}

void* GObject::gnew(size_t size)
{
	void* p = malloc(size + sizeof(_ObjExData));
	_ObjExDataInit(GX_CAST_R(_ObjExData*,p));
	return GX_CAST_R(guint8*, p) + sizeof(_ObjExData);
}
void GObject::gdel(void* p)
{
	_ObjExData* exData = GX_CAST_R(_ObjExData*, p) - 1;
	_ObjExDataFina(exData);
	free(exData);
}


GClass GObject::gclass(sizeof(GObject),GX_CAST_R(GClass::Alloc,GObject::alloc),NULL);
GClass* GObject::getClass()
{
    return &gclass;
}
GObject* GObject::alloc()
{
    return new GObject();
}
GObject* GObject::autoAlloc()
{
    GObject* res=new GObject();
    autorelease(res);
    return res;
}
void* GObject::operator new(size_t size)
{
	return gnew(size);
}
void GObject::operator delete(void* p)
{
	gdel(p);
}

GObject::GObject()
{
    
}
GObject::~GObject()
{
    
}

bool  GObject::isMemberOfClass(GClass* pClass)
{
    return getClass()->isMemberOf(pClass);
}
bool  GObject::isMemberOfClass(GClass& cls)
{
    return getClass()->isMemberOf(&cls);
}
bool  GObject::isKindOfClass(GClass* pClass)
{
    return getClass()->isKindOf(pClass);
}

bool  GObject::isKindOfClass(GClass& cls)
{
    return getClass()->isKindOf(&cls);
}

guint GObject::getHash()
{
    return GX_CAST_R(guint, this);
}

bool GObject::isEqual(GObject* obj)
{
    return this==obj;
}

//void GObject::exLock()
//{
//	_ObjMutexLock(this);
//}
//void GObject::exUnlock()
//{
//	_ObjMutexUnlock(this);
//}






