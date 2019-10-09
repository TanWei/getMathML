// XMLCDataSection.cpp: implementation of the CXMLCDataSection class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLCDataSection::CXMLCDataSection(const MSXML2::IXMLDOMCDATASectionPtr& ptr)
:CXMLText(ptr.GetInterfacePtr())
{

}
CXMLCDataSection::CXMLCDataSection(const CXMLNode& other)
{
	//use an empty node to init this CDATA, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_CDATA_SECTION))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to CDATA Section Node");
	}
}

CXMLCDataSection::~CXMLCDataSection()
{

}
CXMLCDataSection& CXMLCDataSection::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_CDATA_SECTION))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a CDATA Section node");
	}

	return *this;
}
CXMLCDataSection::operator bool()const
{
	return (GetNodePtr().operator bool());
}

MSXML2::IXMLDOMCDATASectionPtr CXMLCDataSection::GetCDATASectionPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMCDATASectionPtr>(this);
}

CXMLCDataSection::operator MSXML2::IXMLDOMCDATASectionPtr()const
{
	return GetCDATASectionPtr();
}

void CXMLCDataSection::ValidateState() const
{
	CXMLNode::ValidateState();
}
