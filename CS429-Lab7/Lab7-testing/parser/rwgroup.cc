#include <algorithm>
#include "rwgroup.h"
#include <stdio.h>

rwgroup::rwgroup()
	:type_(rwaction::UNKNOWN)
{
#if VERBOSE
	fprintf(stderr, "Create %s %p\n", __func__, this);
#endif
}

rwgroup::~rwgroup()
{
#if VERBOSE
	fprintf(stderr, "Free %s %p\n", __func__, this);
#endif
	actlist_.clear();
}

int rwgroup::size() const
{
	return actlist_.size();
}

bool rwgroup::append(const rwaction& act)
{
	bool accept = false;
	if (type_ == rwaction::UNKNOWN)
		type_ = act.type;
	if (type_ == act.type)
		accept = true;
	if (accept)
		actlist_.push_back(act);
#if VERBOSE
	fprintf(stderr, "RWGroud size increased to %lu\n", actlist_.size());
#endif
	return accept;
}

typedef std::vector<rwaction>::const_iterator ai;

int rwgroup::match(const rwgroup& g) const
{
	std::vector<rwaction> lhuni = get_unique();
	std::vector<rwaction> rhuni = g.get_unique();
	size_t matched = 0;

	for(ai iter = rhuni.begin(); iter != rhuni.end(); iter++) {
		ai f = std::find(lhuni.begin(), lhuni.end(), *iter);
		if (f != lhuni.end())
			matched++;
	}
	size_t notmatched = std::max(lhuni.size() - matched, rhuni.size() - matched);
#if VERBOSE
	fprintf(stderr, "%lu matched %lu not matched\n", matched, notmatched);
#endif
	return size() - notmatched;
}

std::vector<rwaction> rwgroup::get_unique() const
{
	std::vector<rwaction> uni;
	// We should use std::unique if we could sort actlist_, but we can't
	for(ai iter = actlist_.begin(); iter != actlist_.end(); iter++) {
		ai f = std::find(uni.begin(), uni.end(), *iter);
		if (f == uni.end())
			uni.push_back(*iter);
	}
	return uni;
}
