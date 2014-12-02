#pragma once
#include <memory>
#include "ListNode.h"
#include "CompilerNode.h"

class ListIterator
{
	friend class LinkedList;
public:
	ListIterator();
	virtual ~ListIterator();

	// iterator methods
	ListIterator& operator++();
	std::shared_ptr<CompilerNode> operator*();
	bool operator==(const ListIterator& rvalue);
    bool operator!=(const ListIterator& rvalue);

private:
	std::shared_ptr<ListNode> listNode;
	ListIterator(std::shared_ptr<ListNode> node);
    std::shared_ptr<CompilerNode> GetCompilerNode() const;
};

