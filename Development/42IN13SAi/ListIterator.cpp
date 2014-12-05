#include "ListIterator.h"

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

ListIterator& ListIterator::operator--()
{
    listNode = listNode->GetPrev();
    return *this;
}

std::shared_ptr<CompilerNode> ListIterator::operator*()
{
	return reinterpret_cast<ListNode*>(&listNode)->GetData();
}

bool ListIterator::operator == (const ListIterator& rvalue)
{
	return operator*() == rvalue.GetCompilerNode();
}

bool ListIterator::operator != (const ListIterator& rvalue)
{
	return !operator==(rvalue);
}

std::shared_ptr<CompilerNode> ListIterator::GetCompilerNode() const
{
    return listNode->GetData();
}

ListIterator::~ListIterator()
{
}
