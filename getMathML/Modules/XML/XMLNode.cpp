// XMLNode.cpp: implementation of the CXMLNode class.
//
//////////////////////////////////////////////////////////////////////
#include "XML.h"
#include "TFuncCalls.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning( push )
#pragma warning( disable : 4310)

CXMLNode::CXMLNode(const MSXML2::IXMLDOMNodePtr& ptr)
:m_ptrNode(ptr)
{
}

CXMLNode::~CXMLNode()
{

}

MSXML2::IXMLDOMNodePtr CXMLNode::GetNodePtr()const
{
	return m_ptrNode;
}

MSXML2::IXMLDOMNodePtr CXMLNode::SetNodePtr(const MSXML2::IXMLDOMNodePtr& ptr)
{
	m_ptrNode = ptr;
	return m_ptrNode;
}

wstring CXMLNode::GetNodeName()const
{
	ValidateState();

	return ConvertBSTRTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetnodeName));
}

wstring CXMLNode::GetNodeValue()
{
	ValidateState();

	return ConvertVARIANTTowstring(TCallMemberFunc0(
		m_ptrNode.GetInterfacePtr(),&MSXML2::IXMLDOMNode::GetnodeValue));
}

void CXMLNode::SetNodeValue(const wchar_t *cszValue)
{
	ValidateState();

	TCallVoidMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::PutnodeValue,cszValue);
}

CXMLNode::NodeType CXMLNode::GetNodeType()const
{
	ValidateState();

	return (NodeType)TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetnodeType);
}

void CXMLNode::ValidateState()const 
{
	//using a node that points to nothing to do something 
	//will produce exception
	if(m_ptrNode==NULL)
	{
		throw CXMLError(
			L"CXMLNode::ValidateState() NULL Pointer Detected");
	}
}

CXMLNode CXMLNode::GetParentNode() const
{
	ValidateState();

	return CXMLNode(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetparentNode));
}

CXMLNodeList CXMLNode::GetChildNodes()const
{
	ValidateState();

	return CXMLNodeList(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetchildNodes));
}

CXMLNode CXMLNode::GetFirstChild() const
{
	ValidateState();
	
	return CXMLNode(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetfirstChild));
}

CXMLNode CXMLNode::GetLastChild() const
{
	ValidateState();
	
	return CXMLNode(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetlastChild));
}

CXMLNode CXMLNode::GetPreviousSibling() const
{
	ValidateState();
	
	return CXMLNode(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetpreviousSibling));

}

CXMLNode CXMLNode::GetNextSibling() const
{
	ValidateState();
	
	return CXMLNode(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetnextSibling));
}

CXMLNodeMap CXMLNode::GetAttributes() const
{
	ValidateState();

	return CXMLNodeMap(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::Getattributes));
}

CXMLNode CXMLNode::InsertChildBefore(const CXMLNode &newNode, const CXMLNode& refNode)
{
	ValidateState();

	return CXMLNode(TCallMemberFunc2(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::insertBefore,newNode.GetNodePtr(),refNode.GetNodePtr().GetInterfacePtr()));
}

CXMLNode CXMLNode::ReplaceChild(const CXMLNode &newNode, const CXMLNode &oldNode)
{
	ValidateState();

	return CXMLNode(TCallMemberFunc2(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::replaceChild,
		newNode.GetNodePtr(),oldNode.GetNodePtr()));
}

CXMLNode CXMLNode::RemoveChild(const CXMLNode &oldNode)
{
	ValidateState();

	return CXMLNode(TCallMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::removeChild,oldNode.GetNodePtr()));
}

CXMLNode CXMLNode::AppendChild(const CXMLNode &newNode)
{
	ValidateState();

	return CXMLNode(TCallMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::appendChild,newNode.GetNodePtr()));
}

bool CXMLNode::HasChildNodes() const
{
	ValidateState();

	
	return (VARIANT_TRUE == TCallMemberFunc0(
		m_ptrNode.GetInterfacePtr(),&MSXML2::IXMLDOMNode::hasChildNodes));
}

CXMLTree CXMLNode::GetOwnerTree() const
{
	ValidateState();

	return CXMLTree(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetownerDocument));
}

CXMLNode CXMLNode::DuplicateNode(bool bDeep)const
{
	ValidateState();

	VARIANT_BOOL vb = (bDeep) ? VARIANT_TRUE : VARIANT_FALSE;
	return CXMLNode(TCallMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::cloneNode,vb));
}

wstring CXMLNode::GetNodeTypeString() const
{
	ValidateState();

	return ConvertBSTRTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetnodeTypeString));
}

std::string  CXMLNode::GetText() const
{
	ValidateState();
	return ((char*)TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::Gettext));
}

std::wstring CXMLNode::Getwchar_tText()const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::Gettext));	
}

void CXMLNode::SetText(const wchar_t *cszText)
{
	ValidateState();
	TCallVoidMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::Puttext,cszText);
}

bool CXMLNode::IsSpecified() const
{
	ValidateState();
	return (VARIANT_TRUE ==
		TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::Getspecified));
}

CXMLNode CXMLNode::GetDefinitionNode() const
{
	ValidateState();

	return CXMLNode(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::Getdefinition));
}

wstring CXMLNode::GetTypedValueInString() const
{
	ValidateState();

	return ConvertVARIANTTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetnodeTypedValue));
}

void CXMLNode::SetTypedValuebyString(const wchar_t *cszValue)
{
	ValidateState();

	TCallVoidMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::PutnodeTypedValue,cszValue);
}

wstring CXMLNode::GetDataTypeName() const
{
	ValidateState();

	return ConvertVARIANTTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetdataType));
}

void CXMLNode::SetDataTypeName(const wchar_t *cszType)
{
	ValidateState();
	TCallVoidMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::PutdataType,cszType);
}

wstring CXMLNode::GetXMLSrc() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::Getxml));
}

CXMLNodeList CXMLNode::GetNodes(const wchar_t *cszQuery) const
{
	ValidateState();
	return CXMLNodeList(TCallMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::selectNodes,cszQuery));
}

CXMLNode CXMLNode::GetNode(const wchar_t *cszQuery) const
{
	ValidateState();
	return CXMLNode(TCallMemberFunc1(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::selectSingleNode,cszQuery));
}


wstring CXMLNode::GetNamespaceURI() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetnamespaceURI));
}

wstring CXMLNode::GetNamespacePrefix() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::Getprefix));
}

wstring CXMLNode::GetBaseName() const
{
	ValidateState();
	return ConvertBSTRTowstring(TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),
		&MSXML2::IXMLDOMNode::GetbaseName));
}

CXMLNode::operator MSXML2::IXMLDOMNodePtr()const
{
	return m_ptrNode;
}

CXMLNode::operator bool()const
{
	return m_ptrNode.operator bool();
}

bool CXMLNode::IsAValidNode(NodeType t) const
{
	if(!m_ptrNode.operator bool())
	{
		return false;
	}

	if(t != GetNodeType())
	{
		return false;
	}

	//use smart pointer to test if we can query the specified
	//interface
	bool bRet = true;
	try
	{
		switch(t)
		{
		case NODE_INVALID:
			bRet = false;
			break;

		case NODE_ELEMENT:
			{
				MSXML2::IXMLDOMElementPtr ptr = m_ptrNode;
				bRet = ptr.operator bool();
			}
			break;

		case NODE_ATTRIBUTE:
			{
				MSXML2::IXMLDOMAttributePtr ptr = m_ptrNode;
				bRet = ptr.operator bool();
			}
			break;

		case NODE_TEXT:
			{
				MSXML2::IXMLDOMTextPtr ptr = m_ptrNode;
				bRet = ptr.operator bool();
			}
			break;

		case NODE_CDATA_SECTION:
			{
				MSXML2::IXMLDOMCDATASectionPtr ptr = m_ptrNode.GetInterfacePtr();
				bRet = ptr.operator bool();
			}
			break;

		case NODE_ENTITY_REFERENCE:
			{
				MSXML2::IXMLDOMEntityReferencePtr ptr = m_ptrNode.GetInterfacePtr();
				bRet = ptr.operator bool();
			}
			break;

		case NODE_ENTITY:
			{
				MSXML2::IXMLDOMEntityPtr ptr = m_ptrNode;
				bRet = ptr.operator bool();
			}
			break;

		case NODE_PROCESSING_INSTRUCTION:
			{
				MSXML2::IXMLDOMProcessingInstructionPtr ptr = m_ptrNode.GetInterfacePtr();
				bRet = ptr.operator bool();
			}
			break;

		case NODE_COMMENT:
			{
				MSXML2::IXMLDOMCommentPtr ptr = m_ptrNode;
				bRet = ptr.operator bool();
			}
			break;

		case NODE_DOCUMENT:
			{
				MSXML2::IXMLDOMDocumentPtr ptr = m_ptrNode;
				bRet = ptr.operator bool();
			}
			break;

		case NODE_DOCUMENT_TYPE:
			{
				MSXML2::IXMLDOMDocumentTypePtr ptr = m_ptrNode.GetInterfacePtr();
				bRet = ptr.operator bool();
			}
			break;

		case NODE_DOCUMENT_FRAGMENT:
			{
				MSXML2::IXMLDOMDocumentFragmentPtr ptr = m_ptrNode.GetInterfacePtr();
				bRet = ptr.operator bool();
			}
			break;

		case NODE_NOTATION:
			{
				MSXML2::IXMLDOMNotationPtr ptr = m_ptrNode;
				bRet = ptr.operator bool();
			}
			break;
		default:
			bRet = false;
		}
	}catch(_com_error e)
	{
		bRet = false;
	}

	return bRet;
}

_variant_t CXMLNode::GetTypedValue() const
{
	ValidateState();
	return TCallMemberFunc0(m_ptrNode.GetInterfacePtr(),&MSXML2::IXMLDOMNode::GetnodeTypedValue);
}

void CXMLNode::SetTypedValue(const _variant_t &v)
{
	ValidateState();
	TCallVoidMemberFunc1(m_ptrNode.GetInterfacePtr(),&MSXML2::IXMLDOMNode::PutnodeTypedValue,v);
}

#pragma warning( pop )