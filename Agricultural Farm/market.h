/*******************************************************************
*
*  DESCRIPTION: Atomic Model Market
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.com
*
*  DATE: 28-10-2017
*
*******************************************************************/

#ifndef __MARKET_H
#define __MARKET_H

#include <string>
#include "request.h"
#include "atomic.h"

class market : public Atomic
{
	public:
	market( const string &name = "market" );

		virtual string className() const ;

	protected:
		Model &initFunction();
		Model &externalFunction( const ExternalMessage & );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage & );

	private:
		const Port &in_bushels;
		const Port &in_type;
		Port &out_type;
		Port &out_bushels;
		Port &out_sales;
	    double values[3];
	    int cType;
	    double cBushels;
};

inline
string market::className() const
{
	return "market" ;
}

#endif
