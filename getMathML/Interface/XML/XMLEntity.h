// XMLEntity.h: interface for the CXMLEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLENTITY_H__11404A95_9B49_483F_87ED_5933DFB96AEF__INCLUDED_)
#define AFX_XMLENTITY_H__11404A95_9B49_483F_87ED_5933DFB96AEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLEntity : public CXMLNode  
{
public:
	CXMLEntity(const  MSXML2::IXMLDOMEntityPtr& ptr=NULL);
	CXMLEntity(const CXMLNode& other);
	virtual ~CXMLEntity();

	/*convert a node to Entity.
	if the conversion can not be completed, an exception
	occurs.*/
	CXMLEntity& operator=(const CXMLNode& other);

	//get the public id
	wstring GetPublicID()const;
	//get the system id
	wstring GetSystemID()const;
	//get the notation name
	wstring GetNotationName()const;

	//get interface ptr
	 MSXML2::IXMLDOMEntityPtr GetEntityPtr()const;
	//convert this object to a interface pointer
	operator  MSXML2::IXMLDOMEntityPtr()const;

	//is in valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLENTITY_H__11404A95_9B49_483F_87ED_5933DFB96AEF__INCLUDED_)
