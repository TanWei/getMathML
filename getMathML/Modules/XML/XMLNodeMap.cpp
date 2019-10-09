// XMLNodeMap.cpp: implementation of the CXMLNodeMap class.
//CXMLNodeMap: Node collection. can be accessed by name
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLNodeMap::CXMLNodeMap(const MSXML2::IXMLDOMNamedNodeMapPtr& ptr)
:m_ptrMap(ptr)
{

}

CXMLNodeMap::~CXMLNodeMap()
{

}

unsigned long CXMLNodeMap::GetLength() const
{
	ValidateState();

	long lLength = TCallMemberFunc0(
		m_ptrMap.GetInterfacePtr(),&MSXML2::IXMLDOMNamedNodeMap::Getlength);

	return ((lLength<0) ? 0 : lLength);
}

CXMLNode CXMLNodeMap::operator [](unsigned long ulIndex) const
{
	ValidateState();

	return CXMLNode(TCallMemberFunc1(m_ptrMap.GetInterfacePtr(),
		&MSXML2::IXMLDOMNamedNodeMap::Getitem,ulIndex));
}

CXMLNode CXMLNodeMap::GetItemByName(const wchar_t *cszName) const
{
	ValidateState();
	return CXMLNode(TCallMemberFunc1(m_ptrMap.GetInterfacePtr(),
		&MSXML2::IXMLDOMNamedNodeMap::getNamedItem,cszName));
}

CXMLNode CXMLNodeMap::RemoveItemByName(const wchar_t *cszName)
{
	ValidateState();
	return CXMLNode(TCallMemberFunc1(m_ptrMap.GetInterfacePtr(),
		&MSXML2::IXMLDOMNamedNodeMap::removeNamedItem,cszName));
}

CXMLNode CXMLNodeMap::RemoveQualifiedItem(const wchar_t *cszBaseName, const wchar_t *cszURI)
{
	ValidateState();
	return CXMLNode(TCallMemberFunc2(m_ptrMap.GetInterfacePtr(),
		&MSXML2::IXMLDOMNamedNodeMap::removeQualifiedItem,cszBaseName,cszURI));
}

CXMLNode CXMLNodeMap::GetQualifiedItem(const wchar_t *cszBaseName, const wchar_t *cszURI) const
{
	ValidateState();
	return CXMLNode(TCallMemberFunc2(m_ptrMap.GetInterfacePtr(),
		&MSXML2::IXMLDOMNamedNodeMap::getQualifiedItem,cszBaseName,cszURI));
}

CXMLNode CXMLNodeMap::SetNamedItem(const CXMLNode &newNode)
{
	ValidateState();
	return CXMLNode(TCallMemberFunc1(m_ptrMap.GetInterfacePtr(),
		&MSXML2::IXMLDOMNamedNodeMap::setNamedItem,newNode.GetNodePtr()));
}

CXMLNode CXMLNodeMap::NextNode() const
{
	ValidateState();
	return CXMLNode(TCallMemberFunc0(m_ptrMap.GetInterfacePtr(),
		&MSXML2::IXMLDOMNamedNodeMap::nextNode));
}

void CXMLNodeMap::Reset() const
{
	ValidateState();
	TCallMemberFunc0(m_ptrMap.GetInterfacePtr(),
		&MSXML2::IXMLDOMNamedNodeMap::reset);
}

MSXML2::IXMLDOMNamedNodeMapPtr CXMLNodeMap::GetMapPtr() const
{
	return m_ptrMap;
}

CXMLNodeMap::operator MSXML2::IXMLDOMNamedNodeMapPtr() const
{
	return m_ptrMap;
}

CXMLNodeMap::operator bool() const
{
	return m_ptrMap.operator bool();
}

void CXMLNodeMap::ValidateState() const
{
	if(! m_ptrMap.operator bool())
	{
		throw CXMLError(L"CXMLNodeMap::ValidateState() NULL Pointer Detected");
	}
}
