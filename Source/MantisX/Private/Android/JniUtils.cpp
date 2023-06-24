// Copyright (c) ChimeraXR. All Rights Reserved.

#include "JniUtils.h"

#include "Android/AndroidJNI.h"

jobject JniUtils::ToJavaString(FString String)
{
	JNIEnv*	JEnv = AndroidJavaEnv::GetJavaEnv();
	jstring Local = JEnv->NewStringUTF(TCHAR_TO_UTF8(*String));
	jstring Result = (jstring)JEnv->NewGlobalRef(Local);
	JEnv->DeleteLocalRef(Local);
	return Result;	
}

FString JniUtils::ToFString(jstring String)
{
	FString Result;
	if (!String) return Result;

	JNIEnv* JEnv = AndroidJavaEnv::GetJavaEnv();
	const char* Characters = JEnv->GetStringUTFChars(String, 0);
	Result = FString(UTF8_TO_TCHAR(Characters));
	JEnv->ReleaseStringUTFChars(String, Characters);

	return Result;
}

jbyteArray JniUtils::ToJavaByteArray(TArray<uint8> Data)
{
	const int DataSize = Data.Num();
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jbyteArray Array = Env->NewByteArray(DataSize);

	jbyte Buffer[DataSize];
	for (int i = 0; i < DataSize; i++)
	{
		Buffer[i] = Data[i];
	}

	Env->SetByteArrayRegion(Array, 0, DataSize, Buffer);
	jbyteArray ResultArray = (jbyteArray)Env->NewGlobalRef(Array);
	Env->DeleteLocalRef(Array);

	return ResultArray;
}
