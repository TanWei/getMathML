// XMLTree.h: interface for the CXMLTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLTREE_H__558945E8_738B_4662_A99E_DF17754B6CBD__INCLUDED_)
#define AFX_XMLTREE_H__558945E8_738B_4662_A99E_DF17754B6CBD__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XMLNode.h"
#include "XMLDocFrag.h"
#include "XMLAttribute.h"	// Added by ClassView
#include "XMLPI.h"	// Added by ClassView
#include "XMLCDataSection.h"	// Added by ClassView
#include "XMLText.h"	// Added by ClassView
#include "XMLElement.h"	// Added by ClassView
#include "XMLComment.h"	// Added by ClassView
#include "XMLEntityRef.h"	// Added by ClassView
#include "XMLNodeList.h"	// Added by ClassView
#include "XMLDocType.h"	// Added by ClassView
#include "XMLParseError.h"



class CXMLTree:public CXMLNode 
{
public:
	CXMLTree(const MSXML2::IXMLDOMDocumentPtr& ptr=NULL);
	CXMLTree(const CXMLNode& other);
	virtual ~CXMLTree();

	CXMLTree& operator=(const CXMLNode& other);

	//create the xml tree(XML document)
	bool Create(bool bNewParser=true);
	//destroy the created xml tree
	bool Destroy();

	//load the specified XML file,create a tree if necessary
	bool Load(const wchar_t* cszSource);
	//save the document to a specified destination, set bIndentation to true to indent the output XML
	bool Save(const wchar_t* cszDest, bool bIndentation = true)const;
	//load the document from a wstring,create a tree if necessary
	bool LoadXML(const wchar_t* cszXML);
	//validate the xml file using specified schema while loading
	bool ValidateAndLoad(const wchar_t* cszXMLFile, const wchar_t* cszSchemaFile=NULL);
	//transform using an xslt file, returning the result string
	wstring Transform(const wchar_t* cszXSLTFile);
	//transform using an xslt file, saving to the output XML file
	bool Transform(const wchar_t* cszXSLTFile, const wchar_t* cszOutputXMLFile);

	//use DTD or schema to validate current xml
	CXMLParseError Validate()const;
	//use schema to validate Element or Attribute Node
	CXMLParseError ValidateNode(CXMLNode xmlNode)const;
	//get the root element
	CXMLElement GetRootElement()const;
	//set the root element
	void SetRootElement(const CXMLElement& newRoot);

	//get the parse error information,if load or loadXML return false
	wstring GetParseError()const;
	//get the detail information of parse error,including error position
	//and the text source of error
	CXMLParseError GetDetailedParseError()const;

	//get url of last loaded document
	wstring GetURL()const;
	//get document type node
	CXMLDocType GetDocType()const;

	//create a new element with tagname
	CXMLElement CreateElement(const wchar_t* cszTagName)const;
	//create a text node
	CXMLText CreateTextNode(const wchar_t* cszData)const;
	//create a comment node
	CXMLComment CreateComment(const wchar_t* cszData)const;
	//create a CDATA section
	CXMLCDataSection CreateCDataSection(const wchar_t* cszData)const;
	//create an entity reference
	CXMLEntityRef CreateEntityRef(const wchar_t* cszName)const;
	//create an attribute 
	CXMLAttribute CreateAttribute(const wchar_t* cszName)const;
	//create a PI
	CXMLPI CreateProcessingInstruction(const wchar_t* cszTarget,const wchar_t* cszData)const;
	//create a document fragment
	CXMLDocFrag CreateDocFrag()const;
	//create a node of the specified node type and name
	CXMLNode CreateNode(NodeType type,const wchar_t* cszName,const wchar_t* cszNamespaceURI)const;

	//get the node by ID
	CXMLNode GetNodeByID(const wchar_t* cszID)const;
	//build a list of elements by name
	CXMLNodeList GetElementsByTagName(const wchar_t* cszTagName)const;


	//get whether to validate when parsing
	bool GetValidateOnParse()const;
	//set whether to validate when parsing
	void SetValidateOnParse(bool b);
	//get whether to resolve externals when parsing
	bool GetResolveExternals()const;
	//set whether to resolve externals when parsing
	void SetResolveExternals(bool b);
	//Set whether to preserve whitespace when parsing
	void SetPreserveWhitespace(bool b);
	//get whether to preserve whitespace when parsing
	bool GetPreserveWhitespace()const;
	//set the XPath query namespace and prefix,so that you can use
	//XPath with namespace
	void SetXPathNamespace(const wchar_t* cszPrefix,const wchar_t* cszNS);
	//"xmlns:example1='http://myserver.com' xmlns:example2='http://yourserver.com'"
	void SetXPathNamespace(const wchar_t* cszNS);
	//add a new namespace declaration that can be used in xpath
	void AddXPathNamespace(const wchar_t* cszPrefix,const wchar_t* cszNS);
	//clear all added namespaces that are used in xpath
	void ClearXPathNamespace();


	//get interface pointer
	MSXML2::IXMLDOMDocumentPtr GetDocumentPtr()const;
	//get interface pointer
	MSXML2::IXMLDOMDocument3Ptr GetDocument3Ptr() const;
	//convert this object to interface pointe
	operator MSXML2::IXMLDOMDocumentPtr()const;
	//is in valid state?
	operator bool()const;
protected:
	//clear all schema constraints;
	void ClearSchemas();
	//get IXMLDOMDocument2 Interface pointer
	MSXML2::IXMLDOMDocument2Ptr GetDocument2Ptr()const;
	virtual void ValidateState()const;
};

#endif // !defined(AFX_XMLTREE_H__558945E8_738B_4662_A99E_DF17754B6CBD__INCLUDED_)
