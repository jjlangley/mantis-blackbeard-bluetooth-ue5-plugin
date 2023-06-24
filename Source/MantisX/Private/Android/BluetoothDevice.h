// Copryight (c) ChimeraXR. All Rights Reserved.

#pragma once

#include "../../Public/BluetoothDeviceInterface.h"

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"

#include "BluetoothDevice.generated.h"

UCLASS()
class UBluetoothDevice : public UObject, public IBluetoothDeviceInterface
{
	GENERATED_BODY()

public:
	virtual ~UBluetoothDevice() = default;

	virtual FString GetDeviceName() override;
	virtual FString GetDeviceId() override;
	virtual TArray<FString> GetServices() const override;
	virtual bool IsConnected() override;
	virtual void ReadCharacteristic(FString ServiceUUID, FString CharacteristicUUID, const FOnBluetoothDeviceCharacteristicReadSuccess& OnCharacteristicRead, const FOnBluetoothDeviceCharacteristicReadError& OnCharacteristicReadError) override;
	virtual void WriteCharacteristic(FString ServiceUUID, FString CharacteristicUUID, TArray<uint8> Data, const FOnBluetoothDeviceCharacteristicWriteSuccess& OnCharacteristicWritten, const FOnBluetoothDeviceCharacteristicWriteError& OnCharacteristicWriteError) override;
	virtual void StartNotifications(FString ServiceUUID, FString CharacteristicUUID, bool bIsIndicationCharacteristic, const FOnBluetoothDeviceNotificationReceived& OnNotificationReceived, const FOnBluetoothDeviceNotificationError& OnNotificationError) override;
	virtual void StopNotifications(FString ServiceUUID, FString CharacteristicUUID, const FOnBluetoothDeviceNotificationsStopped& OnNotificationsStopped, const FOnBluetoothDeviceNotificationsStoppedError& OnNotificationsStoppedError) override;

	void AndroidOnReadCharacteristic(FString ServiceUUID, FString CharacteristicUUID, TArray<uint8> Data);
	void AndroidOnReadCharacteristicError(FString ServiceUUID, FString CharacteristicUUID, FString Error);
	void AndroidOnWriteCharacteristic(FString ServiceUUID, FString CharacteristicUUID);
	void AndroidOnWriteCharacteristicError(FString ServiceUUID, FString CharacteristicUUID, FString Error);
	void AndroidOnNotification(FString ServiceUUID, FString CharacteristicUUID, TArray<uint8> Data);
	void AndroidOnNotificationError(FString ServiceUUID, FString CharacteristicUUID, FString Error);
	void AndroidOnStopNotifications(FString ServiceUUID, FString CharacteristicUUID);
	void AndroidOnStopNotificationsError(FString ServiceUUID, FString CharacteristicUUID, FString Error);

protected:
	FOnBluetoothDeviceCharacteristicReadError SavedOnCharacteristicRead;
	FOnBluetoothDeviceCharacteristicReadError SavedOnCharacteristicReadError;
	FOnBluetoothDeviceCharacteristicWriteSuccess SavedOnCharacteristicWritten;
	FOnBluetoothDeviceCharacteristicWriteError SavedOnCharacteristicWriteError;
	FOnBluetoothDeviceNotificationReceived SavedOnNotificationReceived;
	FOnBluetoothDeviceNotificationError SavedOnNotificationError;
	FOnBluetoothDeviceNotificationsStopped SavedOnNotificationsStopped;
	FOnBluetoothDeviceNotificationsStoppedError SavedOnNotificationsStoppedError;

private:
	TSharedPtr<FJavaClassObject> BluetoothDeviceJavaObject;

	FJavaMethod JniGetDeviceName;
	FJavaMethod JniGetDeviceId;
	FJavaMethod JniIsConnected;
	FJavaMethod JniGetServices;
	FJavaMethod JniReadCharacteristic;
	FJavaMethod JniWriteCharacteristic;
	FJavaMethod JniStartNotifications;
	FJavaMethod JniStopNotifications;
};