#ifndef RWGROUP_H
#define RWGROUP_H

#define STRICT_SEQUENCE		1

#include <vector>
#include "rwaction.h"

class rwgroup {
public:
	rwgroup();
	~rwgroup();
	bool append(const rwaction& act);
	int size() const;
	int match(const rwgroup& g) const; // Returns actions matched by g

	std::vector<rwaction> get_unique() const;
	int type() const { return type_; }
private:
	rwaction::actiontype type_;
	std::vector<rwaction> actlist_;
};

#endif
