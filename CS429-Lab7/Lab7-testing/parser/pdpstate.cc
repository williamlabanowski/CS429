#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "pdp.h"
#include "pdpstate.h"
#include "pdpaction.h"

pdpstate::pdpstate()
	:matches_(0), samples_(0)
{
#if 0
	char hdr[PDP_OBJG_HDR_SIZE];
	fread(hdr, PDP_OBJG_HDR_SIZE, 1, objf);
	if (memcmp(hdr, PDP_OBJG_HDR, PDP_OBJG_HDR_SIZE)) {
		fprintf(stderr, "File doesn't have a valid OBJ8 header\n");
		fprintf(stderr, "Header dump:\n");
		for(int i = 0; i < PDP_OBJG_HDR_SIZE; i++) {
			fprintf(stderr, "\t%X ", hdr[i]);
		}
	}
	ep_ = deflat(objf);
#endif
#if 0
	memset(&mem_[0], 0, sizeof(mem_));

	while (!feof(objf)) {
		/* Read blocks */
		uint8_t bsize;
		pdpword baddr;

		long bstart = ftell(objf);
		size_t attempt = fread(&bsize, sizeof(bsize), 1, objf);
		if (attempt == 0) // EOF
			break; 
		long bend = bstart + bsize;
		baddr = deflat(objf);
		int pos = 0;
		while (ftell(objf) < bend) {
			mem_[baddr+pos] = deflat(objf);
			pos++;
		}
	}
#endif
}

template<> bool pdpstate::match(bool valid, const rwgroup& lhs, const rwgroup& rhs)
{
	bool ret = false;
	int matched = 0;
	if (valid) {
		matched = lhs.match(rhs);
		matches_ += matched;
		ret = (matched == lhs.size());
	}
	samples_ += lhs.size();
#if VERBOSE
	fprintf(stderr, "RWGroup Matches %ld Samples %ld\n", matches_, samples_);
#endif
	return ret;
}

#define MATCH(item)						\
	match(validgrade, ref.item, grade.item)

void pdpstate::compare(pdpaction& ref, pdpaction& grade)
{
	if (!ref.valid())
		return;
	bool validgrade = grade.valid();
	validgrade = validgrade && MATCH(pc());
	MATCH(cycles());
	MATCH(disa());

#if STRICT_SEQUENCE
	rwg_array_t &refarr = ref.get_rwgarray();
	rwg_array_t &gradearr = grade.get_rwgarray();
#if VERBOSE
	fprintf(stderr, "REFARR size %lu\n", refarr.size());
#endif
	for(size_t i = 0; i < refarr.size(); i++) {
		validgrade = validgrade && (i < gradearr.size());
		match(validgrade, refarr[i], gradearr[i]);
	}
#else
	match(validgrade, ref.get_readg(), grade.get_readg());
	match(validgrade, ref.get_writeg(), grade.get_writeg());
#endif
}
