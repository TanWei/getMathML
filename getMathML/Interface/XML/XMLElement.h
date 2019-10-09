// XMLElement.h: interface for the CXMLElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLELEMENT_H__E1354FD8_706D_4B99_8C16_549568833641__INCLUDED_)
#define AFX_XMLELEMENT_H__E1354FD8_706D_4B99_8C16_549568833641__INCLUDED_

#include "XMLNodeList.h"	// Added by ClassView
#include "XMLAttribute.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLElement : public CXMLNode  
{
public:
	CXMLElement(const MSXML2::IXMLDOMElementPtr& ptr=NULL);
	CXMLElement(const CXMLNode& other);
	virtual ~CXMLElement();

	/* convert a node to Element,if the node doesn's reference an
	element,an exception will be throwed*/
	CXMLElement& operator=(const CXMLNode& other);

	//get element name
	wstring GetTagName()const;

	//is have the attribute node by name
	bool HaveAttribute(const wchar_t* cszName) const;

	//look up the wstring value of an attribute by name
	wstring GetAttributeValueByName(const wchar_t* cszName)const;
	//set the wstring value of an attribute by name
	void SetAttributeValueByName(const wchar_t* cszName,const wchar_t* cszValue);

	//remove an attribute or replace it with default value if it does have one
	void RemoveAttributeByName(const wchar_t* cszName);

	//look up the attribute node by name
	CXMLAttribute GetAttributeNodeByName(const wchar_t* cszName)const;
	//set the specified attribute on the element, if an old attribute with
	//the same name exists,return a reference to the old one.
	CXMLAttribute SetAttributeNode(const CXMLAttribute& newNode);
	//remove the specified attribute
	CXMLAttribute RemoveAttributeNode(const CXMLAttribute& oldNode);

	//build a list of elements by name
	CXMLNodeList GetElementsByTagName(const wchar_t* cszTagName)const;

	//Normalizes all descendant elements by combining two or more 
	//adjacent text nodes into one unified text node
	void Normalize();

	//get interface pointer
	MSXML2::IXMLDOMElementPtr GetElementPtr()const;
	//convert this object to interface pointer
	operator MSXML2::IXMLDOMElementPtr()const;

	//is in valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLELEMENT_H__E1354FD8_706D_4B99_8C16_549568833641__INCLUDED_)
