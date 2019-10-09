// XMLDocType.cpp: implementation of the CXMLDocType class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLDocType::CXMLDocType(const MSXML2::IXMLDOMDocumentTypePtr& ptr)
:CXMLNode(ptr.GetInterfacePtr())
{

}
CXMLDocType::CXMLDocType(const CXMLNode& other)
{
	//use an empty node to init this Document-Type, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_DOCUMENT_TYPE))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to Document-Type Node");
	}
}
CXMLDocType::~CXMLDocType()
{

}

CXMLDocType& CXMLDocType::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_DOCUMENT_TYPE))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a document-type node");
	}

	return *this;
}

wstring CXMLDocType::GetName() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(GetDocTypePtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocumentType::Getname));
}

CXMLNodeMap CXMLDocType::GetEntities() const
{
	ValidateState();
	return CXMLNodeMap(TCallMemberFunc0(GetDocTypePtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocumentType::Getentities));
}

CXMLNodeMap CXMLDocType::GetNotations() const
{
	ValidateState();
	return CXMLNodeMap(TCallMemberFunc0(
		GetDocTypePtr().GetInterfacePtr(),&MSXML2::IXMLDOMDocumentType::Getnotations));
}

MSXML2::IXMLDOMDocumentTypePtr CXMLDocType::GetDocTypePtr() const
{
	return TGetPtr<MSXML2::IXMLDOMDocumentTypePtr>(this);
}
CXMLDocType::operator MSXML2::IXMLDOMDocumentTypePtr()const
{
	return GetDocTypePtr();
}
CXMLDocType::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLDocType::ValidateState() const
{
	CXMLNode::ValidateState();
}
