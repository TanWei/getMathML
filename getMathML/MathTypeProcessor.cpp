#include "StdAfx.h"
#include "MathTypeProcessor.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <iterator>
#include <algorithm>
#include <iomanip>

#include "MathTypeHelper.h"

typedef long (WINAPI *MTAPIConnectFunc)(int,int);
typedef long (WINAPI *MTAPIDisconnectFunc)();
typedef long (WINAPI *MTXFormResetFunc)();
typedef long (WINAPI *MTXFormSetTranslatorFunc)(int,char*);
typedef long (WINAPI *MTXFormEqnFunc)(int, int, BYTE*, long, int, int, char*, long, std::string, MTAPI_DIMS&);
//Public Function MTXFormSetTranslator (
//	options As Integer,
//	transName As String
//	) As Long
//typedef long (WINAPI *MTGetPrefsMTDefault)(char*,int);

std::wstring GetMathTypePath()
{
	HKEY hKEY;
	std::wstring strRegPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\MathType.exe"; 
	if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE,strRegPath.c_str(), 0, KEY_READ, &hKEY)!=ERROR_SUCCESS)
	{
		return L"";
	}

	DWORD dwType = REG_SZ;
	BYTE byWordConvPath[1024];
	ZeroMemory(byWordConvPath,1024);
	DWORD dwSize = 1024;
	if (::RegQueryValueEx(hKEY, L"Path", NULL, &dwType, byWordConvPath, &dwSize)!=ERROR_SUCCESS)
	{
		return L"";
	}
	::RegCloseKey(hKEY); 

	return (wchar_t*)(byWordConvPath);
}

std::string FindReplaceString(const std::string& source, const std::string& findstr, const std::string& replacestr)
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

void WriteMathmlFFxTdlLine(std::ofstream& fout)
{
	const std::string ffx_mathml_tdl_head = "MathType Output Translator 1.0: \"MathML 2.0 (FFx)\", \"MathML 2.0 (FFx)\"";
	fout << ffx_mathml_tdl_head << std::endl;
	//trans char_style(plain, bold, italic, or bold_italic)
	std::string ffx_run_command = "run/char_style/name/\"char_style\" = \"#\";";
	std::string ffx_char_command = "char/0x0000/0xFFFF/\"char_style\" = \"<mtext mathtypevariant='char_style'>&$#x(CharHex);</mtext>\";";
	fout << FindReplaceString(ffx_run_command, "char_style", "plain") << std::endl << FindReplaceString(ffx_char_command, "char_style", "plain") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "bold") << std::endl << FindReplaceString(ffx_char_command, "char_style", "bold") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "italic") << std::endl << FindReplaceString(ffx_char_command, "char_style", "italic") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "bold_italic") << std::endl << FindReplaceString(ffx_char_command, "char_style", "bold_italic") << std::endl;
	//trans style(text, function, variable, greek, lc_greek, uc_greek, symbol, vector, number, extra_math, user1style, or user2style)
	fout << FindReplaceString(ffx_run_command, "char_style", "text") << std::endl << FindReplaceString(ffx_char_command, "char_style", "text") << std::endl;
	//fout << FindReplaceString(ffx_run_command, "char_style", "function") << std::endl << FindReplaceString(ffx_char_command, "char_style", "function") << std::endl;
	fout << "run/function/name/\"function_style\" = \"#\";" << std::endl
		<< "char/0x0000/0xFFFF/\"function_style\" = \"<mtext mathtypevariant='function'>&$#x(CharHex);</mtext>\";" << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "variable") << std::endl << FindReplaceString(ffx_char_command, "char_style", "variable") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "greek") << std::endl << FindReplaceString(ffx_char_command, "char_style", "greek") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "lc_greek") << std::endl << FindReplaceString(ffx_char_command, "char_style", "lc_greek") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "uc_greek") << std::endl << FindReplaceString(ffx_char_command, "char_style", "uc_greek") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "symbol") << std::endl << FindReplaceString(ffx_char_command, "char_style", "symbol") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "vector") << std::endl << FindReplaceString(ffx_char_command, "char_style", "vector") << std::endl;
	//fout << FindReplaceString(ffx_run_command, "char_style", "number") << std::endl << FindReplaceString(ffx_char_command, "char_style", "number") << std::endl;
	fout << "run/number/name/\"number_style\" = \"#\";" << std::endl
		<< "char/0x0000/0xFFFF/\"number_style\" = \"<mtext mathtypevariant='number'>&$#x(CharHex);</mtext>\";" << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "extra_math") << std::endl << FindReplaceString(ffx_char_command, "char_style", "extra_math") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "user1style") << std::endl << FindReplaceString(ffx_char_command, "char_style", "user1style") << std::endl;
	fout << FindReplaceString(ffx_run_command, "char_style", "user2style") << std::endl << FindReplaceString(ffx_char_command, "char_style", "user2style") << std::endl;
}

void CreateMathmlFFxTdl(std::string file)
{
	//gtl::deletefile(file);
	std::ifstream fin("C:\\Users\\tanwei.HOLD\\Documents\\Visual Studio 2010\\Projects\\getMathML\\Debug\\MathML2 (m namespace).tdl", std::ios::in);
	std::ofstream fout("C:\\Users\\tanwei.HOLD\\Documents\\Visual Studio 2010\\Projects\\getMathML\\Debug\\MathML2 (FFx).tdl", std::ios::out);
	if (!fin.is_open() || !fout.is_open())
	{
		return;
	}
	const std::string namespace_mathml_tdl_head = "MathType Output Translator 1.0: \"MathML 2.0 (m namespace)\"";
	std::string file_line;
	while(std::getline(fin, file_line))
	{
		if (file_line.find(namespace_mathml_tdl_head) != -1)
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
}

CMathTypeProcessor::CMathTypeProcessor(void)
{
	//std::string strMT6DllPath = GetMathTypePath();
	//strMT6DllPath.append(L"\\System\\MT6.dll");
	//std::string strMT6DllPath =  "";
	//HMODULE hDll = LoadLibrary(strMT6DllPath.c_str());
	std::wstring strMT6DllPath = GetMathTypePath();
	strMT6DllPath.append(L"\\System\\MT6.dll");
	m_hMT6Dll = LoadLibrary(strMT6DllPath.c_str());
}

//void CreatTxt(char* pathName, unsigned char* rBuffer,int length)//创建txt文件
//{
//	//char* path = "C:\\1.txt"; // 你要创建文件的路径
//	ofstream fout(pathName);
//	if (fout) { // 如果创建成功
//		for (int i = 0; i < length; i++)
//		{
//			fout <<“”写入的内容“”<< endl; // 使用与cout同样的方式进行写入
//
//
//		}
//
//		fout.close();  // 执行完操作后关闭文件句柄
//	}
//}

CMathTypeProcessor::~CMathTypeProcessor(void)
{
	FreeLibrary(m_hMT6Dll);
}

void CMathTypeProcessor::ConvertToXml(char * fn)
{
	//CreateMathmlFFxTdl("");

	long nResult;
	MTAPIConnectFunc mtConnect = (MTAPIConnectFunc)GetProcAddress(m_hMT6Dll, "MTAPIConnect");
	MTXFormResetFunc mtFormReset = (MTXFormResetFunc)GetProcAddress(m_hMT6Dll, "MTXFormReset");
	MTXFormSetTranslatorFunc mtFormSetTranslator = (MTXFormSetTranslatorFunc)GetProcAddress(m_hMT6Dll, "MTXFormSetTranslator");
	MTXFormEqnFunc mtFormEqn = (MTXFormEqnFunc)GetProcAddress(m_hMT6Dll, "MTXFormEqn");
	MTAPIDisconnectFunc mtDisConnect = (MTAPIDisconnectFunc)GetProcAddress(m_hMT6Dll, "MTAPIDisconnect");
	if (!mtConnect)
	{
	}

	//char * fn="E:\\Doc2 - 副本\\word\\embeddings\\oleObject1\\Equation Native";
	//char * fn1="E:\\Doc2 - 副本\\word\\embeddings\\oleObject1\\Equation Native1";
	char * styleXML="E:\\Doc2 - 副本\\word\\embeddings\\oleObject1\\Equation Native1";
	char * MTXML ="E:\\Doc2 - 副本\\word\\embeddings\\oleObject1\\Equation Native1";

	MTParsParam param;
	param.m_ParseMTFileName = fn;
	//param.m_DesMTFileName = fn1;
	CMathTypeHelper mathTypeHelper;
	mathTypeHelper.Init(param);
	int nR1 = mathTypeHelper.GetStyleXml(styleXML);
	int nR2  = mathTypeHelper.GetMTTransXml(MTXML);

	std::vector<BYTE> arrHeight = mathTypeHelper.GetByteData();
	BYTE* buffer = new BYTE[sizeof(arrHeight)];  
	if (!arrHeight.empty())  
	{  
		memcpy(buffer, &arrHeight[28], (arrHeight.size()-27)*sizeof(BYTE));  
	}
	//启动
	nResult = mtConnect(0,30);
	nResult = mtFormReset();
	nResult = mtFormSetTranslator(3, "MathML2 (m namespace).tdl");
	//int, int, char*, long, int, int, char*, long, std::string, MTAPI_DIMS&
	char* ref_char = new char[5000];
	MTAPI_DIMS aa;
	nResult = mtFormEqn(-3, 4, buffer, arrHeight.size(), -3, 7, ref_char, 5000, "D:\\out.a", aa);
	//断开
	nResult = mtDisConnect();
}