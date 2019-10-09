// XMLError.h: interface for the CXMLError class.

// CXMLError: exception type of XML class library
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLERROR_H__E9F7D6E0_0F56_40D5_ADEC_27DEEAF19242__INCLUDED_)
#define AFX_XMLERROR_H__E9F7D6E0_0F56_40D5_ADEC_27DEEAF19242__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>

#pragma warning( push,3 )
#pragma warning(disable : 4018)
#include <string>
#pragma warning( pop)

using std::wstring;

class CXMLError  
{
public:
	CXMLError(const _com_error& e);
	CXMLError(const wchar_t* cszDesc=NULL);
	virtual ~CXMLError();

	//Get Error Description
	const wchar_t* GetDescription()const;

private:
	wstring m_strDesc;
};

#endif // !defined(AFX_XMLERROR_H__E9F7D6E0_0F56_40D5_ADEC_27DEEAF19242__INCLUDED_)
