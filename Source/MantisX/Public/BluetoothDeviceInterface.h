// Copyright (c) ChimeraXR. All Rights Reserved.

#pragma once

#include "Interface.h"
#include "BluetoothDeviceInterface.generated.h"

class IBluetoothDeviceInterface;

DECLARE_DYNAMIC_DELEGATE_FourParams(FOnBluetoothDeviceCharacteristicReadSuccess, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service UUID, FString, CharacteristicUUID, const TArray<uint8>&, Data);
DECLARE_DYNAMIC_DELEGATE_FourParams(FOnBluetoothDeviceCharacteristicReadError, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service UUID, FString, CharacteristicUUID, FString, Error);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnBluetoothDeviceCharacteristicWriteSuccess, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service UUID, FString, CharacteristicUUID);
DECLARE_DYNAMIC_DELEGATE_FourParams(FOnBluetoothDeviceCharacteristicWriteError, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service UUID, FString, CharacteristicUUID, FString, Error);
DECLARE_DYNAMIC_DELEGATE_FourParams(FOnBluetoothDeviceNotificationReceived, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service UUID, FString, CharacteristicUUID, const TArray<uint8>&, Data);
DECLARE_DYNAMIC_DELEGATE_FourParams(FOnBluetoothDeviceNotificationError, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service UUID, FString, CharacteristicUUID, FString, Error);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnBluetoothDeviceNotificationsStopped, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service UUID, FString, CharacteristicUUID);
DECLARE_DYNAMIC_DELEGATE_FourParams(FOnBluetoothDeviceNotificationsStoppedError, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service UUID, FString, CharacteristicUUID, FString, Error);

UINTERFACE(BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class UBluetoothDeviceInterface : public UInterface
{
	GENERATED_BODY()
};

class IBluetoothDeviceInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual FString GetDeviceName() = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual FString GetDeviceId() = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions", DisplayName = "Get Service UUID")
	virtual TArray<FString> GetServices() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual bool IsConnected() = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void ReadCharacteristic(FString ServiceUUID, FString CharacteristicUUID, const FOnBluetoothDeviceCharacteristicReadSuccess& OnCharacteristicRead, const FOnBluetoothDeviceCharacteristicReadError& OnCharacteristicReadError) = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void WriteCharacteristic(FString ServiceUUID, FString CharacteristicUUID, TArray<uint8> Data, const FOnBluetoothDeviceCharacteristicWriteSuccess& OnCharacteristicWritten, const FOnBluetoothDeviceCharacteristicWriteError& OnCharacteristicWriteError) = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void StartNotifications(FString ServiceUUID, FString CharacteristicUUID, bool bIsIndicationCharacteristic, const FOnBluetoothDeviceNotificationReceived& OnNotificationReceived, const FOnBluetoothDeviceNotificationError& OnNotificationError) = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void StopNotifications(FString ServiceUUID, FString CharacteristicUUID, const FOnBluetoothDeviceNotificationsStopped& OnNotificationsStopped, const FOnBluetoothDeviceNotificationsStoppedError& OnNotificationsStoppedError) = 0;
};