// XMLPI.h: interface for the CXMLPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPI_H__153D9174_944D_47F4_B473_6BC6ADB604C8__INCLUDED_)
#define AFX_XMLPI_H__153D9174_944D_47F4_B473_6BC6ADB604C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLPI : public CXMLNode  
{
public:
	CXMLPI(const MSXML2::IXMLDOMProcessingInstructionPtr& ptr=NULL);
	CXMLPI(const CXMLNode& other);
	virtual ~CXMLPI();

	/*convert a node to PI,if the conversion can not 
	be completed, an exception occurs.*/
	CXMLPI& operator=(const CXMLNode& other);

	//get target of PI
	wstring GetTarget()const;
	//get content of PI
	wstring GetData()const;
	//set content of PI
	void SetData(const wchar_t* cszData);

	//get interface pointer
	MSXML2::IXMLDOMProcessingInstructionPtr GetPIPtr()const;
	//convert this object to interface pointer
	operator MSXML2::IXMLDOMProcessingInstructionPtr()const;
	//is in valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLPI_H__153D9174_944D_47F4_B473_6BC6ADB604C8__INCLUDED_)
