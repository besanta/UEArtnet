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
#include "Components/ActorComponent.h"

#include "ArtnetNode.h"
#include "ArtnetNodeComponent.generated.h"

#define DEFAULT_SUBNET	0x00
#define DEFAULT_PORT_IN_1	0x01 
#define DEFAULT_PORT_IN_2	0x02 
#define DEFAULT_PORT_IN_3	0x03
#define DEFAULT_PORT_IN_4	0x04

#define DEFAULT_PORT_OUT_1	0x05 
#define DEFAULT_PORT_OUT_2	0x06
#define DEFAULT_PORT_OUT_3	0x07 
#define DEFAULT_PORT_OUT_4	0x08 

UCLASS(ClassGroup = (ArtNet), meta = (BlueprintSpawnableComponent))
class UArtnetNodeComponent : public UActorComponent {
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArtNet")
	FString Ip;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "15", UIMin = "0", UIMax = "15"), Category = "ArtNet")
	int32 Port;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0", ClampMax = "15", UIMin = "0", UIMax = "15"), Category = "ArtNet")
	uint8 Subnet;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArtNet")
	FString ShortName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArtNet")
	FString LongName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1", ClampMax = "4", UIMin = "1", UIMax = "4"), Category = "ArtNet")
	uint8 NumPort;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArtNet")
	EArtnetPortDirection PortDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ArtNet")
	TArray<FString> Interfaces;

	UPROPERTY(BlueprintAssignable, Category = "ArtNet")
	FDMXListenerDelegate OnDMXUpdated;

	UFUNCTION(BlueprintCallable)
	void GetDMXPort(uint8 InPort, TArray<uint8>& Buffer);


private:
	//artnet_node Node;

	UPROPERTY()
	UArtnetNode* ArtnetNode;

	//FArtnetWorker* Runnable;

	//TDMXBuffer DMX; // port=>buffer
public:

	UArtnetNodeComponent();

	UFUNCTION()
		void OnDMXNodeUpdateCB(int32 Universe, TArray<uint8> Buffer);

	UFUNCTION(BlueprintPure)
		UArtnetNode* GetNode();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};