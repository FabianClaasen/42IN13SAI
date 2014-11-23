//
//  ListNode.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "ListNode.h"

ListNode::ListNode()
{
	
}

ListNode::ListNode(const ListNode& other) : prev(other.prev), next(other.next), data(other.data)
{

}

ListNode& ListNode::operator=(const ListNode& other)
{
	if (this != &other)
	{
		ListNode* listNode = new ListNode(other);
		return *listNode;
	}
	return *this;
}

std::shared_ptr<ListNode> ListNode::GetPrev()
{
	return prev;
}

void ListNode::SetPrev(std::shared_ptr<ListNode> node)
{
	prev = node;
}

std::shared_ptr<ListNode> ListNode::GetNext()
{
	return next;
}

void ListNode::SetNext(std::shared_ptr<ListNode> node)
{
	next = node;
}

std::shared_ptr<CompilerNode> ListNode::GetData()
{
	return data;
}

void ListNode::SetData(std::shared_ptr<CompilerNode> node)
{
	data = node;
}

ListNode::~ListNode()
{
	
}