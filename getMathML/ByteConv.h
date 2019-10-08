#pragma once

#include "MTEFTypes.h"
#include <vector>
#include <string>
namespace MTEF
{
	namespace BYTECONV
	{
		struct bit4
		{
			BYTE val:4;
			bit4(BYTE by_val)
			{
				val = by_val;
			}
			bool operator == (BYTE by_val) const
			{
				return val == static_cast<BYTE>(by_val & 0x0F);
			}
		};
		static bit4 lowbit(BYTE by_val)
		{
			return by_val & 0x0F;
		}
		static bit4 highbit(BYTE by_val)
		{
			return (by_val & 0xF0) >> 4;
		}

		static void conv(const std::vector<BYTE>& buff, UINT& idx, WORD& rV)
		{
			rV = WORD();

			rV = rV | buff[idx] << 0;
			rV = rV | buff[idx+1] << 8;

			idx += sizeof(WORD);
		}

		static void conv(const std::vector<BYTE>& buff, UINT& idx, DWORD& rV)
		{
			rV = DWORD();
			rV = rV | buff[idx] << 0;
			rV = rV | buff[idx+1] << 8;
			rV = rV | buff[idx+2] << 16;
			rV = rV | buff[idx+3] << 24;

			idx += sizeof(DWORD);
		}

		static void conv(const std::vector<BYTE>& buff, UINT& idx, BYTE& rV)
		{
			rV = BYTE();
			rV = buff[idx];

			idx += 1;
		}

		static void conv(const std::vector<BYTE>& buff, UINT& idx, std::string& rV)
		{
			rV.clear();

			for (; idx<buff.size(); idx++)
			{
				rV.push_back(buff[idx]);
				if (buff[idx] == (BYTE)0x00)
				{
					break;
				}
			}
			idx += 1;
		}

		static void conv(const std::vector<BYTE>& buff, UINT& idx, SignedIntVal& rV)
		{
			rV.val = 0;
			if (buff[idx] == 0xFF)
			{
				rV.val = (buff[idx+2]<<8) + buff[idx+1];
				idx += 3;
			}
			else
			{
				rV.val = static_cast<UINT>(buff[idx]) - 128;
				idx += 1;
			}
			
		}

		static void conv(const std::vector<BYTE>& buff, UINT& idx, UnSignedIntVal& rV)
		{
			rV.val = 0;
			if (buff[idx] == 0xFF)
			{
				rV.val = (buff[idx+2]<<8) + buff[idx+1];
				idx += 3;
			}
			else
			{
				rV.val = static_cast<UINT>(buff[idx]);
				idx += 1;
			}
		}

		enum KBitFlag
		{
			UNIT_BIT,
			SIGN_BIT,
			INTEGER_BIT,
			DECIMAL_BIT
		};

		static void convBit4ToDimensionVal(const bit4& b4, KBitFlag& kBitFlag, DimensionVal& rV)
		{
			switch(kBitFlag)
			{
			case UNIT_BIT:
				{
					rV.kUnit = static_cast<KUNITS>(b4.val);
					kBitFlag = SIGN_BIT;
				}
				break;
			case SIGN_BIT:
				{
					if (b4 == 0xB)
					{
						rV.bNegtive = true;
					}
					else
					{
						rV.int_bit = rV.int_bit * 10 + static_cast<UINT>(b4.val);
					}
					kBitFlag = INTEGER_BIT;
				}
				break;
			case INTEGER_BIT:
				{
					if (b4 == 0xA)
					{
						kBitFlag = DECIMAL_BIT;
					}
					else
					{
						rV.int_bit = rV.int_bit * 10 + static_cast<UINT>(b4.val);
					}
				}
				break;
			case DECIMAL_BIT:
				{
					rV.decimal_bit = rV.decimal_bit * 10 + static_cast<UINT>(b4.val);
				}
				break;
			default:
				{
					ASSERT(false);
				}
			}
		}

		static void conv(const std::vector<BYTE>& buff, UINT& idx, bool& high_byte, DimensionVal& rV)
		{
			rV.Clear();

			bit4 by4high = highbit(buff[idx]);
			KBitFlag kBitFlag = UNIT_BIT;
			if (by4high == 0xF)
			{
				bit4 by4low = lowbit(buff[idx]);
				convBit4ToDimensionVal(by4low, kBitFlag, rV);
				idx ++;
			}
			for (; idx<buff.size(); idx++)
			{
				bit4 by4high = highbit(buff[idx]);
				if (by4high == 0xF)
				{
					high_byte = true;
					break;
				}
				convBit4ToDimensionVal(by4high, kBitFlag, rV);
				bit4 by4low = lowbit(buff[idx]);
				if (by4low == 0xF)
				{
					idx ++;
					high_byte = false;
					break;
				}
				convBit4ToDimensionVal(by4low, kBitFlag, rV);
			}
		}
	}
}