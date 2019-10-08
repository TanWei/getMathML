#pragma once

#include <string>
#include <map>
#include <memory>

#include "ByteConv.h"
#include "MTEFTypes.h"
#include <functional>

struct MTParsParam
{
	std::string m_ParseMTFileName;
	//std::string m_DesMTFileName;
	MTParsParam()
	{
		Clear();
	}
	void Clear()
	{
		m_ParseMTFileName.clear();
		//m_DesMTFileName.clear();
	}
};

typedef MTEF::RecordBasePtr MTRecordPtr;
typedef std::shared_ptr<MTEF::MTEFHeader> MTEFHeaderPtr;
typedef std::shared_ptr<MTEF::MTEFData> MTEFDataPtr;
typedef std::shared_ptr<MTEF::EQNOLEFILEHDR> EQNOLEFILEHDRPtr;
typedef std::shared_ptr<MTEF::AppsMFCCHeader> AppsMFCCHeaderPtr;

class CMathTypeHelper
{
public:

	std::vector<BYTE> GetByteData() {return m_ByteData;}
	CMathTypeHelper(void);
	virtual ~CMathTypeHelper(void);
	bool CMathTypeHelper::Init(const MTParsParam& param);
	int GetStyleXml(const char * filename);
	int GetMTTransXml(const char * filename);
private:
	EQNOLEFILEHDRPtr ParseEQN_OLE_Header();
	AppsMFCCHeaderPtr ParseAppsMFCCHeader();

	MTEFDataPtr ParseMTEFData();
	MTEFHeaderPtr ParseMTEFHeader();
	MTRecordPtr ParseMTRecord();
	std::vector<MTRecordPtr> DoParseMTRecord();

	bool ReadFileIntoBinary(const char * filename, std::vector<BYTE>& buff);
	bool WriteBinaryIntoFile(const char * filename, const std::vector<BYTE>& buff);
private:
	void InitRecordParseFuncMap();
	MTRecordPtr ParseEND();
	MTRecordPtr ParseLINE();
	MTRecordPtr ParseCHAR();
	MTRecordPtr ParseTMPL();
	MTRecordPtr ParsePILE();
	MTRecordPtr ParseMATRIX();
	MTRecordPtr ParseEMBELL();
	MTRecordPtr ParseRULER();
	MTRecordPtr ParseFONT_STYLE_DEF();
	MTRecordPtr ParseSIZE();
	MTRecordPtr ParseFULL();
	MTRecordPtr ParseSUB();
	MTRecordPtr ParseSUB2();
	MTRecordPtr ParseSYM();
	MTRecordPtr ParseSUBSYM();
	MTRecordPtr ParseCOLOR();
	MTRecordPtr ParseCOLOR_DEF();
	MTRecordPtr ParseFONT_DEF();
	MTRecordPtr ParseEQN_PREFS();
	MTRecordPtr ParseENCODING_DEF();

	MTEF::KRecordType ParseRecordType();
	BYTE ParseOptions();
	MTEF::NudgeVal ParseNUDGE(BYTE options);
private:
	std::string GetFileExt(const char* wstrFileName);
private:
	MTParsParam m_MTParsParam;
	UINT m_idx;
	std::vector<BYTE> m_ByteData;
	UINT m_mtef_bdx;
	UINT m_mtef_edx;
	MTEFDataPtr m_mtef_data;

	typedef std::function<MTRecordPtr()> ParseRecordFunc;
	std::map<MTEF::KRecordType, ParseRecordFunc> m_RecordParseFuncMap;
};

