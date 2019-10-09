// XMLEntity.cpp: implementation of the CXMLEntity class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLEntity::CXMLEntity(const MSXML2::IXMLDOMEntityPtr& ptr)
:CXMLNode(ptr)
{

}
CXMLEntity::CXMLEntity(const CXMLNode& other)
{
	//use an empty node to init this entity, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_ENTITY))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to Entity Node");
	}
}
CXMLEntity::~CXMLEntity()
{

}

CXMLEntity& CXMLEntity::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_ENTITY))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a entity node");
	}

	return *this;
}

wstring CXMLEntity::GetPublicID() const
{
	ValidateState();
	return ConvertVARIANTTowstring(TCallMemberFunc0(
		GetEntityPtr().GetInterfacePtr(),&MSXML2::IXMLDOMEntity::GetpublicId));
}

wstring CXMLEntity::GetSystemID() const
{
	ValidateState();
	return ConvertVARIANTTowstring(TCallMemberFunc0(
		GetEntityPtr().GetInterfacePtr(),&MSXML2::IXMLDOMEntity::GetsystemId));
}

wstring CXMLEntity::GetNotationName() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(GetEntityPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMEntity::GetnotationName));
}

MSXML2::IXMLDOMEntityPtr CXMLEntity::GetEntityPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMEntityPtr>(this);
}
CXMLEntity::operator MSXML2::IXMLDOMEntityPtr()const
{
	return GetEntityPtr();
}
CXMLEntity::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLEntity::ValidateState() const
{
	CXMLNode::ValidateState();
}
