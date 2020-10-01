/*******************************************************************
*
*  DESCRIPTION: Atomic Model Irrigation
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.ca
*
*  DATE: 27-10-2017
*
*******************************************************************/

#include "irrigation.h"
#include "helper.h"
#include "request.h"

#include "strutil.h"
#include "time.h"
#include "message.h"
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )


/** public functions **/

/*******************************************************************
* Function Name: Irrigation
* Description:
********************************************************************/
irrigation::irrigation( const string &name )
: Atomic( name )
, in_id( addInputPort( "in_id" ) )
, in_surface( addInputPort( "in_surface" ) )
, in_type( addInputPort( "in_type" ) )
, out_id( addOutputPort( "out_id" ) )
{
	speed = str2float(MainSimulator::Instance().getParameter(description(), "speed")); // in acres per hour
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition:
********************************************************************/
Model &irrigation::initFunction()
{
	this->passivate();

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &irrigation::externalFunction( const ExternalMessage &msg )
{
	Helper h;

	// Ignore new inputs when busy
	if (this->state() == active) return *this;

	if( msg.port() == in_id) this->request.id = static_cast<int>(msg.value());

	if( msg.port() == in_surface) this->request.surface = static_cast <double>(msg.value());

	if( msg.port() == in_type) this->request.type = static_cast <int>(msg.value());

	if (!this->request.IsReady())return *this;

	// I can't figure out how to use time.h outside of an atomic model so I made my own helper to read time properly.
	_Time t = h.TimeFromHours(this->request.surface / speed);

	Time duration = Time(t.h, t.m, t.s, t.ms);

	holdIn(active, duration);

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &irrigation::internalFunction( const InternalMessage & )
{
	this->passivate();

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &irrigation::outputFunction( const InternalMessage &msg )
{
	double value = this->request.id;

	this->request.Clear();

	//if (this->state() == active)
	//{
		sendOutput( msg.time(), out_id, value) ;
	//}

	return *this ;
}
