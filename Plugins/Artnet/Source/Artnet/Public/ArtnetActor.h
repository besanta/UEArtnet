/*
 *  Artnet -- An Unreal Plugin integrating libartnet permitting Actors to be
 *	recognized as artnetNode, Artnet Server, or MediaServer.
 *  Copyright (C) 2015-2019 BrightNightGames <nicosanta@brightnightgames.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
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

#include "ArtnetNodeComponent.h"

#include "ArtnetActor.generated.h"

UCLASS()
class AArtnetActor : public AActor {
	GENERATED_BODY()

private:
	UPROPERTY()
	class UArtnetNodeComponent* ArtnetNodeComponent;
public:
	AArtnetActor();
	virtual void PostInitializeComponents() override;

	FORCEINLINE class UArtnetNodeComponent* GetNode() const { return ArtnetNodeComponent; }
};
