﻿#include "AppDge.h"


AppDge* AppDge::shared()
{
	static AppDge g_Obj;
	return &g_Obj;
}

AppDge::AppDge()
{
}


AppDge::~AppDge()
{
}

GWindow* AppDge::appCanCreateWindow(GApplication* application,void* osWindow)
{
    GX_UNUSED(application);

	GWindow* win = GWindow::autoAlloc();
	win->create(osWindow);
    return win;
}
