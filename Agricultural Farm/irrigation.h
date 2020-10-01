/*******************************************************************
*
*  DESCRIPTION: Atomic Model Irrigation
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.com
*
*  DATE: 28-10-2017
*
*******************************************************************/

#ifndef __IRRIGATION_H
#define __IRRIGATION_H

#include "request.h"
#include "atomic.h"

class irrigation : public Atomic
{
	public:
		irrigation( const string &name = "irrigation" );

		virtual string className() const ;
	protected:
		Model &initFunction();
		Model &externalFunction( const ExternalMessage & );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage & );

	private:
		const Port &in_id;
		const Port &in_surface;
		const Port &in_type;
		Port &out_id;
		TaskRequest request;
		double speed;
};

inline
string irrigation::className() const
{
	return "irrigation" ;
}

#endif
