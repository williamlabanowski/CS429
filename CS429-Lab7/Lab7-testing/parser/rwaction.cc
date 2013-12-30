#include <stdlib.h>
#include "rwaction.h"
#include <stdio.h>
using std::string;

rwaction::rwaction(const string& ssrc, const string& sdst)
	:src(ssrc), dst(sdst)
{
	if (src.type == verbobj::CONSTANT)
		type = WRITE;
	else
		type = READ;
}

verbobj::verbobj(const string& str)
{
	switch (str[0]) {
		case 'M': 
			{
				type = MEMORY;
				const char* part = str.c_str();
				part += 2; // Skip "M["
				value = strtoul(part, NULL, 0) & 0xFFFF;
			}
			break;
		case '0':
			type = CONSTANT;
			value = strtoul(str.c_str(), NULL, 0) & 0xFFFF;
			break;
		default:
			type = REGISTER;
			regname = str;
	}
}

bool operator==(const verbobj& lhs, const verbobj& rhs)
{
	if (lhs.type != rhs.type)
		return false;
	if (lhs.type == verbobj::REGISTER)
		return lhs.regname == rhs.regname;
	return lhs.value == rhs.value;
}

bool operator==(const rwaction& lhs, const rwaction& rhs)
{
	return lhs.src == rhs.src && lhs.dst == rhs.dst;
}
