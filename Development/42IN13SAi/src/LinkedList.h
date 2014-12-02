//
//  LinkedList.h
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//
#pragma once
#include <list>
#include "ListNode.h"
#include "ListIterator.h"

class LinkedList {
public:
	LinkedList();
	LinkedList(std::list<std::shared_ptr<CompilerNode>> nodes);
	virtual ~LinkedList();
	
	int size();

	std::shared_ptr<ListNode> GetNext();
	std::shared_ptr<ListNode> PeekPrevious();
	std::shared_ptr<ListNode> GetHead();
	std::shared_ptr<ListNode> GetTail();
	std::shared_ptr<ListNode> GetCurrent();

	// Data methods
	std::shared_ptr<CompilerNode> GetCurrentData();
	std::shared_ptr<CompilerNode> GetTailData();

	void PushBack(std::shared_ptr<CompilerNode> node);
	void SetCurrent(std::shared_ptr<ListNode> node);
	void SetCurrent(std::shared_ptr<CompilerNode> node);
	void SetCurrent(std::shared_ptr<CompilerNode> node, bool isLoopEnd);

	//iterator methods
	ListIterator begin();
	ListIterator end();
	
private:
	std::shared_ptr<ListNode> current;
	std::shared_ptr<ListNode> head;
	std::shared_ptr<ListNode> tail;
	int count;
	
	std::shared_ptr<ListNode> findNode(std::shared_ptr<CompilerNode> node);
};
