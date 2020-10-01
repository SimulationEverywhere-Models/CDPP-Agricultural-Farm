/*******************************************************************
*
*  DESCRIPTION: Atomic Model Field
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.ca
*
*  DATE: 27-10-2017
*
*******************************************************************/

#include "field.h"

#include "_time.h"
#include "helper.h"
#include "request.h"
#include "strutil.h"
#include "message.h"
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )


/** public functions **/

/*******************************************************************
* Function Name: Field
* Description:
********************************************************************/
field::field( const string &name )
: Atomic( name )
, in_done( addInputPort( "in_done" ) )
, in_stop( addInputPort( "in_stop" ) )
, out_water_field( addOutputPort( "out_water_field" ) )
, out_water_surface( addOutputPort( "out_water_surface" ) )
, out_water_type( addOutputPort( "out_water_type" ) )
, out_sower_field( addOutputPort( "out_sower_field" ) )
, out_sower_surface( addOutputPort( "out_sower_surface" ) )
, out_sower_type( addOutputPort( "out_sower_type" ) )
, out_harvest_field( addOutputPort( "out_harvest_field" ) )
, out_harvest_surface( addOutputPort( "out_harvest_surface" ) )
, out_harvest_type( addOutputPort( "out_harvest_type" ) )
, iState( "empty" )
{
	surface = str2float(MainSimulator::Instance().getParameter(description(), "surface")); 	// in acres
	type = str2Int(MainSimulator::Instance().getParameter(description(), "type")); 			// culture type
	id = str2Int(MainSimulator::Instance().getParameter(description(), "id")); 				// field id

	Helper h;

	_Time t;

	t = h.TimeFromDays(str2float(MainSimulator::Instance().getParameter(description(), "growth_rate_1")));
	growths[0] = Time(t.h, t.m, t.s, t.ms);

	t = h.TimeFromDays(str2float(MainSimulator::Instance().getParameter(description(), "growth_rate_2")));
	growths[1] = Time(t.h, t.m, t.s, t.ms);

	t = h.TimeFromDays(str2float(MainSimulator::Instance().getParameter(description(), "growth_rate_3")));
	growths[2] = Time(t.h, t.m, t.s, t.ms);

	t = h.TimeFromDays(str2float(MainSimulator::Instance().getParameter(description(), "evaporation_rate_1")));
	evap[0] = Time(t.h, t.m, t.s, t.ms);

	t = h.TimeFromDays(str2float(MainSimulator::Instance().getParameter(description(), "evaporation_rate_2")));
	evap[1] = Time(t.h, t.m, t.s, t.ms);

	t = h.TimeFromDays(str2float(MainSimulator::Instance().getParameter(description(), "evaporation_rate_3")));
	evap[2] = Time(t.h, t.m, t.s, t.ms);

	// cout << "evaporation rate 3 is " << t << endl;
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition:
********************************************************************/
Model &field::initFunction()
{
	this->passivate();

	// auto start??
	timeLeft = growths[type - 1];
	holdIn(active, Time::Zero);

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &field::externalFunction( const ExternalMessage &msg )
{
	if (msg.port() == in_stop)
	{
		iState = "stopped";

		this->passivate();
	}

	// received seeds
	else if (iState == "empty")
	{
		// cout << msg.time() << " Field " << id << " is receiving seeds..." << endl;
		iState = "dry";

		type = (type) % 3 + 1;							// get next type
		timeLeft = growths[type - 1];

		holdIn(active, Time::Zero);	// Zero because ask for water immediately
	}

	// received water
	else if (iState == "dry")
	{
		// cout << msg.time() << " Field " << id << " is receiving water..." << endl;
		iState = "watered";

		Time duration = (timeLeft < evap[type - 1]) ? timeLeft : evap[type - 1];

		holdIn(active, duration);	// Ask for water once evaporated
	}

	// received harvest
	else if (iState == "ready")
	{
		// cout << msg.time() << " Field " << id << " is receiving harvest..." << endl;
		iState = "empty";

		holdIn(active, Time::Zero);	// Ask for seeds
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &field::internalFunction( const InternalMessage &msg )
{
	this->passivate();

	// cout << iState << "... passivating" << endl;

	if (iState != "watered") return *this;

	// Field is finished growing, note: timeLeft == 0 never happens.
	// I would've had to use the elapsed time
	if (timeLeft <= evap[type - 1])
	{
		// cout << msg.time() << " Field " << id << " is ready for harvest..." << endl;

		timeLeft = Time::Zero;
		iState = "ready";
		holdIn(active, Time::Zero);	// Zero because ask for harvest immediately
	}

	// Field still growing, needs water
	else
	{
		// cout << msg.time() << " Field " << id << " is thirsty..." << endl;

		iState = "dry";
		timeLeft = timeLeft - evap[type - 1]; // decrease timeleft for full growth
		holdIn(active, Time::Zero);	// Zero because ask for water immediately
	}

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &field::outputFunction( const InternalMessage &msg )
{
	if (iState == "dry")
	{
		// cout << msg.time() << " Field " << id << " is requesting water..." << endl;
		sendOutput( msg.time(), out_water_field, id) ;
		sendOutput( msg.time(), out_water_surface, surface) ;
		sendOutput( msg.time(), out_water_type, type) ;
	}

	if (iState == "ready")
	{
		// cout << msg.time() << " Field " << id << " is requesting harvest..." << endl;
		sendOutput( msg.time(), out_harvest_field, id) ;
		sendOutput( msg.time(), out_harvest_surface, surface) ;
		sendOutput( msg.time(), out_harvest_type, type) ;
	}

	if (iState == "empty")
	{
		// cout << msg.time() << " Field " << id << " is requesting seeds..." << endl;
		sendOutput( msg.time(), out_sower_field, id) ;
		sendOutput( msg.time(), out_sower_surface, surface) ;
		sendOutput( msg.time(), out_sower_type, type) ;
	}

	return *this ;
}
