#include <stdio.h>
#include <stdint.h>
#include <string>
#include <getopt.h>

#include "loader.h"

void hexdump (uint8_t *buf, int len) {
    int i;
	char chars[20];

    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
			if (i != 0)
				printf("  %s\n", chars);
            printf ("  %08x ", i);
        }

        printf ("%02x", buf[i]); //, (char)buf[i]);
        if ((i % 2) == 1)
            printf (" ");

        if ((buf[i] < 0x20) || (buf[i] > 0x7e)){
            chars[i % 16] = '.';
        } else {
			chars[i % 16] = buf[i];
        }

        chars[(i % 16) + 1] = '\0';
    }

    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    printf ("  %s\n", chars);
}

int main(int argc, char *argv[])
{
    Binary bin;
    Section *sec;
    Symbol *sym;

	int opt;
	std::string fname;
	std::string dsection;


	while ((opt = getopt(argc, argv, "d:")) != -1) {
		switch (opt) {
		case 'd':
			dsection.assign(optarg);
			break;
		case '?':
			fprintf(stderr, "USAGE: %s [-d section] binary\n", argv[0]);
			return 1;
		}
	}

	if (optind >= argc) {
        fprintf(stderr, "USAGE: %s [-d section] binary\n", argv[0]);
        return 1;
    }

    fname.assign(argv[optind]);
    if (load_binary(fname, &bin, Binary::BIN_TYPE_AUTO) < 0) {
        return 1;
    }

    printf("loaded binary '%s' %s/%s (%u bits) entry@%016jx\n",
           bin.filename.c_str(),
           bin.type_str.c_str(), bin.arch_str.c_str(),
           bin.bits, bin.entry);

	if (! dsection.empty()) {
		int i;

		for (i = 0; i < bin.sections.size(); i++) {
			sec = &bin.sections[i];
			if (sec->name == dsection) {
				printf("dumping %s (%ju)\n", dsection.c_str(), sec->size);
				hexdump(sec->bytes, sec->size);
			}
		}
		if (i == bin.sections.size()) {
			printf("section %s not found!\n", dsection.c_str());
		}
	} else {
		for (auto i = 0; i < bin.sections.size(); i++) {
			sec = &bin.sections[i];
			printf("  0x%016jx %-8ju %-20s %s\n",
				   sec->vma, sec->size, sec->name.c_str(),
				   sec->type == Section::SEC_TYPE_CODE ? "CODE" : "DATA");
		}

		if (bin.symbols.size() > 0) {
			printf("scanned symbol table\n");
			for (auto i = 0; i < bin.symbols.size(); i++) {
				sym = &bin.symbols[i];
				printf("  %-40s 0x%016jx %s\n",
					   sym->name.c_str(), sym->addr,
					   (sym->type & Symbol::SYM_TYPE_FUNC) ? "FUNC" : "");
			}
		}
	}
    unload_binary(&bin);

    return 0;
}
