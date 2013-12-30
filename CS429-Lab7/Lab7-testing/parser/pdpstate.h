#ifndef PDPSTATE_H
#define PDPSTATE_H

#include "pdp.h"

class pdpaction;

class pdpstate {
public:
	pdpstate();
#if 0
	pdpword ep() const { return ep_; }
#endif

	void compare(pdpaction& ref, pdpaction& grade);
	unsigned long matches() const { return matches_; }
	unsigned long samples() const { return samples_; }

private:
#if 0
	pdpword ep_;
	pdpword	mem_[PDP_MEM_SIZE];
#endif

	unsigned long matches_;
	unsigned long samples_;

	template<typename T> bool match(bool valid, const T& lhs, const T& rhs)
	{
		bool ret = false;
		if (valid && lhs == rhs) {
			matches_++;
			ret = true;
		}
		samples_++;
#if VERBOSE
		fprintf(stderr, "Matches %ld Samples %ld\n", matches_, samples_);
#endif
		return ret;
	}
};

#endif
