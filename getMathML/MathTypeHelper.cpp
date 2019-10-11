#include "StdAfx.h"
#include "MathTypeHelper.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <iterator>
#include <algorithm>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MTEF;

CMathTypeHelper::CMathTypeHelper(void)
	:	m_idx(0)
{
	m_MTParsParam.Clear();
	m_ByteData.clear();
	InitRecordParseFuncMap();
}


CMathTypeHelper::~CMathTypeHelper(void)
{
}


bool CMathTypeHelper::ReadFileIntoBinary(const char * filename, std::vector<BYTE>& buff)
{
	buff.clear();
	std::ifstream ifile(filename, std::ios::in | std::ios::binary);
	if (!ifile.is_open())
	{
		return false;
	}
	std::istreambuf_iterator<char> iter_f2( ifile );
	std::copy( iter_f2, std::istreambuf_iterator<char>(), back_inserter(buff));
	ifile.close();
	return true;
}

bool CMathTypeHelper::WriteBinaryIntoFile(const char * filename, const std::vector<BYTE>& buff)
{
	std::ofstream ofile(filename, std::ios::out | std::ios::binary);
	if (!ofile.is_open())
	{
		return false;
	}
	std::ostreambuf_iterator<char> out_it (ofile);
	std::copy (buff.begin(), buff.end(), out_it );
	ofile.close();
	return true;
}

void CMathTypeHelper::InitRecordParseFuncMap()
{
	m_RecordParseFuncMap.clear();
	m_RecordParseFuncMap.insert(std::make_pair(END_RECORD, std::bind(&CMathTypeHelper::ParseEND, this)));
	m_RecordParseFuncMap.insert(std::make_pair(LINE_RECORD, std::bind(&CMathTypeHelper::ParseLINE, this)));
	m_RecordParseFuncMap.insert(std::make_pair(CHAR_RECORD, std::bind(&CMathTypeHelper::ParseCHAR, this)));
	m_RecordParseFuncMap.insert(std::make_pair(TMPL_RECORD, std::bind(&CMathTypeHelper::ParseTMPL, this)));
	m_RecordParseFuncMap.insert(std::make_pair(PILE_RECORD, std::bind(&CMathTypeHelper::ParsePILE, this)));
	m_RecordParseFuncMap.insert(std::make_pair(MATRIX_RECORD, std::bind(&CMathTypeHelper::ParseMATRIX, this)));
	m_RecordParseFuncMap.insert(std::make_pair(EMBELL_RECORD, std::bind(&CMathTypeHelper::ParseEMBELL, this)));
	m_RecordParseFuncMap.insert(std::make_pair(RULER_RECORD, std::bind(&CMathTypeHelper::ParseRULER, this)));
	m_RecordParseFuncMap.insert(std::make_pair(FONT_STYLE_DEF_RECORD, std::bind(&CMathTypeHelper::ParseFONT_STYLE_DEF, this)));
	m_RecordParseFuncMap.insert(std::make_pair(SIZE_RECORD, std::bind(&CMathTypeHelper::ParseSIZE, this)));
	m_RecordParseFuncMap.insert(std::make_pair(FULL_RECORD, std::bind(&CMathTypeHelper::ParseFULL, this)));
	m_RecordParseFuncMap.insert(std::make_pair(SUB_RECORD, std::bind(&CMathTypeHelper::ParseSUB, this)));
	m_RecordParseFuncMap.insert(std::make_pair(SUB2_RECORD, std::bind(&CMathTypeHelper::ParseSUB2, this)));
	m_RecordParseFuncMap.insert(std::make_pair(SYM_RECORD, std::bind(&CMathTypeHelper::ParseSYM, this)));
	m_RecordParseFuncMap.insert(std::make_pair(SUBSYM_RECORD, std::bind(&CMathTypeHelper::ParseSUBSYM, this)));
	m_RecordParseFuncMap.insert(std::make_pair(COLOR_RECORD, std::bind(&CMathTypeHelper::ParseCOLOR, this)));
	m_RecordParseFuncMap.insert(std::make_pair(COLOR_DEF_RECORD, std::bind(&CMathTypeHelper::ParseCOLOR_DEF, this)));
	m_RecordParseFuncMap.insert(std::make_pair(FONT_DEF_RECORD, std::bind(&CMathTypeHelper::ParseFONT_DEF, this)));
	m_RecordParseFuncMap.insert(std::make_pair(EQN_PREFS_RECORD, std::bind(&CMathTypeHelper::ParseEQN_PREFS, this)));
	m_RecordParseFuncMap.insert(std::make_pair(ENCODING_DEF_RECORD, std::bind(&CMathTypeHelper::ParseENCODING_DEF, this)));
}

std::string CMathTypeHelper::GetFileExt(const char* wstrFileName)
{
	std::string str(wstrFileName);
	int iFind = static_cast<int>(str.rfind('.'));
	int iFindLimit = static_cast<int>(str.rfind('\\'));
	if (iFind<0 || iFind < iFindLimit)
		return "";
	return str.substr(iFind+1);
};

bool CMathTypeHelper::Init(const MTParsParam& param)
{
	bool bRet = false;

	m_MTParsParam = param;
	if (!ReadFileIntoBinary(param.m_ParseMTFileName.c_str(), m_ByteData))
	{
		return bRet;
	}
	std::string file_ext = GetFileExt(m_MTParsParam.m_ParseMTFileName.c_str());
	std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), ::tolower);
	if (file_ext.empty())
	{
		ParseEQN_OLE_Header();
		m_mtef_bdx = m_idx;
		m_mtef_edx = m_ByteData.size();
	}
	else if (file_ext.compare("wmf") == 0)
	{
		AppsMFCCHeaderPtr app_mfcc_header = ParseAppsMFCCHeader();
		m_mtef_bdx = m_idx;
		//522 == 92
		m_mtef_edx = m_idx + app_mfcc_header->dataLen;
	}
	
	m_mtef_data = ParseMTEFData();
	if (m_mtef_data == nullptr)
	{
		return bRet;
	}
	return bRet = true;
}

int CMathTypeHelper::GetStyleXml(const char * filename)
{

	return 0;
}

int CMathTypeHelper::GetMTTransXml(const char * filename)
{
	return 0;
}

MTEFDataPtr CMathTypeHelper::ParseMTEFData()
{
	std::shared_ptr<MTEFData> mtef_data = std::make_shared<MTEFData>();
	mtef_data->mtef_header = ParseMTEFHeader();
	if (
		mtef_data->mtef_header->MTEF_version != (BYTE)(0x05) ||
		mtef_data->mtef_header->generating_platform != (BYTE)(0x01)
	   )
	{
		//mtef版本不是5，程序版本不是windows，return
		return nullptr;
	}
	mtef_data->contents = DoParseMTRecord();
	ASSERT(m_idx == m_mtef_edx);
	return mtef_data;
}

EQNOLEFILEHDRPtr CMathTypeHelper::ParseEQN_OLE_Header()
{
	EQNOLEFILEHDRPtr eqn_ole_header = std::make_shared<EQNOLEFILEHDR>();
	UINT index = m_idx;
	BYTECONV::conv(m_ByteData, index, eqn_ole_header->cbHdr);
	BYTECONV::conv(m_ByteData, index, eqn_ole_header->version);
	BYTECONV::conv(m_ByteData, index, eqn_ole_header->cf);
	BYTECONV::conv(m_ByteData, index, eqn_ole_header->cbObject);

	m_idx += eqn_ole_header->size();

	return eqn_ole_header;
}

AppsMFCCHeaderPtr CMathTypeHelper::ParseAppsMFCCHeader()
{
	AppsMFCCHeaderPtr app_mfcc_header = std::make_shared<AppsMFCCHeader>();
	const std::string APPSMFCCSTR = "AppsMFCC";
	const std::vector<BYTE> APPSMFCC(APPSMFCCSTR.begin(), APPSMFCCSTR.end());
	auto iter = std::search(m_ByteData.begin(), m_ByteData.end(), APPSMFCC.begin(), APPSMFCC.end());
	if (iter == m_ByteData.end())
	{
		return nullptr;
	}
	m_idx = std::distance(m_ByteData.begin(), iter);
	//char  id[8];		// Must be "AppsMFCC"
	//WORD  version;		// Must be 1
	//DWORD totalLen;		// Total of data bytes in all comments
	//DWORD dataLen;		// # of data bytes in this comment (see 'data' below) 
	//char  signature[1];	// Variable length, null terminated string (see below for full details)
	for (int i=0; i<8; i++)
	{
		BYTE id;
		BYTECONV::conv(m_ByteData, m_idx, id);
		app_mfcc_header->id[i] = id;
	}
	BYTECONV::conv(m_ByteData, m_idx, app_mfcc_header->version);
	BYTECONV::conv(m_ByteData, m_idx, app_mfcc_header->totalLen);
	BYTECONV::conv(m_ByteData, m_idx, app_mfcc_header->dataLen);
	BYTECONV::conv(m_ByteData, m_idx, app_mfcc_header->signature);

	return app_mfcc_header;
}

MTEFHeaderPtr CMathTypeHelper::ParseMTEFHeader()
{
	std::shared_ptr<MTEFHeader> mtef_header = std::make_shared<MTEFHeader>();
	BYTECONV::conv(m_ByteData, m_idx, mtef_header->MTEF_version);
	BYTECONV::conv(m_ByteData, m_idx, mtef_header->generating_platform);
	BYTECONV::conv(m_ByteData, m_idx, mtef_header->generating_product);
	BYTECONV::conv(m_ByteData, m_idx, mtef_header->product_version);
	BYTECONV::conv(m_ByteData, m_idx, mtef_header->product_subversion);
	BYTECONV::conv(m_ByteData, m_idx, mtef_header->application_key);
	BYTECONV::conv(m_ByteData, m_idx, mtef_header->equation_options);
	return mtef_header;
}

std::vector<MTRecordPtr> CMathTypeHelper::DoParseMTRecord()
{
	std::vector<MTRecordPtr> record_base_ptr_arr;
	while (m_idx < m_ByteData.size())
	{
		MTRecordPtr mt_record_ptr = ParseMTRecord();
		if (mt_record_ptr == nullptr)
		{
			break;
		}
		record_base_ptr_arr.push_back(mt_record_ptr);
		if (mt_record_ptr->record_type == END_RECORD)
		{
			break;
		}
	}

	return record_base_ptr_arr;
}

KRecordType CMathTypeHelper::ParseRecordType()
{
	BYTE record_type;
	BYTECONV::conv(m_ByteData, m_idx, record_type);
	return static_cast<KRecordType>(record_type);
}

MTRecordPtr CMathTypeHelper::ParseMTRecord()
{
	KRecordType record_type = ParseRecordType();
	std::map<KRecordType, ParseRecordFunc>::iterator iter = m_RecordParseFuncMap.find(record_type);
	if (iter == m_RecordParseFuncMap.end())
	{
		return nullptr;
	}

	ParseRecordFunc parseRecordFunc = iter->second;
	MTRecordPtr record = parseRecordFunc();
	record->record_type = record_type;
	return record;
}

MTRecordPtr CMathTypeHelper::ParseEND()
{
	return std::make_shared<MTEnd>();
}

MTRecordPtr CMathTypeHelper::ParseLINE()
{
	std::shared_ptr<MTLine> mt_line = std::make_shared<MTLine>();
	mt_line->options = ParseOptions();
	mt_line->nudge = ParseNUDGE(mt_line->options);
	if (mt_line->options & OptionValue::mtefOPT_LINE_LSPACE)
	{
		BYTECONV::conv(m_ByteData, m_idx, mt_line->line_spacing);
	}
	if (mt_line->options & OptionValue::mtefOPT_LP_RULER )
	{
		MTRecordPtr mt_record_ptr = ParseMTRecord();
		ASSERT(mt_record_ptr->record_type == RULER_RECORD);
		mt_line->mt_ruler = std::dynamic_pointer_cast<MTRuler>(mt_record_ptr);
	}
	if ( !mt_line->options & OptionValue::mtefOPT_LINE_NULL)
	{
		std::vector<MTRecordPtr> record_base_ptr_arr = DoParseMTRecord();
		mt_line->contents = record_base_ptr_arr;
	}
	
	return mt_line;
}

NudgeVal CMathTypeHelper::ParseNUDGE(BYTE options)
{
	//待完善
	NudgeVal nudge_val;
	if ((options & OptionValue::mtefOPT_NUDGE) == OptionValue::mtefOPT_NUDGE)
	{
		if (m_ByteData[m_idx] == 0x80 && m_ByteData[m_idx+1] == 0x80 )
		{
			m_idx += 6;
		}
		m_idx += 2;
	}
	return nudge_val;
}

BYTE CMathTypeHelper::ParseOptions()
{
	BYTE options;
	BYTECONV::conv(m_ByteData, m_idx, options);
	return options;
}

MTRecordPtr CMathTypeHelper::ParseCHAR()
{
	std::shared_ptr<MTChar> mt_char = std::make_shared<MTChar>();
	mt_char->options = ParseOptions();
	mt_char->nudge = ParseNUDGE(mt_char->options);
	BYTECONV::conv(m_ByteData, m_idx, mt_char->typeface);

	if (!(mt_char->options & OptionValue::mtefOPT_CHAR_ENC_NO_MTCODE))
	{
		WORD val;
		BYTECONV::conv(m_ByteData, m_idx, val);
		mt_char->character_value = static_cast<wchar_t>(val);
	}
	if (mt_char->options & OptionValue::mtefOPT_CHAR_ENC_CHAR_8)
	{
		BYTE byVal;
		BYTECONV::conv(m_ByteData, m_idx, byVal);
		mt_char->font_position = static_cast<wchar_t>(byVal);
	}
	else if (mt_char->options & OptionValue::mtefOPT_CHAR_ENC_CHAR_16)
	{
		BYTECONV::conv(m_ByteData, m_idx, mt_char->font_position);
	}

	if (mt_char->options & OptionValue::mtefOPT_CHAR_EMBELL)
	{
		mt_char->mt_embells = DoParseMTRecord();
	}

	return mt_char;
}

MTRecordPtr CMathTypeHelper::ParseTMPL()
{
	//BYTE options,
	//NudgeVal nudge;
	//BYTE selector;
	//WORD variation;
	//BYTE template_specific_options;
	//std::vector<RecordBasePtr> contents;
	std::shared_ptr<MTTmpl> mt_tmpl = std::make_shared<MTTmpl>();
	mt_tmpl->options = ParseOptions();
	mt_tmpl->nudge = ParseNUDGE(mt_tmpl->options);
	BYTECONV::conv(m_ByteData, m_idx, mt_tmpl->selector);
	//跳过读取，待完善
	if (m_ByteData[m_idx] & 0x80)
	{
		m_idx+=2;
	}
	else
	{
		m_idx++;
	}
	BYTECONV::conv(m_ByteData, m_idx, mt_tmpl->template_specific_options);
	mt_tmpl->contents = DoParseMTRecord();

	return mt_tmpl;
}

MTRecordPtr CMathTypeHelper::ParsePILE()
{
	//BYTE options;
	//NudgeVal nudge;
	//BYTE halign;
	//BYTE valign;
	//std::shared_ptr<MTRuler> mt_ruler;
	//std::vector<RecordBasePtr> contents;
	std::shared_ptr<MTPile> mt_pile = std::make_shared<MTPile>();
	mt_pile->options = ParseOptions();
	mt_pile->nudge = ParseNUDGE(mt_pile->options);
	BYTECONV::conv(m_ByteData, m_idx, mt_pile->halign);
	BYTECONV::conv(m_ByteData, m_idx, mt_pile->valign);
	if (mt_pile->options & OptionValue::mtefOPT_LP_RULER )
	{
		MTRecordPtr mt_record_ptr = ParseMTRecord();
		ASSERT(mt_record_ptr->record_type == RULER_RECORD);
		mt_pile->mt_ruler = std::dynamic_pointer_cast<MTRuler>(mt_record_ptr);
	}
	std::vector<MTRecordPtr> record_base_ptr_arr = DoParseMTRecord();
	mt_pile->contents = record_base_ptr_arr;

	return mt_pile;
}

MTRecordPtr CMathTypeHelper::ParseMATRIX()
{
	//BYTE options;
	//NudgeVal nudge;
	//BYTE valign;
	//BYTE h_just;
	//BYTE v_just;
	//BYTE rows;
	//BYTE cols;
	//std::vector<BYTE> row_parts;
	//std::vector<BYTE> col_parts;
	//std::vector<RecordBasePtr> contents;
	std::shared_ptr<MTMatrix> mt_matrix = std::make_shared<MTMatrix>();
	mt_matrix->options = ParseOptions();
	mt_matrix->nudge = ParseNUDGE(mt_matrix->options);
	BYTECONV::conv(m_ByteData, m_idx, mt_matrix->valign);
	BYTECONV::conv(m_ByteData, m_idx, mt_matrix->h_just);
	BYTECONV::conv(m_ByteData, m_idx, mt_matrix->v_just);
	BYTECONV::conv(m_ByteData, m_idx, mt_matrix->rows);
	BYTECONV::conv(m_ByteData, m_idx, mt_matrix->cols);
	UINT row_lines = static_cast<UINT>(mt_matrix->rows) + 1;
	UINT row_byte_count = static_cast<UINT>(ceil(row_lines / 4.0f));
	for (UINT i=0; i<row_byte_count; i++)
	{
		BYTE row_part;
		BYTECONV::conv(m_ByteData, m_idx, row_part);
		mt_matrix->row_parts.push_back(row_part);	
	}
	UINT col_lines = static_cast<UINT>(mt_matrix->cols) + 1;
	UINT col_byte_count = static_cast<UINT>(ceil(col_lines / 4.0f));
	for (UINT i=0; i<col_byte_count; i++)
	{
		BYTE col_part;
		BYTECONV::conv(m_ByteData, m_idx, col_part);
		mt_matrix->col_parts.push_back(col_part);	
	}

	std::vector<MTRecordPtr> record_base_ptr_arr = DoParseMTRecord();
	mt_matrix->contents = record_base_ptr_arr;
	return mt_matrix;
}

MTRecordPtr CMathTypeHelper::ParseEMBELL()
{
	//待完善
	std::shared_ptr<MTEmbell> mt_embell = std::make_shared<MTEmbell>();
	mt_embell->options = ParseOptions();
	mt_embell->nudge = ParseNUDGE(mt_embell->options);
	BYTECONV::conv(m_ByteData, m_idx, mt_embell->embell_type);

	return mt_embell;
}

MTRecordPtr CMathTypeHelper::ParseRULER()
{
	std::shared_ptr<MTRuler> mt_ruler = std::make_shared<MTRuler>();
	BYTECONV::conv(m_ByteData, m_idx, mt_ruler->tab_stops_count);

	for (int i=0; i<mt_ruler->tab_stops_count; i++)
	{
		TabStops tab_stops;
		BYTECONV::conv(m_ByteData, m_idx, tab_stops.type);
		BYTECONV::conv(m_ByteData, m_idx, tab_stops.offset);
		mt_ruler->tab_stops_arr.push_back(tab_stops);
	}
	
	return mt_ruler;
}

MTRecordPtr CMathTypeHelper::ParseFONT_STYLE_DEF()
{
	std::shared_ptr<MTFont_Style_Def> mt_font_style_def = std::make_shared<MTFont_Style_Def>();
	BYTECONV::conv(m_ByteData, m_idx, mt_font_style_def->font_def_index);
	BYTECONV::conv(m_ByteData, m_idx, mt_font_style_def->char_style);
	return mt_font_style_def;
}

MTRecordPtr CMathTypeHelper::ParseSIZE()
{
	std::shared_ptr<MTSize> mt_size = std::make_shared<MTSize>();
	if (m_ByteData[m_idx] == 0x64 || m_ByteData[m_idx] == 0x65)
	{
		BYTECONV::conv(m_ByteData, m_idx, mt_size->size_options);
	}
	else
	{
		mt_size->size_options = 0x00;
	}
	switch (mt_size->size_options)
	{
	case 0x65:
		{
			BYTECONV::conv(m_ByteData, m_idx, mt_size->point_size);
		}
		break;
	case 0x00:
		{
			BYTECONV::conv(m_ByteData, m_idx, mt_size->lsize);
			BYTE by_dsize;
			BYTECONV::conv(m_ByteData, m_idx, by_dsize);
			mt_size->dsize = static_cast<UINT>(by_dsize) - 128;
		}
		break;
	case 0x64:
		{
			BYTECONV::conv(m_ByteData, m_idx, mt_size->lsize);
			BYTECONV::conv(m_ByteData, m_idx, mt_size->dsize);
		}
		break;
	default:
		ASSERT(false);
		break;
	}

	return mt_size;
}

MTRecordPtr CMathTypeHelper::ParseFULL()
{
	//待完善
	return std::make_shared<MTFull>();
}

MTRecordPtr CMathTypeHelper::ParseSUB()
{
	//待完善
	return std::make_shared<MTSub>();
}

MTRecordPtr CMathTypeHelper::ParseSUB2()
{
	//待完善
	return std::make_shared<MTSub2>();
}

MTRecordPtr CMathTypeHelper::ParseSYM()
{
	//待完善
	return std::make_shared<MTSym>();
}

MTRecordPtr CMathTypeHelper::ParseSUBSYM()
{
	//待完善
	return std::make_shared<MTSubSym>();
}

MTRecordPtr CMathTypeHelper::ParseCOLOR()
{
	std::shared_ptr<MTColor> mt_color = std::make_shared<MTColor>();
	BYTECONV::conv(m_ByteData, m_idx, mt_color->color_def_index);
	return mt_color;
}

MTRecordPtr CMathTypeHelper::ParseCOLOR_DEF()
{
	//BYTE options;
	//std::vector<WORD> color_values;
	//std::string name;
	std::shared_ptr<MTColor_Def> mt_color_def = std::make_shared<MTColor_Def>();
	mt_color_def->options = ParseOptions();
	UINT color_value_count = mt_color_def->options & OptionValue::mtefCOLOR_CMYK ? 4 : 3;
	for (UINT i=0; i<color_value_count; i++)
	{
		WORD color_value;
		BYTECONV::conv(m_ByteData, m_idx, color_value);
		mt_color_def->color_values.push_back(color_value);
	}
	if (mt_color_def->options & OptionValue::mtefCOLOR_NAME)
	{
		BYTECONV::conv(m_ByteData, m_idx, mt_color_def->name);
	}
	return mt_color_def;
}

MTRecordPtr CMathTypeHelper::ParseFONT_DEF()
{
	std::shared_ptr<MTFont_Def> font_def = std::make_shared<MTFont_Def>();
	BYTECONV::conv(m_ByteData, m_idx, font_def->enc_def_index);
	BYTECONV::conv(m_ByteData, m_idx, font_def->font_name);
	return font_def;
}

MTRecordPtr CMathTypeHelper::ParseEQN_PREFS()
{
	std::shared_ptr<MTEqn_Prefs> eqn_prefs = std::make_shared<MTEqn_Prefs>();

	BYTECONV::conv(m_ByteData, m_idx, eqn_prefs->options);

	bool high_byte = true;
	BYTECONV::conv(m_ByteData, m_idx, eqn_prefs->size_count);
	for (int i=0; i<eqn_prefs->size_count; i++)
	{
		DimensionVal dimensionVal;
		BYTECONV::conv(m_ByteData, m_idx, high_byte, dimensionVal);
		eqn_prefs->size_arr.push_back(dimensionVal);
	}
	if (high_byte)
	{
		m_idx++;
	}

	high_byte = true;
	BYTECONV::conv(m_ByteData, m_idx, eqn_prefs->spaceing_count);
	for (int i=0; i<eqn_prefs->spaceing_count; i++)
	{
		DimensionVal dimensionVal;
		BYTECONV::conv(m_ByteData, m_idx, high_byte, dimensionVal);
		eqn_prefs->spaceing_arr.push_back(dimensionVal);
	}
	if (high_byte)
	{
		m_idx++;
	}
	
	BYTECONV::conv(m_ByteData, m_idx, eqn_prefs->style_count);
	for (int i=0; i<eqn_prefs->style_count; i++)
	{
		std::pair<BYTE, BYTE> byVal;
		BYTECONV::conv(m_ByteData, m_idx, byVal.first);
		if (byVal.first != 0x00)
		{
			BYTECONV::conv(m_ByteData, m_idx, byVal.second);
		}
		else
		{
			byVal.second = 0x00;
		}
		eqn_prefs->style_arr.push_back(byVal);
	}

	return eqn_prefs;
}

MTRecordPtr CMathTypeHelper::ParseENCODING_DEF()
{
	std::shared_ptr<MTEnCoding_Def> encoding_def = std::make_shared<MTEnCoding_Def>();
	BYTECONV::conv(m_ByteData, m_idx, encoding_def->encoding_name);
	return encoding_def;
}

void CMathTypeHelper::GetCharStyleArr(std::vector<BYTE>& char_style_arr) const
{
	std::vector<std::pair<BYTE, BYTE>> style_arr;
	std::shared_ptr<MTEqn_Prefs> eqn_prefs;
	for (int i=0; i<m_mtef_data->contents.size(); i++)
	{
		RecordBasePtr record_ptr = m_mtef_data->contents[i];
		if (record_ptr->record_type == EQN_PREFS_RECORD)
		{
			eqn_prefs = std::dynamic_pointer_cast<MTEqn_Prefs>(record_ptr);
		}	
	}

	for (int i=0; i<eqn_prefs->style_arr.size(); i++)
	{
		BYTE char_style = eqn_prefs->style_arr[i].second;
		char_style_arr.push_back(char_style);
	}
}

