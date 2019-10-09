// XMLComment.h: interface for the CXMLComment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLCOMMENT_H__9F6098F8_979A_4A51_ABE3_32766CBE9C43__INCLUDED_)
#define AFX_XMLCOMMENT_H__9F6098F8_979A_4A51_ABE3_32766CBE9C43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLCharData.h"

class CXMLComment : public CXMLCharData  
{
public:
	CXMLComment(const MSXML2::IXMLDOMCommentPtr& ptr=NULL);
	CXMLComment(const CXMLNode& other);
	virtual ~CXMLComment();

	/*convert a node to comment, if the conversion can 
	 not be completed, an exception occurs*/
	CXMLComment& operator=(const CXMLNode& other);

	//get interface pointer
	MSXML2::IXMLDOMCommentPtr GetCommentPtr()const;

	//convert this object to interface pointer
	operator MSXML2::IXMLDOMCommentPtr()const;
	//is in a valid state?
	operator bool()const;
protected:
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLCOMMENT_H__9F6098F8_979A_4A51_ABE3_32766CBE9C43__INCLUDED_)
