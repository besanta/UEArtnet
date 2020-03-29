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

#include "ArtnetNodeComponent.h"

void UArtnetNodeComponent::GetDMXPort(uint8 InPort, TArray<uint8>& Buffer)
{
	Buffer.SetNumUninitialized(ARTNET_DMX_LENGTH);
	FMemory::Memcpy(Buffer.GetData(), ArtnetNode->DMX[InPort].GetData()+1, ARTNET_DMX_LENGTH);
}

UArtnetNodeComponent::UArtnetNodeComponent()
	: NumPort(1)
{
	if (NumPort > 4) NumPort = 4;
	if (ShortName.Len() > ARTNET_SHORT_NAME_LENGTH - 1) ShortName = ShortName.Mid(0, ARTNET_SHORT_NAME_LENGTH -1);
	if (LongName.Len() > ARTNET_LONG_NAME_LENGTH - 1) LongName = LongName.Mid(0, ARTNET_LONG_NAME_LENGTH -1);

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

UArtnetNode* UArtnetNodeComponent::GetNode()
{
	return ArtnetNode;
}

void UArtnetNodeComponent::BeginPlay()
{
	Super::BeginPlay();

	ArtnetNode = UArtnetNode::Create(this, Ip, Port, Subnet, ShortName, LongName, NumPort, PortDirection);
	if (ArtnetNode) {
		ArtnetNode->OnDMXUpdated.AddDynamic(this, &UArtnetNodeComponent::OnDMXNodeUpdateCB);
	}
}

void UArtnetNodeComponent::OnDMXNodeUpdateCB(int32 Universe, TArray<uint8> Buffer)
{
	OnDMXUpdated.Broadcast(Universe, Buffer);
}

void UArtnetNodeComponent::BeginDestroy()
{
	Super::BeginDestroy();

	ArtnetNode = NULL;
}

void UArtnetNodeComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if (ArtnetNode && ArtnetNode->IsUpdated()) {
	//	ArtnetNode
	//}
}

