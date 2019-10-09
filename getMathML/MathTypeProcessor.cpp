#include "StdAfx.h"
#include "MathTypeProcessor.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <iterator>
#include <algorithm>
#include <iomanip>

#include "MathTypeHelper.h"

#include "Interface/XML/XML.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MTSDKDN;

static const int MTAPICONNECT_TIME_OUT = 30;
static const std::string MATHML2_FFX_TDL = "MathML2 (FFx).tdl";
static const std::string MATHML2_MNAMESPACE_TDL = "MathML2 (m namespace).tdl";
static const int CHAR_STYLE_NUM = 12;
static const std::string CHAR_STYLE_NAMES[] = {"text","function","variable","greek","lc_greek","uc_greek","symbol","vector","number","extra_math","user1style","user2style"};
static const std::vector<std::string> CHAR_STYLE_NAME_ARR(CHAR_STYLE_NAMES, CHAR_STYLE_NAMES+CHAR_STYLE_NUM);

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
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "text") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "text") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "function") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "function") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "variable") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "variable") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "greek") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "greek") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "lc_greek") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "lc_greek") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "uc_greek") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "uc_greek") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "symbol") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "symbol") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "vector") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "vector") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "number") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "number") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "extra_math") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "extra_math") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "user1style") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "user1style") << std::endl;
	//fout << CreateMathmlFFxTdlLine(ffx_run_command, "user2style") << std::endl 
	//	 << CreateMathmlFFxTdlLine(ffx_char_command, "user2style") << std::endl;
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
		return bRet=true;
	}
	std::string mathml2_mnamespace_tdl_full = mathtype_path_ + "\\Translators\\" + MATHML2_MNAMESPACE_TDL;
	std::ifstream fin(mathml2_mnamespace_tdl_full, std::ios::in);
	std::ofstream fout(mathml2_ffx_tdl_full, std::ios::out);
	if (!fin.is_open() || !fout.is_open())
	{
		return bRet;
	}
	bool flag = true;
	const std::string namespace_mathml_tdl_head = "MathType Output Translator 1.0: \"MathML 2.0 (m namespace)\"";
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

KMathTypeReturnValue CMathTypeProcessor::ConvertMTEFtoXmlByTdl(const std::vector<BYTE>& mtef_byte_data, const std::string& tdl_name, std::string& xml_string)
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

void CMathTypeProcessor::ConvertToXml(char * fn)
{
	MTParsParam param;
	param.m_ParseMTFileName = fn;
	CMathTypeHelper mathTypeHelper;
	mathTypeHelper.Init(param);
	std::vector<BYTE> file_byte_data = mathTypeHelper.GetByteData();
	UINT mtef_bdx = mathTypeHelper.GetMtef_bdx();
	UINT mtef_edx = mathTypeHelper.GetMtef_edx();
	std::vector<BYTE> mtef_byte_data(file_byte_data.begin()+mtef_bdx, file_byte_data.begin()+mtef_edx);
	std::vector<BYTE> char_style_arr;
	mathTypeHelper.GetCharStyleArr(char_style_arr);
	if (char_style_arr.size() != CHAR_STYLE_NUM)
	{
		return;
	}
	std::map<std::string, std::string> map_name_to_style;
	for (int i=0; i<CHAR_STYLE_NUM; i++)
	{
		std::string style;
		switch (char_style_arr[i])
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
		default:
			break;
		}
		map_name_to_style.insert(std::make_pair(CHAR_STYLE_NAME_ARR[i], style));
	}

	CreateMathmlFFxTdl(MATHML2_FFX_TDL);
	std::string mathml_mnamespace;
	KMathTypeReturnValue mtef_to_mathml_mnamespace_ok = ConvertMTEFtoXmlByTdl(mtef_byte_data, MATHML2_MNAMESPACE_TDL, mathml_mnamespace);
	std::string mathml_ffx;
	KMathTypeReturnValue mtef_to_mathml_ffx_ok = ConvertMTEFtoXmlByTdl(mtef_byte_data, MATHML2_FFX_TDL, mathml_ffx);
	if (mtef_to_mathml_mnamespace_ok != mtOK || mtef_to_mathml_ffx_ok !=mtOK)
	{
		return;
	}
	CXMLTree xmlTree;
	xmlTree.LoadXML(mathml_mnamespace);
	xmlTree.AddXPathNamespace(L"m", L"http://www.w3.org/1998/Math/MathML");
}