// XMLCharData.h: interface for the CXMLCharData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLCHARDATA_H__47F4E0F9_1421_48B8_A070_41808A88706F__INCLUDED_)
#define AFX_XMLCHARDATA_H__47F4E0F9_1421_48B8_A070_41808A88706F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"

class CXMLCharData : public CXMLNode  
{
protected:
	virtual void ValidateState()const;
	//you can not create a CXMLCharData Object
	CXMLCharData(const MSXML2::IXMLDOMCharacterDataPtr& ptr=NULL);
	virtual ~CXMLCharData();
public:

	//get value of the node
	wstring GetData()const;
	//set value of the node
	void SetData(const wchar_t* cszData);

	//get length of the data
	unsigned long GetLength()const;

	//get substring
	wstring GetSubString(unsigned long ulOffset,unsigned long ulCount)const;

	//append data to the current data
	void AppendData(const wchar_t* cszData);
	//insert data into current data
	void InsertData(unsigned long ulOffset,const wchar_t* cszData);
	//delete substring from current data
	void DeleteData(unsigned long ulOffset, unsigned long ulCount);
	//replace a substring
	void ReplaceData(unsigned long ulOffset, unsigned long ulCount, const wchar_t* cszData);

	//get the interface pointer
	MSXML2::IXMLDOMCharacterDataPtr GetCharDataPtr()const;
};

#endif // !defined(AFX_XMLCHARDATA_H__47F4E0F9_1421_48B8_A070_41808A88706F__INCLUDED_)
