// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Artnet.h"
#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"


#define LOCTEXT_NAMESPACE "FArtnetModule"
DEFINE_LOG_CATEGORY(LogArtnet);
void FArtnetModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// Get the base directory of this plugin
	FString BaseDir = IPluginManager::Get().FindPlugin("Artnet")->GetBaseDir();

	// Add on the relative location of the third party dll and load it
	FString LibraryPath;
#if PLATFORM_WINDOWS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/libartnet/x64/Release/libartnet.dll"));
#elif PLATFORM_MAC
    LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/libartnet/Mac/Release/libartnet.dylib"));
#endif // PLATFORM_WINDOWS

	LibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (LibraryHandle)
	{
		// Call the test function in the third party library that opens a message box
		/*node = artnet_new("192.168.1.34", 1);

		artnet_start(node);*/
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
}

void FArtnetModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Free the dll handle
	FPlatformProcess::FreeDllHandle(LibraryHandle);
	LibraryHandle = nullptr;

	//artnet_stop(node);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FArtnetModule, Artnet)
