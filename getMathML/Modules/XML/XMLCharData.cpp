// XMLCharData.cpp: implementation of the CXMLCharData class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLCharData::CXMLCharData(const MSXML2::IXMLDOMCharacterDataPtr& ptr)
:CXMLNode(ptr.GetInterfacePtr())
{

}

CXMLCharData::~CXMLCharData()
{

}

wstring CXMLCharData::GetData() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(
		GetCharDataPtr().GetInterfacePtr(),&MSXML2::IXMLDOMCharacterData::Getdata));
}

void CXMLCharData::SetData(const wchar_t *cszData)
{
	ValidateState();

	TCallVoidMemberFunc1(GetCharDataPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMCharacterData::Putdata,cszData);
}

unsigned long CXMLCharData::GetLength() const
{
	ValidateState();
	long lLength = TCallMemberFunc0(GetCharDataPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMCharacterData::Getlength);

	return ((lLength<0) ? 0 : lLength);
}

wstring CXMLCharData::GetSubString(unsigned long ulOffset, unsigned long ulCount) const
{
	ValidateState();

	return ConvertBSTRTowstring(TCallMemberFunc2(GetCharDataPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMCharacterData::substringData,ulOffset,ulCount));
}

void CXMLCharData::AppendData(const wchar_t *cszData)
{
	ValidateState();

	TCallMemberFunc1(GetCharDataPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMCharacterData::appendData,cszData);
}

void CXMLCharData::InsertData(unsigned long ulOffset, const wchar_t *cszData)
{
	ValidateState();
	TCallMemberFunc2(GetCharDataPtr().GetInterfacePtr(),&MSXML2::IXMLDOMCharacterData::insertData,ulOffset,cszData);
}

void CXMLCharData::DeleteData(unsigned long ulOffset, unsigned long ulCount)
{
	ValidateState();
	TCallMemberFunc2(GetCharDataPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMCharacterData::deleteData,ulOffset,ulCount);
}

void CXMLCharData::ReplaceData(unsigned long ulOffset, unsigned long ulCount, const wchar_t *cszData)
{
	ValidateState();
	TCallMemberFunc3(GetCharDataPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMCharacterData::replaceData,ulOffset,ulCount,cszData);
}

MSXML2::IXMLDOMCharacterDataPtr CXMLCharData::GetCharDataPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMCharacterDataPtr>(this);
}

void CXMLCharData::ValidateState() const
{
	CXMLNode::ValidateState();
}
