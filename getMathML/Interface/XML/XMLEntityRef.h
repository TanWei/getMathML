// XMLEntityRef.h: interface for the CXMLEntityRef class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLENTITYREF_H__499A6253_0316_483C_9BF3_1A1FC8643622__INCLUDED_)
#define AFX_XMLENTITYREF_H__499A6253_0316_483C_9BF3_1A1FC8643622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLEntityRef : public CXMLNode  
{
public:
	CXMLEntityRef(const MSXML2::IXMLDOMEntityReferencePtr& ptr=NULL);
	CXMLEntityRef(const CXMLNode& other);
	virtual ~CXMLEntityRef();

	/*convert a node to Entity Reference.
	if the conversion can not be completed, an exception
	occurs.*/
	CXMLEntityRef& operator=(const CXMLNode& other);

	//get interface pointer
	MSXML2::IXMLDOMEntityReferencePtr GetEntityRefPtr()const;
	//convert this object to interface pointer
	operator MSXML2::IXMLDOMEntityReferencePtr()const;
	//is in valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLENTITYREF_H__499A6253_0316_483C_9BF3_1A1FC8643622__INCLUDED_)
