// Copyright (c) ChimeraXR. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BluetoothFunctionLibrary.generated.h"

UCLASS()
class UBluetoothFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Bluetooth Functionality")
	static TScriptInterface<IBluetoothManager> GetBluetoothManager(TArray<FString> ServiceFilters);
};
