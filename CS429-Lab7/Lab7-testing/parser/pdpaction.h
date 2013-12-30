#ifndef PDPACTION_H
#define PDPACTION_H

#include <string.h>
#include <vector>
#include "rwgroup.h" // Read Write group

class pdpstate;
typedef std::vector<rwgroup> rwg_array_t;

class pdpaction {
public:
	pdpaction();
	void parse(const char* lbuf);

	bool valid() { return valid_; }
	unsigned long cycles() const { return current_cycle_ - last_cycle_; }
	pdpword pc() const { return pc_; }
	pdpword inst() const { return inst_; }
	std::string disa() const { return inst_disa_; } 
#if STRICT_SEQUENCE
	rwg_array_t& get_rwgarray() { return rwg_array_; }
#else 
	rwgroup& get_readg() { return readg_; }
	rwgroup& get_writeg() { return writeg_; }
#endif
private:
	unsigned long current_cycle_;
	unsigned long last_cycle_;
	pdpword pc_;
	pdpword inst_;
	std::string inst_disa_;

	bool valid_;

	const char* base_;
	const char* cur_;

	void parse_time();
	void parse_pc();
	void parse_inst();
	void parse_disassembly();
	void parse_verbose();
	rwaction parse_rw();

#if STRICT_SEQUENCE
	rwg_array_t rwg_array_;
#else
	rwgroup readg_, writeg_;
#endif
	void expect(const char*);
	void expect(std::string*);
	void expect_asm(std::string*);
	void expect(pdpword*);
	void expect(unsigned long*);
	void skip(const char*);
	void skip_spaces();

	rwgroup* get_new_rwgroup();
};

#endif
