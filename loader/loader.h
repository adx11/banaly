#pragma once

#include <stdint.h>
#include <string>
#include <vector>

class Symbol;
class Section;
class Binary;

class Symbol {
public:
	enum SymbolType {
		SYM_TYPE_UKN = 0,
		SYM_TYPE_FUNC,
	};

	Symbol() : type(SYM_TYPE_UKN), name(), addr(0) {}

	SymbolType	type;
	std::string name;
	uint64_t	addr;
};

class Section {
public:
	enum SectionType {
		SEC_TYPE_NONE = 0,
		SEC_TYPE_CODE,
		SEC_TYPE_DATA,
	};

	Section() : binary(NULL), type(SEC_TYPE_NONE),
				vma(0), size(0), bytes(NULL) {}

	bool contains(uint64_t addr) { return (addr >= vma) && (addr-vma < size); }

	Binary *binary;
	std::string name;
	SectionType type;
	uint64_t vma;
	uint64_t size;
	uint8_t *bytes;
};

class Binary {
public:
	enum BinaryType {
		BIN_TYPE_AUTO = 0,
		BIN_TYPE_ELF,
		BIN_TYPE_PE,
	};

	enum BinaryArch {
		ARCH_NONE = 0,
		ARCH_X86,
	};

	Binary() : type(BIN_TYPE_AUTO), arch(ARCH_NONE), bits(0), entry(0) {}

	Section *get_tyest_section()
	{ for(auto &s : sections) if (s.name == ".text") return &s; return NULL; }

	std::string filename;
	BinaryType type;
	std::string type_str;
	BinaryArch arch;
	std::string arch_str;
	unsigned bits;
	uint64_t entry;
	std::vector<Section> sections;
	std::vector<Symbol> symbols;
};

int load_binary(std::string &fname, Binary *bin, Binary::BinaryType type);
void unload_binary(Binary *bin);