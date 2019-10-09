// XMLAttribute.h: interface for the CXMLAttribute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLATTRIBUTE_H__136B0A98_3D69_466D_AADA_135C755527AA__INCLUDED_)
#define AFX_XMLATTRIBUTE_H__136B0A98_3D69_466D_AADA_135C755527AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLAttribute : public CXMLNode  
{
public:
	CXMLAttribute(const MSXML2::IXMLDOMAttributePtr& ptr=NULL);
	CXMLAttribute(const CXMLNode& other);
	virtual ~CXMLAttribute();

	/*assign a attribute node to this object
	if the specified node can not be converted to this object,
	an exception occurs
	*/
	CXMLAttribute& operator=(const CXMLNode& other);

	//get name of the attribute
	wstring GetName()const;

	//get wstring value of the attribute
	wstring GetValue()const;
	//set wstring value of the attribute
	void SetValue(const wchar_t* cszValue);

	//get interface pointer
	MSXML2::IXMLDOMAttributePtr GetAttributePtr()const;

	//convert this object to interface pointer
	operator MSXML2::IXMLDOMAttributePtr()const;

	//is this object in a valid state
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLATTRIBUTE_H__136B0A98_3D69_466D_AADA_135C755527AA__INCLUDED_)
