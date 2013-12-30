#ifndef RWACTION_H
#define RWACTION_H

#include "pdp.h"
#include <string>

struct verbobj {
	enum verbtype {
		MEMORY,
		REGISTER,
		CONSTANT,
	};
	verbtype type;
	pdpword value; //avaliable for mem and const
	std::string regname;

	verbobj(const std::string&);
};

struct rwaction {
	enum actiontype {
		UNKNOWN,
		READ,
		WRITE
	} type;
	rwaction(const std::string&, const std::string&);
	verbobj src, dst;
};

bool operator==(const verbobj& lhs, const verbobj& rhs);
bool operator==(const rwaction& lhs, const rwaction& rhs);

#endif
