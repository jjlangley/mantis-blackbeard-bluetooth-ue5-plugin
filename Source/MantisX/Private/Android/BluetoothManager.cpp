// Copyright (c) ChimeraXR. All Rights Reserved.

#include "BluetoothManager.h"

#include "BluetoothDevice.h"
#include "JniUtils.h"
#include "../../Public/BluetoothDeviceInterface.h"

UBluetoothManager UBluetoothManager::*Instance = nullptr;

UBluetoothManager::UBluetoothManager()
{
	Instance = this;
}

void UBluetoothManager::SetServiceUUIDFilter(TArray<FString> Filter)
{
	SavedFilter = Filter;

	if (bIsScanning)
	{
		// TODO: Stop scanning, start again with filter applied
	}
}

UBluetoothDevice* UBluetoothManager::GetDeviceFromId(FString DeviceId)
{
	TArray<TSharedPtr<UBluetoothDeviceInterface>> Devices = DeviceServiceList.GetKeys();
	for (const TSharedPtr<UBluetoothDeviceInterface>& Device : Devices)
	{
		if (Device.IsValid()) continue;
		const auto BluetoothDevice = static_cast<UBluetoothDevice*>(Device.Get());
		if (!IsValid(BluetoothDevice)) continue;

		if (BluetoothDevice->GetDeviceId().Equals(DeviceId))
		{
			return BluetoothDevice;
		}
	}

	return nullptr;
}

void UBluetoothManager::ConnectToBluetoothDevice(TScriptInterface<IBluetoothDeviceInterface> Device,
	const FOnBluetoothManagerDeviceConnected& OnDeviceConnected,
	const FOnBluetoothManagerDeviceConnectionError& OnDeviceConnectionError)
{
	SavedOnDeviceConnected = OnDeviceConnected;
	SavedOnDeviceConnectionError = OnDeviceConnectionError;

	if (ConnectedBluetoothDevice.IsValid())
	{
		SavedOnDeviceConnectionError.ExecuteIfBound(Device, TEXT("Must disconnect from active device first"));
		return;
	}

	bIsConnected = false;

	if (!BluetoothManagerJavaObject.IsValid())
	{
		SavedOnDeviceConnectionError.ExecuteIfBound(Device, TEXT("Bluetooth manager not initialized"));
		return;
	}
	ConnectedBluetoothDevice = Device;
	BluetoothManagerJavaObject->CallMethod<void>(JniConnectToBluetoothDevice, JniUtils::ToJavaString(Device->GetDeviceId()));
}

void UBluetoothManager::DisconnectBluetoothDevice(TScriptInterface<IBluetoothDeviceInterface> Device,
	const FOnBluetoothManagerDeviceDisconnected& OnDeviceDisconnected,
	const FOnBluetoothManagerDeviceDisconnectError& OnDeviceDisconnectError)
{
	SavedOnDeviceDisconnected = OnDeviceDisconnected;
	SavedOnDeviceDisconnectError = OnDeviceDisconnectError;

	if (!ConnectedBluetoothDevice.IsValid())
	{
		bIsConnected = false;
		OnDeviceDisconnected.ExecuteIfBound(nullptr);
		return;
	}

	if (!BluetoothManagerJavaObject.IsValid())
	{
		SavedOnDeviceDisconnectError.ExecuteIfBound(Device, TEXT("Bluetooth manager not initialized"));
		return;
	}

	// TODO: So, that means this parameter is probably unnecessary, eh, John? ;-) 
	if (ConnectedBluetoothDevice != Device)
	{
		SavedOnDeviceDisconnectError.ExecuteIfBound(Device, TEXT("Bluetooth device parameter doesn't match already connected device"));
		return;
	}

	BluetoothManagerJavaObject->CallMethod<void>(JniDisconnectBluetoothDevice, JniUtils::ToJavaString(Device->GetDeviceId()));
}

bool UBluetoothManager::DoesSupportBle()
{
	const auto BluetoothManager = GetBluetoothManager();
	if (!BluetoothManager.IsValid()) return false;

	return BluetoothManager->CallMethod<bool>(JniDoesSupportBle);
}

bool UBluetoothManager::IsBluetoothEnabled()
{
	const auto BluetoothManager = GetBluetoothManager();
	if (!BluetoothManager.IsValid()) return false;

	return BluetoothManager->CallMethod<bool>(JniIsBluetoothEnabled);
}

void UBluetoothManager::StartScanningForBluetoothDevices(const FOnBluetoothManagerDeviceFound& OnDeviceFound)
{
	const auto BluetoothManager = GetBluetoothManager();
	if (!BluetoothManager.IsValid()) return;
	
	bIsScanning = true;
	SavedOnDeviceFound = OnDeviceFound;
	BluetoothManager->CallMethod<void>(JniStartScanningForBluetoothDevices);
}

void UBluetoothManager::StopScanningForBluetoothDevices()
{
	bIsScanning = false;
	SavedOnDeviceFound.Clear();
	const auto BluetoothManager = GetBluetoothManager();
	if (!BluetoothManager.IsValid()) return;
	BluetoothManager->CallMethod<void>(JniStopScanningForBluetoothDevices);
}

void UBluetoothManager::GetAvailableBluetoothServicesForDevice(TScriptInterface<IBluetoothDeviceInterface> Device,
	const FOnBluetoothManagerGetServices& OnDeviceServiceListAvailable,
	const FOnBluetoothManagerGetServicesError& OnDeviceServiceListError)
{
	if (!bIsConnected || !ConnectedBluetoothDevice.IsValid())
	{
		OnDeviceServiceListError.ExecuteIfBound(Device, TEXT("Not connected to a device"));
		return;
	}

	if (!BluetoothManagerJavaObject.IsValid())
	{
		OnDeviceServiceListError.ExecuteIfBound(Device, TEXT("Bluetooth manager not valid"));
		return;
	}

	SavedOnDeviceServiceListAvailable = OnDeviceServiceListAvailable;
	SavedOnDeviceServiceListError = OnDeviceServiceListError;

	if (DeviceServiceList.Contains(Device))
	{
		DeviceServiceList[Device].RemoveAll();
	}

	BluetoothManagerJavaObject->CallMethod<void>(JniGetAvailableBluetoothServicesForDevice, JniUtils::ToJavaString(Device->GetDeviceId()));
}

void UBluetoothManager::GetAvailableBluetoothCharacteristicsForDeviceService(TScriptInterface<IBluetoothDeviceInterface> Device, FString ServiceUUID, const FOnBluetoothManagerGetServiceCharacteristics& OnDeviceServiceCharacteristicListAvailable, const FOnBluetoothManagerGetServiceCharacteristicsError& OnDeviceServiceCharacteristicListError)
{
	if (!bIsConnected || !ConnectedBluetoothDevice.IsValid())
	{
		OnDeviceServiceCharacteristicListError.ExecuteIfBound(Device, ServiceUUID, TEXT("Not connected to a device"));
		return;
	}

	if (!BluetoothManagerJavaObject.IsValid())
	{
		OnDeviceServiceCharacteristicListError.ExecuteIfBound(Device, ServiceUUID, TEXT("Bluetooth manager not valid"));
		return;
	}

	SavedOnDeviceServiceCharacteristicListAvailable = OnDeviceServiceCharacteristicListAvailable;
	SavedOnDeviceServiceCharacteristicListError = OnDeviceServiceCharacteristicListError;

	if (DeviceServiceList.Contains(Device) && DeviceServiceList[Device].Contains(ServiceUUID))
	{
		DeviceServiceList[Device][ServiceUUID].RemoveAll();
	}

	BluetoothManagerJavaObject->CallMethod<void>(JniGetAvailableBluetoothCharacteristicsForDeviceService, JniUtils::ToJavaString(Device->GetDeviceId()), JniUtils::ToJavaString(ServiceUUID));
}

void UBluetoothManager::AndroidOnDeviceFound(UBluetoothDeviceInterface* Device)
{
	DetectedDevices.Add(Device);
	SavedOnDeviceFound.ExecuteIfBound(Device);
}

void UBluetoothManager::AndroidOnDeviceConnected()
{
	bIsConnected = true;
	SavedOnDeviceConnected.ExecuteIfBound(ConnectedBluetoothDevice);
}

void UBluetoothManager::AndroidOnDeviceConnectionError(FString Error)
{
	bIsConnected = false;
	SavedOnDeviceConnectionError.ExecuteIfBound(ConnectedBluetoothDevice, Error);
	ConnectedBluetoothDevice.Reset();
}

void UBluetoothManager::AndroidOnDeviceDisconnected()
{
	bIsConnected = false;
	SavedOnDeviceDisconnected.ExecuteIfBound(ConnectedBluetoothDevice);
	ConnectedBluetoothDevice.Reset();
}

void UBluetoothManager::AndroidOnDeviceDisconnectError(FString Error)
{
	// We still mostly pretend that the disconnection was successful...  This may be right or wrong..
	bIsConnected = false;
	SavedOnDeviceDisconnectError.ExecuteIfBound(ConnectedBluetoothDevice, Error);
	ConnectedBluetoothDevice.Reset();
}

TSharedPtr<FJavaClassObject>& UBluetoothManager::GetBluetoothManager()
{
	if (!BluetoothManagerJavaObject.IsValid())
	{
		BluetoothManagerJavaObject = MakeShareable(
			new FJavaClassObject(
				"com/chimeraxr/mantisapi/MantisBluetoothManager", 
				"(Landroid/app/Activity;)V)", 
				FJavaWrapper::GameActivityThis));

		if (BluetoothManagerJavaObject.IsValid())
		{
			JniIsBluetoothEnabled = BluetoothManagerJavaObject->GetClassMethod("isBluetoothEnabled", "()Z");
			JniDoesSupportBle = BluetoothManagerJavaObject->GetClassMethod("doesSupportBle", "()Z");
			JniStartScanningForBluetoothDevices = BluetoothManagerJavaObject->GetClassMethod("startScanningForBluetoothDevices", "()V");
			JniStopScanningForBluetoothDevices = BluetoothManagerJavaObject->GetClassMethod("stopScanningForBluetoothDevices", "()V");
			JniConnectToBluetoothDevice = BluetoothManagerJavaObject->GetClassMethod("connectToBluetoothDevice", "(Ljava/lang/String;)V");
			JniDisconnectBluetoothDevice = BluetoothManagerJavaObject->GetClassMethod("disconnectBluetoothDevice", "(Ljava/lang/String;)V");
			JniGetAvailableBluetoothServicesForDevice = BluetoothManagerJavaObject->GetClassMethod("getAvailableBluetoothServicesForDevice", "(Ljava/lang/String;)V");
			JniGetAvailableBluetoothCharacteristicsForDeviceService = BluetoothManagerJavaObject->GetClassMethod("getAvailableBluetoothCharacteristicsForDeviceService", "(Ljava/lang/String;Ljava/lang/String;)V");
		}
	}

	return BluetoothManagerJavaObject;
}
