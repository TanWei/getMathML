// XMLElement.cpp: implementation of the CXMLElement class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLElement::CXMLElement(const MSXML2::IXMLDOMElementPtr& ptr)
:CXMLNode(ptr)
{

}
CXMLElement::CXMLElement(const CXMLNode& other)
{
	//use an empty node to init this element, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_ELEMENT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to element Node");
	}
}

CXMLElement::~CXMLElement()
{	
}

CXMLElement& CXMLElement::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_ELEMENT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a element node");
	}

	return *this;
}

wstring CXMLElement::GetTagName() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(GetElementPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMElement::GettagName));
}

bool CXMLElement::HaveAttribute(const wchar_t *cszName)const
{
	ValidateState();
	try
	{
		TCallMemberFunc1(GetElementPtr().GetInterfacePtr(), &MSXML2::IXMLDOMElement::getAttributeNode, cszName);
	}
	catch(CXMLError e)
	{
		return false;
	}
	return true;
}

wstring CXMLElement::GetAttributeValueByName(const wchar_t *cszName)const
{
	try
	{
		ValidateState();
		return ConvertVARIANTTowstring(TCallMemberFunc1(
			GetElementPtr().GetInterfacePtr(),&MSXML2::IXMLDOMElement::getAttribute,cszName));
	}
	catch (CXMLError & error)  
	{
		return  L"";
	}
	
}

void CXMLElement::SetAttributeValueByName(const wchar_t *cszName, const wchar_t *cszValue)
{
	ValidateState();
	TCallMemberFunc2(GetElementPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMElement::setAttribute,cszName,cszValue);
}

void CXMLElement::RemoveAttributeByName(const wchar_t *cszName)
{
	ValidateState();
	TCallMemberFunc1(GetElementPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMElement::removeAttribute,cszName);
}

CXMLAttribute CXMLElement::GetAttributeNodeByName(const wchar_t *cszName) const
{
	ValidateState();
	return CXMLAttribute(TCallMemberFunc1(
		GetElementPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMElement::getAttributeNode,cszName));
}

CXMLAttribute CXMLElement::SetAttributeNode(const CXMLAttribute &newNode)
{
	ValidateState();
	return CXMLAttribute(TCallMemberFunc1(
		GetElementPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMElement::setAttributeNode,newNode.GetAttributePtr()));
}

CXMLAttribute CXMLElement::RemoveAttributeNode(const CXMLAttribute &oldNode)
{
	ValidateState();
	return CXMLAttribute(TCallMemberFunc1(
		GetElementPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMElement::removeAttributeNode,oldNode.GetAttributePtr()));
}

CXMLNodeList CXMLElement::GetElementsByTagName(const wchar_t *cszTagName) const
{
	ValidateState();
	return CXMLNodeList(TCallMemberFunc1(
		GetElementPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMElement::getElementsByTagName,cszTagName));
}

void CXMLElement::Normalize()
{
	ValidateState();
	TCallMemberFunc0(GetElementPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMElement::normalize);
}

MSXML2::IXMLDOMElementPtr CXMLElement::GetElementPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMElementPtr>(this);
}

CXMLElement::operator MSXML2::IXMLDOMElementPtr()const
{
	return GetElementPtr();
}

CXMLElement::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLElement::ValidateState() const
{
	CXMLNode::ValidateState();
}
