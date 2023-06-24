// Copyright Epic Games, Inc. All Rights Reserved.

#include "MantisX.h"

#include "MantisXPluginLog.h"

#define LOCTEXT_NAMESPACE "FMantisXModule"

void FMantisXModule::StartupModule()
{
	UE_LOG(LogMantisX, Verbose, TEXT("[MantisX Plugin Initializaiton]"));
}

void FMantisXModule::ShutdownModule()
{
	UE_LOG(LogMantisX, Verbose, TEXT("[MantisX Plugin Shutdown]"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMantisXModule, MantisX)

DEFINE_LOG_CATEGORY(LogMantisX);