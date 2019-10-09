// XMLParseError.cpp: implementation of the CXMLParseError class.
//
//////////////////////////////////////////////////////////////////////

#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLParseError::CXMLParseError(const MSXML2::IXMLDOMParseError2Ptr& ptr)
:m_ptrError(ptr.GetInterfacePtr())
{

}

CXMLParseError::~CXMLParseError()
{

}
CXMLParseError::operator bool()const
{
	//if we have not a reference, it's not a valid parse error object
	if(! m_ptrError.operator bool())
	{
		return false;
	}

	//0 means we have no error
	MSXML2::IXMLDOMParseErrorPtr ptrError = m_ptrError;
 	return (0 != TCallMemberFunc0(ptrError.GetInterfacePtr(),
 		&MSXML2::IXMLDOMParseError::GeterrorCode));
}

long CXMLParseError::GetErrorCode() const
{
	ValidateState();

	MSXML2::IXMLDOMParseErrorPtr ptrError = m_ptrError;
 	return TCallMemberFunc0(ptrError.GetInterfacePtr(),
 		&MSXML2::IXMLDOMParseError::GeterrorCode);
}

long CXMLParseError::GetFilePos()
{
	ValidateState();

	MSXML2::IXMLDOMParseErrorPtr ptrError = m_ptrError;
 	return TCallMemberFunc0(ptrError.GetInterfacePtr(),
 		&MSXML2::IXMLDOMParseError::Getfilepos);
}

void CXMLParseError::ValidateState() const
{
	if(! m_ptrError.operator bool())
	{
		throw CXMLError(L"CXMLParseError::ValidateState()-Null pointer detected");
	}
}

long CXMLParseError::GetLine() const
{
	ValidateState();

	MSXML2::IXMLDOMParseErrorPtr ptrError = m_ptrError;
 	return TCallMemberFunc0(ptrError.GetInterfacePtr(),
 		&MSXML2::IXMLDOMParseError::Getline);
}

long CXMLParseError::GetCol() const
{
	ValidateState();

	MSXML2::IXMLDOMParseErrorPtr ptrError = m_ptrError;
 	return TCallMemberFunc0(ptrError.GetInterfacePtr(),
 		&MSXML2::IXMLDOMParseError::Getlinepos);
}

wstring CXMLParseError::GetReason() const
{
	ValidateState();

	MSXML2::IXMLDOMParseErrorPtr ptrError = m_ptrError;
 	return ConvertBSTRTowstring(TCallMemberFunc0(
 		ptrError.GetInterfacePtr(),&MSXML2::IXMLDOMParseError::Getreason));
}

wstring CXMLParseError::GetLineText() const
{
	ValidateState();

	MSXML2::IXMLDOMParseErrorPtr ptrError = m_ptrError;
 	return ConvertBSTRTowstring(TCallMemberFunc0(
 		ptrError.GetInterfacePtr(),&MSXML2::IXMLDOMParseError::GetsrcText));
}

wstring CXMLParseError::GetURL() const
{
	ValidateState();

	MSXML2::IXMLDOMParseErrorPtr ptrError = m_ptrError;
 	return ConvertBSTRTowstring(TCallMemberFunc0(
 		ptrError.GetInterfacePtr(),&MSXML2::IXMLDOMParseError::Geturl));
}

MSXML2::IXMLDOMParseErrorCollectionPtr CXMLParseError::GetallErrors()
{
	ValidateState();

	return  TCallMemberFunc0(m_ptrError.GetInterfacePtr(),&MSXML2::IXMLDOMParseError2::GetallErrors);
}

wstring CXMLParseError::GetXPath() const
{
	ValidateState();

	return  ConvertBSTRTowstring(TCallMemberFunc0(
		m_ptrError.GetInterfacePtr(),&MSXML2::IXMLDOMParseError2::GeterrorXPath));
}
