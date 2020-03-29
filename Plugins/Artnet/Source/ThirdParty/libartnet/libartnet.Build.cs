// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class libartnet : ModuleRules
{
	public libartnet(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        PublicIncludePaths.AddRange(
            new string[] {
                ModuleDirectory
				// ... add public include paths required here ...
                //Path.Combine(ModuleDirectory, "../ThirdParty/libartnet")
            }
            );

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				//"artnet",
				// ... add other public dependencies that you statically link with here ...
			}
		);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x64", "Release"));
			PublicAdditionalLibraries.Add("libartnet.lib");

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("libartnet.dll");
		}
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libartnet.dylib"));
        }
	}
}
