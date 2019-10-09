// XMLCDataSection.h: interface for the CXMLCDataSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLCDATASECTION_H__3EB0712C_F59A_496A_BFC9_88B5FA01CAAA__INCLUDED_)
#define AFX_XMLCDATASECTION_H__3EB0712C_F59A_496A_BFC9_88B5FA01CAAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLText.h"

class CXMLCDataSection : public CXMLText  
{
public:
	CXMLCDataSection(const MSXML2::IXMLDOMCDATASectionPtr& ptr=NULL);
	CXMLCDataSection(const CXMLNode& other);
	virtual ~CXMLCDataSection();

	/*assign a CDATASection Node to this object
	if the specified node can not be converted to this object,
	an exception occurs
	*/
	CXMLCDataSection& operator=(const CXMLNode& other);

	//get interface pointer
	MSXML2::IXMLDOMCDATASectionPtr GetCDATASectionPtr()const;

	//convert this object to interface pointer
	operator MSXML2::IXMLDOMCDATASectionPtr()const;

	//is this object in a valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLCDATASECTION_H__3EB0712C_F59A_496A_BFC9_88B5FA01CAAA__INCLUDED_)
