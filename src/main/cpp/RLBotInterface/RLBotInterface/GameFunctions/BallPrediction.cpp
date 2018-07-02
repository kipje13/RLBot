#include "BallPrediction.hpp"
#include <BoostUtilities\BoostUtilities.hpp>
#include <BoostUtilities\BoostConstants.hpp>

namespace BallPrediction
{
	static BoostUtilities::SharedMemReader ballPredictionMem(BoostConstants::BallPredictionName);

	extern "C" ByteBuffer RLBOT_CORE_API FetchBallPrediction()
	{
		return ballPredictionMem.fetchData();
	}
}