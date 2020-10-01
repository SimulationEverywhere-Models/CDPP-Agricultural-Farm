/*******************************************************************
*
*  DESCRIPTION: Atomic Model Field
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.com
*
*  DATE: 28-10-2017
*
*******************************************************************/

#ifndef __FIELD_H
#define __FIELD_H

#include <string>
#include "request.h"
#include "atomic.h"

class field : public Atomic
{
	public:
	field( const string &name = "field" );

		virtual string className() const ;

	protected:
		Model &initFunction();
		Model &externalFunction( const ExternalMessage & );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage & );

	private:
		const Port &in_done;
		const Port &in_stop;
		Port &out_water_field;
		Port &out_water_surface;
		Port &out_water_type;
		Port &out_sower_field;
		Port &out_sower_surface;
		Port &out_sower_type;
		Port &out_harvest_field;
		Port &out_harvest_surface;
		Port &out_harvest_type;
		double surface;
		int type;
		int id;
		string iState;
	    Time growths[3];
	    Time evap[3];
	    Time timeLeft;

};

inline
string field::className() const
{
	return "field" ;
}

#endif
