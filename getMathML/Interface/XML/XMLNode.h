// XMLNode.h: interface for the CXMLNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLNODE_H__034E87D6_FCE2_4C5F_AE2F_E3B7A259B745__INCLUDED_)
#define AFX_XMLNODE_H__034E87D6_FCE2_4C5F_AE2F_E3B7A259B745__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import <msxml6.dll> named_guids

#include "XMLError.h"

using namespace MSXML2;

class CXMLTree;
class CXMLNodeList;
class CXMLNodeMap;

class CXMLNode  
{
public:
	CXMLNode(const MSXML2::IXMLDOMNodePtr& ptr=NULL);
	virtual ~CXMLNode();
	
	//Node Type
	typedef enum tagNodeType
	{
		NODE_INVALID = MSXML2::NODE_INVALID,
		NODE_ELEMENT = MSXML2::NODE_ELEMENT,
        NODE_ATTRIBUTE = MSXML2::NODE_ATTRIBUTE,
        NODE_TEXT = MSXML2::NODE_TEXT,
        NODE_CDATA_SECTION = MSXML2::NODE_CDATA_SECTION,
        NODE_ENTITY_REFERENCE = MSXML2::NODE_ENTITY_REFERENCE,
        NODE_ENTITY = MSXML2::NODE_ENTITY,
        NODE_PROCESSING_INSTRUCTION = MSXML2::NODE_PROCESSING_INSTRUCTION,
        NODE_COMMENT = MSXML2::NODE_COMMENT,
        NODE_DOCUMENT = MSXML2::NODE_DOCUMENT,
        NODE_DOCUMENT_TYPE = MSXML2::NODE_DOCUMENT_TYPE,
        NODE_DOCUMENT_FRAGMENT = MSXML2::NODE_DOCUMENT_FRAGMENT,
        NODE_NOTATION = MSXML2::NODE_NOTATION
	} NodeType;

	//test if this node can conver to another type
	bool IsAValidNode(NodeType t)const;

	//get node name
	wstring GetNodeName()const;
	//get node type
	NodeType GetNodeType()const;
	//get wstring representation of node type
	wstring GetNodeTypeString()const;
	/*Indicates whether the node is explicitly specified 
	or derived from a default value */
	bool IsSpecified()const;
	//get the definition or schema
	CXMLNode GetDefinitionNode()const;
	//get this node and it's descendants' xml source
	wstring GetXMLSrc()const;

	//get namespace uri
	wstring GetNamespaceURI()const;
	//get namespace prefix
	wstring GetNamespacePrefix()const;
	//get basename(without prefix)
	wstring GetBaseName()const;

	//get the owner tree of this node
	CXMLTree GetOwnerTree()const;

	//has this node any children?
	bool HasChildNodes()const;

	//get parent node
	CXMLNode GetParentNode()const;
	//get child nodes
	CXMLNodeList GetChildNodes()const;
	//get first child node
	CXMLNode GetFirstChild()const;
	//get last child node
	CXMLNode GetLastChild()const;
	//get previous sibling
	CXMLNode GetPreviousSibling()const;
	//get next sibling
	CXMLNode GetNextSibling()const;
	//get attributes
	CXMLNodeMap GetAttributes()const;

	//get nodes by wstring query
	CXMLNodeList GetNodes(const wchar_t* cszQuery)const;
	//get the first matched node with the query
	CXMLNode GetNode(const wchar_t* cszQuery)const;

	//get node value
	wstring GetNodeValue();
	//set node value
	void SetNodeValue(const wchar_t* cszValue);
	//get text
	std::string  GetText()const;

	std::wstring Getwchar_tText()const;
	//set text
	void SetText(const wchar_t* cszText);

	//get the node value expressed in wstring
	wstring GetTypedValueInString()const;
	//set the node value by a wstring
	void SetTypedValuebyString(const wchar_t* cszValue);
	//get the node value in defined type
	_variant_t GetTypedValue()const;
	//set the node value in defined type
	void SetTypedValue(const _variant_t& v);

	//get the data type of the node
	wstring GetDataTypeName()const;
	//set the data type of the node
	void SetDataTypeName(const wchar_t* cszType);


	//insert a child node before the specified child node
	CXMLNode InsertChildBefore(const CXMLNode& newNode,const CXMLNode& refNode);
	//replace a child node with a new node
	CXMLNode ReplaceChild(const CXMLNode& newNode, const CXMLNode& oldNode);
	//remove an old child
	CXMLNode RemoveChild(const CXMLNode& oldNode);
	//append a new child
	CXMLNode AppendChild(const CXMLNode& newNode);
	//Duplicate this node, if bDeep true, Duplicate the whole subtree
	CXMLNode DuplicateNode(bool bDeep=false)const;



	//get XML com object's Interface pointer
	MSXML2::IXMLDOMNodePtr GetNodePtr()const;
	//you can convert this object to IXMLDOMNodePtr
	operator MSXML2::IXMLDOMNodePtr()const;
	//test if this object is valid
	operator bool()const;
protected:
	//set XML com object's Interface pointer
	MSXML2::IXMLDOMNodePtr SetNodePtr(const MSXML2::IXMLDOMNodePtr& ptr);
	//validate the pointer
	virtual void ValidateState()const;
private:
	//pointer to XML com object
	MSXML2::IXMLDOMNodePtr m_ptrNode;

};

#endif // !defined(AFX_XMLNODE_H__034E87D6_FCE2_4C5F_AE2F_E3B7A259B745__INCLUDED_)
