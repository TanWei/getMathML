// XMLTree.cpp: implementation of the CXMLTree class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"

#pragma warning(push)
#pragma warning(disable:4310)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXMLTree::CXMLTree(const MSXML2::IXMLDOMDocumentPtr& ptr)
:CXMLNode(ptr)
{

}
CXMLTree::CXMLTree(const CXMLNode& other)
{
	//use an empty node to init this tree, ok!
	if(!other.GetNodePtr().operator bool())
		return;

	//if the node can be converted, just copy it
	//if can not, we have to issue an error
	if(other.IsAValidNode(CXMLNode::NODE_DOCUMENT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		assert(false);
		throw CXMLError(L"CXMLTree::CopyConstructor-Failed to convert the node to Document Node");
	}
}
CXMLTree::~CXMLTree()
{

}
CXMLTree& CXMLTree::operator=(const CXMLNode& other)
{
	if(this == &other)
	{
		return *this;
	}

	if(!other.GetNodePtr().operator bool())
	{
		SetNodePtr(NULL);
		return *this;
	}

	if(other.IsAValidNode(CXMLNode::NODE_DOCUMENT))
	{
		SetNodePtr(other.GetNodePtr());
	}else
	{
		throw CXMLError(L"CXMLTree assign operator-Failed to convert the node to a document node");
	}

	return *this;
}

bool CXMLTree::Load(const wchar_t* cszSource)
{
	//do we need to create one object?
	if(!GetNodePtr().operator bool() && !Create())
	{
		throw CXMLError(L"CXMLTree::Load-Failed to create a XML Document Object");
	}

	//here we must clear the schemas,so the load process will
	//not use the schemas to validate the specified xml
	ClearSchemas();

	return (VARIANT_TRUE ==	TCallMemberFunc1(
		GetDocumentPtr().GetInterfacePtr(),&MSXML2::IXMLDOMDocument::load,cszSource));
}


bool CXMLTree::Save(const wchar_t* cszFileName, bool bIndentation /*= true*/)const
{
	try
	{
		ValidateState();

		if (bIndentation)
		{
			// Use xslt to indent the output xml
			MSXML2::IXMLDOMDocument2Ptr pXSLTIndenter;
			_bstr_t bstrXSLTIndenter = 
				L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
				L"<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">\n"
				L"<xsl:output method=\"xml\" version=\"1.0\" indent=\"yes\" encoding=\"utf-8\" omit-xml-declaration=\"yes\" />\n"
				L"<xsl:template match=\"@* | node()\">\n"
				L"	<xsl:copy>\n"
				L"		<xsl:apply-templates select=\"@* | node()\" />\n"
				L"	</xsl:copy>\n"
				L"</xsl:template>\n"
				L"</xsl:stylesheet>\n";

			if (FAILED(pXSLTIndenter.CreateInstance(__uuidof(DOMDocument60))))
				return false;
			if (!pXSLTIndenter->loadXML(bstrXSLTIndenter))
				return false;

			wstring wstrResult = ConvertBSTRTowstring(GetDocumentPtr().GetInterfacePtr()->transformNode(pXSLTIndenter));
			if (wstrResult.empty())
				return false;

			// Create another tree to write out the indented xml
			CXMLTree treeAnother;
			if (!treeAnother.Create())
				return false;

			if (!treeAnother.LoadXML(wstrResult.c_str()))
				return false;

			// Write process instruction
			CXMLElement elementRoot = treeAnother.GetRootElement();
			CXMLPI xmlPI = treeAnother.CreateProcessingInstruction(L"xml", L"version=\"1.0\" encoding=\"utf-8\"");
			treeAnother.InsertChildBefore(xmlPI, elementRoot);

			return (SUCCEEDED(TCallMemberFunc1(
				treeAnother.GetDocumentPtr().GetInterfacePtr(), &MSXML2::IXMLDOMDocument::save, cszFileName)));
		}
		else
		{
			return (SUCCEEDED(TCallMemberFunc1(
				GetDocumentPtr().GetInterfacePtr(), &MSXML2::IXMLDOMDocument::save, cszFileName)));
		}
	}
	catch (_com_error e)
	{
		return false;
	}
	catch (CXMLError e)
	{
		return false;
	}
}

wstring CXMLTree::Transform(const wchar_t* cszXSLTFile)
{
	ValidateState();

	try
	{
		MSXML2::IXMLDOMDocument2Ptr ptrXSLT;
		_bstr_t bstrResult = L"";
		if (FAILED(ptrXSLT.CreateInstance(__uuidof(DOMDocument60))))
			return L"";
		ptrXSLT->setProperty(_bstr_t(L"ValidateOnParse"), false);
		ptrXSLT->setProperty(_bstr_t(L"ResolveExternals"), true);
		ptrXSLT->setProperty(_bstr_t(L"AllowDocumentFunction"), true);
		if (!ptrXSLT->load(_variant_t(cszXSLTFile)))
			return L"";		

		return ConvertBSTRTowstring(GetDocumentPtr().GetInterfacePtr()->transformNode(ptrXSLT));

	}
	catch(_com_error e)
	{
		throw CXMLError(e);
		return L"";
	}
}

bool CXMLTree::Transform(const wchar_t* cszXSLTFile, const wchar_t* cszOutputXMLFile)
{
    wstring wstrTransformed = Transform(cszXSLTFile);
	if (wstrTransformed.empty())
		return false;

	try
	{
		CXMLTree treeAnother;
		if (!treeAnother.Create())
			return false;

		if (!treeAnother.LoadXML(wstrTransformed.c_str()))
			return false;

		CXMLElement elementRoot = treeAnother.GetRootElement();
		CXMLPI xmlPI = treeAnother.CreateProcessingInstruction(L"xml", L"version=\"1.0\" encoding=\"utf-8\"");
		treeAnother.InsertChildBefore(xmlPI, elementRoot);

		return treeAnother.Save(cszOutputXMLFile);
	}
	catch (_com_error e)
	{
		return false;
	}
}

CXMLElement CXMLTree::GetRootElement() const
{
	ValidateState();

	return CXMLElement(TCallMemberFunc0(
		GetDocumentPtr().GetInterfacePtr(),&MSXML2::IXMLDOMDocument::GetdocumentElement));
}

void CXMLTree::SetRootElement(const CXMLElement &newRoot)
{
	ValidateState();

	TCallVoidMemberFunc1(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::PutRefdocumentElement,newRoot.GetElementPtr());
}

CXMLElement CXMLTree::CreateElement(const wchar_t *cszTagName)const
{
	ValidateState();

	return CXMLElement(TCallMemberFunc1(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createElement,cszTagName));
}

CXMLText CXMLTree::CreateTextNode(const wchar_t* cszData) const
{
	ValidateState();
	return CXMLText(TCallMemberFunc1(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createTextNode,cszData));
}

CXMLComment CXMLTree::CreateComment(const wchar_t *cszData) const
{
	ValidateState();
	return CXMLComment(TCallMemberFunc1(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createComment,cszData));
}

CXMLCDataSection CXMLTree::CreateCDataSection(const wchar_t *cszData) const
{
	ValidateState();

	return CXMLCDataSection(TCallMemberFunc1(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createCDATASection,cszData));
}

CXMLPI CXMLTree::CreateProcessingInstruction(const wchar_t *cszTarget, const wchar_t *cszData) const
{
	ValidateState();
	return CXMLPI(TCallMemberFunc2(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createProcessingInstruction,
		cszTarget,cszData));
}

CXMLAttribute CXMLTree::CreateAttribute(const wchar_t *cszName)const
{
	ValidateState();
	return CXMLAttribute(TCallMemberFunc1(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createAttribute,cszName));
}

CXMLEntityRef CXMLTree::CreateEntityRef(const wchar_t *cszName) const
{
	ValidateState();
	return CXMLEntityRef(TCallMemberFunc1(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createEntityReference,cszName));
}


CXMLDocFrag CXMLTree::CreateDocFrag()const
{
	ValidateState();
	return CXMLDocFrag(TCallMemberFunc0(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createDocumentFragment));
}
CXMLNodeList CXMLTree::GetElementsByTagName(const wchar_t *cszTagName) const
{
	ValidateState();
	return CXMLNodeList(TCallMemberFunc1(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::getElementsByTagName,cszTagName));
}

CXMLNode CXMLTree::CreateNode(NodeType type, const wchar_t *cszName, const wchar_t *cszNamespaceURI) const
{
	ValidateState();
	
	return CXMLNode(TCallMemberFunc3(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::createNode,(long)type,cszName,cszNamespaceURI));
}

CXMLNode CXMLTree::GetNodeByID(const wchar_t *cszID) const
{
	ValidateState();
	return CXMLNode(TCallMemberFunc1(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::nodeFromID,cszID));
}


wstring CXMLTree::GetParseError() const
{
	ValidateState();
	
	//try to get error information by interface IXMLDOMParseError
	wstring strRet;
	try
	{
		MSXML2::IXMLDOMParseErrorPtr ptr = GetDocumentPtr()->GetparseError();
		if(ptr)
		{
			strRet = ConvertBSTRTowstring(ptr->Getreason());
		}else
		{
			strRet = L"Failed to get IXMLDOMParseError interface";
		}
	}catch(_com_error e)
	{
		strRet = ConvertBSTRTowstring(e.Description());
	}
	return strRet;
}

CXMLParseError CXMLTree::GetDetailedParseError()const
{
	ValidateState();
	return CXMLParseError(TCallMemberFunc0(
		GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::GetparseError));	
}

wstring CXMLTree::GetURL() const
{
	ValidateState();

	return ConvertBSTRTowstring(
		TCallMemberFunc0(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::Geturl));
}


bool CXMLTree::LoadXML(const wchar_t *cszXML)
{
	//do we need to create one object first?
	if(!GetNodePtr().operator bool() && !Create())
	{
		throw CXMLError(L"CXMLTree::Load-Failed to create a XML Document Object");
	}

	//here we must clear the schemas,so the load process will
	//not use the schemas to validate the specified xml
	ClearSchemas();

	return (VARIANT_TRUE ==
		TCallMemberFunc1(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::loadXML,cszXML));
}

bool CXMLTree::GetValidateOnParse() const
{
	ValidateState();

	return (VARIANT_TRUE ==
		TCallMemberFunc0(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::GetvalidateOnParse));
}

void CXMLTree::SetValidateOnParse(bool b)
{
	ValidateState();

	VARIANT_BOOL vb = (b) ? VARIANT_TRUE : VARIANT_FALSE;
	TCallVoidMemberFunc1(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::PutvalidateOnParse,vb);
}

bool CXMLTree::GetResolveExternals() const
{
	ValidateState();

	return (VARIANT_TRUE ==
		TCallMemberFunc0(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::GetresolveExternals));
}

void CXMLTree::SetResolveExternals(bool b)
{
	ValidateState();

	VARIANT_BOOL vb = (b) ? VARIANT_TRUE : VARIANT_FALSE;
	TCallVoidMemberFunc1(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::PutresolveExternals,vb);
}

bool CXMLTree::GetPreserveWhitespace() const
{
	ValidateState();

	return (VARIANT_TRUE ==
		TCallMemberFunc0(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::GetpreserveWhiteSpace));
}

void CXMLTree::SetPreserveWhitespace(bool b)
{
	ValidateState();

	VARIANT_BOOL vb = (b) ? VARIANT_TRUE : VARIANT_FALSE;
	TCallVoidMemberFunc1(GetDocumentPtr().GetInterfacePtr(),
		&MSXML2::IXMLDOMDocument::PutpreserveWhiteSpace,vb);
}

MSXML2::IXMLDOMDocumentPtr CXMLTree::GetDocumentPtr() const
{
	return TGetPtr<MSXML2::IXMLDOMDocumentPtr>(this);
}

CXMLTree::operator MSXML2::IXMLDOMDocumentPtr()const
{
	return GetDocumentPtr();
}

CXMLTree::operator bool()const
{
	//since we can assure that the pointer is ok by
	//copy constructor and assign operator, here
	//we just test whether it is null.
	return (GetNodePtr().operator bool());
}

CXMLDocType CXMLTree::GetDocType() const
{
	ValidateState();

	return CXMLDocType(TCallMemberFunc0(
		GetDocumentPtr().GetInterfacePtr(),&MSXML2::IXMLDOMDocument::Getdoctype));
}

bool CXMLTree::Create(bool bNewParser)
{
	// if we already have one reference, issue an error
	if(GetNodePtr())
	{
		throw CXMLError(L"CXMLTree::Create-Already Created");
	}


	//if failed to create, just return false 
	MSXML2::IXMLDOMDocumentPtr ptr;
	if(FAILED(ptr.CreateInstance(__uuidof(DOMDocument60))))
	{
		return false;
	}

	//store the reference in base class
	try
	{
		IXMLDOMDocument2Ptr ptr2 = ptr.GetInterfacePtr();
		if(ptr2 == NULL)
		{
			throw CXMLError(L"CXMLTree::Create-Failed to query IXMLDOMDocument2 interface");
		}
		
		if(FAILED(ptr2->setProperty(_bstr_t(L"SelectionLanguage"),_variant_t(L"XPath"))))
		{
			throw CXMLError(L"CXMLTree::Create-Failed to switch XPath support on");
		}

		if(FAILED(ptr2->setProperty(_bstr_t(L"NewParser"),bNewParser)))
		{
			throw CXMLError(L"CXMLTree::Create-Failed to set NewParser property");
		}

		if(FAILED(ptr2->setProperty(_bstr_t(L"ValidateOnParse"), false)))
		{
			throw CXMLError(L"CXMLTree::Create-Failed to set ValidateOnParse property");
		}

		if(FAILED(ptr2->setProperty(_bstr_t(L"ResolveExternals"), true)))
		{
			throw CXMLError(L"CXMLTree::Create-Failed to set ResolveExternals property");
		}

		if(FAILED(ptr2->setProperty(_bstr_t(L"ProhibitDTD"), false)))
		{
			throw CXMLError(L"CXMLTree::Create-Failed to set ProhibitDTD property");
		}

		if(FAILED(ptr->put_async(VARIANT_FALSE)))
		{
			throw CXMLError(L"CXMLTree::Create-Failed to switch async load off");
		}
		SetNodePtr(ptr);
			
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
	return true;
}

bool CXMLTree::Destroy()
{
	try
	{
		SetNodePtr(NULL);
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
	return true;
}

void CXMLTree::ValidateState() const
{
	//since we can assure that the pointer is ok by
	//copy constructor and assign operator, here
	//we just test whether it is null.
	CXMLNode::ValidateState();
}

IXMLDOMDocument2Ptr CXMLTree::GetDocument2Ptr() const
{
	IXMLDOMDocument2Ptr ptrDoc;
	try
	{
		ptrDoc = GetNodePtr().GetInterfacePtr();
	}catch(_com_error e)
	{
		//just catch is enough
	}
	return ptrDoc;
}

bool CXMLTree::ValidateAndLoad(const wchar_t *cszXMLFile, const wchar_t *cszSchemaFile)
{
	//if no schema is specified, we don't use schema, just load the file
	//and only use xml rules to validate it.
	if(cszSchemaFile==NULL)
	{
		return Load(cszXMLFile);
	}

	bool bRet = true;

	try
	{
		//first try to load schemas from schema file
		IXMLDOMDocument2Ptr ptrDocSchema(__uuidof(DOMDocument60));
		if(!ptrDocSchema.operator bool())
		{
			throw CXMLError(L"CXMLTree::ValidateAndLoad-Failed to create DOM");
		}
		ptrDocSchema->Putasync(VARIANT_FALSE);

		ptrDocSchema->setProperty(_bstr_t(L"NewParser"),true);

		ptrDocSchema->setProperty(_bstr_t(L"SelectionLanguage"),L"XPath");

		ptrDocSchema->setProperty(_bstr_t(L"ResolveExternals"), true);

		ptrDocSchema->setProperty(_bstr_t(L"ProhibitDTD"), false);

		if(ptrDocSchema->load(cszSchemaFile)==VARIANT_FALSE)
		{
			throw CXMLError(L"CXMLTree::ValidateAndLoad-Specified Schema File Not Valid!");
		}


		//get namespace from schema file
		_bstr_t bstrNS;
		if(MSXML2::IXMLDOMNodePtr ptrNode=ptrDocSchema->selectSingleNode(L"//*/@targetNamespace"))
		{
			bstrNS = ptrNode->Gettext();
		}else
		{
			bstrNS = L"";
		}
		
		//construct schema collection
		MSXML2::IXMLDOMSchemaCollection2Ptr ptrSchema(__uuidof(XMLSchemaCache60));
		if(!ptrSchema.operator bool())
		{
			throw CXMLError(L"CXMLTree::ValidateAndLoad-Failed to create Schema collection");
		}
		ptrSchema->add(bstrNS,ptrDocSchema.GetInterfacePtr());

		//do we need to create one object?
		if(!GetNodePtr().operator bool() && !Create())
		{
			throw CXMLError(L"CXMLTree::Load-Failed to create a XML Document Object");
		}

		GetDocument2Ptr()->setProperty(_bstr_t(L"MultipleErrorMessages"), VARIANT_TRUE);
		//set the schemas
		GetDocument2Ptr()->PutRefschemas(ptrSchema.GetInterfacePtr());
		//validate  while loading
		bRet = (VARIANT_TRUE==GetDocumentPtr()->load(cszXMLFile));

		//if validation success, we need to check if the
		//root element's namespace is the same with schema's
		//targetNamespace,because if the namespaces are not the
		//same, it's impossible to validate with schema constraints
		if(bRet)
		{
			if(CXMLElement root = GetRootElement())
			{
				_bstr_t bstrNS2 = root.GetElementPtr()->GetnamespaceURI();
				if(bstrNS2 != bstrNS)
				{
					//namespaces are not identical
					throw CXMLError(L"CXMLTree::ValidateAndLoad-the top level element of specified xml document"
						L"doesn't use the same namespace with the declared targetNamespace in schema file");
				}
			}else
			{
				//this should never happen, because if a document
				//has no root element, load should have returned
				//false
				throw CXMLError(L"CXMLTree::ValidateAndLoad-MSXML failed to report error for a "
					L"XML document without a top level element when parsing.");
			}
		}
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}

	
	return bRet;
}

void CXMLTree::ClearSchemas()
{
	if(IXMLDOMDocument2Ptr ptr=GetDocument2Ptr())
	{
		try
		{
			_variant_t vtSchemas = ptr->Getschemas();
			if(vtSchemas.vt == VT_EMPTY || vtSchemas.vt == VT_NULL)
			{
				return;
			}

			
			IXMLDOMSchemaCollectionPtr ptrSchemas = (IUnknown*)vtSchemas;

			if(! ptrSchemas.operator bool())
			{
				return;
			}

			//remove in reverse order,so that we don't need 
			//to deal with changing index
			long lLength = ptrSchemas->Getlength();
			for(long i=lLength-1;i>=0;i--)
			{
				_bstr_t bstrNS = ptrSchemas->GetnamespaceURI(i);
				ptrSchemas->remove(bstrNS);
			}
		}catch(_com_error)
		{
			//just catch is enough
		}
	}
}
void CXMLTree::SetXPathNamespace(const wchar_t* cszPrefix,const wchar_t* cszNS)
{
	ValidateState();
	IXMLDOMDocument2Ptr ptr = GetDocument2Ptr();
	if(!ptr.operator bool())
	{
		throw CXMLError(L"CXMLTree::SetXPathNamespace-Failed to get IXMLDocuement2 Interface!");
	}

	if(cszPrefix==NULL || cszNS==NULL)
	{
		throw CXMLError(L"CXMLTree::SetXPathNameSpace-Invalid auguments!");
	}
	wstring strNS = L"xmlns:";
	strNS += cszPrefix;
	strNS += L"='";
	strNS += cszNS;
	strNS += L"'";
	try
	{
		ptr->setProperty(L"SelectionNamespaces",strNS.c_str());
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
}

void CXMLTree::AddXPathNamespace(const wchar_t *cszPrefix, const wchar_t *cszNS)
{
	ValidateState();
	IXMLDOMDocument2Ptr ptr = GetDocument2Ptr();
	if(!ptr.operator bool())
	{
		throw CXMLError(L"CXMLTree::AddXPathNamespace-Failed to get IXMLDocuement2 Interface!");
	}

	if(cszPrefix==NULL || cszNS==NULL)
	{
		throw CXMLError(L"CXMLTree::AddXPathNamespace-Invalid auguments!");
	}

	try
	{
		std::wstring strNS = ConvertVARIANTTowstring(ptr->getProperty("SelectionNamespaces"));
		strNS += L" xmlns:";
		strNS += cszPrefix;
		strNS += L"='";
		strNS += cszNS;
		strNS += L"'";

		ptr->setProperty(L"SelectionNamespaces",strNS.c_str());
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
}

void CXMLTree::ClearXPathNamespace()
{
	ValidateState();
	IXMLDOMDocument2Ptr ptr = GetDocument2Ptr();
	if(!ptr.operator bool())
	{
		throw CXMLError(L"CXMLTree::ClearXPathNamespace-Failed to get IXMLDocuement2 Interface!");
	}

	try
	{
		ptr->setProperty(L"SelectionNamespaces",L"");
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
}

void CXMLTree::SetXPathNamespace(const wchar_t *cszNS)
{
	ValidateState();
	IXMLDOMDocument2Ptr ptr = GetDocument2Ptr();
	if(!ptr.operator bool())
	{
		throw CXMLError(L"CXMLTree::SetXPathNamespace-Failed to get IXMLDocuement2 Interface!");
	}

	if(cszNS==NULL)
	{
		throw CXMLError(L"CXMLTree::SetXPathNameSpace-Invalid auguments!");
	}
	try
	{
		ptr->setProperty(L"SelectionNamespaces",cszNS);
	}catch(_com_error e)
	{
		throw CXMLError(e);
	}
}

CXMLParseError CXMLTree::Validate()const
{
	ValidateState();

	return CXMLParseError(TCallMemberFunc0(GetDocument2Ptr().GetInterfacePtr(),&IXMLDOMDocument2::validate));
}

MSXML2::IXMLDOMDocument3Ptr CXMLTree::GetDocument3Ptr() const
{
	return TGetPtr<MSXML2::IXMLDOMDocument3Ptr>(this);
}

CXMLParseError CXMLTree::ValidateNode( CXMLNode xmlNode ) const
{
	ValidateState();

	return CXMLParseError(TCallMemberFunc1(GetDocument3Ptr().GetInterfacePtr(),&IXMLDOMDocument3::validateNode, xmlNode.GetNodePtr()));
}

#pragma warning(pop)
