/*******************************************************************
*
*  DESCRIPTION: Atomic Model Harvester
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.ca
*
*  DATE: 27-10-2017
*
*******************************************************************/

#include "harvester.h"
#include "helper.h"
#include "request.h"

#include "strutil.h"
#include "time.h"
#include "message.h"
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )


/** public functions **/

/*******************************************************************
* Function Name: Harvester
* Description:
********************************************************************/
harvester::harvester( const string &name )
: Atomic( name )
, in_id( addInputPort( "in_id" ) )
, in_surface( addInputPort( "in_surface" ) )
, in_type( addInputPort( "in_type" ) )
, out_id( addOutputPort( "out_id" ) )
, out_bushels( addOutputPort( "out_bushels" ) )
, out_type( addOutputPort( "out_type" ) )
{
	speed = str2float(MainSimulator::Instance().getParameter(description(), "speed")); // in acres per hour

	yields[0] = str2float(MainSimulator::Instance().getParameter(description(), "yield_type_1")); // in bushels per acre
	yields[1] = str2float(MainSimulator::Instance().getParameter(description(), "yield_type_2")); // in bushels per acre
	yields[2] = str2float(MainSimulator::Instance().getParameter(description(), "yield_type_3")); // in bushels per acre
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition:
********************************************************************/
Model &harvester::initFunction()
{
	this->passivate();

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &harvester::externalFunction( const ExternalMessage &msg )
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
Model &harvester::internalFunction( const InternalMessage & )
{
	this->passivate();

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &harvester::outputFunction( const InternalMessage &msg )
{
	double id = this->request.id;

	int i = this->request.type - 1;

	double bushels = yields[i] * this->request.surface;

	int type = this->request.type;

	this->request.Clear();

	//if (this->state() == active)
	//{
		// cout << msg.time() << " Harvester is generating " << bushels << " bushels of type " << type << endl;
		sendOutput( msg.time(), out_id, id) ;
		sendOutput( msg.time(), out_bushels, bushels) ;
		sendOutput( msg.time(), out_type, type) ;
	//}

	return *this ;
}
