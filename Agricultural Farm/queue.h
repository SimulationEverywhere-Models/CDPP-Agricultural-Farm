/*******************************************************************
*
*  DESCRIPTION: Atomic Model Queue Water
*
*  AUTHOR: Bruno St-Aubin
*
*  EMAIL: mailto://staubibr@gmail.com
*
*  DATE: 28-10-2017
*
*******************************************************************/

#ifndef __QUEUE_H
#define __QUEUE_H

#include <string>
#include "request.h"
#include "atomic.h"

class queue : public Atomic
{
	public:
	queue( const string &name = "queue" );

		virtual string className() const ;
	protected:
		Model &initFunction();
		Model &externalFunction( const ExternalMessage & );
		Model &internalFunction( const InternalMessage & );
		Model &outputFunction( const InternalMessage & );

	private:
		const Port &in_done;
		const Port &in_surface;
		const Port &in_field;
		const Port &in_type;
		Port &out_id;
		Port &out_surface;
		Port &out_type;
		Port &out_field_1;
		Port &out_field_2;
		Port &out_field_3;
		list<QueueRequest> requests;
		QueueRequest * request;
		int n;
		string iState; 	// internal state
};

inline
string queue::className() const
{
	return "queue" ;
}

#endif
