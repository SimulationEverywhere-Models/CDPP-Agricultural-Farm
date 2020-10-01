/*******************************************************************
*
*  DESCRIPTION: Atomic Model Market
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.ca
*
*  DATE: 27-10-2017
*
*******************************************************************/

#include "market.h"
#include "helper.h"
#include "request.h"

#include "strutil.h"
#include "time.h"
#include "message.h"
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )


/** public functions **/

/*******************************************************************
* Function Name: Market
* Description:
********************************************************************/
market::market( const string &name )
: Atomic( name )
, in_type( addInputPort( "in_type" ) )
, in_bushels( addInputPort( "in_bushels" ) )
, out_type( addOutputPort( "out_type" ) )
, out_bushels( addOutputPort( "out_bushels" ) )
, out_sales( addOutputPort( "out_sales" ) )
, cType(-1)
, cBushels(-1)
{
	values[0] = str2float(MainSimulator::Instance().getParameter(description(), "value_type_1")); // in dollars
	values[1] = str2float(MainSimulator::Instance().getParameter(description(), "value_type_2")); // in dollars
	values[2] = str2float(MainSimulator::Instance().getParameter(description(), "value_type_3")); // in dollars
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition:
********************************************************************/
Model &market::initFunction()
{
	this->passivate();

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &market::externalFunction( const ExternalMessage &msg )
{
	if (msg.port() == in_type) this->cType = static_cast<int>(msg.value());

	else if (msg.port() == in_bushels) this->cBushels = static_cast <double>(msg.value());

	if (this->cType == -1 || this->cBushels == -1) return *this;

	holdIn(active, Time::Zero);

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &market::internalFunction( const InternalMessage & )
{
	this->passivate();

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &market::outputFunction( const InternalMessage &msg )
{
	int i = this->cType - 1;

	double sales = this->values[i] * this->cBushels;

	sendOutput( msg.time(), out_type, this->cType) ;
	sendOutput( msg.time(), out_bushels, this->cBushels) ;
	sendOutput( msg.time(), out_sales, sales) ;

	this->cType = -1;
	this->cBushels = -1;

	return *this ;
}
