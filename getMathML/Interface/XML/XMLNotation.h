// XMLNotation.h: interface for the CXMLNotation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLNOTATION_H__B7C4D7ED_C86C_426E_B291_9E15BF9FF514__INCLUDED_)
#define AFX_XMLNOTATION_H__B7C4D7ED_C86C_426E_B291_9E15BF9FF514__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLNotation : public CXMLNode  
{
public:
	CXMLNotation(const  MSXML2::IXMLDOMNotationPtr& ptr=NULL);
	CXMLNotation(const CXMLNode& other);
	virtual ~CXMLNotation();

	/*convert a node to notation,if the conversion
	can not be completed, an exception occurs.*/
	CXMLNotation& operator=(const CXMLNode& other);

	//the public identifier for the notation
	wstring GetPublicID()const;
	//the system identifier for the notation
	wstring GetSystemID()const;

	//get interface pointer
	 MSXML2::IXMLDOMNotationPtr GetNotationPtr()const;
	//convert this object to interface pointer
	operator  MSXML2::IXMLDOMNotationPtr()const;
	//is in valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLNOTATION_H__B7C4D7ED_C86C_426E_B291_9E15BF9FF514__INCLUDED_)
