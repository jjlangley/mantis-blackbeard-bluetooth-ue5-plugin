// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMantisXModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FMantisXModule& Get() { return FModuleManager::LoadModuleChecked<FMantisXModule>("MantisX"); }
	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("MantisX"); }
};
