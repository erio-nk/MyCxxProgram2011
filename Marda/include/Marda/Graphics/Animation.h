/*!
	@file	Animation.h
	
	@author	E.Nakayama
*/
#pragma once
	
namespace Marda
{
	template <class DataType>
	struct KeyFrameAnimationLinear
	{
		typedef std::map<float, DataType> KeyFrames;
		KeyFrames _keyFrames;
		
		
		
	};
	
	template <class DataType>
	inline void AnimationLinear(DataType timeCurrent, float timePrev, DataType dataPrev, float timeNext, DataType dataNext, DataType& result)
	{
		float rate (timeCurrent - timePrev) / (timeNext - timePrev);
		result = (dataNext - dataPrev) * rate + dataPrev;
	}

}	// end namespace Marda
