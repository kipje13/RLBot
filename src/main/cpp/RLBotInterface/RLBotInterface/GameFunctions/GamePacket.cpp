#include <CapnProto\capnproto.hpp>
#include "game_data.pb.h"
#include <DebugHelper.hpp>

#include "GamePacket.hpp"

#include <chrono>
#include <thread>
#include "..\CallbackProcessor\SharedMemoryDefinitions.hpp"

namespace GameFunctions
{
	// FIELD INFO

	// Capn
	static SharedMemReader capnFieldMem(BoostConstants::FieldInfoSharedMemName, BoostConstants::FieldInfoMutexName);

	extern "C" ByteBuffer RLBOT_CORE_API UpdateFieldInfoCapnp()
	{
		return capnFieldMem.fetchData();
	}

	// Flat
	static SharedMemReader flatFieldMem(BoostConstants::FieldInfoFlatSharedMemName, BoostConstants::FieldInfoFlatMutexName);

	extern "C" ByteBuffer RLBOT_CORE_API UpdateFieldInfoFlatbuffer()
	{
		return flatFieldMem.fetchData();
	}

	// Proto
	extern "C" ByteBuffer RLBOT_CORE_API UpdateFieldInfoProto()
	{
		ByteBuffer capnp = UpdateFieldInfoCapnp();
		return CapnConversions::capnpFieldInfoToProtobuf(capnp);
	}

	//////////////
	// GAME PACKET
	//////////////

	// Capnp
	static SharedMemReader capnTickMem(BoostConstants::GameDataSharedMemName, BoostConstants::GameDataMutexName);

	extern "C" ByteBuffer RLBOT_CORE_API UpdateLiveDataPacketCapnp()
	{
		return capnTickMem.fetchData();
	}

	// Flat
	static SharedMemReader flatTickMem(BoostConstants::GameDataFlatSharedMemName, BoostConstants::GameDataFlatMutexName);

	extern "C" ByteBuffer RLBOT_CORE_API UpdateLiveDataPacketFlatbuffer()
	{
		return flatTickMem.fetchData();
	}

	// Proto
	extern "C" ByteBuffer RLBOT_CORE_API UpdateLiveDataPacketProto()
	{
		ByteBuffer capnp = UpdateLiveDataPacketCapnp();
		return CapnConversions::capnpGameTickToProtobuf(capnp);
	}

	// Ctypes
	extern "C" RLBotCoreStatus RLBOT_CORE_API UpdateLiveDataPacket(LiveDataPacket* pLiveData)
	{
		ByteBuffer capnp = UpdateLiveDataPacketCapnp();
		CapnConversions::capnpGameTickToStruct(capnp, pLiveData);
		delete[] capnp.ptr;

		ByteBuffer fieldInfo = UpdateFieldInfoCapnp();
		CapnConversions::applyFieldInfoToStruct(fieldInfo, pLiveData);
		delete[] fieldInfo.ptr;

		return RLBotCoreStatus::Success;
	}
}