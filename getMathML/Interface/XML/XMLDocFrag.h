// XMLDocFrag.h: interface for the CXMLDocFrag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLDOCFRAG_H__4D4AF4EB_63DE_49A6_A645_5C1F05D10A95__INCLUDED_)
#define AFX_XMLDOCFRAG_H__4D4AF4EB_63DE_49A6_A645_5C1F05D10A95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLDocFrag : public CXMLNode  
{
public:
	CXMLDocFrag(const MSXML2::IXMLDOMDocumentFragmentPtr& ptr=NULL);
	CXMLDocFrag(const CXMLNode& other);
	virtual ~CXMLDocFrag();
	
	/*convert a node to Document Fragment,if the conversion
	can not be completed, an exception occurs.*/
	CXMLDocFrag& operator=(const CXMLNode& other);

	//get interface pointer
	MSXML2::IXMLDOMDocumentFragmentPtr GetDocFragPtr()const;
	//convert this object to interface pointer
	operator MSXML2::IXMLDOMDocumentFragmentPtr()const;

	//is in valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLDOCFRAG_H__4D4AF4EB_63DE_49A6_A645_5C1F05D10A95__INCLUDED_)
