#pragma once
#ifndef _VIEW_MANAGER_H_
#define _VIEW_MANAGER_H_

class CMapActor;
class CMapNPC;
class CCharacter;
struct ViewRing;

typedef StlVector<CCharacter *, ManagedAllocator<CCharacter *> > CharacterPtrList;

struct ActorView
{
	union
	{
		CMapActor*		pActor;
		CCharacter*		pChar;
		CMapNPC*		pNpc;
	};
	RPCNetID		netId;
	ClassID			classId;
	UInt8			level;						// update level: 0 - simple, 1 - all 
	Boolean			appear;
	RPCMask*		concernMask;
	RPCMask			updateMask;
	UInt32			ageTime;
	ActorView*		prev;
	ActorView*		next;

	ActorView() :	pActor(NULL), classId(0), concernMask(NULL), ageTime(UINT32_MAX_VALUE), 
					level(0), appear(BOOLEAN_FALSE), prev(NULL), next(NULL) {}
};

class ViewManager
{
public:
	ViewManager(CCharacter* owner);
	virtual ~ViewManager();
	void Init();
	Boolean AddView(CMapActor *actor, Boolean bNext=BOOLEAN_TRUE);			// add actor in view
	Boolean RemoveView(CMapActor *actor, Boolean bDelete=BOOLEAN_FALSE,	// remove actor from view
						Boolean bNext=BOOLEAN_TRUE);
	void RemoveAllView();						// remove all actors in view
	void SafeRemove(CMapActor *actor);			// safe remove actor in all actors
	void ClearView(CMapActor *actor, RPCMask &mask);	// clear view update mask of actor
	void ViewUpdate(CMapActor *actor);			// this character view updated
	void SyncClient(CONST UInt32 dt);			// synchronize views to client
	void ResendView();							// resend all active view
	void SetConcernMask(CMapActor *actor, RPCMask &mask);	// change concern mask of actor view
	Boolean IsValid();							// is valid view manager

	ViewManager*	m_nextView;			// next view manager for Character Ring
	ViewRing*		m_ringBelong;		// view ring belong to

	static UInt32	m_agingTime;		// actor aging time
	static UInt16	m_syncLimit;		// sync actor limit for each SyncClient

	void GetCharacters(CharacterPtrList& chars);
	CCharacter * FindCharacter(UInt32 char_id);

private:
	Boolean CanAddView(CMapActor *actor);					// can add view
	void RemoveLink(ActorView* pview, CMapActor* actor=NULL);	// remove view link
	ActorView* FindView(CMapActor *actor);					// find view from link
	ActorView* CreateView();								// create new view
	void DeleteView(ActorView* pview);						// delete view
	void CheckView(NetGroup &grp, RPCNetIDVector &delIds, 
				const RPCNetIDVector &netIds);			// check all views for regions

	typedef StlMap<RPCNetID, ActorView*, std::less<RPCNetID>,
			ManagedAllocator<std::pair<RPCNetID, ActorView*> > > ActorViewMap;

	CCharacter*		m_owner;			// character owner
	ActorViewMap	m_actorView;		// active actor view of this character
	ActorView*		m_rootLink;			// root view link list
	ActorView*		m_curLink;			// current order view link pointing
	ActorView*		m_delLink;			// cached deleted view link list
	RPCNetIDVector	m_delIds;			// deleted actor net ID
	UInt16			m_viewCount;		// actual view count
};

#endif //_VIEW_MANAGER_H_
