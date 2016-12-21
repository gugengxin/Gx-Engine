﻿//
// Created by Gengxin Gu on 2016/12/9.
//

#ifndef AS_GJNI_H
#define AS_GJNI_H


#include "GXPrefix.h"
#if defined(GX_OS_ANDROID)

#include <jni.h>

class GJNI {
public:
    static GJNI* shared();

protected:
    GJNI();
    virtual ~GJNI();

    void init(JNIEnv *env, jobject classLoader);
    void uninit(JNIEnv *env);

public:
    JNIEnv *attachEnv(bool *needDetach);
    void detachEnv();

    jclass findClass(JNIEnv *jniEnv, const char *name);

private:
    JavaVM*     m_VM;
    jobject     m_CLInst;
    jclass      m_CLClass;
    jmethodID   m_CLLoadClassMd;

public:
    class EnvHolder {
    public:
        EnvHolder() {
            m_Env=shared()->attachEnv(&m_NeedDetach);
        }

        ~EnvHolder() {
            if(m_NeedDetach) {
                shared()->detachEnv();
            }
        }

        JNIEnv* get() {
            return m_Env;
        }

        JNIEnv* operator->(){
            return m_Env;
        }

    private:
        JNIEnv* m_Env;
        bool m_NeedDetach;
    };

public:

    class Caller {
    protected:
        Caller();
        virtual ~Caller();

        virtual void init(JNIEnv* jniEnv,const char* className);

        inline jclass getClass() {
            return m_Class;
        }

        jmethodID getMethod(JNIEnv* jniEnv,const char* name,const char* sig,bool isStatic);

        void callStaticVoidMethod(JNIEnv* jniEnv,jmethodID method,...);
        jobject callStaticObjectMethod(JNIEnv* jniEnv,jmethodID method,...);
        jbyte callStaticByteMethod(JNIEnv* jniEnv,jmethodID method,...);
        jfloat callStaticFloatMethod(JNIEnv* jniEnv,jmethodID method,...);


        void callVoidMethod(JNIEnv* jniEnv,jobject obj,jmethodID method,...);
        jobject callObjectMethod(JNIEnv* jniEnv,jobject obj,jmethodID method,...);
        jint callIntMethod(JNIEnv* jniEnv,jobject obj,jmethodID method,...);
    private:
        jclass 		m_Class;
    };

};


#endif

#endif //AS_GJNI_H
