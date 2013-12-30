#ifndef PARSER_EXCEPTION_H
#define PARSER_EXCEPTION_H

#include <string>

struct line_terminated {
};

struct bad_syntax {
	bad_syntax(const char* ptr, const char* e)
		:position(ptr), expection(e)
	{
	}
	const char* position;
	std::string expection;
};

#endif
