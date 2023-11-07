#pragma once
#ifndef _EXECUTECHEAT_H_
#define _EXECUTECHEAT_H_

#define ExecuteCheat_CLASSID 54000

class CExecuteCheat
	: public Reuben::Simulation::Object
	, public Reuben::Simulation::IEventHandler<CExecuteCheat>
{
public:
	DEFINE_CLASS(ExecuteCheat_CLASSID);
	DEFINE_PARENT_CLASS(Object);

	CExecuteCheat();
	virtual ~CExecuteCheat();

	void Start();
	void GetCheatCommand();
	void PrepareCheat(NetGroup* grp);
	Boolean SendCheat(NetGroup* grp);
	void PostCheatCommandSent();

	// Event & State
	virtual void HandleEvent(CONST Reuben::Simulation::Event &event);

	enum EventEnum
	{
		EVT_UPDATE_GETNEWCHEAT_TIMEOUT	= MAKE_EVENTID(ExecuteCheat_CLASSID, 1),	// update object
	};
};

#endif // _EXECUTECHEAT_H_