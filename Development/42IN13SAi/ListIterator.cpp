#include "ListIterator.h"
#include "ListNode.h"
#include "CompilerNode.h"


ListIterator::ListIterator() : listNode(std::make_shared<ListNode>())
{
}

ListIterator::ListIterator(std::shared_ptr<ListNode> node) : listNode(node)
{
}

ListIterator& ListIterator::operator++()
{
	listNode = listNode->GetNext();
	return *this;
}

std::shared_ptr<CompilerNode> ListIterator::operator*()
{
	return reinterpret_cast<ListNode*>(&listNode)->GetData();
}

bool ListIterator::operator == (ListIterator& other)
{
	return *other == operator*();
}

bool ListIterator::operator != (ListIterator& other)
{
	return !operator==(other);
}

ListIterator::~ListIterator()
{
}
