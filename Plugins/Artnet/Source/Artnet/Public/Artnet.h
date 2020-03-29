// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"


DECLARE_LOG_CATEGORY_EXTERN(LogArtnet, Log, All)
class FArtnetModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	LibraryHandle;

	//artnet_node node;
};
