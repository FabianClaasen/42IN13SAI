//
//  LinkedList.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "LinkedList.h"

LinkedList::LinkedList() : count(0)
{
	
}

LinkedList::LinkedList(std::list<std::shared_ptr<CompilerNode>> nodes) : count(0)
{
	// Create the linkedlist from a node list
	for (std::shared_ptr<CompilerNode> node : nodes)
	{
		PushBack(node);
	}
}

int LinkedList::size()
{
	return count;
}

std::shared_ptr<ListNode> LinkedList::GetNext()
{
	if (current)
	{
		std::shared_ptr<ListNode> node = std::shared_ptr<ListNode>(current->GetNext());
		std::shared_ptr<ListNode> tempCurrent = current;
		if  (node)
			current = node;
		return tempCurrent;
	}
	return std::make_shared<ListNode>(ListNode());
}

std::shared_ptr<ListNode> LinkedList::PeekPrevious()
{
	if (current)
	{
		std::shared_ptr<ListNode> node = current->GetPrev();
		if (node)
		{
			return node;
		}
	}
	return std::make_shared<ListNode>(ListNode());
}

std::shared_ptr<ListNode> LinkedList::GetCurrent()
{
	return current;
}

std::shared_ptr<ListNode> LinkedList::GetHead()
{
	return head;
}

std::shared_ptr<ListNode> LinkedList::GetTail()
{
	return tail;
}

std::shared_ptr<CompilerNode> LinkedList::GetCurrentData()
{
	return current->GetData();
}

std::shared_ptr<CompilerNode> LinkedList::GetTailData()
{
	return tail->GetData();
}

void LinkedList::SetCurrent(std::shared_ptr<ListNode> node)
{
	current = node;
}

void LinkedList::SetCurrent(std::shared_ptr<CompilerNode> node)
{
	std::shared_ptr<ListNode> listNode = findNode(node);
	SetCurrent(listNode);
}

void LinkedList::SetCurrent(std::shared_ptr<CompilerNode> node, bool isLoopEnd)
{
	std::shared_ptr<ListNode> listNode = findNode(node);
	SetCurrent(listNode->GetNext());
}

std::shared_ptr<ListNode> LinkedList::findNode(std::shared_ptr<CompilerNode> node)
{
	std::shared_ptr<ListNode> listNode = head;
    
	while (listNode != std::shared_ptr<ListNode>())
	{
		if (listNode->GetData()->GetJumpTo() == node->GetJumpTo())
			return listNode;
		listNode = listNode->GetNext();
	}
	return std::make_shared<ListNode>(ListNode());
}

void LinkedList::PushBack(std::shared_ptr<CompilerNode> node)
{
	// Default settings
	std::shared_ptr<ListNode> sNode = std::make_shared<ListNode>();
	sNode->SetData(node);
	sNode->SetNext(std::make_shared<ListNode>());

	if (!head)
	{
		current = sNode;
		head = current;
		tail = current;
		
	}
	else
	{
		sNode->SetPrev(tail);
		tail->SetNext(sNode);
		tail = sNode;
	}
	count++;
}

ListIterator LinkedList::begin()
{
	if (!head)
		return ListIterator();
	else
		return ListIterator(head);
}

ListIterator LinkedList::end()
{
	return ListIterator();
}

LinkedList::~LinkedList()
{
	
}
