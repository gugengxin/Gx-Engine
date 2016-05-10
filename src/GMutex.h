﻿//
//  GMutex.h
//  GX
//
//  Created by Gengxin Gu on 16/4/27.
//  Copyright © 2016年 Gengxin Gu. All rights reserved.
//

#ifndef GMutex_h
#define GMutex_h

#include "GXPrefix.h"
#include "GObject.h"
#include "GXPthread.h"

class GMutex : public GObject {
	friend class GRunLoop;
    GX_OBJECT(GMutex);
public:
    void lock();
    void unlock();
    
protected:
    GX::pthread_mutex_t* getMutexPtr() {
        return &m_PData;
    }
private:
    GX::pthread_mutex_t m_PData;
};



#endif /* GMutex_h */
