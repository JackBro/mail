#include <stdio.h>
//#include <curl/curl.h>

//#include <string>

#include "getter.h"
#include "excp.h"

//#include <errno.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <string.h>
//#include <fcntl.h>

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

	} catch (MchainException exp) {
		fprintf(stderr, "%s\n", exp.message());
	}
}
