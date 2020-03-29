/*
 *  Artnet -- An Unreal Plugin integrating libartnet permitting Actors to be
 *	recognized as artnetNode, Artnet Server, or MediaServer.
 *  Copyright (C) 2015-2019 BrightNightGames <nicosanta@brightnightgames.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 3
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
THIRD_PARTY_INCLUDES_START
#include "artnet/artnet.h"
THIRD_PARTY_INCLUDES_END

#include "ArtnetNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FArtnetListenerDelegate, int32, Universe);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDMXListenerDelegate, int32, Universe, TArray<uint8>, Buffer);


// https://wiki.unrealengine.com/Multi-Threading:_How_to_Create_Threads_in_UE4
UENUM(BlueprintType)
enum class EArtnetPortDirection :uint8 {
	APD_OUT 	UMETA(DisplayName = "Out"),
	APD_IN 		UMETA(DisplayName = "In"),
	APD_BOTH 	UMETA(DisplayName = "Both")
};
typedef TArray<uint8, TFixedAllocator<ARTNET_DMX_LENGTH + 1>> TDMXArray;
typedef TMap<uint8, TDMXArray> TDMXBuffer;

class FArtnetWorker : public FRunnable {
private:

	FRunnableThread* Thread;
	artnet_node Node;
	FString Name;
	TDMXBuffer& Buffer;

public:
	//FThreadSafeBool IsUpdated;
public:
	FArtnetWorker(artnet_node node, TDMXBuffer& Buffer);
	~FArtnetWorker();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	void Shutdown();

	/*static int dmx_handler(artnet_node n, int port, void* d);
	static int program_handler(artnet_node n, void* d);
	static int firmware_handler(artnet_node n, int ubea, uint16_t* data, int length, void* d);*/
};

UCLASS(BlueprintType)
class UArtnetNode : public UObject
{
	GENERATED_BODY()
public:
	UArtnetNode();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create ArtNet Node", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "ArtNet")
		static UArtnetNode* Create(UObject* WorldContextObject, FString Ip, int32 Port, uint8 Subnet, FString ShortName, FString LongName, uint8 NumPorts, EArtnetPortDirection PortDirection);
	~UArtnetNode();

private:
	artnet_node Node;
	FArtnetWorker* Runnable;
public:
	
	TDMXBuffer DMX; // port=>buffer
	FDMXListenerDelegate OnDMXUpdated;

	//FORCEINLINE bool IsUpdated() { if (Runnable) return Runnable->IsUpdated; return false; };

	UFUNCTION(BlueprintPure)
		int GetUniverseAddress(int32 Port);

	static int dmx_handler(artnet_node n, int port, void* d);
	static int program_handler(artnet_node n, void* d);
	static int firmware_handler(artnet_node n, int ubea, uint16_t* data, int length, void* d);
};