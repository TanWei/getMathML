// XMLNotation.cpp: implementation of the CXMLNotation class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLNotation::CXMLNotation(const MSXML2::IXMLDOMNotationPtr& ptr)
:CXMLNode(ptr)
{

}
CXMLNotation::CXMLNotation(const CXMLNode& other)
{
	//use an empty node to init this notation, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_NOTATION))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to Notation Node");
	}

}
CXMLNotation::~CXMLNotation()
{

}
CXMLNotation& CXMLNotation::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_NOTATION))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a notation node");
	}

	return *this;
}
wstring CXMLNotation::GetPublicID() const
{
	ValidateState();
	return ConvertVARIANTTowstring(TCallMemberFunc0(
		GetNotationPtr().GetInterfacePtr(),&MSXML2::IXMLDOMNotation::GetpublicId));
}

wstring CXMLNotation::GetSystemID() const
{
	ValidateState();
	return ConvertVARIANTTowstring(TCallMemberFunc0(
		GetNotationPtr().GetInterfacePtr(),&MSXML2::IXMLDOMNotation::GetsystemId));
}

MSXML2::IXMLDOMNotationPtr CXMLNotation::GetNotationPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMNotationPtr>(this);
}
CXMLNotation::operator MSXML2::IXMLDOMNotationPtr()const
{
	return GetNotationPtr();
}
CXMLNotation::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLNotation::ValidateState() const
{
	CXMLNode::ValidateState();
}
