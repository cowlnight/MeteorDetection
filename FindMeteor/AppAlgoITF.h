/**
* @file AppAlgoITF.h
*
* Base class for algorithm Interface
*
*/

#ifndef _APPALGOITF_H_
#define _APPALGOITF_H_

#include "pch.h"
#include "./Data/MeteorData.h"

namespace FindMeteor
{
	/**
	*	@brief Base class for the algorithm interface
	*/
	class IAppAlgoITF
	{
	public:
	
		IAppAlgoITF(void) {}
		virtual ~IAppAlgoITF(void) {}
		/// Execute the algorithm
		virtual bool Execute(CMeteorData&) = 0;

		
	};
}

#endif