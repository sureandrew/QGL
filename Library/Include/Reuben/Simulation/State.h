#pragma once
#ifndef _REUBEN_SIMULATION_STATE_H_
#define _REUBEN_SIMULATION_STATE_H_

#include "..\Utility\Process.h"
#include "Simulation.h"

#define DEFINE_STATE(_stateid_) \
	static CONST Reuben::Simulation::StateID _GetStateID(VOID) \
	{ \
		return (_stateid_); \
	} \
	virtual CONST Reuben::Simulation::StateID GetStateID(VOID) CONST \
	{ \
		return (_stateid_); \
	}

namespace Reuben { namespace Simulation {

	class State : public Reuben::Utility::Process
	{
	private:

		Boolean m_bBlocked;

	public:

		INLINE State(VOID);

		INLINE CONST Boolean IsBlocked(VOID) CONST;
		INLINE VOID Block(VOID);
		INLINE VOID Unblock(VOID);

	protected:

		virtual CONST Boolean CanTransitionTo(CONST StateID targetId) CONST;
		virtual CONST Boolean CanTransitionFrom(CONST StateID currentId) CONST;
		virtual VOID Enter(VOID);
		virtual VOID Exit(VOID);

	public:

		static INLINE CONST Boolean TestChangeState(State*& pCurrent, State* pTarget, Boolean bDelete = BOOLEAN_TRUE);
		static INLINE CONST Boolean ChangeState(State*& pCurrent, State* pTarget, Boolean bDelete = BOOLEAN_TRUE);

		virtual CONST StateID GetStateID(VOID) CONST = 0;

	};

	INLINE State::State(VOID)
		: m_bBlocked(BOOLEAN_FALSE)
	{}

	INLINE CONST Boolean State::IsBlocked(VOID) CONST
	{
		return (m_bBlocked);
	}

	INLINE VOID State::Block(VOID)
	{
		m_bBlocked = BOOLEAN_TRUE;
	}

	INLINE VOID State::Unblock(VOID)
	{
		m_bBlocked = BOOLEAN_FALSE;
	}

	INLINE CONST Boolean State::CanTransitionTo(CONST StateID targetId) CONST
	{
		if (IsBlocked())
			return (BOOLEAN_FALSE);
		return (BOOLEAN_TRUE);
	}

	INLINE CONST Boolean State::CanTransitionFrom(CONST StateID currentId) CONST
	{
		return (BOOLEAN_TRUE);
	}

	INLINE VOID State::Enter(VOID)
	{}

	INLINE VOID State::Exit(VOID)
	{}

	INLINE CONST Boolean State::TestChangeState(State*& pCurrent, State* pTarget, Boolean bDelete /*BOOLEAN_TRUE*/)
	{
		if (pCurrent != NULL && pTarget != NULL)
		{
			if (pCurrent->CanTransitionTo(pTarget->GetStateID()) == BOOLEAN_FALSE ||
				pTarget->CanTransitionFrom(pCurrent->GetStateID()) == BOOLEAN_FALSE)
			{
				if (bDelete)
					SafeDeleteObject(pTarget);				
				return (BOOLEAN_FALSE);
			}
		}
		if (bDelete)
			SafeDeleteObject(pTarget);
		return (BOOLEAN_TRUE);
	}

	INLINE CONST Boolean State::ChangeState(State*& pCurrent, State* pTarget, Boolean bDelete /*BOOLEAN_TRUE*/)
	{
		if (pCurrent != NULL && pTarget != NULL)
		{
			if (pCurrent->CanTransitionTo(pTarget->GetStateID()) == BOOLEAN_TRUE &&
				pTarget->CanTransitionFrom(pCurrent->GetStateID()) == BOOLEAN_TRUE)
			{
				pCurrent->Exit();
				if (bDelete)
					SafeDeleteObject(pCurrent);
			}
			else
			{
				if (bDelete)
					SafeDeleteObject(pTarget);
				return (BOOLEAN_FALSE);
			}
		}
		else if (pCurrent != NULL && pTarget == NULL)
		{
			pCurrent->Exit();
			if (bDelete)
				SafeDeleteObject(pCurrent);
		}

		pCurrent = pTarget;
		if (pCurrent != NULL)
			pCurrent->Enter();
		return (BOOLEAN_TRUE);
	}

}; }; // Reuben::Simulation

#endif // _REUBEN_SIMULATION_STATE_H_
