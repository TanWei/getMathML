// XMLComment.cpp: implementation of the CXMLComment class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLComment::CXMLComment(const MSXML2::IXMLDOMCommentPtr& ptr)
:CXMLCharData(ptr.GetInterfacePtr())
{

}
CXMLComment::CXMLComment(const CXMLNode& other)
{
	//use an empty node to init this comment, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_COMMENT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to Comment Node");
	}
}
CXMLComment::~CXMLComment()
{

}
CXMLComment& CXMLComment::operator=(const CXMLNode& other)
{
	if(this == &other)
	{
		return *this;
	}

	if(!other.GetNodePtr().operator bool())
	{
		SetNodePtr(NULL);
		return *this;
	}

	if(other.IsAValidNode(CXMLNode::NODE_COMMENT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a comment node");
	}

	return *this;
}

MSXML2::IXMLDOMCommentPtr CXMLComment::GetCommentPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMCommentPtr>(this);
}

CXMLComment::operator MSXML2::IXMLDOMCommentPtr()const
{
	return GetCommentPtr();
}
CXMLComment::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLComment::ValidateState() const
{
	CXMLNode::ValidateState();
}
