#include <stdio.h>

#include "getter.h"
#include "excp.h"


int main()
{
	try {
		Getter getter("mypipe", 3 /*mchain_rate*/); 
		printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
		getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter3.html");

		printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
		getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter1.html");

		printf("^^^^^^^^^^^^^ GET ^^^^^^^^^^^^^^^^^^\n");
		getter.get("http://london.sonoma.edu/writings/BeforeAdam/chapter2.html");

		getter.save("/tmp/mchain");
	} catch (MchainException exp) {
		fprintf(stderr, "%s\n", exp.message());
	}
}
