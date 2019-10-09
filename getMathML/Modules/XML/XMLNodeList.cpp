// XMLNodeList.cpp: implementation of the CXMLNodeList class.

//CXMLNodeList: Node List read-only collection
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLNodeList::CXMLNodeList(const MSXML2::IXMLDOMNodeListPtr& ptr)
:m_ptrList(ptr)
{
}

CXMLNodeList::~CXMLNodeList()
{

}

CXMLNode CXMLNodeList::operator[](unsigned long ulIndex)const
{
	ValidateState();

	return CXMLNode(TCallMemberFunc1(
		m_ptrList.GetInterfacePtr(),&MSXML2::IXMLDOMNodeList::Getitem,ulIndex));
}

unsigned long CXMLNodeList::GetLength() const
{
	ValidateState();

	long lLength = TCallMemberFunc0(
		m_ptrList.GetInterfacePtr(),&MSXML2::IXMLDOMNodeList::Getlength);

	return ((lLength<0) ? 0 : lLength);
}

CXMLNode CXMLNodeList::NextNode() const
{
	ValidateState();

	return CXMLNode(TCallMemberFunc0(
		m_ptrList.GetInterfacePtr(),&MSXML2::IXMLDOMNodeList::nextNode));
}

void CXMLNodeList::Reset() const
{
	ValidateState();

	TCallMemberFunc0(m_ptrList.GetInterfacePtr(),
		&MSXML2::IXMLDOMNodeList::reset);
}

MSXML2::IXMLDOMNodeListPtr CXMLNodeList::GetListPtr() const
{
	return m_ptrList;
}
CXMLNodeList::operator MSXML2::IXMLDOMNodeListPtr()const
{
	return m_ptrList;
}

CXMLNodeList::operator bool()const
{
	return m_ptrList.operator bool();
}

void CXMLNodeList::ValidateState() const
{
	if(! m_ptrList.operator bool())
	{
		throw CXMLError(
			L"CXMLNodeList::ValidateState() NULL Pointer Detected");
	}
}
