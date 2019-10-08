#pragma once
#include <string>
#include <vector>
namespace MTEF
{
	struct MTEFBase{
		unsigned int idx;
		unsigned int length;
		virtual ~MTEFBase() {}
	};

	enum KRecordType
	{
		END_RECORD = 0,//
		LINE_RECORD = 1,//
		CHAR_RECORD = 2,//
		TMPL_RECORD = 3,//
		PILE_RECORD = 4,//
		MATRIX_RECORD = 5,//
		EMBELL_RECORD = 6,//
		RULER_RECORD = 7,//
		FONT_STYLE_DEF_RECORD = 8,//
		SIZE_RECORD = 9,//
		FULL_RECORD = 10,//
		SUB_RECORD = 11,//
		SUB2_RECORD = 12,//
		SYM_RECORD = 13,//
		SUBSYM_RECORD = 14,//
		COLOR_RECORD = 15,
		COLOR_DEF_RECORD = 16,
		FONT_DEF_RECORD = 17,//
		EQN_PREFS_RECORD = 18,//
		ENCODING_DEF_RECORD = 19,//
		FUTURE_RECORD = 100 //
	};
	//Option values
	namespace OptionValue
	{
		enum KAllOption
		{
			mtefOPT_NUDGE = 8
		};

		enum KCharOption
		{
			mtefOPT_CHAR_EMBELL = 1,
			mtefOPT_CHAR_FUNC_START = 2,
			mtefOPT_CHAR_ENC_CHAR_8 = 4,
			mtefOPT_CHAR_ENC_CHAR_16 = 16,
			mtefOPT_CHAR_ENC_NO_MTCODE = 32
		};

		enum KLineOption
		{
			mtefOPT_LINE_NULL = 1,
			mtefOPT_LINE_LSPACE = 4
		};

		enum KLinePileOption
		{
			mtefOPT_LP_RULER = 2
		};

		enum KColorRefOption
		{
			mtefCOLOR_CMYK = 1,
			mtefCOLOR_SPOT = 2,
			mtefCOLOR_NAME = 4
		};
	}
	//Signed integer values
	struct SignedIntVal
	{
		int val;
	};

	//Unsigned integer values
	struct UnSignedIntVal
	{
		UINT val;
	};

	struct NudgeVal
	{

	};

	enum KUNITS
	{
		inches = 0,
		centimeters = 1,
		points = 2,
		picas = 3,
		percentage = 4
	};

	struct DimensionVal
	{
		KUNITS kUnit;
		bool bNegtive;
		UINT int_bit;
		UINT decimal_bit;
		void Clear()
		{
			kUnit = inches;
			bNegtive = false;
			int_bit = 0;
			decimal_bit = 0;
		}
	};
	typedef std::vector<DimensionVal> DimensionValArr;

	struct RecordBase : MTEFBase
	{
		KRecordType record_type;
		virtual ~RecordBase() {}
	};
	typedef std::shared_ptr<MTEF::RecordBase> RecordBasePtr;

	struct MTEnd : RecordBase
	{
		
	};

	struct TabStops
	{
		BYTE type;
		WORD offset;
	};
	typedef std::vector<TabStops> TabStopsArr;

	struct MTRuler : RecordBase
	{
		//Consists of:
		//	record type (7)
		//	[n_stops] number of tab-stops
		//	[tab-stop list] tab-stops in order from left-to-right
		BYTE tab_stops_count; 
		TabStopsArr tab_stops_arr;
	};

	struct MTLine : RecordBase
	{
		//Consists of:
		//	record type (1)
		//	options
		//	[nudge] if mtefOPT_NUDGE is set
		//	[line spacing] if mtefOPT_LINE_LSPACE is set (16-bit integer)
		//	[RULER record] if mtefOPT_LP_RULER is set
		//	object list contents of line (a single pile, characters and templates, or nothing)
		BYTE options;
		NudgeVal nudge;
		WORD line_spacing;
		std::shared_ptr<MTRuler> mt_ruler;
		std::vector<RecordBasePtr> contents;
	};

	struct MTEmbell : RecordBase
	{
		//Consists of:
		//	record type (6)
		//	options
		//	[nudge] if mtefOPT_NUDGE is set
		//	[embell] embellishment type
		BYTE options;
		NudgeVal nudge;
		BYTE embell_type;
	};

	struct MTChar : RecordBase
	{
		//Consists of:
		//	record type (2)
		//	options
		//	[nudge] if mtefOPT_NUDGE is set
		//	[typeface] typeface value (signed integer; see FONT_STYLE_DEF record below)
		//	[character] character value (see below)
		//	[embellishment list] if mtefOPT_CHAR_EMBELL is set (embellishments)
		BYTE options;
		NudgeVal nudge;
		SignedIntVal typeface;
		wchar_t character_value;
		WORD font_position;
		std::vector<RecordBasePtr> mt_embells;
	};

	struct MTTmpl : RecordBase
	{
		//Consists of:
		//	record type (3)
		//	options
		//	[nudge] if mtefOPT_NUDGE is set
		//	[selector] template selector code
		//	[variation] template variation code (1 or 2 bytes; see below)
		//	[options] template-specific options
		//	[subobject list] either a single character (e.g. sigma in a summation) and/or lines
		BYTE options;
		NudgeVal nudge;
		BYTE selector;
		WORD variation;
		BYTE template_specific_options;
		std::vector<RecordBasePtr> contents;
	};

	struct MTPile : RecordBase
	{
		//Consists of:
		//	record type (4)
		//	options
		//	[nudge] if mtefOPT_NUDGE is set
		//	[halign] horizontal alignment
		//	[valign] vertical alignment
		//	[[RULER record]] if mtefOPT_LP_RULER is set
		//	[object list] list of lines contained by the pile
		BYTE options;
		NudgeVal nudge;
		BYTE halign;
		BYTE valign;
		std::shared_ptr<MTRuler> mt_ruler;
		std::vector<RecordBasePtr> contents;
	};

	struct MTMatrix : RecordBase
	{
		//Consists of:
		//	record type (5)
		//	options
		//	[nudge] if mtefOPT_NUDGE is set
		//	[valign] vertical alignment of matrix within container
		//	[h_just] horizontal alignment within columns
		//	[v_just] vertical alignment within columns
		//	[rows] number of rows
		//	[cols] number of columns
		//	[row_parts] row partition line types (see below)
		//	[col_parts] column partition line types (see below)
		//	[object list] list of lines, one for each element of the matrix, in order from left-to-right and top-to-bottom
		BYTE options;
		NudgeVal nudge;
		BYTE valign;
		BYTE h_just;
		BYTE v_just;
		BYTE rows;
		BYTE cols;
		std::vector<BYTE> row_parts;
		std::vector<BYTE> col_parts;
		std::vector<RecordBasePtr> contents;
	};

	struct MTFont_Style_Def : RecordBase
	{
		//Consists of:
		//	record type (8)
		//	[font_def_index] index of mtefFONT_DEF record (unsigned integer)
		//	[char_style] character style bits
		UnSignedIntVal font_def_index;
		BYTE char_style;
	};

	struct MTSize : RecordBase
	{
		//Consists of one of the following cases:
		//	if lsize < 0 (explicit point size):
		//		record type (9)
		//		101
		//		-point size (16 bit integer)
		//	else if -128 < dsize < +128:
		//		record type (9)
		//		lsize (typesize)
		//		dsize + 128
		//	else: (large delta)
		//		record type (9)
		//		100
		//		lsize (typesize)
		//		dsize (16 bit integer)
		BYTE size_options;
		BYTE lsize;
		WORD dsize;
		WORD point_size;
	};

	struct MTFull : RecordBase
	{
		//Consists of:
		// record type (10-14)
	};

	struct MTSub : RecordBase
	{
		//Consists of:
		// record type (10-14)
	};

	struct MTSub2 : RecordBase
	{
		//Consists of:
		// record type (10-14)
	};

	struct MTSym : RecordBase
	{
		//Consists of:
		// record type (10-14)
	};

	struct MTSubSym : RecordBase
	{
		//Consists of:
		// record type (10-14)
	};

	struct MTColor : RecordBase
	{
		//Consists of:
		//	record type (15)
		//	[color_def_index] index of corresponding COLOR_DEF record (unsigned integer)
		UnSignedIntVal color_def_index;
	};

	struct MTColor_Def : RecordBase
	{
		//Consists of:
		//	record type (16)
		//	[options] model is RGB unless mtefCOLOR_CMYK bit is set; type is process unless mtefCOLOR_SPOT bit is set; color is unnamed unless mtefCOLOR_NAME bit is set
		//	[color values] if RGB, 3 values (red, green, blue); if CMYK, 4 values (cyan, magenta, yellow, black); see below for details
		//	[name] null-terminated color name; appears only if mtefCOLOR_NAME option is set
		BYTE options;
		std::vector<WORD> color_values;
		std::string name;
	};

	struct MTFont_Def : RecordBase
	{
		//Consists of:
		//	record type (17)
		//	[font_def_index] index of mtefFONT_DEF record (unsigned integer)
		//	[char_style] character style bits
		UnSignedIntVal enc_def_index;
		std::string font_name;
	};

	struct MTEqn_Prefs : RecordBase
	{
		//Consists of:
		//	record type (18)
		//	[options] none defined in this version of MTEF
		//	[sizes] dimension array for typesize definitions
		//	[spaces] dimension array for spacing definitions (see below)
		//	[styles] array of style definitions (see below)

		BYTE options;
		BYTE size_count; 
		DimensionValArr size_arr;
		BYTE spaceing_count; 
		DimensionValArr spaceing_arr;
		BYTE style_count; 
		std::vector<std::pair<BYTE, BYTE>> style_arr;
	};

	struct MTEnCoding_Def : RecordBase
	{
		//Consists of:
		//	record type (19)
		//	[name] null-terminated encoding name
		std::string encoding_name;
	};
}
