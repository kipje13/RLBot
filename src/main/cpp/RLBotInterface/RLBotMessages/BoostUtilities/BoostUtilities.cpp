#include "BoostUtilities.hpp"

namespace GameFunctions {

	ByteBuffer SharedMemReader::fetchData() 
	{
		// The lock will be released when this object goes out of scope
		boost::interprocess::sharable_lock<boost::interprocess::named_sharable_mutex> myLock(mutex);

		boost::interprocess::offset_t size;
		sharedMem.get_size(size);
		if (size == 0)
		{
			// Bail out early because mapped_region will freak out if size is zero.
			ByteBuffer empty;
			empty.ptr = new char[1]; // Arbitrary valid pointer to an array. We'll be calling delete[] on this later.
			empty.size = 0;
			return empty;
		}

		boost::interprocess::mapped_region region(sharedMem, boost::interprocess::read_only);
		unsigned char *buffer = new unsigned char[region.get_size()];
		memcpy(buffer, region.get_address(), region.get_size());

		ByteBuffer buf;
		buf.ptr = buffer;
		buf.size = region.get_size();

		return buf;
	}

	RLBotCoreStatus QueueSender::sendMessage(void* message, int messageSize)
	{
		try {
			bool sent = queue.try_send(message, messageSize, 0);
			if (!sent) {
				return RLBotCoreStatus::BufferOverfilled;
			}
			return RLBotCoreStatus::Success;
		}
		catch (boost::interprocess::interprocess_exception &ex) {
			// TODO: check the exception and see if message larger than max is really the cause.
			return RLBotCoreStatus::MessageLargerThanMax;
		}
	}

}
