// XMLDocFrag.cpp: implementation of the CXMLDocFrag class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLDocFrag::CXMLDocFrag(const MSXML2::IXMLDOMDocumentFragmentPtr& ptr)
:CXMLNode(ptr.GetInterfacePtr())
{

}
CXMLDocFrag::CXMLDocFrag(const CXMLNode& other)
{
	//use an empty node to init this Document fragment, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_DOCUMENT_FRAGMENT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to Document-Fragment Node");
	}
}
CXMLDocFrag::~CXMLDocFrag()
{

}
CXMLDocFrag& CXMLDocFrag::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_DOCUMENT_FRAGMENT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a document-fragment node");
	}

	return *this;
}

MSXML2::IXMLDOMDocumentFragmentPtr CXMLDocFrag::GetDocFragPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMDocumentFragmentPtr>(this);
}
CXMLDocFrag::operator MSXML2::IXMLDOMDocumentFragmentPtr()const
{
	return GetDocFragPtr();
}
CXMLDocFrag::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLDocFrag::ValidateState() const
{
	CXMLNode::ValidateState();
}
