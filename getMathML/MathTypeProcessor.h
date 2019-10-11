#pragma once

#include <string>
#include <vector>
#include "XML/XML.H"

namespace MTSDKDN
{
	enum KMathTypeReturnValue
	{
		mtOK = 0,					// no error
		mtNOT_FOUND = -1,			// trouble finding the MT erver, usually indicates a bad session ID
		mtCANT_RUN = -2,				// could not start the MT server
		mtBAD_VERSION = -3,			// server / DLL version mismatch
		mtIN_USE = -4,				// server is busy/in-use
		mtNOT_RUNNING = -5,			// server aborted
		mtRUN_TIMEOUT = -6,			// connection to the server failed due to time-out 
		mtNOT_EQUATION = -7,			// an API call that expects an equation could not find one
		mtFILE_NOT_FOUND = -8,		// a preference, translator or other file could not be found
		mtMEMORY = -9,				// a buffer too small to hold the result of an API call was passed in
		mtBAD_FILE = -10,			// file found was not a translator
		mtDATA_NOT_FOUND = -11,		// unable to read preferences from MTEF on the clipboard
		mtTOO_MANY_SESSIONS = -12,	// too many open connections to the SDK
		mtSUBSTITUTION_ERROR = -13,	// problem with substition error during a call to MTXFormEqn
		mtTRANSLATOR_ERROR = -14,	// there was an error in compiling or in execution of a translator 
		mtPREFERENCE_ERROR = -15,	// could not set preferences
		mtBAD_PATH = -16,			// a bad path was encountered when trying to write to a file
		mtFILE_ACCESS = -17,			// a file could not be written to
		mtFILE_WRITE_ERROR = -18,	// a file could not be written to
		mtBAD_DATA = -19,			// (deprecated)
		mtERROR = -9999,				// other error
	};

	enum KMTApiStartValues
	{
		mtinitLAUNCH_AS_NEEDED = 0,	// launch MathType when first needed
		mtinitLAUNCH_NOW = 1,		// launch MathType server immediately
	};

	enum KMTXFormSetTranslator
	{
		// options values for MTXFormSetTranslator
		mtxfmTRANSL_INC_NONE = 0,		// include neither the translator name nor equation data in output
		mtxfmTRANSL_INC_NAME = 1,		// include the translator's name in translator output
		mtxfmTRANSL_INC_DATA = 2,		// include MathType equation data in translator output
		mtxfmTRANSL_INC_MTDEFAULT = 4,	// use defaults for translator name and equation data

		// append 'Z' to text equations placed on clipboard
		// kludge to fix Word's trailing CRLF truncation bug
		mtxfmTRANSL_INC_CLIPBOARD_EXTRA = 8,
	};

	enum KMTXFormEqnDataLocale
	{
		// see comment for MTXFormEqnMgn below, for a description of the following constants

		// data sources/destinations for MTXFormEqn
		mtxfmPREVIOUS = -1,
		mtxfmCLIPBOARD = -2,
		mtxfmLOCAL = -3,
		mtxfmFILE = -4,
	};

	enum KMTXFormEqnDataFormats
	{
		// data formats for MTXFormEqn
		mtxfmMTEF = 4,
		mtxfmHMTEF = 5,
		mtxfmPICT = 6,
		mtxfmTEXT = 7,
		mtxfmHTEXT = 8,
		mtxfmGIF = 9,
		mtxfmEPS_NONE = 10,
		mtxfmEPS_WMF = 11,
		mtxfmEPS_TIFF = 12,
	};
}

struct MTAPI_RECT
{
	long _left;
	long _top;
	long _right;
	long _bottom;
};

struct MTAPI_DIMS
{
	int baseline;
	MTAPI_RECT bounds;
};

//struct MTProcessorParam
//{
//	std::string m_;
//	MTProcessorParam()
//	{
//		Clear();
//	}
//	void Clear()
//	{
//		m_ParseMTFileName.clear();
//		//m_DesMTFileName.clear();
//	}
//};
class CXMLTree;
class CMathTypeHelper;
class CMathTypeProcessor
{
public:
	CMathTypeProcessor(void);
	virtual ~CMathTypeProcessor(void);
	std::wstring ConvertToXml(const std::wstring& fn);
private:
	bool GetMathTypeCharacterStylePrefs(const CMathTypeHelper& mathTypeHelper, std::map<std::string, std::string>& map_name_to_style);
	std::string GetStrCharacterStyle(BYTE byte_val);
	bool ProcessStyleXml(const std::map<std::string, std::string>& map_name_to_style, CXMLTree& xmlStyleTree);
	bool GetMathmlStyleXml(const CMathTypeHelper& mathTypeHelper, CXMLTree& xmlStyleTree);
	bool BuildDataStyleMap(const CXMLNodeList& dataNodes, const CXMLTree& xmlStyleTree, 
		std::map<wchar_t, std::vector<std::wstring>>& text_key_to_style_arr);
	bool ApplyStyleToDataXML(std::map<wchar_t, std::vector<std::wstring>>& text_key_to_style_arr, CXMLNodeList& dataNodes);

	bool Extract(const wchar_t * wcsArchiveFileName, const wchar_t * wcsOutPath);
	std::wstring s2ws(const std::string& str);
	std::string ws2s(const std::wstring& wstr);
	std::string GetMathTypePath();
	bool ConvertMTEFtoXmlByTdl(const CMathTypeHelper& mathTypeHelper, const std::string& tdl_name, CXMLTree& xmlTree);
	MTSDKDN::KMathTypeReturnValue ConvertMTEFtoXmlStringByTdl(const std::vector<BYTE>& mtef_byte_data, const std::string& tdl_name, std::string& xml_string);
	bool CreateMathmlFFxTdl(const std::string& mathml2_ffx_tdl);
	void WriteMathmlFFxTdlLine(std::ofstream& fout);
	std::string CreateMathmlFFxTdlLine(const std::string& source, const std::string& base_str);
	std::string FindReplaceString(const std::string& source, const std::string& findstr, const std::string& replacestr);
	void BuildStyleArr(wchar_t text_key, const CXMLTree& xmlStyleTree, std::vector<std::wstring>& style_arr);
private:
	std::string mathtype_path_;
	HMODULE m_hMT6Dll;
};

