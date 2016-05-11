﻿#pragma once

#include "GXPrefix.h"
#include "GObject.h"

namespace GX {
	typedef void(GObject::*Selector)(GObject*);
	typedef void(*Callback)(GObject*);
}


class GAction : public GObject
{
	GX_OBJECT(GAction);
public:
	inline GObject* getTarget() {
		return m_Target;
	}
	inline GX::Selector getSelector() {
		return m_Action.sel;
	}
	inline GX::Callback getCallback() {
		return m_Action.cbk;
	}
	inline GObject* getObj() {
		return m_Obj;
	}

	void set(GObject* target, GX::Selector sel, GObject* obj);
	void set(GX::Callback cbk, GObject* obj);
	void run();
private:
	GObject* m_Target;
	union {
		GX::Selector sel;
		GX::Callback cbk;
	} m_Action;
	GObject* m_Obj;
};

