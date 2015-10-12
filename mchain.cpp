#include <stdio.h>
#include <curl/curl.h>

#include <string>

#include "loader.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	CURL *curl;

	curl = curl_easy_init();
	if (curl) {
		printf("Hello curl!\n");
		curl_easy_cleanup(curl);
	}
	printf("Hello!\n");

	Loader loader("mypipe");
	std::string res = loader.load("http://habrahabr.ru/post/155201/");

	int fd;
	char buf[255];
	fd = open("mypipe", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Could not open pipe\n");
		return 1;
	}

	if (read(fd, &buf, sizeof(buf)-1) == -1) {
		fprintf(stderr, "Could not read from pipe\n");
		return 1;
	}
	buf[254] = 0;
	printf((const char *)buf);
	return 0;
}
