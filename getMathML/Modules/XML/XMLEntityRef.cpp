// XMLEntityRef.cpp: implementation of the CXMLEntityRef class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLEntityRef::CXMLEntityRef(const MSXML2::IXMLDOMEntityReferencePtr& ptr)
:CXMLNode(ptr.GetInterfacePtr())
{

}
CXMLEntityRef::CXMLEntityRef(const CXMLNode& other)
{
	//use an empty node to init this entity reference, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_ENTITY_REFERENCE))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to Entity Reference Node");
	}
}
CXMLEntityRef::~CXMLEntityRef()
{

}
CXMLEntityRef& CXMLEntityRef::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_ENTITY_REFERENCE))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a entity reference node");
	}

	return *this;
}

MSXML2::IXMLDOMEntityReferencePtr CXMLEntityRef::GetEntityRefPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMEntityReferencePtr>(this);
}	
CXMLEntityRef::operator MSXML2::IXMLDOMEntityReferencePtr()const
{
	return GetEntityRefPtr();
}

CXMLEntityRef::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLEntityRef::ValidateState() const
{
	CXMLNode::ValidateState();
}
