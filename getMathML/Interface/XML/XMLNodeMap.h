// XMLNodeMap.h: interface for the CXMLNodeMap class.

//CXMLNodeMap: Node collection.can be accessed by name
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLNODEMAP_H__6D0895C6_EDF7_47FA_A696_74E8B0AE8E6E__INCLUDED_)
#define AFX_XMLNODEMAP_H__6D0895C6_EDF7_47FA_A696_74E8B0AE8E6E__INCLUDED_

#include "XMLNode.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import <msxml6.dll> named_guids
using namespace MSXML2;

class CXMLNodeMap  
{
public:
	CXMLNodeMap(const MSXML2::IXMLDOMNamedNodeMapPtr& ptr=NULL);
	virtual ~CXMLNodeMap();

	//get length of the map
	unsigned long GetLength()const;
	//get a node by index
	CXMLNode operator [](unsigned long ulIndex) const;

	//get a node by name
	CXMLNode GetItemByName(const wchar_t *cszName) const;
	//remove an item by name
	CXMLNode RemoveItemByName(const wchar_t* cszName);
	//set item by name
	CXMLNode SetNamedItem(const CXMLNode& newNode);

	//remove an item by it's full qualified name
	CXMLNode RemoveQualifiedItem(const wchar_t* cszBaseName,const wchar_t* cszURI);
	//get an item by it's full qualified name
	CXMLNode GetQualifiedItem(const wchar_t* cszBaseName,const wchar_t* cszURI)const;

	//Returns the next node in the collection.
	CXMLNode NextNode()const;
	//reset the position of iterator
	void Reset()const;

	//access the interface pointer
	MSXML2::IXMLDOMNamedNodeMapPtr GetMapPtr()const;
	//you can convert it to IXMLDOMNamedNodeMapPtr
	operator MSXML2::IXMLDOMNamedNodeMapPtr() const;

	//test if the object is in a valid state
	operator bool() const;
private:
	MSXML2::IXMLDOMNamedNodeMapPtr m_ptrMap;
protected:
	void ValidateState()const;
};

#endif // !defined(AFX_XMLNODEMAP_H__6D0895C6_EDF7_47FA_A696_74E8B0AE8E6E__INCLUDED_)
