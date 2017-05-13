﻿//
//  GString.cpp
//  GX
//
//  Created by Gengxin Gu on 16/4/29.
//  Copyright © 2016年 Gengxin Gu. All rights reserved.
//

#include "GString.h"
//Down include other h file

//Up include other h file
#include "GXGObject.h"

namespace GX {
    static const gchar* g_NumChars = "0123456789abcdef0123456789ABCDEF";
    
    template <typename T> void _strSwap(T* str, gint len)
    {
        T temp;
        for (gint i = 0; i < len / 2; i++) {
            temp = str[i];
            str[i] = str[len - i-1];
            str[len - i - 1] = temp;
        }
    }
    
    template <typename T> gint strlen(const T* v)
    {
        gint res=0;
        while (*v) {
            res++;
            v++;
        }
        return res;
    }
}


namespace GX
{
    template <typename T> gint gu16toa(guint16 v, T* strOut, StringRadix radix)
    {
        gint co = 0;
        if (radix == SR_HEX) {
            co = 16;
            radix = SR_Hex;
        }
        gint res = 0;
        guint16 vTemp;
        do {
            vTemp = v%radix;
            v /= radix;
            strOut[res] = GX_CAST_S(T, g_NumChars[co + vTemp]);
            res++;
        } while (v > 0);
        _strSwap(strOut, res);
        return res;
    }
    template <typename T> gint gi16toa(gint16 v, T* strOut, StringRadix radix)
    {
        if (radix == SR_Decimal && v<0) {
            strOut[0] = GX_CAST_S(T, '-');
            return gu16toa(GX_CAST_S(guint16, -v), strOut+1, radix) + 1;
        }
        return gu16toa(GX_CAST_S(guint16, v), strOut, radix);
    }
    
    template <typename T> gint gu32toa(guint32 v, T* strOut, StringRadix radix)
    {
        gint co=0;
        if (radix == SR_HEX) {
            co = 16;
            radix = SR_Hex;
        }
        gint res = 0;
        guint32 vTemp;
        do {
            vTemp = v%radix;
            v /= radix;
            strOut[res] = GX_CAST_S(T, g_NumChars[co+vTemp]);
            res++;
        } while (v > 0);
        _strSwap(strOut, res);
        return res;
    }
    template <typename T> gint gi32toa(gint32 v, T* strOut, StringRadix radix)
    {
        if (radix == SR_Decimal && v<0) {
            strOut[0] = GX_CAST_S(T, '-');
            return gu32toa(GX_CAST_S(guint32, -v), strOut+1, radix) + 1;
        }
        return gu32toa(GX_CAST_S(guint32, v), strOut, radix);
    }
    
    template <typename T> gint gu64toa(guint64 v, T* strOut, StringRadix radix)
    {
        gint co = 0;
        if (radix == SR_HEX) {
            co = 16;
            radix = SR_Hex;
        }
        gint res = 0;
        guint64 vTemp;
        do {
            vTemp = v%radix;
            v /= radix;
            strOut[res] = GX_CAST_S(T, g_NumChars[co + vTemp]);
            res++;
        } while (v > 0);
        _strSwap(strOut, res);
        return res;
    }
    template <typename T> gint gi64toa(gint64 v, T* strOut, StringRadix radix)
    {
        if (radix == SR_Decimal && v<0) {
            strOut[0] = GX_CAST_S(T, '-');
            return gu64toa(GX_CAST_S(guint64, -v), strOut+1, radix) + 1;
        }
        return gu64toa(GX_CAST_S(guint64, v), strOut, radix);
    }
    
    
    template <typename T> gint gf32toa(gfloat32 v, T* strOut, gint precision)
    {
        gfloat32 vTemp;
        gint32 decPos = 1;
        gfloat32 rv = 0.5f;
        for (gint i = 0; i < precision; i++) {
            rv *= 0.1f;
        }
        gint res = 0;
        
        if (v >= 0) {
            vTemp = v;
        }
        else {
            vTemp = -v;
            strOut[res++] = GX_CAST_S(T, '-');
        }
        vTemp += rv;
        while (vTemp>10.0f)
        {
            decPos++;
            vTemp /= 10.0f;
        }
        
        gint32 nTemp;
        
        if (precision <= 0) {
            for (gint i = 0; i < decPos; i++, res++) {
                nTemp = GX_CAST_S(gint32, vTemp);
                strOut[res] = GX_CAST_S(T, nTemp);
                vTemp -= nTemp;
                vTemp *= 10.0f;
            }
        }
        else {
            for (gint i = 0; i < decPos + precision + 1; i++, res++) {
                if (i == decPos) {
                    strOut[res] = GX_CAST_S(T, '.');
                }
                else {
                    nTemp = GX_CAST_S(gint32, vTemp);
                    strOut[res] = GX_CAST_S(T, nTemp);
                    vTemp -= nTemp;
                    vTemp *= 10.0f;
                }
            }
        }
        return res;
    }
    template <typename T> gint gf64toa(gfloat64 v, T* strOut, gint precision)
    {
        gfloat64 vTemp;
        gint64 decPos = 1;
        gfloat64 rv = 0.5;
        for (gint i = 0; i < precision; i++) {
            rv *= 0.1f;
        }
        gint res = 0;
        
        if (v >= 0) {
            vTemp = v;
        }
        else {
            vTemp = -v;
            strOut[res++] = GX_CAST_S(T, '-');
        }
        vTemp += rv;
        while (vTemp>10.0)
        {
            decPos++;
            vTemp /= 10.0;
        }
        
        gint64 nTemp;
        if (precision <= 0) {
            for (gint i = 0; i < decPos; i++, res++) {
                nTemp = GX_CAST_S(gint64, vTemp);
                strOut[res] = GX_CAST_S(T, nTemp);
                vTemp -= nTemp;
                vTemp *= 10.0;
            }
        }
        else {
            for (gint i = 0; i < decPos + precision + 1; i++, res++) {
                if (i == decPos) {
                    strOut[res] = GX_CAST_S(T, '.');
                }
                else {
                    nTemp = GX_CAST_S(gint64, vTemp);
                    strOut[res] = GX_CAST_S(T, nTemp);
                    vTemp -= nTemp;
                    vTemp *= 10.0;
                }
            }
        }
        return res;
    }
    
    template <typename T> gint gatoi(const T* buf, gint len=-1)
    {
        if (len < 0) {
            len = GX::strlen(buf);
        }
        gint res = 0;
        gint m = 1;
        gint i = 0;
        if (buf[i] == '-') {
            m = -1;
            i++;
        }
        for (; i < len; i++) {
            res = res*10+(buf[i]-'0');
        }
        return res*m;
    }
    template <typename T> gfloat gatof(const T* buf, gint len)
    {
        if (len < 0) {
            len = GX::strlen(buf);
        }
        gfloat res = 0.0f;
        gfloat frac = 0.0f;
        gfloat m = 1.0f;
        gint i = 0;
        if (buf[i] == '-') {
            m = -1.0f;
            i++;
        }
        for (; i < len; i++) {
            if (frac <= 0.0f) {
                if (buf[i] != '.') {
                    res = res * 10.0f + (buf[i] - '0');
                }
                else {
                    frac = 10.0f;
                }
            }
            else {
                res += (buf[i] - '0')/frac;
                frac *= 10.0f;
            }
        }
        return res*m;
    }
    
}

namespace GX
{
    void strUTF8toUTF16(const gchar* utf8Text, gint cbUtf8Text, guchar* utf16Text, gint& ccUtf16Text)
    {
        gint oldccUtf16Text = ccUtf16Text;
        while (cbUtf8Text > 0 && ccUtf16Text > 0)
        {
            if ((*utf8Text & 0x80) == 0)
            {
                *utf16Text = (guchar)utf8Text[0];
                cbUtf8Text -= 1;
                utf8Text += 1;
            }
            else if ((*utf8Text & 0xE0) == 0xC0)
            {
                if (cbUtf8Text >= 2 && ((utf8Text[1] & 0xC0) == 0x80))
                {
                    *utf16Text = 0;
                    *utf16Text |= (utf8Text[0] & 0x1F) << 6;
                    *utf16Text |= (utf8Text[1] & 0x3F);
                    cbUtf8Text -= 2;
                    utf8Text += 2;
                }
                else
                {
                    break;
                }
            }
            else if ((*utf8Text & 0xF0) == 0xE0)
            {
                if (cbUtf8Text >= 3 && ((utf8Text[1] & 0xC0) == 0x80) && ((utf8Text[2] & 0xC0) == 0x80))
                {
                    *utf16Text = 0;
                    *utf16Text |= (utf8Text[0] & 0x1F) << 12;
                    *utf16Text |= (utf8Text[1] & 0x3F) << 6;
                    *utf16Text |= (utf8Text[2] & 0x3F);
                    cbUtf8Text -= 3;
                    utf8Text += 3;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
            ccUtf16Text -= 1;
            utf16Text += 1;
        }
        ccUtf16Text = oldccUtf16Text - ccUtf16Text;
    }
    gint strUTF8toUTF16Count(const gchar* utf8Text, gint cbUtf8Text)
    {
        gint ccUtf16Text = 0;
        while (cbUtf8Text > 0)
        {
            if ((*utf8Text & 0x80) == 0)
            {
                cbUtf8Text -= 1;
                utf8Text += 1;
            }
            else if ((*utf8Text & 0xE0) == 0xC0)
            {
                if (cbUtf8Text >= 2 && ((utf8Text[1] & 0xC0) == 0x80))
                {
                    cbUtf8Text -= 2;
                    utf8Text += 2;
                }
                else
                {
                    break;
                }
            }
            else if ((*utf8Text & 0xF0) == 0xE0)
            {
                if (cbUtf8Text >= 3 && ((utf8Text[1] & 0xC0) == 0x80) && ((utf8Text[2] & 0xC0) == 0x80))
                {
                    cbUtf8Text -= 3;
                    utf8Text += 3;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
            ++ccUtf16Text;
        }
        return ccUtf16Text;
    }
    gint strUTF8toUTF16Count(const gchar* utf8Text,gint* utf8TextLenOut)
    {
        gint res = 0;
        gint& utf8TextLen=(*utf8TextLenOut);
        utf8TextLen=0;
        
        while (utf8Text[0] != 0) {
            if ((utf8Text[0] & 0x80) == 0) {
                utf8TextLen++;
                utf8Text++;
            }
            else if ((utf8Text[0] & 0xE0) == 0xC0) {
                if (utf8Text[1] != 0 && ((utf8Text[1] & 0xC0) == 0x80))
                {
                    utf8TextLen += 2;
                    utf8Text += 2;
                }
                else
                {
                    break;
                }
            }
            else if ((utf8Text[0] & 0xF0) == 0xE0)
            {
                if (utf8Text[1] != 0 && utf8Text[2] != 0 && ((utf8Text[1] & 0xC0) == 0x80) && ((utf8Text[2] & 0xC0) == 0x80))
                {
                    utf8TextLen += 3;
                    utf8Text += 3;
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
            ++res;
        }
        return res;
    }
    
    gint strUTF8OneChartoUTF16(const gchar* utf8Text, gint cbUtf8Text, guchar& utf16Out)
    {
        if ((*utf8Text & 0x80) == 0)
        {
            utf16Out = (guchar)utf8Text[0];
            return 1;
        }
        else if ((*utf8Text & 0xE0) == 0xC0)
        {
            if (cbUtf8Text >= 2 && ((utf8Text[1] & 0xC0) == 0x80))
            {
                utf16Out = 0;
                utf16Out |= (utf8Text[0] & 0x1F) << 6;
                utf16Out |= (utf8Text[1] & 0x3F);
                return 2;
            }
        }
        else if ((*utf8Text & 0xF0) == 0xE0)
        {
            if (cbUtf8Text >= 3 && ((utf8Text[1] & 0xC0) == 0x80) && ((utf8Text[2] & 0xC0) == 0x80))
            {
                utf16Out = 0;
                utf16Out |= (utf8Text[0] & 0x1F) << 12;
                utf16Out |= (utf8Text[1] & 0x3F) << 6;
                utf16Out |= (utf8Text[2] & 0x3F);
                return 3;
            }
        }
        return 0;
    }
    
    
    void strUTF16toUTF8(const guchar* utf16Text, gint ccUtf16Text, gchar* utf8Text, gint& cbUtf8Text)
    {
        gint oldcbUtf8Text = cbUtf8Text;
        while (ccUtf16Text > 0)
        {
            if ((*utf16Text & ~0x007F) == 0)
            {
                if (cbUtf8Text >= 1)
                {
                    utf8Text[0] = GX_CAST_S(gchar, *utf16Text);
                    cbUtf8Text -= 1;
                    utf8Text += 1;
                }
                else
                {
                    break;
                }
            }
            else if ((*utf16Text & ~0x07FF) == 0)
            {
                if (cbUtf8Text >= 2)
                {
                    utf8Text[0] = GX_CAST_S(gchar, 0xC0 | ((*utf16Text & 0x07C0) >> 6));
                    utf8Text[1] = GX_CAST_S(gchar, 0x80 | (*utf16Text & 0x003F));
                    
                    cbUtf8Text -= 2;
                    utf8Text += 2;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (cbUtf8Text >= 3)
                {
                    utf8Text[0] = GX_CAST_S(gchar, 0xE0 | ((*utf16Text & 0xF000) >> 12));
                    utf8Text[1] = GX_CAST_S(gchar, 0x80 | ((*utf16Text & 0x0FC0) >> 6));
                    utf8Text[2] = GX_CAST_S(gchar, 0x80 | (*utf16Text & 0x003F));
                    
                    cbUtf8Text -= 3;
                    utf8Text += 3;
                }
                else
                {
                    break;
                }
            }
            ccUtf16Text -= 1;
            utf16Text += 1;
        }
        cbUtf8Text = oldcbUtf8Text - cbUtf8Text;
    }
    gint strUTF16toUTF8Count(const guchar* utf16Text, gint ccUtf16Text)
    {
        gint cbUtf8Text = 0;
        while (ccUtf16Text > 0)
        {
            if ((*utf16Text & ~0x007F) == 0)
            {
                cbUtf8Text += 1;
            }
            else if ((*utf16Text & ~0x07FF) == 0)
            {
                cbUtf8Text += 2;
            }
            else
            {
                cbUtf8Text += 3;
            }
            ccUtf16Text -= 1;
            utf16Text += 1;
        }
        return cbUtf8Text;
    }
    gint strUTF16OneChartoUTF8(const guchar utf16Char, gchar* utf8Out)
    {
        if ((utf16Char & ~0x007F) == 0)
        {
            utf8Out[0] = GX_CAST_S(gchar,utf16Char);
            return 1;
        }
        else if ((utf16Char & ~0x07FF) == 0)
        {
            utf8Out[0] = GX_CAST_S(gchar, 0xC0 | ((utf16Char & 0x07C0) >> 6));
            utf8Out[1] = GX_CAST_S(gchar, 0x80 | (utf16Char & 0x003F));
            return 2;
        }
        else
        {
            utf8Out[0] = GX_CAST_S(gchar, 0xE0 | ((utf16Char & 0xF000) >> 12));
            utf8Out[1] = GX_CAST_S(gchar, 0x80 | ((utf16Char & 0x0FC0) >> 6));
            utf8Out[2] = GX_CAST_S(gchar, 0x80 | (utf16Char & 0x003F));
            return 3;
        }
        return 0;
    }
}



GString::Formater::Formater(GString* str, gint cursor, gint cursorEnd)
{
    m_String = str;
    m_Cursor = cursor;
    m_CursorEnd = cursorEnd;
}

bool GString::Formater::moveCursor()
{
    gint curEnd = m_String->getLength() - m_CursorEnd;
    while (m_Cursor<curEnd-1) {
        if (m_String->at(m_Cursor)==(guchar)'%' && m_String->at(m_Cursor + 1) == (guchar)'@') {
            return true;
        }
        else if (m_String->at(m_Cursor)==(guchar)'%' && m_String->at(m_Cursor + 1) == (guchar)'%') {
            m_String->remove(m_Cursor + 1, 1);
            m_Cursor++;
        }
        else {
            m_Cursor++;
        }
    }
    return false;
}
gint GString::Formater::replaceStart()
{
    return m_String->getLength();
}
void GString::Formater::replaceEnd(gint lastLen)
{
    m_Cursor=m_Cursor+2+(m_String->getLength()-lastLen);
}

GString::Formater& GString::Formater::arg(gchar v, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChar(getCursor(), 2, v, count);
        replaceEnd(lastLen);
    }
    return *this;
}

GString::Formater& GString::Formater::arg(guchar v, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChar(getCursor(), 2, v, count);
        replaceEnd(lastLen);
    }
    return *this;
}

GString::Formater& GString::Formater::arg(gwchar v,gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChar(getCursor(), 2, v, count);
        replaceEnd(lastLen);
    }
    return *this;
}

GString::Formater& GString::Formater::arg(const gchar* v, gint len, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChars(getCursor(), 2, v, len, count);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(const guchar* v, gint len, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChars(getCursor(), 2, v, len, count);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(const gwchar* v, gint len, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChars(getCursor(), 2, v, len, count);
        replaceEnd(lastLen);
    }
    return *this;
}

GString::Formater& GString::Formater::arg(gchar preChar, gint preCount,
                                          gchar sufChar, gint sufCount,
                                          const gchar* v, gint len, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChars(getCursor(), 2, preChar, preCount, sufChar, sufCount, v, len, count);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(guchar preChar, gint preCount,
                                          guchar sufChar, gint sufCount,
                                          const guchar* v, gint len, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChars(getCursor(), 2, preChar, preCount, sufChar, sufCount, v, len, count);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(gwchar preChar, gint preCount,
                                          gwchar sufChar, gint sufCount,
                                          const gwchar* v, gint len, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceChars(getCursor(), 2, preChar, preCount, sufChar, sufCount, v, len, count);
        replaceEnd(lastLen);
    }
    return *this;
}


GString::Formater& GString::Formater::arg(gint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceNumber(getCursor(), 2, v, radix, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(guint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceNumber(getCursor(), 2, v, radix, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(gint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceNumber(getCursor(), 2, v, radix, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(guint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceNumber(getCursor(), 2, v, radix, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(gint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceNumber(getCursor(), 2, v, radix, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(guint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceNumber(getCursor(), 2, v, radix, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(gfloat32 v, gint precision, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceNumber(getCursor(), 2, v, precision, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(gfloat64 v, gint precision, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceNumber(getCursor(), 2, v, precision, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}
GString::Formater& GString::Formater::arg(void* v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replacePtr(getCursor(), 2, v, radix, vsLen, fillChar);
        replaceEnd(lastLen);
    }
    return *this;
}

GString::Formater& GString::Formater::arg(GObject* v, gint count)
{
    if (moveCursor()) {
        gint lastLen=replaceStart();
        getString()->replaceString(getCursor(), 2, v->description(), count);
        replaceEnd(lastLen);
    }
    return *this;
}


void GString::Formater::end()
{
    gint curEnd = m_String->getLength() - m_CursorEnd;
    while (m_Cursor<curEnd-1) {
        if (m_String->at(m_Cursor)==(guchar)'%' && m_String->at(m_Cursor + 1) == (guchar)'%') {
            m_String->remove(m_Cursor + 1, 1);
            m_Cursor++;
        }
        else {
            m_Cursor++;
        }
    }
}



GX_GOBJECT_IMPLEMENT(GString, GObject);


GString* GString::chars(const gchar* str, gint len)
{
    GString* res=GString::autoAlloc();
    res->setChars(str,len);
    return res;
}
GString* GString::chars(const guchar* str, gint len)
{
    GString* res=GString::autoAlloc();
    res->setChars(str,len);
    return res;
}
GString* GString::chars(const gwchar* str, gint len)
{
    GString* res=GString::autoAlloc();
    res->setChars(str,len);
    return res;
}

GString* GString::number(gint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    GString* res=GString::autoAlloc();
    res->setNumber(v,radix,vsLen,fillChar);
    return res;
}
GString* GString::number(guint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    GString* res=GString::autoAlloc();
    res->setNumber(v,radix,vsLen,fillChar);
    return res;
}
GString* GString::number(gint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    GString* res=GString::autoAlloc();
    res->setNumber(v,radix,vsLen,fillChar);
    return res;
}
GString* GString::number(guint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    GString* res=GString::autoAlloc();
    res->setNumber(v,radix,vsLen,fillChar);
    return res;
}
GString* GString::number(gint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    GString* res=GString::autoAlloc();
    res->setNumber(v,radix,vsLen,fillChar);
    return res;
}
GString* GString::number(guint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    GString* res=GString::autoAlloc();
    res->setNumber(v,radix,vsLen,fillChar);
    return res;
}
GString* GString::number(gfloat32 v, gint precision, gint vsLen, gwchar fillChar)
{
    GString* res=GString::autoAlloc();
    res->setNumber(v,precision,vsLen,fillChar);
    return res;
}
GString* GString::number(gfloat64 v, gint precision, gint vsLen, gwchar fillChar)
{
    GString* res=GString::autoAlloc();
    res->setNumber(v,precision,vsLen,fillChar);
    return res;
}


GString::GString()
{
    m_OWHash.codeA=0;
    m_OWHash.codeB=0;
    m_OWHash.codeC=0;
}

GString::~GString()
{
    
}

guint GString::getHash()
{
    if (m_OWHash.codeA==0) {
        m_OWHash=GOWHash::compute(m_Buffer.getPtr(0));
    }
    return m_OWHash.codeA;
}

bool GString::isEqual(GObject* obj)
{
    if (obj->isKindOfClass(GString::gclass)) {
        if (getHash() == obj->getHash()) {
            return getOWHashB() == GX_CAST_R(GString*, obj)->getOWHashB() &&
            getOWHashC() == GX_CAST_R(GString*, obj)->getOWHashC();
        }
    }
    return false;
}

GString* GString::description()
{
    return this;
}

gint GString::getLength()
{
    gint res=m_Buffer.getCount();
    if (res<=0) {
        return 0;
    }
    return res-1;
}

guchar GString::at(gint idx)
{
    return m_Buffer.get(idx);
}

void GString::remove(gint idx, gint len)
{
    m_Buffer.remove(idx, len);
    modifyDone();
}

void GString::clear()
{
    m_Buffer.removeAll();
    modifyDone();
}

guchar* GString::prepareBuffer(gint len)
{
    m_Buffer.changeCount(len+1);
    setCStringEnd();
    modifyDone();
    return (guchar*)m_Buffer.getPtr(0);
}

void GString::cutOff(gint len)
{
    m_Buffer.changeCount(len+1);
    setCStringEnd();
    modifyDone();
}

const gchar* GString::utf8String()
{
    return GX_CAST_R(const gchar*, getCString(GX::SE_Utf8));
}
const gwchar* GString::unicodeString()
{
    return GX_CAST_R(const gwchar*, getCString(GX::SE_Unicode));
}
const gtchar* GString::pathString()
{
#if defined(GX_PATH_IS_WCHAR)
    return GX_CAST_R(const gwchar*, getCString(GX::SE_Unicode));
#else
    return GX_CAST_R(const gchar*, getCString(GX::SE_Utf8));
#endif
}

void GString::setChar(gchar v, gint count)
{
	setChar(GX_CAST_S(guchar, v), count);
}
void GString::appendChar(gchar v, gint count)
{
	appendChar(GX_CAST_S(guchar, v), count);
}
void GString::insertChar(gint idx, gchar v, gint count)
{
	insertChar(idx, GX_CAST_S(guchar, v), count);
}
void GString::replaceChar(gint idx, gint lenR, gchar v, gint count)
{
	replaceChar(idx, lenR, GX_CAST_S(guchar, v), count);
}

void GString::setChar(guchar v, gint count)
{
	if (count <= 0) {
		return;
	}
	if (m_Buffer.changeCount(count + 1)) {
		for (gint i = 0; i<count; i++) {
			m_Buffer.set(i, v);
		}
		setCStringEnd();
		modifyDone();
	}
}
void GString::appendChar(guchar v, gint count)
{
	if (count <= 0) {
		return;
	}
	gint lenCur = getLength();
	if (m_Buffer.changeCount(lenCur + count + 1)) {
		for (gint i = 0; i<count; i++) {
			m_Buffer.set(lenCur + i, v);
		}
		setCStringEnd();
		modifyDone();
	}
}
void GString::insertChar(gint idx, guchar v, gint count)
{
	replaceChar(idx, 0, v, count);
}
void GString::replaceChar(gint idx, gint lenR, guchar v, gint count)
{
	if (idx < 0 || lenR<0) {
		return;
	}
	if (count <= 0) {
		return;
	}
	gint lenCur = getLength();
	if (idx > lenCur) {
		return;
	}
	if (m_Buffer.expand(idx, lenR, count)) {
		for (gint i = 0; i<count; i++) {
			m_Buffer.set(idx + i, v);
		}
		setCStringEnd();
		modifyDone();
	}
}


void GString::setChar(gwchar v, gint count)
{
	setChar(GX_CAST_S(guchar, v), count);
}
void GString::appendChar(gwchar v, gint count)
{
	appendChar(GX_CAST_S(guchar, v), count);
}
void GString::insertChar(gint idx, gwchar v, gint count)
{
	insertChar(idx, GX_CAST_S(guchar, v), count);
}
void GString::replaceChar(gint idx, gint lenR, gwchar v, gint count)
{
	replaceChar(idx, lenR, GX_CAST_S(guchar, v), count);
}

void GString::setChars(const gchar* v, gint len, gint count)
{
    if (count <= 0) {
        return;
    }
    gint utf16Len;
    if (len<0) {
        utf16Len=GX::strUTF8toUTF16Count(v, &len);
    }
    else {
        utf16Len=GX::strUTF8toUTF16Count(v, len);
    }
    if (m_Buffer.changeCount(utf16Len*count + 1)) {
        GX::strUTF8toUTF16(v, len, (guchar*)m_Buffer.getPtr(0), utf16Len);
        for (gint j=1; j<count; j++) {
            memcpy((void*)m_Buffer.getPtr(0+j*utf16Len), (const void*)m_Buffer.getPtr(0), utf16Len*sizeof(guchar));
        }
        setCStringEnd();
        modifyDone();
    }
}
void GString::appendChars(const gchar* v, gint len, gint count)
{
    if (count <= 0) {
        return;
    }
    gint utf16Len;
    if (len<0) {
        utf16Len=GX::strUTF8toUTF16Count(v, &len);
    }
    else {
        utf16Len=GX::strUTF8toUTF16Count(v, len);
    }
    gint lenCur = getLength();
    if (m_Buffer.changeCount(lenCur + utf16Len*count + 1)) {
        GX::strUTF8toUTF16(v, len, (guchar*)m_Buffer.getPtr(lenCur), utf16Len);
        for (gint j=1; j<count; j++) {
            memcpy((void*)m_Buffer.getPtr(lenCur+j*utf16Len), (const void*)m_Buffer.getPtr(lenCur), utf16Len*sizeof(guchar));
        }
        setCStringEnd();
        modifyDone();
    }
}
void GString::insertChars(gint idx, const gchar* v, gint len, gint count)
{
    replaceChars(idx,0, v, len, count);
}
void GString::replaceChars(gint idx, gint lenR, const gchar* v, gint len, gint count)
{
    if (count <= 0) {
        return;
    }
    if (idx < 0 || lenR<0) {
        return;
    }
    gint lenCur = getLength();
    if (idx > lenCur) {
        return;
    }
    gint utf16Len;
    if (len<0) {
        utf16Len=GX::strUTF8toUTF16Count(v, &len);
    }
    else {
        utf16Len=GX::strUTF8toUTF16Count(v, len);
    }
    if (m_Buffer.expand(idx, lenR, utf16Len*count)) {
        GX::strUTF8toUTF16(v, len, (guchar*)m_Buffer.getPtr(idx), utf16Len);
        for (gint j=1; j<count; j++) {
            memcpy((void*)m_Buffer.getPtr(idx+j*utf16Len), (const void*)m_Buffer.getPtr(idx), utf16Len*sizeof(guchar));
        }
        setCStringEnd();
        modifyDone();
    }
}
void GString::replaceChars(gint idx, gint lenR,
                      gchar preChar, gint preCount,
                      gchar sufChar, gint sufCount,
                      const gchar* v, gint len, gint count)
{
    if (preCount <0 || count < 0 || sufCount<0 || preCount+count+sufCount<=0) {
        return;
    }
    if (idx < 0 || lenR<0) {
        return;
    }
    gint lenCur = getLength();
    if (idx > lenCur) {
        return;
    }
    gint utf16Len;
    if (len<0) {
        utf16Len=GX::strUTF8toUTF16Count(v, &len);
    }
    else {
        utf16Len=GX::strUTF8toUTF16Count(v, len);
    }
    if (m_Buffer.expand(idx, lenR, preCount + utf16Len*count + sufCount)) {
        gint start = idx;
        for (gint i=0; i<preCount; i++) {
            m_Buffer.set(start+i, GX_CAST_S(guchar, preChar));
        }
        start += preCount;
        GX::strUTF8toUTF16(v, len, (guchar*)m_Buffer.getPtr(start), utf16Len);
        for (gint j=1; j<count; j++) {
            memcpy((void*)m_Buffer.getPtr(start+j*utf16Len), (const void*)m_Buffer.getPtr(start), utf16Len*sizeof(guchar));
        }
        start += utf16Len*count;
        for (gint i=0; i<sufCount; i++) {
            m_Buffer.set(start+i, GX_CAST_S(guchar, sufChar));
        }
        start += sufCount;
        
        setCStringEnd();
        modifyDone();
    }
}


void GString::setChars(const guchar* v, gint len, gint count)
{
	if (count <= 0) {
		return;
	}
	if (len<0) {
		len = GX::strlen(v);
	}
	if (m_Buffer.changeCount(len*count + 1)) {
		for (gint j = 0; j<count; j++) {
			memcpy((void*)m_Buffer.getPtr(j*len), (const void*)v, len*sizeof(guchar));
		}
		setCStringEnd();
		modifyDone();
	}
}
void GString::appendChars(const guchar* v, gint len, gint count)
{
	if (count <= 0) {
		return;
	}
	if (len<0) {
		len = GX::strlen(v);
	}
	gint lenCur = getLength();
	if (m_Buffer.changeCount(lenCur + len*count + 1)) {
		for (gint j = 0; j<count; j++) {
			memcpy((void*)m_Buffer.getPtr(lenCur + j*len), (const void*)v, len*sizeof(guchar));
		}
		setCStringEnd();
		modifyDone();
	}
}
void GString::insertChars(gint idx, const guchar* v, gint len, gint count)
{
	replaceChars(idx, 0, v, len, count);
}
void GString::replaceChars(gint idx, gint lenR, const guchar* v, gint len, gint count)
{
	if (count <= 0) {
		return;
	}
	if (idx < 0 || lenR<0) {
		return;
	}
	gint lenCur = getLength();
	if (idx > lenCur) {
		return;
	}
	if (len<0) {
		len = GX::strlen(v);
	}
	if (m_Buffer.expand(idx, lenR, len*count)) {
		for (gint j = 0; j<count; j++) {
			memcpy((void*)m_Buffer.getPtr(idx + j*len), (const void*)v, len*sizeof(guchar));
		}
		setCStringEnd();
		modifyDone();
	}
}
void GString::replaceChars(gint idx, gint lenR,
	guchar preChar, gint preCount,
	guchar sufChar, gint sufCount,
	const guchar* v, gint len, gint count)
{
	if (preCount <0 || count < 0 || sufCount<0 || preCount + count + sufCount <= 0) {
		return;
	}
	if (idx < 0 || lenR<0) {
		return;
	}
	gint lenCur = getLength();
	if (idx > lenCur) {
		return;
	}
	if (len<0) {
		len = GX::strlen(v);
	}
	if (m_Buffer.expand(idx, lenR, preCount + len*count + sufCount)) {
		gint start = idx;
		for (gint i = 0; i<preCount; i++) {
			m_Buffer.set(start + i, GX_CAST_S(guchar, preChar));
		}
		start += preCount;
		for (gint j = 0; j<count; j++) {
			memcpy((void*)m_Buffer.getPtr(start + j*len), (const void*)v, len*sizeof(guchar));
		}
		start += len*count;
		for (gint i = 0; i<sufCount; i++) {
			m_Buffer.set(start + i, GX_CAST_S(guchar, sufChar));
		}
		start += sufCount;

		setCStringEnd();
		modifyDone();
	}
}

void GString::setChars(const gwchar* v, gint len, gint count)
{
#if GX_WCHAR_16BIT
	setChars(GX_CAST_R(const guchar*, v), len, count);
#else
    if (count <= 0) {
        return;
    }
    if (len<0) {
        len = GX::strlen(v);
    }
    if (m_Buffer.changeCount(len*count + 1)) {
        for (gint j=0; j<count; j++) {
            for (gint i = 0; i < len; i++) {
                m_Buffer.set(j*len+i, GX_CAST_S(guchar, v[i]));
            }
        }
        setCStringEnd();
        modifyDone();
    }
#endif
}
void GString::appendChars(const gwchar* v, gint len, gint count)
{
#if GX_WCHAR_16BIT
	appendChars(GX_CAST_R(const guchar*, v), len, count);
#else
    if (count <= 0) {
        return;
    }
    if (len<0) {
        len = GX::strlen(v);
    }
    gint lenCur = getLength();
    if (m_Buffer.changeCount(lenCur + len*count + 1)) {
        for (gint j=0; j<count; j++) {
            for (gint i = 0; i < len; i++) {
                m_Buffer.set(lenCur+j*len+i, GX_CAST_S(guchar, v[i]));
            }
        }
        setCStringEnd();
        modifyDone();
    }
#endif
}
void GString::insertChars(gint idx, const gwchar* v, gint len, gint count)
{
#if GX_WCHAR_16BIT
	insertChars(idx, GX_CAST_R(const guchar*, v), len, count);
#else
    replaceChars(idx,0, v, len, count);
#endif
}
void GString::replaceChars(gint idx, gint lenR, const gwchar* v, gint len, gint count)
{
#if GX_WCHAR_16BIT
	replaceChars(idx, lenR, GX_CAST_R(const guchar*, v), len, count);
#else
    if (count <= 0) {
        return;
    }
    if (idx < 0 || lenR<0) {
        return;
    }
    gint lenCur = getLength();
    if (idx > lenCur) {
        return;
    }
    if (len<0) {
        len = GX::strlen(v);
    }
    if (m_Buffer.expand(idx, lenR, len*count)) {
        for (gint j=0; j<count; j++) {
            for (gint i = 0; i < len; i++) {
                m_Buffer.set(idx+j*len+i, GX_CAST_S(guchar, v[i]));
            }
        }
        setCStringEnd();
        modifyDone();
	}
#endif
}
void GString::replaceChars(gint idx, gint lenR,
                      gwchar preChar, gint preCount,
                      gwchar sufChar, gint sufCount,
                      const gwchar* v, gint len, gint count)
{
#if GX_WCHAR_16BIT
	replaceChars(idx, lenR, preChar, preCount, sufChar, sufCount, GX_CAST_R(const guchar*, v), len, count);
#else
    if (preCount <0 || count < 0 || sufCount<0 || preCount+count+sufCount<=0) {
        return;
    }
    if (idx < 0 || lenR<0) {
        return;
    }
    gint lenCur = getLength();
    if (idx > lenCur) {
        return;
    }
    if (len<0) {
        len = GX::strlen(v);
    }
    if (m_Buffer.expand(idx, lenR, preCount + len*count + sufCount)) {
        gint start = idx;
        for (gint i=0; i<preCount; i++) {
            m_Buffer.set(start+i, GX_CAST_S(guchar, preChar));
        }
        start += preCount;
        for (gint j=0; j<count; j++) {
            for (gint i = 0; i < len; i++) {
                m_Buffer.set(start+j*len+i, GX_CAST_S(guchar, v[i]));
            }
        }
        start += len*count;
        for (gint i=0; i<sufCount; i++) {
            m_Buffer.set(start+i, GX_CAST_S(guchar, sufChar));
        }
        start += sufCount;
        
        setCStringEnd();
        modifyDone();
    }
#endif
}

void GString::setString(GString* v, gint count)
{
    if (count <= 0) {
        return;
    }
    gint lenV=v->getLength();
    if (m_Buffer.changeCount(lenV*count+1)) {
        const void* src=(const void*)v->m_Buffer.getPtr(0);
        for (gint j=0; j<count; j++) {
            memcpy((void*)m_Buffer.getPtr(0+j*lenV), src, lenV*sizeof(guchar));
        }
        setCStringEnd();
        modifyDone();
    }
    
}
void GString::appendString(GString* v, gint count)
{
    if (count <= 0) {
        return;
    }
    gint lenCur = getLength();
    gint lenV=v->getLength();
    if (m_Buffer.changeCount(lenCur + lenV*count + 1)) {
        const void* src=(const void*)v->m_Buffer.getPtr(0);
        for (gint j=0; j<count; j++) {
            memcpy((void*)m_Buffer.getPtr(lenCur+j*lenV), src, lenV*sizeof(guchar));
        }
        setCStringEnd();
        modifyDone();
    }
}
void GString::insertString(gint idx, GString* v, gint count)
{
    replaceString(idx,0, v, count);
}
void GString::replaceString(gint idx, gint lenR, GString* v, gint count)
{
    if (count <= 0) {
        return;
    }
    if (idx < 0 || lenR<0) {
        return;
    }
    gint lenCur = getLength();
    if (idx > lenCur) {
        return;
    }
    gint lenV=v->getLength();
    
    if (m_Buffer.expand(idx, lenR, lenV*count)) {
        const void* src=(const void*)v->m_Buffer.getPtr(0);
        for (gint j=0; j<count; j++) {
            memcpy((void*)m_Buffer.getPtr(idx+j*lenV), src, lenV*sizeof(guchar));
        }
        setCStringEnd();
        modifyDone();
    }
}




#define M_SET \
if (vsLen < 0 && tempLen < -vsLen) {\
    setChar(fillChar, -vsLen - tempLen);\
    appendChars(temp, tempLen);\
}\
else if (vsLen > 0 && tempLen < vsLen) {\
    setChars(temp, tempLen);\
    appendChar(fillChar, vsLen - tempLen);\
}\
else {\
    setChars(temp, tempLen);\
}
#define M_APPEND \
if (vsLen < 0 && tempLen < -vsLen) {\
    appendChar(fillChar, -vsLen - tempLen);\
    appendChars(temp, tempLen);\
}\
else if (vsLen > 0 && tempLen < vsLen) {\
    appendChars(temp, tempLen);\
    appendChar(fillChar, vsLen - tempLen);\
}\
else {\
    appendChars(temp, tempLen);\
}
#define M_REPLACE \
replaceChars(idx, lenR, \
    fillChar, (vsLen < 0 && tempLen < -vsLen) ? -vsLen - tempLen : 0, \
    fillChar, (vsLen > 0 && tempLen < vsLen) ? vsLen - tempLen : 0, \
    temp, tempLen, 1);


void GString::setNumber(gint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[16];
    gint tempLen = GX::gi16toa(v, temp, radix);
    M_SET
}
void GString::appendNumber(gint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[16];
    gint tempLen = GX::gi16toa(v, temp, radix);
    M_APPEND
}
void GString::insertNumber(gint idx, gint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, 0, v, radix, vsLen, fillChar);
}
void GString::replaceNumber(gint idx, gint lenR, gint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[16];
    gint tempLen = GX::gi16toa(v, temp, radix);
    M_REPLACE
}
void GString::setNumber(guint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[16];
    gint tempLen = GX::gu16toa(v, temp, radix);
    M_SET
}
void GString::appendNumber(guint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[16];
    gint tempLen = GX::gu16toa(v, temp, radix);
    M_APPEND
}
void GString::insertNumber(gint idx, guint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, 0, v, radix, vsLen, fillChar);
}
void GString::replaceNumber(gint idx, gint lenR, guint16 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[16];
    gint tempLen = GX::gu16toa(v, temp, radix);
    M_REPLACE
}

void GString::setNumber(gint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[32];
    gint tempLen = GX::gi32toa(v, temp, radix);
    M_SET
}
void GString::appendNumber(gint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[32];
    gint tempLen = GX::gi32toa(v, temp, radix);
    M_APPEND
}
void GString::insertNumber(gint idx, gint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, 0, v, radix, vsLen, fillChar);
}
void GString::replaceNumber(gint idx, gint lenR, gint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[32];
    gint tempLen = GX::gi32toa(v, temp, radix);
    M_REPLACE
}
void GString::setNumber(guint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[32];
    gint tempLen = GX::gu32toa(v, temp, radix);
    M_SET
}
void GString::appendNumber(guint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[32];
    gint tempLen = GX::gu32toa(v, temp, radix);
    M_APPEND
}
void GString::insertNumber(gint idx, guint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, 0, v, radix, vsLen, fillChar);
}
void GString::replaceNumber(gint idx, gint lenR, guint32 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[32];
    gint tempLen = GX::gu32toa(v, temp, radix);
    M_REPLACE
}

void GString::setNumber(gint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[64];
    gint tempLen = GX::gi64toa(v, temp, radix);
    M_SET
}
void GString::appendNumber(gint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[64];
    gint tempLen = GX::gi64toa(v, temp, radix);
    M_APPEND
}
void GString::insertNumber(gint idx, gint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, 0, v, radix, vsLen, fillChar);
}
void GString::replaceNumber(gint idx, gint lenR, gint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[64];
    gint tempLen = GX::gi64toa(v, temp, radix);
    M_REPLACE
}
void GString::setNumber(guint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[64];
    gint tempLen = GX::gu64toa(v, temp, radix);
    M_SET
}
void GString::appendNumber(guint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[64];
    gint tempLen = GX::gu64toa(v, temp, radix);
    M_APPEND
}
void GString::insertNumber(gint idx, guint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, 0, v, radix, vsLen, fillChar);
}
void GString::replaceNumber(gint idx, gint lenR, guint64 v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    gwchar temp[64];
    gint tempLen = GX::gu64toa(v, temp, radix);
    M_REPLACE
}

void GString::setNumber(gfloat32 v, gint precision, gint vsLen, gwchar fillChar)
{
    gwchar temp[8 + 32];
    gint tempLen = GX::gf32toa(v, temp, precision);
    M_SET
}
void GString::appendNumber(gfloat32 v, gint precision, gint vsLen, gwchar fillChar)
{
    gwchar temp[8 + 32];
    gint tempLen = GX::gf32toa(v, temp, precision);
    M_APPEND
}
void GString::insertNumber(gint idx, gfloat32 v, gint precision, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, 0, v, precision, vsLen, fillChar);
}
void GString::replaceNumber(gint idx, gint lenR, gfloat32 v, gint precision, gint vsLen, gwchar fillChar)
{
    if (precision > 30) {
        precision = 30;
    }
    gwchar temp[8 + 32];
    gint tempLen = GX::gf32toa(v, temp, precision);
    M_REPLACE
}

void GString::setNumber(gfloat64 v, gint precision, gint vsLen, gwchar fillChar)
{
    if (precision > 30) {
        precision = 30;
    }
    gwchar temp[17 + 32];
    gint tempLen = GX::gf64toa(v, temp, precision);
    M_SET
}
void GString::appendNumber(gfloat64 v, gint precision, gint vsLen, gwchar fillChar)
{
    if (precision > 30) {
        precision = 30;
    }
    gwchar temp[17 + 32];
    gint tempLen = GX::gf64toa(v, temp, precision);
    M_APPEND
}
void GString::insertNumber(gint idx, gfloat64 v, gint precision, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, 0, v, precision, vsLen, fillChar);
}
void GString::replaceNumber(gint idx, gint lenR, gfloat64 v, gint precision, gint vsLen, gwchar fillChar)
{
    if (precision > 30) {
        precision = 30;
    }
    gwchar temp[17 + 32];
    gint tempLen = GX::gf64toa(v, temp, precision);
    M_REPLACE
}

#undef M_SET
#undef M_APPEND
#undef M_REPLACE

void GString::setPtr(void* v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    setNumber(GX_CAST_R(guint, v), radix, vsLen, fillChar);
}
void GString::appendPtr(void* v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    appendNumber(GX_CAST_R(guint, v), radix, vsLen, fillChar);
}
void GString::insertPtr(gint idx, void* v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    insertNumber(idx, GX_CAST_R(guint, v), radix, vsLen, fillChar);
}
void GString::replacePtr(gint idx, gint lenR, void* v, GX::StringRadix radix, gint vsLen, gwchar fillChar)
{
    replaceNumber(idx, lenR, GX_CAST_R(guint, v), radix, vsLen, fillChar);
}


GString::Formater GString::format(const gchar* fmt, gint len)
{
    setChars(fmt, len);
    return Formater(this, 0, 0);
}

GString::Formater GString::appendFormat(const gchar* fmt, gint len)
{
    gint lenCur = getLength();
    appendChars(fmt,len);
    return Formater(this, lenCur, 0);
}
GString::Formater GString::insertFormat(gint idx, const gchar* fmt, gint len)
{
    gint lenTemp = getLength() - idx;
    insertChars(idx, fmt, len);
    return Formater(this, idx, lenTemp);
}
GString::Formater GString::replaceFormat(gint idx, gint lenR, const gchar* fmt, gint len)
{
    gint lenTemp = getLength() - idx - lenR;
    replaceChars(idx, lenR, fmt, len);
    return Formater(this, idx, lenTemp);
}



guchar GString::pathSeparator()
{
#if defined(GX_OS_WINDOWS) || defined(GX_OS_QT_WINDOWS)
	return GX_CAST_S(guchar,'\\');
#else
	return GX_CAST_S(guchar,'/');
#endif
}
bool GString::firstIsSeparator()
{
	gint count = m_Buffer.getCount();
	if (count>0) {
		const guchar c = m_Buffer.first();
		return (c == (guchar)'/' || c == (guchar)'\\');
	}
	return false;
}

bool GString::lastIsSeparator()
{
    gint count=m_Buffer.getCount();
    if (count>0) {
        const guchar c = m_Buffer.last();
        return (c == (guchar)'/' || c == (guchar)'\\');
    }
    return false;
}

void GString::appendPathComponent(const gchar* component, gint len)
{
    if (lastIsSeparator()) {
        if (component[0] == '/' || component[0] == '\\') {
            if (len < 0) {
                len = GX::strlen(component);
            }
            appendChars(component + 1, len - 1);
        }
        else {
            appendChars(component, len);
        }
    }
    else {
        if (component[0] == '/' || component[0] == '\\') {
            appendChars(component, len);
        }
        else {
			appendChar(pathSeparator());
            appendChars(component, len);
        }
    }
}

void GString::appendPathComponent(const guchar* component, gint len)
{
	if (lastIsSeparator()) {
		if (component[0] == GX_CAST_S(guchar, '/') || component[0] == GX_CAST_S(guchar, '\\')) {
			if (len < 0) {
				len = GX::strlen(component);
			}
			appendChars(component + 1, len - 1);
		}
		else {
			appendChars(component, len);
		}
	}
	else {
		if (component[0] == GX_CAST_S(guchar, '/') || component[0] == GX_CAST_S(guchar, '\\')) {
			appendChars(component, len);
		}
		else {
			appendChar(pathSeparator());
			appendChars(component, len);
		}
	}
}

void GString::appendPathComponent(const gwchar* component, gint len)
{
    if (lastIsSeparator()) {
        if (component[0] == L'/' || component[0] == L'\\') {
            if (len < 0) {
                len = GX::strlen(component);
            }
            appendChars(component + 1, len - 1);
        }
        else {
            appendChars(component, len);
        }
    }
    else {
        if (component[0] == L'/' || component[0] == L'\\') {
            appendChars(component, len);
        }
        else {
			appendChar(pathSeparator());
            appendChars(component, len);
        }
    }
}

void GString::appendPathComponent(GString* component)
{
	appendPathComponent(GX_CAST_R(const guchar*, component->m_Buffer.getPtr(0)), component->getLength());
}




void GString::setCStringEnd()
{
    gint count=m_Buffer.getCount();
    if (count>0) {
        m_Buffer.set(count-1, 0);
    }
}

void GString::modifyDone()
{
    m_OWHash.codeA=0;
    if (m_OutBuf.getBytes()>0) {
        m_OutBuf.freeSelf();
    }
}

gint GString::getCStringLength(GX::StringEncoding se)
{
    if (m_Buffer.isEmpty()) {
        return 0;
    }
    
    if (se==GX::SE_Utf8) {
        if (m_OutBuf.getBytes()>0 && m_OutBufSE==se) {
            return GX_CAST_S(gint, m_OutBuf.getBytes()/sizeof(gchar));
        }
        return GX::strUTF16toUTF8Count(m_Buffer.getPtr(0), m_Buffer.getCount()-1);
    }
    else if (se==GX::SE_Utf16 || se==GX::SE_Utf32) {
        return m_Buffer.getCount()-1;
    }
    
    return 0;
}

const void* GString::getCString(GX::StringEncoding se)
{
    if (m_OutBuf.getBytes()>0 && m_OutBufSE==se) {
        return m_OutBuf.getPtr();
    }
    
    if (se==GX::SE_Utf8) {
        if (m_Buffer.isEmpty()) {
            return "";
        }
        gint lenTo=GX::strUTF16toUTF8Count(m_Buffer.getPtr(0), m_Buffer.getCount());
        m_OutBuf.changeBytes(lenTo*sizeof(gchar));
        GX::strUTF16toUTF8(m_Buffer.getPtr(0), m_Buffer.getCount(), GX_CAST_R(gchar*, m_OutBuf.getPtr()), lenTo);
        
        m_OutBufSE=se;
        return m_OutBuf.getPtr();
    }
    else if (se==GX::SE_Utf16) {
        if (m_Buffer.isEmpty()) {
            static guchar g_Buf[]={0};
            return g_Buf;
        }
        return m_Buffer.getPtr(0);
    }
    else if (se==GX::SE_Utf32) {
        if (m_Buffer.isEmpty()) {
            static guint32 g_Buf[]={0u};
            return g_Buf;
        }
        
        gint lenTo=m_Buffer.getCount();
        m_OutBuf.changeBytes(lenTo*sizeof(guint32));
        guint32* p=GX_CAST_R(guint32*, m_OutBuf.getPtr());
        for (gint i=0; i<lenTo; ++i) {
            (*p)=GX_CAST_S(guint32, m_Buffer.get(i));
        }
        
        m_OutBufSE=se;
        return m_OutBuf.getPtr();
    }
    
    return NULL;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const GX::UUID g_UUID(0x3932637B, 0x36333461, 0x32652D33, 0x342D6134);

const GX::UUID& GString::seGetUUID()
{
    return g_UUID;
}
gint GString::seGetBytes()
{
    gint res = seBytesOfKeyAndData(SKBuf, (guint)(getLength()*sizeof(guchar)));
    return res;
}
gint GString::seEncodeFields(GEncoder& coder)
{
    gint res = seEncodeKeyAndData(coder, SKBuf, m_Buffer.getPtr(0), (guint)(getLength()*sizeof(guchar)));
    return res;
}


const GX::UUID& GString::ueGetUUID()
{
    return g_UUID;
}

gint GString::ueDecodeField(GDecoder& coder, guint32 key, guint32 len)
{
    switch (key)
    {
        case SKBuf:
        {
            gint count = (gint)len/sizeof(guchar);
            if (m_Buffer.changeCount(count+1)) {
                gint res=ueDecodeData(coder, (void*)m_Buffer.getPtr(0), (guint)len);
                if (res < 0) {
                    return -1;
                }
                setCStringEnd();
                modifyDone();
                return res;
            }
            return -1;
        }
            break;
        default:
            return 0;
    }
}

