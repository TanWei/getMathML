// XMLText.h: interface for the CXMLText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLTEXT_H__8B4D3BB1_F637_4FD5_AF3E_10FB72EF3505__INCLUDED_)
#define AFX_XMLTEXT_H__8B4D3BB1_F637_4FD5_AF3E_10FB72EF3505__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLCharData.h"

class CXMLText : public CXMLCharData  
{
public:
	CXMLText(const MSXML2::IXMLDOMTextPtr& ptr=NULL);
	CXMLText(const CXMLNode& other);
	virtual ~CXMLText();

	/* convert a node to text node, if the conversion can
	not be completed, an exception occurs.*/
	CXMLText& operator=(const CXMLNode& other);

	//split the text node into two text nodes at the 
	//position specified and return the second node
	CXMLText SplitText(unsigned long ulOffset);

	//get interface pointer
	MSXML2::IXMLDOMTextPtr GetTextPtr()const;
	//convert this object to interface pointer
	operator MSXML2::IXMLDOMTextPtr()const;
	//is in valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLTEXT_H__8B4D3BB1_F637_4FD5_AF3E_10FB72EF3505__INCLUDED_)
