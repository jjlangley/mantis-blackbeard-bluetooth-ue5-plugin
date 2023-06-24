// Copyright (c) ChimeraXR.  All Rights Reserved.

#include "../Public/BluetoothFunctionLibrary.h"

TScriptInterface<IBluetoothManager> UBluetoothFunctionLibrary::GetBluetoothManager(TArray<FString> ServiceFilters)
{
	// TODO: Windows support
#if PLATFORM_ANDROID
	UBluetoothManager* BluetoothManager = NewObject<UBluetoothManager>();
	BluetoothManager->SetServiceUUIDFilter(ServiceFilters);

	TScriptInterface<IBluetoothManagerInterface> BluetoothManagerInterface;
	BluetoothManagerInterface.SetObject(BluetoothManager);
	BluetoothManagerInterface.SetInterface(Cast<IBluetoothManagerInterface>(BluetoothManager));
	return BluetoothManagerInterface;
#endif

	return nullptr;
}
