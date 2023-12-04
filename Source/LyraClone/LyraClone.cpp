// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraClone.h"
#include "Modules/ModuleManager.h"
#include "System/LyraCloneAssetManager.h"

/**
 * FLyraCloneModule
 */
class FLyraCloneModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FLyraCloneModule::StartupModule()
{
	ULyraCloneAssetManager::TestClone();
}

void FLyraCloneModule::ShutdownModule()
{

}

IMPLEMENT_PRIMARY_GAME_MODULE(FLyraCloneModule, LyraClone, "LyraClone");
