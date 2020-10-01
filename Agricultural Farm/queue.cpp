/*******************************************************************
*
*  DESCRIPTION: Atomic Model Queue for water requests
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.ca
*
*  DATE: 27-10-2017
*
*******************************************************************/

#include "queue.h"
#include "helper.h"
#include "request.h"

#include "strutil.h"
#include "time.h"
#include "message.h"
#include "mainsimu.h"   // MainSimulator::Instance().getParameter( ... )


/** public functions **/

/*******************************************************************
* Function Name: Queue for water requests
* Description:
********************************************************************/
queue::queue( const string &name )
: Atomic( name )
, in_done( addInputPort( "in_done" ) )
, in_surface( addInputPort( "in_surface" ) )
, in_field( addInputPort( "in_field" ) )
, in_type( addInputPort( "in_type" ) )
, out_id( addOutputPort( "out_id" ) )
, out_surface( addOutputPort( "out_surface" ) )
, out_type( addOutputPort( "out_type" ) )
, out_field_1( addOutputPort( "out_field_1" ) )
, out_field_2( addOutputPort( "out_field_2" ) )
, out_field_3( addOutputPort( "out_field_3" ) )
, n(0)
, iState("default")
{
	request = new QueueRequest();
	request->id = ++n;
}

/*******************************************************************
* Function Name: initFunction
* Description:
* Precondition:
********************************************************************/
Model &queue::initFunction()
{
	this->passivate();

	return *this ;
}

/*******************************************************************
* Function Name: externalFunction
* Description:
********************************************************************/
Model &queue::externalFunction( const ExternalMessage &msg )
{
	if( msg.port() == in_done && !requests.empty())
	{
		// cout << "Receiving a message in done port." << endl;

		iState = "done";

		holdIn(active, Time::Zero);

		return *this;
	}

	if( msg.port() == in_field) request->field = static_cast<int>(msg.value());

	if( msg.port() == in_surface) request->surface = static_cast <double>(msg.value());

	if( msg.port() == in_type) request->type = static_cast <int>(msg.value());

	// If the request being built is ready, add it to the queue and initialize a new one
	if (request->IsReady()) {

		// cout << "Adding a new request to queue." << endl;

		requests.push_back(*request);

		// cout << "queue has " << requests.size() << " requests pending..." << endl;

		request = new QueueRequest();
		request->id = ++n;

		if (requests.size() == 1) {
			// cout << "Ready to start outputting requests." << endl;

			holdIn(active, Time::Zero);
		}
	}

	return *this;
}

/*******************************************************************
* Function Name: internalFunction
* Description:
********************************************************************/
Model &queue::internalFunction( const InternalMessage & )
{
	this->passivate();

	// cout << "internal transition happening" << endl;

	return *this ;
}

/*******************************************************************
* Function Name: outputFunction
* Description:
********************************************************************/
Model &queue::outputFunction( const InternalMessage &msg )
{
	// cout << "Internal State is : " << iState << endl;

	// Feedback loop to fields
	if(iState == "done")
	{
		// cout << "Done. Outputting to field : " << requests.front().field << endl;

		// Send feedback to fields for last completed job
		if (requests.front().field == 1) sendOutput( msg.time(), out_field_1, requests.front().id );
		if (requests.front().field == 2) sendOutput( msg.time(), out_field_2, requests.front().id );
		if (requests.front().field == 3) sendOutput( msg.time(), out_field_3, requests.front().id );

		// Have to pop here because I still needed the front requests to output back to the field.
		requests.pop_front() ;
	}

	// Send next job to processor
	// cout << "Outputting next job to processor." << endl;

	if (!requests.empty())
	{
		sendOutput( msg.time(), out_id, requests.front().id ) ;
		sendOutput( msg.time(), out_surface, requests.front().surface ) ;
		sendOutput( msg.time(), out_type, requests.front().type ) ;
	}

	iState = "default";

	return *this ;
}
