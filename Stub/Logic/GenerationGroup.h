#ifndef _GENERATION_GROUP_H_
#define _GENERATION_GROUP_H_

#include "Common/common_binary.h"
#include "Generation.h"
#include "Character.h"

class CGenerationGroup : public NetGroup
{
public:
	CGenerationGroup();
	~CGenerationGroup();
public:
	typedef StlMap<UInt32, CGeneration*, std::less<UInt32>, ManagedAllocator<std::pair<UInt32, CGeneration*> > > GenerationMap;
	RPCResult UpdateGenerationGroup(RPCContext &context);
	void SetOwner(CCharacter* pOwner) { m_pOwner = pOwner;}
	CCharacter* GetOwner(){ return m_pOwner;}
	GenerationMap   m_mapGeneration;
private:
	CCharacter* m_pOwner;

};

#endif