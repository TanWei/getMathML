// XMLPI.cpp: implementation of the CXMLPI class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLPI::CXMLPI(const MSXML2::IXMLDOMProcessingInstructionPtr& ptr)
:CXMLNode(ptr.GetInterfacePtr())
{

}

CXMLPI::CXMLPI(const CXMLNode& other)
{
	//use an empty node to init this PI, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_PROCESSING_INSTRUCTION))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to PI Node");
	}
}

CXMLPI::~CXMLPI()
{

}
CXMLPI& CXMLPI::operator=(const CXMLNode& other)
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

	if(other.IsAValidNode(CXMLNode::NODE_PROCESSING_INSTRUCTION))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a PI node");
	}

	return *this;
}
wstring CXMLPI::GetTarget() const
{
	ValidateState();

	return ConvertBSTRTowstring(TCallMemberFunc0(GetPIPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMProcessingInstruction::Gettarget));
}

wstring CXMLPI::GetData() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(GetPIPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMProcessingInstruction::Getdata));
}

void CXMLPI::SetData(const wchar_t *cszData)
{
	ValidateState();

	TCallVoidMemberFunc1(GetPIPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMProcessingInstruction::Putdata,cszData);
}

MSXML2::IXMLDOMProcessingInstructionPtr CXMLPI::GetPIPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMProcessingInstructionPtr>(this);
}
CXMLPI::operator MSXML2::IXMLDOMProcessingInstructionPtr()const
{
	return GetPIPtr();
}
CXMLPI::operator bool()const
{
	return (GetNodePtr().operator bool());
}

void CXMLPI::ValidateState() const
{
	//since we can assure that the pointer is ok by
	//copy constructor and assign operator, here
	//we just test whether it is null.
	CXMLNode::ValidateState();
}
