// XMLDocType.h: interface for the CXMLDocType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLDOCTYPE_H__47CB4FCB_CE78_4864_9773_2BBFC85E73F9__INCLUDED_)
#define AFX_XMLDOCTYPE_H__47CB4FCB_CE78_4864_9773_2BBFC85E73F9__INCLUDED_

#include "XMLNodeMap.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLDocType : public CXMLNode  
{
public:
	CXMLDocType(const MSXML2::IXMLDOMDocumentTypePtr& ptr=NULL);
	CXMLDocType(const CXMLNode& other);
	virtual ~CXMLDocType();

	/*convert a node to Document Type, if the conversion
	can not be completed, an exception occurs.*/
	CXMLDocType& operator=(const CXMLNode& other);

	//get name of the document type
	wstring GetName()const;
	//a list of the entities declared in the DOCTYPE declaration
	CXMLNodeMap GetEntities()const;
	//a list of the notations in the document type declaration
	CXMLNodeMap GetNotations()const;

	//get interface pointer
	MSXML2::IXMLDOMDocumentTypePtr GetDocTypePtr()const;
	//convert this object to interface pointer
	operator MSXML2::IXMLDOMDocumentTypePtr()const;

	//is in valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLDOCTYPE_H__47CB4FCB_CE78_4864_9773_2BBFC85E73F9__INCLUDED_)
