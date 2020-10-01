/*******************************************************************
*
*  DESCRIPTION: Atomic Model Harvester
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.com
*
*  DATE: 28-10-2017
*
*******************************************************************/

#ifndef __HARVESTER_H
#define __HARVESTER_H

#include "request.h"
#include "atomic.h"

class harvester : public Atomic
{
	public:
	harvester( const string &name = "harvester" );

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
		Port &out_bushels;
		Port &out_type;
		TaskRequest request;
		double speed;
	    float yields[3];
};

inline
string harvester::className() const
{
	return "harvester" ;
}

#endif
