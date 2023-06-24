// Copyright (c) ChimeraXR. All Rights Reserved.

#pragma once

#include "../../Public/BluetoothManagerInterface.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "Android/AndroidJava.h"
#endif

#include "BluetoothManager.generated.h"

class UBluetoothDeviceInterface;

UCLASS()
class UBluetoothManager : public UObject, public IBluetoothManagerInterface
{
	GENERATED_BODY()
public:
	virtual ~UBluetoothManager() = default;

	static UBluetoothManager* Instance;
	static UBluetoothManager GetInstance() { return Instance; }
	class UBluetoothDevice* GetDeviceFromId(FString DeviceId);

	virtual void SetServiceUUIDFilter(TArray<FString> Filter) override;
	virtual void ConnectToBluetoothDevice(TScriptInterface<IBluetoothDeviceInterface> Device,
	                                      const FOnBluetoothManagerDeviceConnected& OnDeviceConnected,
	                                      const FOnBluetoothManagerDeviceConnectionError& OnDeviceConnectionError) override;
	virtual void DisconnectBluetoothDevice(TScriptInterface<IBluetoothDeviceInterface> Device,
		const FOnBluetoothManagerDeviceDisconnected& OnDeviceDisconnected,
		const FOnBluetoothManagerDeviceDisconnectError& OnDeviceDisconnectError) override;
	virtual bool DoesSupportBle() override;
	virtual bool IsBluetoothEnabled() override;
	virtual void StartScanningForBluetoothDevices(const FOnBluetoothManagerDeviceFound& OnDeviceFound) override;
	virtual void StopScanningForBluetoothDevices() override;
	virtual bool IsBluetoothDeviceConnected() const override { return bIsConnected; }
	virtual void GetAvailableBluetoothServicesForDevice(TScriptInterface<IBluetoothDeviceInterface> Device, const FOnBluetoothManagerGetServices& OnDeviceServiceListAvailable, const FOnBluetoothManagerGetServicesError& OnDeviceServiceListError) override;
	virtual void GetAvailableBluetoothCharacteristicsForDeviceService(TScriptInterface<IBluetoothDeviceInterface> Device, FString ServiceUUID, const FOnBluetoothManagerGetServiceCharacteristics& OnDeviceServiceCharacteristicListAvailable, const FOnBluetoothManagerGetServiceCharacteristicsError& OnDeviceServiceCharacteristicListError) override;


	// Android/JNI callbacks
	void AndroidOnDeviceFound(UBluetoothDeviceInterface* Device);
	void AndroidOnDeviceConnected();
	void AndroidOnDeviceConnectionError(FString Error);
	void AndroidOnDeviceDisconnected();
	void AndroidOnDeviceDisconnectError(FString Error);

protected:
	TArray<FString> SavedFilter;
	bool bIsScanning = false;
	UPROPERTY()
	FOnBluetoothManagerDeviceFound SavedOnDeviceFound;
	FOnBluetoothManagerDeviceConnected SavedOnDeviceConnected;
	FOnBluetoothManagerDeviceConnectionError SavedOnDeviceConnectionError;
	bool bIsConnected = false;
	TSharedPtr<UBluetoothDeviceInterface> ConnectedBluetoothDevice;
	FOnBluetoothManagerDeviceDisconnected SavedOnDeviceDisconnected;
	FOnBluetoothManagerDeviceDisconnectError SavedOnDeviceDisconnectError;
	FOnBluetoothManagerGetServices SavedOnDeviceServiceListAvailable;
	FOnBluetoothManagerGetServicesError SavedOnDeviceServiceListError;
	FOnBluetoothManagerGetServiceCharacteristics SavedOnDeviceServiceCharacteristicListAvailable;
	FOnBluetoothManagerGetServiceCharacteristicsError SavedOnDeviceServiceCharacteristicListError;

	TSharedPtr<FJavaClassObject>& GetBluetoothManager();

	TSet<TSharedPtr<UBluetoothDeviceInterface>> DetectedDevices;

	// This looks confusing for sure, but basically says:
	// - We can have many devices
	//  - Each device can have many service UUID's
	//    - Each Service UUID can have several characteristic UUID's
	TMap<TSharedPtr<UBluetoothDeviceInterface>, TMap<FString, TArray<FString>>> DeviceServiceList;

private:
	TSharedPtr<FJavaClassObject> BluetoothManagerJavaObject;
	FJavaClassMethod JniIsBluetoothEnabled;
	FJavaClassMethod JniDoesSupportBle;
	FJavaClassMethod JniStartScanningForBluetoothDevices;
	FJavaClassMethod JniStopScanningForBluetoothDevices;
	FJavaClassMethod JniConnectToBluetoothDevice;
	FJavaClassMethod JniDisconnectBluetoothDevice;
	FJavaClassMethod JniGetAvailableBluetoothServicesForDevice;
	FJavaClassMethod JniGetAvailableBluetoothCharacteristicsForDeviceService;

};
