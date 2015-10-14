#include <stdio.h>
#include <stdlib.h>

#include "getter.h"
#include "excp.h"


int main(int argc, char **argv)
{
	if (argc < 5) {
		printf("Invalid format. Usage: %s <N> <filename> <string> <M>\n", argv[0]);
		return 1;
	}

	int chain_rate = atoi(argv[1]);
	int words_count = atoi(argv[4]);

	printf("chain rate: %d\nfilename: %s\nstring: %s\nwords count to complete: %d\n", chain_rate, argv[2], argv[3], words_count);

	if (chain_rate < 1) {
		printf("Invalid chain rate\n");
		return 1;
	}
	if (words_count < 1) {
		printf("Invalid words count\n");
		return 1;
	}

	try {
		Getter getter("mypipe", 3 /*mchain_rate*/); 
		printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
		getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter3.html");

		printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
		getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter1.html");

		printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
		getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter2.html");

		getter.save(argv[2]);
		
		getter.load(argv[2]);

		std::string str;
		getter.complete(argv[3], words_count, str);
		printf("str: %s\n", str.c_str());

		
	} catch (MchainException exp) {
		fprintf(stderr, "%s\n", exp.message());
	}

	return 0;
}
