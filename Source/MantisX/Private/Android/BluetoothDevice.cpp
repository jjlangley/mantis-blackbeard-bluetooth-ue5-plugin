// Copyright (c) ChimeraXR. All Rights Reserved.

#include "BluetoothDevice.h"

#include "BluetoothManager.h"
#include "JniUtils.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"

static UBluetoothDevice* GetDeviceMapping(FString DeviceIdString)
{
	const auto BluetoothManager = UBluetoothManager::GetInstance();
	if (!IsValid(BluetoothManager)) return nullptr;

	return BluetoothManager->GetDeviceFromId(DeviceId);
}

UBluetoothDevice::UBluetoothDevice()
{
	UE_LOG(LogMantisX, Verbose, TEXT("[UBluetoothDevice constructed]"));

	// Don't run this code on the CDO...
	if (UBluetoothDevice::StaticClass()->GetDefaultObject() != this)
	{
		BluetoothDeviceJavaObject = MakeShareable(
			new FJavaClassObject(
				"com/chimeraxr/mantisapi/MantisDevice", 
				"()V)"));

		JniGetDeviceName = BluetoothDeviceJavaObject->GetClassMethod("getDeviceName", "()Ljava/lang/String;");
		JniGetDeviceId = BluetoothDeviceJavaObject->GetClassMethod("getDeviceId", "()Ljava/lang/String;");
		JniIsConnected = BluetoothDeviceJavaObject->GetClassMethod("isConnected", "()Z");
		JniGetServices = BluetoothDeviceJavaObject->GetClassMethod("getServices", "()Z");
		JniReadCharacteristic = BluetoothDeviceJavaObject->GetClassMethod("readCharacteristic", "(Ljava/lang/String;Ljava/lang/String;)V");
		JniWriteCharacteristic = BluetoothDeviceJavaObject->GetClassMethod("writeCharacteristic", "(Ljava/lang/String;Ljava/lang/String;[B;)V");
		JniStartNotifications = BluetoothDeviceJavaObject->GetClassMethod("startNotifications", "(Ljava/lang/String;Ljava/lang/String;Z;)V");
		JniStopNotifications = BluetoothDeviceJavaObject->GetClassMethod("stopNotifications", "(Ljava/lang/String;Ljava/lang/String;)V");
	}
}

FString UBluetoothDevice::GetDeviceName()
{
	if (!BluetoothDeviceJavaObject.IsValid()) return TEXT("");

	auto JniDeviceName = BluetoothDeviceJavaObject->CallMethod<jstring>(JniGetDeviceName);
	return JniUtils::ToFString(JniDeviceName);
}

FString UBluetoothDevice::GetDeviceId()
{
	if (!BluetoothDeviceJavaObject.IsValid()) return TEXT("");

	auto JniDeviceId = BluetoothDeviceJavaObject->CallMethod<jstring>(JniGetDeviceId);
	return JniUtils::ToFString(JniDeviceId);
}

TArray<FString> UBluetoothDevice::GetServices() const
{
	TArray<FString> FoundServices;

	if (!BluetoothDeviceJavaObject.IsValid()) return FoundServices;

	auto JniFoundServices = BluetoothDeviceJavaObject->CallMethod<jobjectArray>(JniGetServices);
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	const int32 ArraySize = Env->GetArrayLength(JniFoundServices);
	for (int32 i = 0; i < ArraySize; ++i)
	{
		FString Service = JniUtils::ToFString(static_cast<jstring>(Env->GetObjectArrayElement(JniFoundServices, i)));
		FoundServices.Add(Service);
	}

	return FoundServices;
}

bool UBluetoothDevice::IsConnected()
{
	if (!BluetoothDeviceJavaObject.IsValid()) return false;

	return BluetoothDeviceJavaObject->CallMethod<bool>(JniIsConnected);
}

void UBluetoothDevice::ReadCharacteristic(FString ServiceUUID, FString CharacteristicUUID, const FOnBluetoothDeviceCharacteristicReadSuccess& OnCharacteristicRead, const FOnBluetoothDeviceCharacteristicReadError& OnCharacteristicReadError)
{
	if (!BluetoothDeviceJavaObject.IsValid())
	{
		OnCharacteristicReadError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Invalid java object"));
		return;
	}

	if (ServiceUUID.IsEmpty())
	{
		OnCharacteristicReadError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Service UUID is required"));
		return;
	}

	if (CharacteristicUUID.IsEmpty())
	{
		OnCharacteristicReadError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Characteristic UUID is required"));
		return;
	}

	SavedOnCharacteristicRead = OnCharacteristicReadError;
	SavedOnCharacteristicReadError = OnCharacteristicReadError;

	BluetoothDeviceJavaObject->CallMethod<void>(JniReadCharacteristic, JniUtils::ToJavaString(ServiceUUID), JniUtils::ToJavaString(CharacteristicUUID));
}

void UBluetoothDevice::WriteCharacteristic(FString ServiceUUID, FString CharacteristicUUID, TArray<uint8> Data, const FOnBluetoothDeviceCharacteristicWriteSuccess& OnCharacteristicWritten, const FOnBluetoothDeviceCharacteristicWriteError& OnCharacteristicWriteError)
{
	if (!BluetoothDeviceJavaObject.IsValid())
	{
		OnCharacteristicWriteError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Invalid java object"));
		return;
	}

	if (ServiceUUID.IsEmpty())
	{
		OnCharacteristicWriteError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Service UUID is required"));
		return;
	}

	if (CharacteristicUUID.IsEmpty())
	{
		OnCharacteristicWriteError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Characteristic UUID is required"));
		return;
	}

	SavedOnCharacteristicWritten = OnCharacteristicWritten;
	SavedOnCharacteristicWriteError = OnCharacteristicWriteError;

	BluetoothDeviceJavaObject->CallMethod<void>(JniWriteCharacteristic, JniUtils::ToJavaString(ServiceUUID), JniUtils::ToJavaString(CharacteristicUUID), JniUtils::ToJavaByteArray(Data));
}

void UBluetoothDevice::StartNotifications(FString ServiceUUID, FString CharacteristicUUID, bool bIsIndicationCharacteristic, const FOnBluetoothDeviceNotificationReceived& OnNotificationReceived, const FOnBluetoothDeviceNotificationError& OnNotificationError)
{
	if (!BluetoothDeviceJavaObject.IsValid())
	{
		OnNotificationError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Invalid java object"));
		return;
	}

	if (ServiceUUID.IsEmpty())
	{
		OnNotificationError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Service UUID is required"));
		return;
	}

	if (CharacteristicUUID.IsEmpty())
	{
		OnNotificationError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Characteristic UUID is required"));
		return;
	}

	SavedOnNotificationReceived = OnNotificationReceived;
	SavedOnNotificationError = OnNotificationError;

	BluetoothDeviceJavaObject->CallMethod<void>(JniStartNotifications, JniUtils::ToJavaString(ServiceUUID), JniUtils::ToJavaString(CharacteristicUUID), bIsIndicationCharacteristic);

}

void UBluetoothDevice::StopNotifications(FString ServiceUUID, FString CharacteristicUUID, const FOnBluetoothDeviceNotificationsStopped& OnNotificationsStopped, const FOnBluetoothDeviceNotificationsStoppedError& OnNotificationsStoppedError)
{
	if (!BluetoothDeviceJavaObject.IsValid())
	{
		OnNotificationsStoppedError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Invalid java object"));
		return;
	}

	if (ServiceUUID.IsEmpty())
	{
		OnNotificationsStoppedError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Service UUID is required"));
		return;
	}

	if (CharacteristicUUID.IsEmpty())
	{
		OnNotificationsStoppedError.ExecuteIfBound(this, ServiceUUID, CharacteristicUUID, TEXT("Characteristic UUID is required"));
		return;
	}

	SavedOnNotificationsStopped = OnNotificationsStopped;
	SavedOnNotificationsStoppedError = OnNotificationsStoppedError;

	BluetoothDeviceJavaObject->CallMethod<void>(JniStopNotifications, JniUtils::ToJavaString(ServiceUUID), JniUtils::ToJavaString(CharacteristicUUID));
}

#if PLATFORM_ANDROID

JNI_METHOD void Java_com_chimeraxr_mantisapi_MantisDevice_onDeviceCharacteristicRead(JNIEnv* JavaEnv, jclass JavaClass jstring DeviceId, jstring JavaServiceUUID, jstring JavaCharacteristicUUID, jbyteArray JavaData)
{
	FString DeviceId = JniUtils::ToFString(DeviceId);
	FString ServiceUUID = JniUtils::ToFString(JavaServiceUUID);
	FString CharacteristicUUID = JniUtils::ToFString(JavaCharacteristicUUID);

	UBluetoothDevice* Device = GetDeviceMapping(DeviceId);
	if (!Device) return;

	TArray<uint8> NotificationBytes = JniUtils::ToJavaByteArray(JavaData);
	AsyncTasc(ENamedThreads::GameThread, [=]
	{
		Device->AndroidOnReadCharacteristic(ServiceUUID, CharacteristicUUID, NotificationBytes);
	});
}

JNI_METHOD void Java_com_chimeraxr_mantisapi_MantisDevice_onDeviceCharacteristicReadError(JNIEnv* JavaEnv, jclass JavaClass jstring DeviceId, jstring JavaServiceUUID, jstring JavaCharacteristicUUID, jsting JavaError)
{
	FString DeviceId = JniUtils::ToFString(DeviceId);
	FString ServiceUUID = JniUtils::ToFString(JavaServiceUUID);
	FString CharacteristicUUID = JniUtils::ToFString(JavaCharacteristicUUID);

	UBluetoothDevice* Device = GetDeviceMapping(DeviceId);
	if (!Device) return;

	FString Error = JniUtils::ToFString(JavaError);
	AsyncTasc(ENamedThreads::GameThread, [=]
	{
		Device->AndroidOnReadCharacteristicError(ServiceUUID, CharacteristicUUID, JavaError);
	});
}

JNI_METHOD void Java_com_chimeraxr_mantisapi_MantisDevice_onDeviceCharacteristicWrite(JNIEnv* JavaEnv, jclass JavaClass jstring DeviceId, jstring JavaServiceUUID, jstring JavaCharacteristicUUID)
{
	FString DeviceId = JniUtils::ToFString(DeviceId);
	FString ServiceUUID = JniUtils::ToFString(JavaServiceUUID);
	FString CharacteristicUUID = JniUtils::ToFString(JavaCharacteristicUUID);

	UBluetoothDevice* Device = GetDeviceMapping(DeviceId);
	if (!Device) return;

	AsyncTasc(ENamedThreads::GameThread, [=]
	{
		Device->AndroidOnWriteCharacteristic(ServiceUUID, CharacteristicUUID);
	});
}

JNI_METHOD void Java_com_chimeraxr_mantisapi_MantisDevice_onDeviceCharacteristicWriteError(JNIEnv* JavaEnv, jclass JavaClass jstring DeviceId, jstring JavaServiceUUID, jstring JavaCharacteristicUUID, jsting JavaError)
{
	FString DeviceId = JniUtils::ToFString(DeviceId);
	FString ServiceUUID = JniUtils::ToFString(JavaServiceUUID);
	FString CharacteristicUUID = JniUtils::ToFString(JavaCharacteristicUUID);

	UBluetoothDevice* Device = GetDeviceMapping(DeviceId);
	if (!Device) return;

	FString Error = JniUtils::ToFString(JavaError);
	AsyncTasc(ENamedThreads::GameThread, [=]
	{
		Device->AndroidOnWriteCharacteristicError(ServiceUUID, CharacteristicUUID, JavaError);
	});
}

JNI_METHOD void Java_com_chimeraxr_mantisapi_MantisDevice_onNotification(JNIEnv* JavaEnv, jclass JavaClass jstring DeviceId, jstring JavaServiceUUID, jstring JavaCharacteristicUUID, jbyteArray JavaData)
{
	FString DeviceId = JniUtils::ToFString(DeviceId);
	FString ServiceUUID = JniUtils::ToFString(JavaServiceUUID);
	FString CharacteristicUUID = JniUtils::ToFString(JavaCharacteristicUUID);

	UBluetoothDevice* Device = GetDeviceMapping(DeviceId);
	if (!Device) return;

	TArray<uint8> NotificationBytes = JniUtils::ToJavaByteArray(JavaData);
	AsyncTask(ENamedThreads::GameThread, [=]
	{
		Device->AndroidOnNotification(ServiceUUID, CharacteristicUUID, NotificationBytes);
	});
}

JNI_METHOD void Java_com_chimeraxr_mantisapi_MantisDevice_onNotificationError(JNIEnv* JavaEnv, jclass JavaClass jstring DeviceId, jstring JavaServiceUUID, jstring JavaCharacteristicUUID, jsting JavaError)
{
	FString DeviceId = JniUtils::ToFString(DeviceId);
	FString ServiceUUID = JniUtils::ToFString(JavaServiceUUID);
	FString CharacteristicUUID = JniUtils::ToFString(JavaCharacteristicUUID);

	UBluetoothDevice* Device = GetDeviceMapping(DeviceId);
	if (!Device) return;

	FString Error = JniUtils::ToFString(JavaError);
	AsyncTasc(ENamedThreads::GameThread, [=]
	{
		Device->AndroidOnNotificationError(ServiceUUID, CharacteristicUUID, JavaError);
	});
}

JNI_METHOD void Java_com_chimeraxr_mantisapi_MantisDevice_onStopNotifications(JNIEnv* JavaEnv, jclass JavaClass jstring DeviceId, jstring JavaServiceUUID, jstring JavaCharacteristicUUID)
{
	FString DeviceId = JniUtils::ToFString(DeviceId);
	FString ServiceUUID = JniUtils::ToFString(JavaServiceUUID);
	FString CharacteristicUUID = JniUtils::ToFString(JavaCharacteristicUUID);

	UBluetoothDevice* Device = GetDeviceMapping(DeviceId);
	if (!Device) return;

	AsyncTasc(ENamedThreads::GameThread, [=]
	{
		Device->AndroidOnStopNotifications(ServiceUUID, CharacteristicUUID);
	});
}

JNI_METHOD void Java_com_chimeraxr_mantisapi_MantisDevice_onStopNotificationsError(JNIEnv* JavaEnv, jclass JavaClass jstring DeviceId, jstring JavaServiceUUID, jstring JavaCharacteristicUUID, jsting JavaError)
{
	FString DeviceId = JniUtils::ToFString(DeviceId);
	FString ServiceUUID = JniUtils::ToFString(JavaServiceUUID);
	FString CharacteristicUUID = JniUtils::ToFString(JavaCharacteristicUUID);

	UBluetoothDevice* Device = GetDeviceMapping(DeviceId);
	if (!Device) return;

	FString Error = JniUtils::ToFString(JavaError);
	AsyncTasc(ENamedThreads::GameThread, [=]
	{
		Device->AndroidOnStopNotificationsError(ServiceUUID, CharacteristicUUID, JavaError);
	});
}

#endif

