// Copyright (c) ChimeraXR. All Rights Reserved.

#pragma once

#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"

class JniUtils
{
public:
	static jobject ToJavaString(FString String);
	static FString ToFString(jstring String);
	static jbyteArray ToJavaByteArray(TArray<uint8> Data);
};