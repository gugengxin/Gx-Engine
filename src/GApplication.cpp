﻿//
//  GApplication.cpp
//  GX
//
//  Created by Gu Gengxin on 16/5/1.
//  Copyright © 2016年 Gengxin Gu. All rights reserved.
//

#include "GApplication.h"
#if defined(GX_OS_IPHONE)
#import <UIKit/UIKit.h>
#elif defined(GX_OS_MACOSX)
#import <Cocoa/Cocoa.h>
#elif defined(GX_OS_ANDROID)
#elif defined(GX_OS_WINDOWS)
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#endif
#include "GThread.h"
#include "GLog.h"


#if defined(GX_OS_APPLE)

class _AppTimerBridge {
public:
    _AppTimerBridge(){}
    ~_AppTimerBridge(){}
    inline void SetTarget(GApplication* v) {
        m_Target=v;
    }
    inline void AppIdle() {
        m_Target->idle();
    }
    inline void AppDidBecomeActive() {
        m_Target->didBecomeActive();
    }
    inline void AppWillResignActive() {
        m_Target->willResignActive();
    }
    inline void AppDidEnterBackground() {
        m_Target->didEnterBackground();
    }
    inline void AppWillEnterForeground() {
        m_Target->willEnterForeground();
    }
    inline void AppWillTerminate() {
        m_Target->willTerminate();
    }
    inline void AppDidReceiveMemoryWarning() {
        m_Target->didReceiveMemoryWarning();
    }
private:
    GApplication* m_Target;
};

@interface _AppTimer : NSObject {
    _AppTimerBridge _delegate;
}
#if defined(GX_OS_IPHONE)
@property (nonatomic,retain) CADisplayLink* displayTimer;
#elif defined(GX_OS_MACOSX)
@property (nonatomic,assign) dispatch_source_t displayTimer;
#endif
@end


@implementation _AppTimer
@synthesize displayTimer=_displayTimer;

- (id)initWithDelegate:(GApplication*)dge
{
    self=[super init];
    if (self) {
        _delegate.SetTarget(dge);
        
#if defined(GX_OS_IPHONE)
        NSNotificationCenter* nc=[NSNotificationCenter defaultCenter];
        [nc addObserver:self
               selector:@selector(noteAppDidBecomeActive:)
                   name:UIApplicationDidBecomeActiveNotification
                 object:nil];
        [nc addObserver:self
               selector:@selector(noteAppWillResignActive:)
                   name:UIApplicationWillResignActiveNotification
                 object:nil];
        [nc addObserver:self
               selector:@selector(noteAppDidEnterBackground:)
                   name:UIApplicationDidEnterBackgroundNotification
                 object:nil];
        [nc addObserver:self
               selector:@selector(noteAppWillEnterForeground:)
                   name:UIApplicationWillEnterForegroundNotification
                 object:nil];
        [nc addObserver:self
               selector:@selector(noteAppWillTerminate:)
                   name:UIApplicationWillTerminateNotification
                 object:nil];
        [nc addObserver:self
               selector:@selector(noteAppDidReceiveMemoryWarning:)
                   name:UIApplicationDidReceiveMemoryWarningNotification
                 object:nil];
#elif defined(GX_OS_MACOSX)
        NSNotificationCenter* nc=[NSNotificationCenter defaultCenter];
        [nc addObserver:self
               selector:@selector(noteAppWillTerminate:)
                   name:NSApplicationWillTerminateNotification
                 object:nil];
        
#endif
    }
    return self;
}

- (void)dealloc
{
#if defined(GX_OS_IPHONE)
    [[NSNotificationCenter defaultCenter] removeObserver:self];
    [_displayTimer release];
#elif defined(GX_OS_MACOSX)
    if (_displayTimer) {
        dispatch_cancel(_displayTimer);
        _displayTimer=NULL;
    }
#endif
    [super dealloc];
}

- (void)start
{
    if (!_displayTimer) {
        
#if defined(GX_OS_IPHONE)
        self.displayTimer=[CADisplayLink displayLinkWithTarget:self selector:@selector(displayTimerFun:)];
        [_displayTimer addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
#elif defined(GX_OS_MACOSX)
        _displayTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0,
                                               dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0));
        dispatch_source_set_timer(_displayTimer, DISPATCH_TIME_NOW, (1.0/60.0) * NSEC_PER_SEC, 0);
        dispatch_source_set_event_handler(_displayTimer, ^{
            dispatch_async(dispatch_get_main_queue(), ^{
                _delegate.AppIdle();
            });
        });
        dispatch_resume(_displayTimer);
#endif
    }
}

- (void)stop
{
    if (_displayTimer) {
#if defined(GX_OS_IPHONE)
        [_displayTimer invalidate];
        self.displayTimer=nil;
#elif defined(GX_OS_MACOSX)
        dispatch_cancel(_displayTimer);
        _displayTimer=NULL;
#endif
    }
}

#if defined(GX_OS_IPHONE)

- (void)displayTimerFun:(id)sender
{
    _delegate.AppIdle();
}

- (void)noteAppDidBecomeActive:(NSNotification*)note
{
    _delegate.AppDidBecomeActive();
}
- (void)noteAppWillResignActive:(NSNotification*)note
{
    _delegate.AppWillResignActive();
}
- (void)noteAppDidEnterBackground:(NSNotification*)note
{
    _delegate.AppDidEnterBackground();
}
- (void)noteAppWillEnterForeground:(NSNotification*)note
{
    _delegate.AppWillEnterForeground();
}
- (void)noteAppDidReceiveMemoryWarning:(NSNotification*)note
{
    _delegate.AppDidReceiveMemoryWarning();
}

#endif

- (void)noteAppWillTerminate:(NSNotification*)note
{
    _delegate.AppWillTerminate();
}

@end

#elif defined(GX_OS_WINDOWS)

LRESULT CALLBACK GApplication::winMsgWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_USER:
	{
		GX_CAST_R(GApplication*, lParam)->idle();
	}
	break;
	default:
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void CALLBACK GApplication::winTimerCallBack(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	GApplication* app = GX_CAST_R(GApplication*, dwUser);
	::PostMessage(app->m_MsgWnd.getHWND(), WM_USER, 0, (LPARAM)app);
}

void GApplication::createWinMsgWndAndStart()
{
	WNDCLASS	wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)winMsgWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = NULL;
	RECT rc = { 0, 0, 1, 1 };
	m_MsgWnd.create(wc, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, rc, NULL);
	m_TimerID = timeSetEvent(1000 / 60, 1000 / 60, winTimerCallBack, (DWORD_PTR)this, TIME_PERIODIC | TIME_KILL_SYNCHRONOUS);
}
void GApplication::destroyWinMsgWnd()
{
	if (m_TimerID) {
		timeKillEvent(m_TimerID);
		m_MsgWnd.destroy();
	}
}

#endif




GApplication* GApplication::shared()
{
    static GApplication g_Obj;
    return &g_Obj;
}

void GApplication::main(Delegate* dge)
{
    GApplication* app=shared();
    app->m_Delegate=dge;
    GThread::current()->setMain();
    
#if defined(GX_OS_APPLE)
    [GX_CAST_R(_AppTimer*, app->m_Timer) start];
    app->didFinishLaunching();
#elif defined(GX_OS_WINDOWS)
	app->createWinMsgWndAndStart();
	app->didFinishLaunching();
#endif
}


GApplication::GApplication()
{
    m_Delegate=NULL;
#if defined(GX_OS_APPLE)
    m_Timer=[[_AppTimer alloc] initWithDelegate:this];
#elif defined(GX_OS_WINDOWS)
	m_TimerID = 0;
#endif
}

GApplication::~GApplication()
{
#if defined(GX_OS_APPLE)
    [GX_CAST_R(_AppTimer*, m_Timer) release];
#elif defined(GX_OS_WINDOWS)
	if (m_TimerID) {
		timeKillEvent(m_TimerID);
	}
#endif
}

void GApplication::idle()
{
	/*
	static int i = 0;
	GX_LOG_P1(PrioDEBUG, "GApplication", "idle:%d ", i++);
	//const gchar* str = "\x41\x42\x43\x48\x65\x6C\x6C\x6F\x21\x20\xE4\xBD\xA0\xE5\xA5\xBD\xEF\xBC\x81\xE3\x82\x82\xE3\x81\x97\xE3\x82\x82\xE3\x81\x97\x21\x20\x41\x56\x41\x56\x41\x56\x41";
	const gchar* str = "中文";
	GX_LOG_W(PrioDEBUG, "GApplication", str);
	//*/

	GThread::current()->popARObj(0);
}

void GApplication::didFinishLaunching()
{
}

void GApplication::didBecomeActive()
{
}

void GApplication::willResignActive()
{
}

void GApplication::didEnterBackground()
{
}

void GApplication::willEnterForeground()
{
}

void GApplication::willTerminate()
{
}

void GApplication::didReceiveMemoryWarning()
{
}



