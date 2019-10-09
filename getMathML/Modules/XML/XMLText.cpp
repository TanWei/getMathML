// XMLText.cpp: implementation of the CXMLText class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLText::CXMLText(const MSXML2::IXMLDOMTextPtr& ptr)
:CXMLCharData(ptr.GetInterfacePtr())
{

}
CXMLText::CXMLText(const CXMLNode& other)
{
	//use an empty node to init this text, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_TEXT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to text Node");
	}
}

CXMLText::~CXMLText()
{

}

CXMLText& CXMLText::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_TEXT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a text node");
	}

	return *this;
}
CXMLText::operator bool()const
{
	return (GetNodePtr().operator bool());
}

CXMLText CXMLText::SplitText(unsigned long ulOffset)
{
	ValidateState();
	return CXMLText(TCallMemberFunc1(GetTextPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMText::splitText,ulOffset));
}

MSXML2::IXMLDOMTextPtr CXMLText::GetTextPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMDocumentPtr>(this);
}

CXMLText::operator MSXML2::IXMLDOMTextPtr()const
{
	return GetTextPtr();
}

void CXMLText::ValidateState() const
{
	CXMLNode::ValidateState();
}
