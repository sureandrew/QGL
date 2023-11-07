#pragma once
#ifndef _REUBEN_SIMULATION_SIMULATION_H_
#define _REUBEN_SIMULATION_SIMULATION_H_

#define INVALID_CLASS_ID	(C_UINT32(0))
#define INVALID_EVENT_ID	(C_UINT32(0))
#define INVALID_OBJECT_ID	(C_UINT(0))
#define INVALID_STATE_ID	(C_UINT32(0))

namespace Reuben { namespace Simulation {

	typedef UInt16	ClassID;
	typedef UInt32	EventID;			// 0-15 bits: class id, 16-23 bits: event enum
	typedef UInt16	HandleID;
	typedef UInt	ObjectID;
	typedef UInt32	StateID;
	typedef UInt8	Priority;

}; }; // Reuben::Simulation

#endif // _REUBEN_SIMULATION_SIMULATION_H_
