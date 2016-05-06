﻿//
//  GApplication.h
//  GX
//
//  Created by Gu Gengxin on 16/5/1.
//  Copyright © 2016年 Gengxin Gu. All rights reserved.
//

#ifndef GApplication_h
#define GApplication_h

#include "GXPrefix.h"
#include "GArray.h"
#include "GWindow.h"
#include "GXCWnd.h"
#if defined(GX_OS_ANDROID)
#include <android/input.h>
#include <android/native_activity.h>
#include <native_app_glue/android_native_app_glue.h>
#endif

class GApplication {
public:
    class Delegate {
    public:

    };
public:
    static GApplication* shared();
    static void main(Delegate* dge);
private:
    GApplication();
    ~GApplication();
    
    void idle();
    
    void didFinishLaunching();
    void didBecomeActive();
    void willResignActive();
    void didEnterBackground();
    void willEnterForeground();
    void willTerminate();
    void didReceiveMemoryWarning();
public:

private:
    Delegate* m_Delegate;
    GArray<GWindow> m_Windows;
    
#if defined(GX_OS_APPLE)
    friend class _AppTimerBridge;
    void* m_Timer;
#elif defined(GX_OS_WINDOWS)
	static LRESULT CALLBACK winMsgWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void CALLBACK winTimerCallBack(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	void createWinMsgWndAndStart();
	void destroyWinMsgWnd();

	GX::CWnd m_MsgWnd;
	UINT m_TimerID;
#elif defined(GX_OS_ANDROID)
    friend class GJavaJAPI;
    friend void android_main(struct android_app*);

    static void androidHandleCmd(struct android_app* androidApp, int32_t cmd);
    static int32_t androidHandleInput(struct android_app* app, AInputEvent* event);

    void androidWindowCreated(ANativeWindow*);
    void androidWindowChanged();
    void androidWindowDestroyed();
    void androidWindowOnTouchEvent(jint action,jint pointerId,jfloat x,jfloat y);

    void androidAppStart();
    void androidAppResume();
    void androidAppPause();
    void androidAppStop();
    void androidAppDestroy();
    void androidAppLowMemory();
#endif
};



#endif /* GApplication_h */
