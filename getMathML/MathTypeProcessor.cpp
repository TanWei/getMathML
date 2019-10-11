#include "StdAfx.h"
#include "MathTypeProcessor.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <iterator>
#include <algorithm>
#include <iomanip>

#include <locale>
#include <codecvt>
#include <string>
#include <set>

#include "MathTypeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MTSDKDN;

static const int MTAPICONNECT_TIME_OUT = 30;
static const std::string MATHML2_FFX_TDL = "MathML2 (FFx).tdl";
static const std::string MATHML2_MNAMESPACE_TDL = "MathML2 (m namespace).tdl";
static const std::string MATHML2_NAMESPACEATTR_TDL = "MathML2 (namespace attr).tdl";
static const int CHAR_STYLE_NUM = 12;
static const std::string CHAR_STYLE_NAMES[] = {"text","function","variable","lc_greek","uc_greek","symbol","vector","number","user1style","user2style","extra_math"};
static const std::vector<std::string> CHAR_STYLE_NAME_ARR(CHAR_STYLE_NAMES, CHAR_STYLE_NAMES+CHAR_STYLE_NUM-1);
static const std::string TEXT_FE = "text_fe";

typedef KMathTypeReturnValue (WINAPI *MTAPIConnectFunc)(int,int);
typedef KMathTypeReturnValue (WINAPI *MTAPIDisconnectFunc)();
typedef KMathTypeReturnValue (WINAPI *MTXFormResetFunc)();
typedef KMathTypeReturnValue (WINAPI *MTXFormSetTranslatorFunc)(int, const char*);
typedef KMathTypeReturnValue (WINAPI *MTXFormEqnFunc)(int, int, const BYTE*, long, int, int, char*, long, char*, MTAPI_DIMS&);

std::string CMathTypeProcessor::GetMathTypePath()
{
	HKEY hKEY;
	std::string strRegPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MathType.exe"; 
	if (::RegOpenKeyExA(HKEY_LOCAL_MACHINE,strRegPath.c_str(), 0, KEY_READ, &hKEY)!=ERROR_SUCCESS)
	{
		return "";
	}

	DWORD dwType = REG_SZ;
	BYTE byWordConvPath[1024];
	ZeroMemory(byWordConvPath,1024);
	DWORD dwSize = 1024;
	if (::RegQueryValueExA(hKEY, "Path", NULL, &dwType, byWordConvPath, &dwSize)!=ERROR_SUCCESS)
	{
		return "";
	}
	::RegCloseKey(hKEY); 

	return (char*)(byWordConvPath);
}

bool CMathTypeProcessor::Extract(const wchar_t * wcsArchiveFileName, const wchar_t * wcsOutPath)
{
	if((wcslen(wcsArchiveFileName) > 256) || (wcslen(wcsOutPath) > 256))
	{
		return false;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW|STARTF_FORCEOFFFEEDBACK;
	si.wShowWindow = SW_HIDE;
	ZeroMemory( &pi, sizeof(pi) );

	wchar_t pwszCommandLine[1024];
	wmemset(pwszCommandLine,0,1024);
	std::wstring str7zaPath = gtl::GetCombinedPathName(gtl::GetModuleDirectoryPath(NULL).c_str(), L"7za.exe");
	swprintf_s(pwszCommandLine,L"\"%s\" x \"%s\" -o\"%s\" -y",str7zaPath.c_str(),wcsArchiveFileName,wcsOutPath);

	if(!::CreateProcess(NULL,pwszCommandLine,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	{
		return false;
	}
	WaitForSingleObject( pi.hProcess, INFINITE);
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
	return gtl::IsFileExist(wcsOutPath);
}

std::string CMathTypeProcessor::FindReplaceString(const std::string& source, const std::string& findstr, const std::string& replacestr)
{
	std::string ret_str(source);
	int pos;
	pos = ret_str.find(findstr);////查找指定的串
	while (pos != -1)
	{
		ret_str.replace(pos, findstr.length(), replacestr);////用新的串替换掉指定的串
		pos = ret_str.find(findstr);//////继续查找指定的串，直到所有的都找到为止
	}
	return ret_str;
}

std::string CMathTypeProcessor::CreateMathmlFFxTdlLine(const std::string& source, const std::string& base_str)
{
	std::string ffx_command(source);
	ffx_command = FindReplaceString(ffx_command, "char_style", base_str);
	ffx_command = FindReplaceString(ffx_command, "run_name", base_str+"_style");
	return ffx_command;
}

void CMathTypeProcessor::WriteMathmlFFxTdlLine(std::ofstream& fout)
{
	const std::string ffx_mathml_tdl_head = "MathType Output Translator 1.0: \"MathML 2.0 (FFx)\", \"MathML 2.0 (FFx)\"";
	fout << ffx_mathml_tdl_head << std::endl;
	//trans char_style(plain, bold, italic, or bold_italic)
	std::string ffx_run_command = "run/char_style/name/\"run_name\" = \"#\";";
	std::string ffx_char_command = "char/0x0000/0xFFFF/\"run_name\" = \"<mtext mathtypevariant='char_style'>&$#x(CharHex);</mtext>\";";
	fout << CreateMathmlFFxTdlLine(ffx_run_command, "plain") << std::endl 
		 << CreateMathmlFFxTdlLine(ffx_char_command, "plain") << std::endl;
	fout << CreateMathmlFFxTdlLine(ffx_run_command, "bold") << std::endl 
		 << CreateMathmlFFxTdlLine(ffx_char_command, "bold") << std::endl;
	fout << CreateMathmlFFxTdlLine(ffx_run_command, "italic")	<< std::endl 
		 << CreateMathmlFFxTdlLine(ffx_char_command, "italic") << std::endl;
	fout << CreateMathmlFFxTdlLine(ffx_run_command, "bold_italic") << std::endl 
		 << CreateMathmlFFxTdlLine(ffx_char_command, "bold_italic") << std::endl;
	//trans style(text, function, variable, greek, lc_greek, uc_greek, symbol, vector, number, extra_math, user1style, or user2style)
	for (int i=0; i<CHAR_STYLE_NAME_ARR.size(); i++)
	{
		fout << CreateMathmlFFxTdlLine(ffx_run_command, CHAR_STYLE_NAME_ARR[i]) << std::endl 
			<< CreateMathmlFFxTdlLine(ffx_char_command, CHAR_STYLE_NAME_ARR[i]) << std::endl;
	}
}

bool CMathTypeProcessor::CreateMathmlFFxTdl(const std::string& mathml2_ffx_tdl)
{
	bool bRet = false;

	std::string mathml2_ffx_tdl_full = mathtype_path_ + "\\Translators\\" + mathml2_ffx_tdl;
	std::ifstream fin_ffx_tdl(mathml2_ffx_tdl_full, std::ios::in);
	if (fin_ffx_tdl.is_open())
	{
		fin_ffx_tdl.clear();
		fin_ffx_tdl.close();
		//return bRet=true; //添加版本控制后放开
	}
	std::string mathml2_mnamespace_tdl_full = mathtype_path_ + "\\Translators\\" + MATHML2_NAMESPACEATTR_TDL;
	std::ifstream fin(mathml2_mnamespace_tdl_full, std::ios::in);
	std::ofstream fout(mathml2_ffx_tdl_full, std::ios::out);
	if (!fin.is_open() || !fout.is_open())
	{
		return bRet;
	}
	bool flag = true;
	const std::string namespace_mathml_tdl_head = "MathType Output Translator 1.0";
	std::string file_line;
	while(std::getline(fin, file_line))
	{
		if (flag && file_line.find(namespace_mathml_tdl_head) != -1)
		{
			WriteMathmlFFxTdlLine(fout);
		}
		else
		{
			fout << file_line << std::endl;
		}
	}


	fin.clear();
	fin.close();
	fout.clear();
	fout.close();

	return bRet = true;
}

CMathTypeProcessor::CMathTypeProcessor(void)
{
	mathtype_path_ = GetMathTypePath();
	std::string MT6_dll_path_ = mathtype_path_ + "\\System\\MT6.dll";
	m_hMT6Dll = LoadLibraryA(MT6_dll_path_.c_str());
}

CMathTypeProcessor::~CMathTypeProcessor(void)
{
	if (m_hMT6Dll != NULL)
	{
		FreeLibrary(m_hMT6Dll);
	}
}

bool CMathTypeProcessor::ConvertMTEFtoXmlByTdl(const CMathTypeHelper& mathTypeHelper, const std::string& tdl_name, CXMLTree& xmlTree)
{
	bool bRet = false;

	std::vector<BYTE> file_byte_data = mathTypeHelper.GetByteData();
	UINT mtef_bdx = mathTypeHelper.GetMtef_bdx();
	UINT mtef_edx = mathTypeHelper.GetMtef_edx();
	std::vector<BYTE> mtef_byte_data(file_byte_data.begin()+mtef_bdx, file_byte_data.begin()+mtef_edx);

	std::string xml_str;
	KMathTypeReturnValue kMathTypeReturnValue = ConvertMTEFtoXmlStringByTdl(mtef_byte_data, tdl_name, xml_str);
	if (kMathTypeReturnValue != mtOK)
	{
		return bRet;
	}
	xmlTree.LoadXML(s2ws(xml_str).c_str());
	xmlTree.AddXPathNamespace(L"m", L"http://www.w3.org/1998/Math/MathML");

	return bRet = true;
}

KMathTypeReturnValue CMathTypeProcessor::ConvertMTEFtoXmlStringByTdl(const std::vector<BYTE>& mtef_byte_data, const std::string& tdl_name, std::string& xml_string)
{
	KMathTypeReturnValue kMathTypeReturnValue = mtERROR;

	MTAPIConnectFunc mtConnect = (MTAPIConnectFunc)GetProcAddress(m_hMT6Dll, "MTAPIConnect");
	MTXFormResetFunc mtFormReset = (MTXFormResetFunc)GetProcAddress(m_hMT6Dll, "MTXFormReset");
	MTXFormSetTranslatorFunc mtFormSetTranslator = (MTXFormSetTranslatorFunc)GetProcAddress(m_hMT6Dll, "MTXFormSetTranslator");
	MTXFormEqnFunc mtFormEqn = (MTXFormEqnFunc)GetProcAddress(m_hMT6Dll, "MTXFormEqn");
	MTAPIDisconnectFunc mtDisConnect = (MTAPIDisconnectFunc)GetProcAddress(m_hMT6Dll, "MTAPIDisconnect");
	if (!mtConnect || !mtFormReset || !mtFormSetTranslator || !mtFormEqn || !mtDisConnect)
	{
		return kMathTypeReturnValue;
	}

	//启动
	kMathTypeReturnValue = mtConnect(mtinitLAUNCH_AS_NEEDED, MTAPICONNECT_TIME_OUT);
	if (kMathTypeReturnValue==mtOK)
	{
		kMathTypeReturnValue==mtOK && (kMathTypeReturnValue = mtFormSetTranslator(mtxfmTRANSL_INC_NONE, tdl_name.c_str()));
		if (kMathTypeReturnValue==mtOK)
		{
			std::vector<char> ref_arr;
			ref_arr.resize(5000);
			kMathTypeReturnValue = mtFormEqn(mtxfmLOCAL, mtxfmMTEF, &mtef_byte_data[0], mtef_byte_data.size(), mtxfmLOCAL, mtxfmTEXT, &ref_arr[0], 5000, "", MTAPI_DIMS());
			xml_string.clear();
			xml_string.append(ref_arr.begin(), ref_arr.end());
		}

		kMathTypeReturnValue = mtDisConnect();
	}
	return kMathTypeReturnValue;
}

void CMathTypeProcessor::BuildStyleArr(wchar_t text_key, const CXMLTree& xmlStyleTree, std::vector<std::wstring>& style_arr)
{
	std::wstring style_xpath = L"//*[contains(./text(), '";
	style_xpath += text_key;
	style_xpath += L"')]";
	CXMLNodeList styles = xmlStyleTree.GetNodes(style_xpath.c_str());
	CXMLNode style = styles.NextNode();
	while (style)
	{
		std::wstring style_str = static_cast<CXMLElement>(style).GetAttributeValueByName(L"mathtypevariant");
		std::wstring style_texts = style.Getwchar_tText();
		for (int i=0; i<style_texts.size(); i++)
		{
			if (style_texts[i] == text_key)
			{
				style_arr.push_back(style_str);
			}	
		}
		style = styles.NextNode();
	}
}

std::string CMathTypeProcessor::GetStrCharacterStyle(BYTE byte_val)
{
	std::string style("");

	switch (byte_val)
	{
	case 0x00:
		{
			style = "plain";
		}
		break;
	case 0x01:
		{
			style = "bold";
		}
		break;
	case 0x02:
		{
			style = "italic";
		}
		break;
	case 0x03:
		{
			style = "bold_italic";
		}
		break;
	default:
		break;
	}

	return style;
}

bool CMathTypeProcessor::GetMathTypeCharacterStylePrefs(const CMathTypeHelper& mathTypeHelper, std::map<std::string, std::string>& map_name_to_style)
{
	bool bRet = false;

	std::vector<BYTE> char_style_arr;
	mathTypeHelper.GetCharStyleArr(char_style_arr);
	if (char_style_arr.size() != CHAR_STYLE_NUM)
	{
		return bRet;
	}
	
	for (int i=0; i<CHAR_STYLE_NAME_ARR.size(); i++)
	{
		std::string style = GetStrCharacterStyle(char_style_arr[i]);
		map_name_to_style.insert(std::make_pair(CHAR_STYLE_NAME_ARR[i], style));
	}
	map_name_to_style.insert(std::make_pair(TEXT_FE, GetStrCharacterStyle(char_style_arr[char_style_arr.size() - 1])));
	return bRet = true;
}

bool CMathTypeProcessor::ProcessStyleXml(const std::map<std::string, std::string>& map_name_to_style, CXMLTree& xmlStyleTree)
{
	//number
	{
		std::wstring no_mathvariant_mn_xpath = L"//m:mn[not(@mathvariant)]";
		CXMLNodeList no_mathvariant_mns = xmlStyleTree.GetNodes(no_mathvariant_mn_xpath.c_str());
		CXMLNode no_mathvariant_mn = no_mathvariant_mns.NextNode();
		while (no_mathvariant_mn)
		{
			static_cast<CXMLElement>(no_mathvariant_mn).SetAttributeValueByName(L"mathtypevariant", L"number");
			no_mathvariant_mn = no_mathvariant_mns.NextNode();
		}
	}
	//mi 汉字
	{
		std::wstring no_mathvariant_mi_xpath = L"//m:mi[not(@mathvariant)]";
		CXMLNodeList no_mathvariant_mis = xmlStyleTree.GetNodes(no_mathvariant_mi_xpath.c_str());
		for (CXMLNode no_mathvariant_mi = no_mathvariant_mis.NextNode(); no_mathvariant_mi; no_mathvariant_mi = no_mathvariant_mis.NextNode())
		{
			std::wstring mi_text = no_mathvariant_mi.Getwchar_tText();
			if (mi_text.empty())
			{
				continue;
			}
			if (mi_text[0]>=0x4E00 && mi_text[0]<=0x9FA5)
			{
				static_cast<CXMLElement>(no_mathvariant_mi).SetAttributeValueByName(L"mathtypevariant", s2ws(TEXT_FE).c_str());
			}
		}
	}
	//other
	{
		for (auto iter=map_name_to_style.begin(); iter!=map_name_to_style.end(); iter++)
		{
			std::wstring style_name = s2ws(iter->first);
			std::wstring style_val = s2ws(iter->second);
			std::wstring xPath = L"//*[@mathtypevariant='" + style_name + L"']";
			CXMLNodeList mtexts = xmlStyleTree.GetNodes(xPath.c_str());
			CXMLNode mtext = mtexts.NextNode();
			while (mtext)
			{
				static_cast<CXMLElement>(mtext).SetAttributeValueByName(L"mathtypevariant", style_val.c_str());
				mtext = mtexts.NextNode();
			}
		}
	}


	return true;
}

bool CMathTypeProcessor::GetMathmlStyleXml(const CMathTypeHelper& mathTypeHelper, CXMLTree& xmlStyleTree)
{
	bool bRet = false;

	CreateMathmlFFxTdl(MATHML2_FFX_TDL);//需要添加版本号
	if (!ConvertMTEFtoXmlByTdl(mathTypeHelper, MATHML2_FFX_TDL, xmlStyleTree))
	{
		return bRet;
	}
	std::map<std::string, std::string> map_name_to_style;
	GetMathTypeCharacterStylePrefs(mathTypeHelper, map_name_to_style);
	ProcessStyleXml(map_name_to_style, xmlStyleTree);

	return bRet=true; 
}

bool CMathTypeProcessor::BuildDataStyleMap(const CXMLNodeList& dataNodes, const CXMLTree& xmlStyleTree, 
							std::map<wchar_t, std::vector<std::wstring>>& text_key_to_style_arr)
{
	dataNodes.Reset();
	for (CXMLNode dataNode=dataNodes.NextNode(); dataNode; dataNode = dataNodes.NextNode())
	{
		std::wstring texts = dataNode.Getwchar_tText();
		for (int i=0; i<texts.size(); i++)
		{
			wchar_t text_key = texts[i];
			auto iter = text_key_to_style_arr.find(text_key);
			if (iter == text_key_to_style_arr.end())
			{
				std::vector<std::wstring> style_arr;
				BuildStyleArr(text_key, xmlStyleTree, style_arr);
				text_key_to_style_arr.insert(make_pair(text_key, style_arr));
			}
		}
	}
	return true;
}

bool CMathTypeProcessor::ApplyStyleToDataXML(std::map<wchar_t, std::vector<std::wstring>>& text_key_to_style_arr, CXMLNodeList& dataNodes)
{
	dataNodes.Reset();
	for (CXMLNode dataNode=dataNodes.NextNode(); dataNode; dataNode = dataNodes.NextNode())
	{
		std::wstring texts = dataNode.Getwchar_tText();
		if (texts.empty())
		{
			continue;
		}
		wchar_t text_key = texts[0];
		auto iter = text_key_to_style_arr.find(text_key);
		if (iter != text_key_to_style_arr.end())
		{
			std::vector<std::wstring>& style_arr = iter->second;
			if (!style_arr.empty() && !style_arr[0].empty())
			{
				CXMLElement elment = static_cast<CXMLElement>(dataNode);
				if (elment.GetAttributeValueByName(L"mathvariant").empty())
				{
					std::wstring mathvariant_val = style_arr[0].compare(L"bold_italic") == 0 ? L"bold-italic" : style_arr[0];
					elment.SetAttributeValueByName(L"mathvariant", mathvariant_val.c_str());
				}		
			}
		}
		for (int i=0; i<texts.size(); i++)
		{
			wchar_t text_key = texts[i];
			auto iter = text_key_to_style_arr.find(text_key);
			if (iter != text_key_to_style_arr.end())
			{
				std::vector<std::wstring>& style_arr = iter->second;
				style_arr.erase(style_arr.begin());
				if (style_arr.empty())
				{
					text_key_to_style_arr.erase(iter);
				}
			}
		}
	}
	return true;
}

std::wstring CMathTypeProcessor::ConvertToXml(const std::wstring& fn)
{
	std::wstring mathml_xml(L"");

	std::wstring parse_mt_file_name = fn;
	std::wstring wsExt = gtl::GetFileExt(fn.c_str());
	if(wsExt == L"bin")
	{
		std::wstring extract_file_path = fn + L"_extract";
		bool bRet = Extract(fn.c_str(), extract_file_path.c_str());
		if (!bRet)
		{
			return mathml_xml;
		}
		parse_mt_file_name = extract_file_path + L"\\Equation Native";
	}

	MTParsParam param;
	param.m_ParseMTFileName = ws2s(parse_mt_file_name);
	CMathTypeHelper mathTypeHelper;
	if (!mathTypeHelper.Init(param))
	{
		return mathml_xml;
	}

	CXMLTree xmlDataTree;
	if (!ConvertMTEFtoXmlByTdl(mathTypeHelper, MATHML2_NAMESPACEATTR_TDL, xmlDataTree))
	{
		return mathml_xml;
	}

	CXMLTree xmlStyleTree;
	if (!GetMathmlStyleXml(mathTypeHelper, xmlStyleTree))
	{
		return mathml_xml;
	}

	std::wstring data_xpath = L"//*[name()='mtext' or name()='mn' or name()='mi' or name()='mo' or name()='ms']";
	CXMLNodeList dataNodes = xmlDataTree.GetNodes(data_xpath.c_str());
	std::map<wchar_t, std::vector<std::wstring>> text_key_to_style_arr;
	BuildDataStyleMap(dataNodes, xmlStyleTree, text_key_to_style_arr);
	ApplyStyleToDataXML(text_key_to_style_arr, dataNodes);

	mathml_xml = xmlDataTree.GetXMLSrc();
	std::wstring findstr = L"xmlns=";
	int pos = mathml_xml.find(findstr);////查找指定的串
	if (pos != -1)
	{
		mathml_xml.replace(pos, findstr.length(), L"xmlns:mml=");////用新的串替换掉指定的串
	}
	return mathml_xml;
}

std::wstring CMathTypeProcessor::s2ws(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
	return conv.from_bytes(str);
}

std::string CMathTypeProcessor::ws2s(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
	return conv.to_bytes(wstr);
}