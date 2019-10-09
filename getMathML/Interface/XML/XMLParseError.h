// XMLParseError.h: interface for the CXMLParseError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLPARSEERROR_H__75EDF25B_BD82_422D_813C_E090D4725E52__INCLUDED_)
#define AFX_XMLPARSEERROR_H__75EDF25B_BD82_422D_813C_E090D4725E52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XMLError.h"

class CXMLParseError  
{
public:
	CXMLParseError(const MSXML2::IXMLDOMParseError2Ptr& ptr=NULL);
	virtual ~CXMLParseError();

	//is there a error?
	operator bool()const;

	//get error code
	long GetErrorCode()const;
	//absolute error position in file
	long GetFilePos();
	//error line
	long GetLine()const;
	//error col in error line
	long GetCol()const;
	//reason for error
	wstring GetReason()const;
	//get text of the line
	wstring GetLineText()const;
	//get url of the document containing the error
	wstring GetURL()const;
	//get xPath
	wstring GetXPath() const;
	//get  a collection of all errors and warnings found during validation
	MSXML2::IXMLDOMParseErrorCollectionPtr GetallErrors();
private:
	MSXML2::IXMLDOMParseError2Ptr m_ptrError;
protected:
	void ValidateState()const;
};

#endif // !defined(AFX_XMLPARSEERROR_H__75EDF25B_BD82_422D_813C_E090D4725E52__INCLUDED_)
