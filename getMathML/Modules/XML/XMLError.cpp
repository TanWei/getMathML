// XMLError.cpp: implementation of the CXMLError class.

// CXMLError: exception type of XML class library
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLError::CXMLError(const wchar_t* cszDesc)
:m_strDesc(cszDesc)
{

}

CXMLError::~CXMLError()
{

}

CXMLError::CXMLError(const _com_error &e)
{
	//use a _bstr_t to init wstring may cause exception
	try
	{
		m_strDesc = ConvertBSTRTowstring(e.Description());
		//if failed to get IErrorInfo::GetDescription, we use
		//HRESULT return code to format message
		if(m_strDesc.empty())
		{
			const wchar_t* p = e.ErrorMessage();
			m_strDesc = ((p==NULL) ? L"" : p);
		}
	}catch(...)
	{
		m_strDesc = L"CXMLError::CXMLError(const _com_error &e)-Serious Error occured when trying to get error description";
	}
	
}

const wchar_t* CXMLError::GetDescription() const
{
	return m_strDesc.c_str();
}

