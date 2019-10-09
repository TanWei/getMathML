// XMLNodeList.h: interface for the CXMLNodeList class.

//CXMLNodeList: Node List read-only collection 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLNODELIST_H__706ADF97_45F0_4784_82F0_F9DE152BB2D6__INCLUDED_)
#define AFX_XMLNODELIST_H__706ADF97_45F0_4784_82F0_F9DE152BB2D6__INCLUDED_

#include "XMLNode.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import <msxml6.dll> named_guids
using namespace MSXML2;
class CXMLNode;

class CXMLNodeList  
{
public:
	CXMLNodeList(const MSXML2::IXMLDOMNodeListPtr& ptr=NULL);
	virtual ~CXMLNodeList();

	//get a node by index, start from 0
	CXMLNode operator [](unsigned long ulIndex)const;
	//get length of the list
	unsigned long GetLength()const;

	//Returns the next node in the collection.
	CXMLNode NextNode()const;
	//reset the position of iterator
	void Reset()const;

	//access the raw com interface
	MSXML2::IXMLDOMNodeListPtr GetListPtr()const;
	//you can convert it to IXMLDOMNodeListPtr
	operator MSXML2::IXMLDOMNodeListPtr()const;
	//test if this list is valid
	operator bool()const;
private:
	MSXML2::IXMLDOMNodeListPtr m_ptrList;
protected:
	void ValidateState()const;
};

#endif // !defined(AFX_XMLNODELIST_H__706ADF97_45F0_4784_82F0_F9DE152BB2D6__INCLUDED_)
