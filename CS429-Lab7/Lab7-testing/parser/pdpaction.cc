#include <ctype.h>
#include <stdlib.h>
#include "pdpaction.h"
#include "pdpstate.h"
#include "rwaction.h"
#include "rwgroup.h"
#include "parser_exception.h"

using std::string;

pdpaction::pdpaction()
	:current_cycle_(0), last_cycle_(0), valid_(false)
{
}

void pdpaction::parse(const char* lbuf)
{
	valid_ = false;
	if (!lbuf)
		return ;
	if (lbuf[0] == '\0')
		return ; // New line
#if VERBOSE
	fprintf(stderr, "Parsing %s", lbuf);
#endif
	base_ = lbuf;
	cur_ = lbuf;
	try {
		parse_time();
		parse_pc();
		parse_inst();
		parse_disassembly();
		parse_verbose();
		valid_ = true;
	} catch (const bad_syntax& bs) {
		fprintf(stderr, "Bad syntax since %s, expecting %s\n", bs.position,
				bs.expection.c_str());
	} catch (...) {
		fprintf(stderr, "Unknown Exception\n");
		return ;
	};
#if VERBOSE
	fprintf(stderr, "Time %lu PC %hX Inst %hX Dis %s Cycle %lu\n",
			current_cycle_, pc_, inst_, inst_disa_.c_str(), cycles());
#endif
}

void pdpaction::parse_time()
{
	expect("Time");
	last_cycle_ = current_cycle_;
	expect(&current_cycle_);
	skip(":");
}

void pdpaction::parse_pc()
{
	expect("PC=");
	expect(&pc_);
}

void pdpaction::parse_inst()
{
	expect("instruction");
	expect("=");
	expect(&inst_);
}

void pdpaction::parse_disassembly()
{
	expect("(");
	expect_asm(&inst_disa_);
	expect(")");
	skip(":");
}

rwaction pdpaction::parse_rw()
{
	string src, dst;
	expect(&src);
	expect("->");
	expect(&dst);
	skip(",");
	return rwaction(src, dst);
}

rwgroup* pdpaction::get_new_rwgroup()
{
#if VERBOSE
	fprintf(stderr, "Begin to push_back\n");
#endif

	rwg_array_.push_back(rwgroup());

#if VERBOSE
	fprintf(stderr, "Finish pushing_back\n"); 
#endif

	rwgroup* current = &rwg_array_.back();

#if VERBOSE
	fprintf(stderr, "Current rwgroup %p\n", current); 
#endif
	return current;
}

void pdpaction::parse_verbose()
{
#if STRICT_SEQUENCE
	rwg_array_.clear();
	rwgroup* current = get_new_rwgroup();
#else
	readg_ = rwgroup();
	writeg_ = rwgroup();
#endif
	while (true) {
		try {
			rwaction rwa = parse_rw();
#if STRICT_SEQUENCE
			if (!current->append(rwa)) {
#if VERBOSE
				fprintf(stderr, "Close a type %d group and create a new RWGroup\n",
						current->type());
#endif
				current = get_new_rwgroup();
				current->append(rwa);
			}
#else
			if (rwa.type == rwaction::READ) {
				readg_.append(rwa);
			} else {
				writeg_.append(rwa);
			}
#endif
		} catch (const line_terminated& plt) {
			break;
		}
	}
}

void pdpaction::expect(const char* e)
{
	skip_spaces();
	size_t n = strlen(e);
	if (strncmp(e, cur_, n))
		throw bad_syntax(cur_, e);
	cur_ += n;
#if VERBOSE
	fprintf(stderr, "Move to %s\n", cur_);
#endif
}

std::string asm_delim("(),:");

static bool isasmdelim(char c)
{
	for(size_t i = 0; i < asm_delim.size(); i++)
		if (c == asm_delim[i])
			return true;
	return false;
}

static bool ispdpdelim(char c)
{
	if (isspace(c))
		return true;
	return isasmdelim(c);
}

void pdpaction::expect(std::string* word)
{
	skip_spaces();
	const char* ptr = cur_;
	while (*ptr != '\0' && (!ispdpdelim(*ptr)))
		ptr++;
	word->assign(cur_, ptr - cur_);
	if (word->size() == 0)
		throw line_terminated();
	cur_ = ptr;
#if VERBOSE
	fprintf(stderr, "Move to %s\n", cur_);
#endif
}

void pdpaction::expect_asm(std::string* word)
{
	skip_spaces();
	const char* ptr = cur_;
	while (*ptr != '\0' && (!isasmdelim(*ptr)))
		ptr++;
	word->assign(cur_, ptr - cur_);
	if (word->size() == 0)
		throw line_terminated();
	cur_ = ptr;
#if VERBOSE
	fprintf(stderr, "Move to %s\n", cur_);
#endif
}

void pdpaction::expect(pdpword* number)
{
	skip_spaces();
	char* next_;
	unsigned long value = strtoul(cur_, &next_, 0);
	*number = value & 0xFFFF;
	cur_ = next_;
#if VERBOSE
	fprintf(stderr, "Move to %s\n", cur_);
#endif
}

void pdpaction::expect(unsigned long* number)
{
	skip_spaces();
	char* next_;
	unsigned long value = strtoul(cur_, &next_, 0);
	*number = value;
	cur_ = next_;
#if VERBOSE
	fprintf(stderr, "Move to %s\n", cur_);
#endif
}

void pdpaction::skip(const char* ss)
{
	try {
		expect(ss);
	} catch (const bad_syntax&) {
		// Don't care
	}
}

void pdpaction::skip_spaces()
{
	while (isspace(*cur_)) {
		//fprintf(stderr, "Skipping %c (offset: %ld)\n", *cur_, cur_-base_);
		cur_++;
	}
}
