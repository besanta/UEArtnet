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

#include "ArtnetNode.h"

#include "Async/Async.h"
THIRD_PARTY_INCLUDES_START
#include "artnet/private.h"
THIRD_PARTY_INCLUDES_END
//FArtnetWorker* FArtnetWorker::Runnable = NULL;



FArtnetWorker::FArtnetWorker(artnet_node vn, TDMXBuffer& Buffer)
	: Thread(NULL)
	, Node(vn)
	, Buffer(Buffer)
{
	node n = (node)vn;
	Name = FString::Printf(TEXT("FArtnetWorker_%s"), ANSI_TO_TCHAR((char*)& n->state.short_name));
	Thread = FRunnableThread::Create(this, *Name, 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FArtnetWorker::~FArtnetWorker()
{
	delete Thread;
	Thread = NULL;
}

void FArtnetWorker::Shutdown()
{
	UE_LOG(LogArtnet, Log, TEXT("Shutdown thread %s"), *Name);
	Stop();
	if (Thread) {

		Thread->WaitForCompletion();
	}
}

bool FArtnetWorker::Init()
{
	/*artnet_set_dmx_handler(Node, &FArtnetWorker::dmx_handler, theNode);
	artnet_set_program_handler(Node, &FArtnetWorker::program_handler, theNode);
	artnet_set_firmware_handler(Node, &FArtnetWorker::firmware_handler, theNode);*/

	if (artnet_start(Node) != ARTNET_EOK) {
		UE_LOG(LogArtnet, Error, TEXT("Error on start %s"), *Name);
	}
	else {
		UE_LOG(LogArtnet, Log, TEXT("Success on start %s"), *Name);
	}
	return true;
}

uint32 FArtnetWorker::Run()
{
	FPlatformProcess::Sleep(0.03);
	while (Node) {
		artnet_read(Node, 1);
	}
	return 0;
}

void FArtnetWorker::Stop()
{
	artnet_stop(Node);
	Node = NULL;
}

UArtnetNode::UArtnetNode()
{

}

UArtnetNode* UArtnetNode::Create(UObject* WorldContextObject, FString Ip, int32 Port, uint8 Subnet, FString ShortName, FString LongName, uint8 NumPorts, EArtnetPortDirection PortDirection)
{
	NumPorts = NumPorts > ARTNET_MAX_PORTS ? ARTNET_MAX_PORTS : NumPorts;

	UArtnetNode* theNode = NewObject<UArtnetNode>(WorldContextObject);
	artnet_node Node = artnet_new(TCHAR_TO_ANSI(*Ip), false);
	if (Node) {
		theNode->Node = Node;
		artnet_set_short_name(Node, TCHAR_TO_ANSI(*ShortName));
		artnet_set_long_name(Node, TCHAR_TO_ANSI(*LongName));
		artnet_set_node_type(Node, ARTNET_NODE);
		artnet_set_subnet_addr(Node, Subnet);

		//artnet_set_port_type(node, 0, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
		//artnet_set_port_addr(node, 0, ARTNET_INPUT_PORT, Port);

		for (int i = 0; i < NumPorts; i++) {

			artnet_port_settings_t ps = ARTNET_ENABLE_OUTPUT;
			switch (PortDirection)
			{
			case EArtnetPortDirection::APD_OUT:
				artnet_set_port_type(Node, i, ARTNET_ENABLE_OUTPUT, ARTNET_PORT_DMX);
				artnet_set_port_addr(Node, i, ARTNET_OUTPUT_PORT, Port + i);
				break;
			case EArtnetPortDirection::APD_IN:
				artnet_set_port_type(Node, i, ARTNET_ENABLE_INPUT, ARTNET_PORT_DMX);
				artnet_set_port_addr(Node, i, ARTNET_INPUT_PORT, Port + i);
				break;
			default:
				artnet_set_port_type(Node, i, (artnet_port_settings_t)(ARTNET_ENABLE_OUTPUT | ARTNET_ENABLE_INPUT), ARTNET_PORT_DMX);
				artnet_set_port_addr(Node, i, ARTNET_OUTPUT_PORT, Port + i);
				artnet_set_port_addr(Node, i, ARTNET_INPUT_PORT, Port + i);
				break;
			}

			theNode->DMX.Emplace(i, TDMXArray());
		}

		theNode->Runnable = new FArtnetWorker(Node, theNode->DMX);

		artnet_set_dmx_handler(Node, &UArtnetNode::dmx_handler, theNode);
		artnet_set_program_handler(Node, &UArtnetNode::program_handler, theNode);
		artnet_set_firmware_handler(Node, &UArtnetNode::firmware_handler, theNode);

		return theNode;
	}
	else {
		UE_LOG(LogArtnet, Error, TEXT("Error on Creation of %s (%s:%d:%d)"), *LongName, *Ip, Port, Subnet);
	}

	return NULL;
}

UArtnetNode::~UArtnetNode()
{
	if (Runnable) {
		Runnable->Shutdown();
		delete Runnable;
		Runnable = NULL;
	}
	if (Node) {
		artnet_stop(Node);
		artnet_destroy(Node);
	}
}

int UArtnetNode::GetUniverseAddress(int32 Port)
{
	return artnet_get_universe_addr(Node, Port, ARTNET_OUTPUT_PORT);
}

int UArtnetNode::dmx_handler(artnet_node n, int port, void* d)
{
	//UE_LOG(LogArtnet, Log, TEXT("DMX Handle %d"), port);
	UArtnetNode* artnetNode = (UArtnetNode*)d;
	int length;
	if (artnetNode) {
		uint8* data = (uint8*)artnet_read_dmx(n, port, &length);
		FMemory::Memcpy(artnetNode->DMX[port].GetData() + 1, data, length);
		//worker->IsUpdated.AtomicSet(true);

		AsyncTask(ENamedThreads::GameThread, [port, d] {
			UArtnetNode* artnetNode = (UArtnetNode*)d;
			if (artnetNode && artnetNode->IsValidLowLevel()) {
				
				TArray<uint8> data;
				data.AddUninitialized(ARTNET_DMX_LENGTH);

				FMemory::Memcpy(data.GetData(), artnetNode->DMX[port].GetData(), ARTNET_DMX_LENGTH);
				artnetNode->OnDMXUpdated.Broadcast(port, data);
			}
		});
	}

	return 0;
}

int UArtnetNode::program_handler(artnet_node n, void* d)
{
	UE_LOG(LogArtnet, Log, TEXT("Program Handle"));
	return 0;
}

int UArtnetNode::firmware_handler(artnet_node n, int ubea, uint16_t* data, int length, void* d)
{
	UE_LOG(LogArtnet, Log, TEXT("Firmware Handle of %d words"), length);
	return 0;
}
