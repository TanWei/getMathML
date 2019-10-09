// XMLAttribute.cpp: implementation of the CXMLAttribute class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLAttribute::CXMLAttribute(const  MSXML2::IXMLDOMAttributePtr& ptr)
:CXMLNode(ptr)
{

}
CXMLAttribute::CXMLAttribute(const CXMLNode& other)
{
	//use an empty node to init this tree, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_ATTRIBUTE))
	{
		SetNodePtr(other.GetNodePtr());
	}
	else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to Attribute Node");
	}
}

CXMLAttribute::~CXMLAttribute()
{
}

CXMLAttribute& CXMLAttribute::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_ATTRIBUTE))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to an attribute node");
	}

	return *this;}

wstring CXMLAttribute::GetName() const
{
	ValidateState();

	return ConvertBSTRTowstring(TCallMemberFunc0(
		GetAttributePtr().GetInterfacePtr(), &MSXML2::IXMLDOMAttribute::Getname));
}

wstring CXMLAttribute::GetValue() const
{
	ValidateState();
	return ConvertVARIANTTowstring(TCallMemberFunc0(
		GetAttributePtr().GetInterfacePtr(), &MSXML2::IXMLDOMAttribute::Getvalue));
}

void CXMLAttribute::SetValue(const wchar_t *cszValue)
{
	ValidateState();

	TCallVoidMemberFunc1(GetAttributePtr().GetInterfacePtr(),
		 &MSXML2::IXMLDOMAttribute::Putvalue,cszValue);
}

 MSXML2::IXMLDOMAttributePtr CXMLAttribute::GetAttributePtr() const
{
	return TGetPtr< MSXML2::IXMLDOMAttributePtr>(this);
}
CXMLAttribute::operator  MSXML2::IXMLDOMAttributePtr()const
{
	return GetAttributePtr();
}
CXMLAttribute::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLAttribute::ValidateState() const
{
	CXMLNode::ValidateState();
}
