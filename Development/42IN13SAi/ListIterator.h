#pragma once
#include <memory>

class ListIterator
{
	friend class LinkedList;
	friend class ListNode;
	friend class CompilerNode;
public:
	ListIterator();
	virtual ~ListIterator();

	// iterator methods
	ListIterator& operator++();
	std::shared_ptr<CompilerNode> operator*();
	bool operator==(ListIterator& other);
	bool operator!=(ListIterator& other);

private:
	std::shared_ptr<ListNode> listNode;
	ListIterator(std::shared_ptr<ListNode> node);
};

