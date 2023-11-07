#pragma once
#ifndef _REUBEN_UTILITY_LINKED_LIST_H_
#define _REUBEN_UTILITY_LINKED_LIST_H_

#include "../Math/Math.h"

namespace Reuben { namespace Utility {

	template<typename _TYPE_>
	class SingleLinkedList
	{
	private:

		struct Node
		{
			_TYPE_ item;
			Node* pNext;

			Node(_TYPE_ _item)
				: item(_item)
				, pNext(NULL)
			{}
		};

	private:

		Node* m_pFirstNode;
		Node* m_pLastNode;
		Node* m_pCurrentNode;
		Count m_cNodes;

	public:

		SingleLinkedList(VOID)
			: m_pFirstNode(NULL)
			, m_pLastNode(NULL)
			, m_pCurrentNode(NULL)
			, m_cNodes(C_COUNT(0))
		{}

		virtual ~SingleLinkedList(VOID)
		{
			RemoveAll();
		}

		Boolean IsEmpty(VOID) CONST
		{
			return (m_pFirstNode == NULL);
		}

		Boolean HasContain(CONST _TYPE_& item) CONST
		{
			Node* pTemp = m_pFirstNode;
			while (pTemp)
			{
				if (pTemp->item == item)
					return (BOOLEAN_TRUE);
				pTemp = pTemp->pNext;
			}
			return (BOOLEAN_FALSE);
		}

		_TYPE_& GetFirst(VOID) CONST
		{
			TRACE_CHECK(!IsEmpty());

			return (m_pFirstNode->item);
		}

		_TYPE_& GetLast(VOID) CONST
		{
			TRACE_CHECK(!IsEmpty());

			return (m_pLastNode->item);
		}

		_TYPE_& GetCurrent(VOID) CONST
		{
			TRACE_CHECK(!IsEmpty());

			return (m_pCurrentNode->item);
		}

		_TYPE_* GetItemPointer(CONST Count index) CONST
		{
			Node* node = m_pFirstNode;
			Count i = C_COUNT(0);
			while (node && i < index)
			{
				node = node->pNext;
				++i;
			}
			if (node)
				return (&node->item);
			else
				return (NULL);
		}

		VOID AddFirst(CONST _TYPE_& item)
		{
			Node* pNode = SafeCreateObject1(Node, item);

			if (IsEmpty())
			{
				m_pFirstNode = m_pLastNode = m_pCurrentNode = pNode;
			}
			else
			{
				pNode->pNext = m_pFirstNode;
				m_pFirstNode = pNode;
			}

			++m_cNodes;
		}

		VOID AddLast(CONST _TYPE_& item)
		{
			Node* pNode = SafeCreateObject1(Node, item);

			if (IsEmpty())
			{
				m_pFirstNode = m_pLastNode = m_pCurrentNode = pNode;
			}
			else
			{
				m_pLastNode->pNext = pNode;
				m_pLastNode = pNode;
			}

			++m_cNodes;
		}

		Boolean Remove(CONST _TYPE_& item)
		{
			if (IsEmpty())
				return BOOLEAN_FALSE;

			Node* pPrev = NULL;
			Node* pTemp = m_pFirstNode;
			while (pTemp && pTemp->item != item) {
				pPrev = pTemp;
				pTemp = pTemp->pNext;
			}

			if (pTemp == NULL)
				return BOOLEAN_FALSE;

			if (pTemp == m_pFirstNode) {
				m_pFirstNode = m_pFirstNode->pNext;
			}
			if (pTemp == m_pLastNode) {
				m_pLastNode = pPrev;
			}
			if (pTemp == m_pCurrentNode) {
				if (m_pCurrentNode->pNext)
					m_pCurrentNode = m_pCurrentNode->pNext;
				else
					m_pCurrentNode = pPrev;
			}
			if (pPrev)
				pPrev->pNext = pTemp->pNext;
			SafeDeleteObject(pTemp);
			--m_cNodes;

			return BOOLEAN_TRUE;
		}

		Boolean RemoveCurrent()
		{
			if (IsEmpty())
				return BOOLEAN_FALSE;

			Node* pPrev = NULL;
			Node* pTemp = m_pFirstNode;
			while (pTemp && pTemp != m_pCurrentNode) {
				pPrev = pTemp;
				pTemp = pTemp->pNext;
			}

			if (pTemp == NULL)
				return BOOLEAN_FALSE;

			if (pTemp == m_pFirstNode) {
				m_pFirstNode = m_pFirstNode->pNext;
			}
			if (pTemp == m_pLastNode) {
				m_pLastNode = pPrev;
			}
			if (pTemp == m_pCurrentNode) {
				if (m_pCurrentNode->pNext)
					m_pCurrentNode = m_pCurrentNode->pNext;
				else
					m_pCurrentNode = pPrev;
			}
			if (pPrev)
				pPrev->pNext = pTemp->pNext;
			SafeDeleteObject(pTemp);
			--m_cNodes;

			return BOOLEAN_TRUE;
		}

		Boolean Replace(CONST _TYPE_& oldItem, CONST _TYPE_& newItem)
		{
			if (IsEmpty())
				return BOOLEAN_FALSE;

			Node* pTemp = m_pFirstNode;
			while (pTemp && pTemp->item != oldItem) {
				pTemp = pTemp->pNext;
			}

			if (pTemp == NULL)
				return BOOLEAN_FALSE;
			
			pTemp->item = newItem;
			return BOOLEAN_TRUE;
		}

		_TYPE_ RemoveFirst(VOID)
		{
			TRACE_CHECK(!IsEmpty());

			_TYPE_ removedItem = m_pFirstNode->item;
			if (m_pFirstNode == m_pLastNode)
			{
				m_pLastNode = NULL;
			}
		
			if (m_pFirstNode == m_pCurrentNode)
			{
				m_pCurrentNode = m_pFirstNode->pNext;
			}

			Node* pTemp = m_pFirstNode;
			m_pFirstNode = m_pFirstNode->pNext;
			SafeDeleteObject(pTemp);

			--m_cNodes;

			return (removedItem);
		}

		_TYPE_ RemoveLast(VOID)
		{
			TRACE_CHECK(!IsEmpty());

			Node* pFindNewLast = NULL;
			if (m_pFirstNode != m_pLastNode)
			{
				pFindNewLast = m_pFirstNode;
				while (pFindNewLast->pNext != m_pLastNode)
				{
					pFindNewLast = pFindNewLast->pNext;
				}
			}

			if (pFindNewLast == NULL)
			{
				m_pFirstNode = NULL;
				m_pCurrentNode = NULL;
			}
			else
			{
				if (m_pCurrentNode == m_pLastNode)
				{
					m_pCurrentNode = pFindNewLast;
				}
			}

			_TYPE_ removedItem = m_pLastNode->item;
			SafeDeleteObject(m_pLastNode);
			m_pLastNode = pFindNewLast;
			--m_cNodes;
	
			return (removedItem);
		}

		VOID RemoveAll(VOID)
		{
			if (!IsEmpty())
			{
				Node* pTemp;
				while (m_pFirstNode != NULL)
				{
					pTemp = m_pFirstNode;
					m_pFirstNode = m_pFirstNode->pNext;
					SafeDeleteObject(pTemp);
				}
				m_pLastNode = NULL;
				m_pCurrentNode = NULL;
				m_cNodes = C_COUNT(0);
			}
		}

		Boolean Forward(VOID)
		{
			if (m_pCurrentNode != NULL && m_pCurrentNode->pNext != NULL)
			{
				m_pCurrentNode = m_pCurrentNode->pNext;
				return (BOOLEAN_TRUE);
			}
			return (BOOLEAN_FALSE);
			
		}

		Boolean MoveToFirst(VOID)
		{
			m_pCurrentNode = m_pFirstNode;
			if (m_pCurrentNode != NULL)
			{
				return (BOOLEAN_TRUE);
			}
			return (BOOLEAN_FALSE);
		}

		Boolean MoveToLast(VOID)
		{
			m_pCurrentNode = m_pLastNode;
			if (m_pCurrentNode != NULL)
			{
				return (BOOLEAN_TRUE);
			}
			return (BOOLEAN_FALSE);
		}

		CONST Count GetCount(VOID) CONST
		{
			return (m_cNodes);
		}

		VOID Swap(SingleLinkedList& list)
		{
			Reuben::Math::Swap(m_pFirstNode, list.m_pFirstNode);
			Reuben::Math::Swap(m_pLastNode, list.m_pLastNode);
			Reuben::Math::Swap(m_pCurrentNode, list.m_pCurrentNode);
			Reuben::Math::Swap(m_cNodes, list.m_cNodes);
		}

		VOID SwitchFrom(SingleLinkedList& list)
		{
			RemoveAll();
			m_pFirstNode = list.m_pFirstNode;
			m_pLastNode = list.m_pLastNode;
			m_pCurrentNode = list.m_pCurrentNode;
			m_cNodes = list.m_cNodes;
			list.m_pFirstNode = NULL;
			list.m_pLastNode = NULL;
			list.m_pCurrentNode = NULL;
			list.m_cNodes = C_COUNT(0);
		}

		VOID SwitchTo(SingleLinkedList& list)
		{
			list.RemoveAll();
			list.m_pFirstNode = m_pFirstNode;
			list.m_pLastNode = m_pLastNode;
			list.m_pCurrentNode = m_pCurrentNode;
			list.m_cNodes = m_cNodes;
			m_pFirstNode = NULL;
			m_pLastNode = NULL;
			m_pCurrentNode = NULL;
			m_cNodes = C_COUNT(0);
		}

		VOID AttachFirst(SingleLinkedList& list)
		{
			if (list.m_pFirstNode)
			{
				if (m_pFirstNode)
				{
					Node* pOldFirst = m_pFirstNode;
					m_pFirstNode = list.m_pFirstNode;
					m_pCurrentNode = list.m_pFirstNode;
					list.m_pLastNode->pNext = pOldFirst;
					m_cNodes += list.m_cNodes;
				}
				else
				{
					m_pFirstNode = list.m_pFirstNode;
					m_pLastNode = list.m_pLastNode;
					m_pCurrentNode = list.m_pFirstNode;
					m_cNodes = list.m_cNodes;
				}
				list.m_pFirstNode = NULL;
				list.m_pLastNode = NULL;
				list.m_pCurrentNode = NULL;
				list.m_cNodes = C_COUNT(0);
			}
		}

		VOID AttachLast(SingleLinkedList& list)
		{
			if (list.m_pFirstNode)
			{
				if (m_pFirstNode)
				{
					m_pLastNode->pNext = list.m_pFirstNode;
					m_pLastNode = list.m_pLastNode;
					m_cNodes += list.m_cNodes;
				}
				else
				{
					m_pFirstNode = list.m_pFirstNode;
					m_pLastNode = list.m_pLastNode;
					m_pCurrentNode = list.m_pFirstNode;
					m_cNodes = list.m_cNodes;
				}
				list.m_pFirstNode = NULL;
				list.m_pLastNode = NULL;
				list.m_pCurrentNode = NULL;
				list.m_cNodes = C_COUNT(0);
			}
		}
	};

};}; // Reuben::Utility

#endif // _REUBEN_UTILITY_LINKED_LIST_H_
