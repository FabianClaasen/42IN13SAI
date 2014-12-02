//
//  ListNode.h
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//
#pragma once
#include <memory>
#include "CompilerNode.h"

class ListNode {
public:
	ListNode();
	ListNode(const ListNode& other);
	virtual ~ListNode();

	ListNode& operator=(const ListNode& other);
	
	
	std::shared_ptr<ListNode> GetPrev();
	std::shared_ptr<ListNode> GetNext();
	std::shared_ptr<CompilerNode> GetData();
	void SetPrev(std::shared_ptr<ListNode> node);
	void SetNext(std::shared_ptr<ListNode> node);
	void SetData(std::shared_ptr<CompilerNode> node);
	
private:
	std::shared_ptr<ListNode> prev;
	std::shared_ptr<ListNode> next;
	std::shared_ptr<CompilerNode> data;
};