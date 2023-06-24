// Copyright (c) ChimeraXR. All Rights Reserved.

#pragma once

#include "Interface.h"
#include "BluetoothManagerInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBluetoothManagerDeviceFound, TScriptInterface<IBluetoothDeviceInterface>, Device);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBluetoothManagerDeviceConnected, TScriptInterface<IBluetoothDeviceInterface>, ConnectedDevice);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnBluetoothManagerDeviceDisconnected, TScriptInterface<IBluetoothDeviceInterface>, DisconnectedDevice);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnBluetoothManagerDeviceConnectionError, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Error);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnBluetoothManagerDeviceDisconnectError, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Error);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnBluetoothManagerGetServices, TScriptInterface<IBluetoothDeviceInterface>, Device, TArray<FString>, Services);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnBluetoothManagerGetServicesError, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Error);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnBluetoothManagerGetServiceCharacteristics, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service, TArray<FString>, Characteristics);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnBluetoothManagerGetServiceCharacteristicsError, TScriptInterface<IBluetoothDeviceInterface>, Device, FString, Service, FString, Error);)

UINTERFACE(BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class UBluetoothManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class IBluetoothManagerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void SetServiceUUIDFilter(TArray<FString> Filter) = 0;
	
	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual bool IsBluetoothEnabled() = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual bool DoesSupportBle() = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void StartScanningForBluetoothDevices(const FOnBluetoothManagerDeviceFound& OnDeviceFound) = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void StopScanningForBluetoothDevices() = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void GetAvailableBluetoothServicesForDevice(TScriptInterface<IBluetoothDeviceInterface> Device, const FOnBluetoothManagerGetServices& OnDeviceServiceListAvailable, const FOnBluetoothManagerGetServicesError& OnDeviceServiceListError) = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void GetAvailableBluetoothCharacteristicsForDeviceService(TScriptInterface<IBluetoothDeviceInterface> Device, FString ServiceUUID, const FOnBluetoothManagerGetServiceCharacteristics& OnDeviceServiceCharacteristicListAvailable, const FOnBluetoothManagerGetServiceCharacteristicsError& OnDeviceServiceCharacteristicListError) = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void ConnectToBluetoothDevice(TScriptInterface<IBluetoothDeviceInterface> Device, const FOnBluetoothManagerDeviceConnected& OnDeviceConnected, const FOnBluetoothManagerDeviceConnectionError& OnDeviceConnectionError) = 0;

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functions")
	virtual void DisconnectBluetoothDevice(TScriptInterface<IBluetoothDeviceInterface> Device, const FOnBluetoothManagerDeviceDisconnected& OnDeviceConnected, const FOnBluetoothManagerDeviceDisconnectError& OnDeviceConnectionError) = 0;

	UFUNCTION(BlueprintPure, Category = "Bluetooth Functions")
	virtual bool IsBluetoothDeviceConnected() const;
};
