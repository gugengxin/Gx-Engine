﻿//
//  GZipReader.cpp
//  GX
//
//  Created by Gengxin Gu on 16/6/22.
//  Copyright © 2016年 Gengxin Gu. All rights reserved.
//

#include "GZipReader.h"
#include "GXGObject.h"
#include "zlib.h"
#include "unzip.h"
#if defined(GX_OS_WINDOWS)
#include "iowin32.h"
#endif

#if GX_PTR_32BIT

#define M_OPENZIP(path,def)			unzOpen2((const char*)path,def)
#define M_FILEFUNC_DEF				zlib_filefunc_def
#define M_FILL_FILEFUNC				fill_fopen_filefunc
#define M_FILL_WIN32_FILEFUNC		fill_win32_filefunc
#define M_FILE_POS					uLong
#define M_GETCURRENTFILEINFO		unzGetCurrentFileInfo
#define M_GETOFFSET					unzGetOffset
#define M_SETOFFSET					unzSetOffset
#define M_TELL						unztell
#define M_FILE_INFO					unz_file_info

#elif GX_PTR_64BIT

#define M_OPENZIP(path,def)			unzOpen2_64((const void*)path,def)
#define M_FILEFUNC_DEF				zlib_filefunc64_def
#define M_FILL_FILEFUNC				fill_fopen64_filefunc
#define M_FILL_WIN32_FILEFUNC		fill_win32_filefunc64
#define M_FILE_POS					ZPOS64_T
#define M_GETCURRENTFILEINFO		unzGetCurrentFileInfo64
#define M_GETOFFSET					unzGetOffset64
#define M_SETOFFSET					unzSetOffset64
#define M_TELL						unztell64
#define M_FILE_INFO					unz_file_info64

#endif

#define M_ZIPFILE() ((unzFile)m_ZipFile)


GX_GOBJECT_IMPLEMENT(GZipReader, GReader);

GZipReader::GZipReader()
{
	m_ZipFile = NULL;
	m_IsCFOpened = false;
}

GZipReader::~GZipReader()
{
	if (m_ZipFile) {
		unzCloseCurrentFile(M_ZIPFILE());
		unzClose(M_ZIPFILE());
	}
}

bool GZipReader::open(const gtchar* path)
{
	M_FILEFUNC_DEF funDef;
#ifdef GX_OS_MICROSOFT
	M_FILL_WIN32_FILEFUNC(&funDef);
#else
	M_FILL_FILEFUNC(&funDef);
#endif
	m_ZipFile = M_OPENZIP(path, &funDef);
	return m_ZipFile!=NULL;
}

void GZipReader::close()
{
	if (m_ZipFile) {
		if (m_IsCFOpened) {
			closeCurrentFile();
		}
		unzClose(M_ZIPFILE());
		m_ZipFile = NULL;
	}
}

guint GZipReader::getOffset()
{
	return M_GETOFFSET(M_ZIPFILE());
}

bool GZipReader::setOffset(guint v)
{
	return M_SETOFFSET(M_ZIPFILE(), v) == UNZ_OK;
}

bool GZipReader::gotoFirstFile()
{
	return unzGoToFirstFile(M_ZIPFILE()) == UNZ_OK;
}
bool GZipReader::gotoNextFile()
{
	return unzGoToNextFile(M_ZIPFILE()) == UNZ_OK;
}
bool GZipReader::gotoFile(const gchar* fileName)
{
	return unzLocateFile(M_ZIPFILE(), fileName, 0) == UNZ_OK;
}
bool GZipReader::getCurrentFileName(gchar* buf, guint bufLen)
{
	return M_GETCURRENTFILEINFO(M_ZIPFILE(), NULL, buf, (uLong)bufLen, NULL, 0, NULL, 0) == UNZ_OK;
}

bool GZipReader::openCurrentFile()
{
	if (unzOpenCurrentFile(M_ZIPFILE()) == UNZ_OK) {
		m_IsCFOpened = true;
		return true;
	}
	return false;
}
bool GZipReader::closeCurrentFile()
{
	if (unzCloseCurrentFile(M_ZIPFILE()) == UNZ_OK) {
		m_IsCFOpened = false;
		return true;
	}
	return false;
}

bool GZipReader::hasData()
{
	return unzeof(M_ZIPFILE())==0;
}

gint GZipReader::read(void* buf, guint len)
{
	return unzReadCurrentFile(M_ZIPFILE(), buf, (unsigned)len);
}

bool GZipReader::skip(guint len)
{
	guint8 buffer[256];
	int bytes;
	while (len>0) {
		if (len<sizeof(buffer)) {
			bytes = unzReadCurrentFile(M_ZIPFILE(), buffer, (unsigned)len);
			if (bytes<(int)len) {
				return false;
			}
		}
		else {
			bytes = unzReadCurrentFile(M_ZIPFILE(), buffer, (unsigned)sizeof(buffer));
			if (bytes<(int)sizeof(buffer)) {
				return false;
			}
		}
		len -= (guint)bytes;
	}
	return true;
}

gint GZipReader::getBytes()
{
	return (gint)M_TELL(M_ZIPFILE());
}

bool GZipReader::canRollback()
{
	return false;
}

bool GZipReader::canGetLength() 
{
	return true;
}
gint GZipReader::getLength()
{
	M_FILE_INFO info;
	if (M_GETCURRENTFILEINFO(M_ZIPFILE(), &info, NULL, 0, NULL, 0, NULL, 0) == UNZ_OK) {
		return (gint)info.uncompressed_size;
	}
	return -1;
}