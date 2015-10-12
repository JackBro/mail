#include <stdio.h>
#include <curl/curl.h>

#include <string>

#include "loader.h"

int main()
{
	CURL *curl;

	curl = curl_easy_init();
	if (curl) {
		printf("Hello curl!\n");
		curl_easy_cleanup(curl);
	}
	printf("Hello!\n");

	Loader loader;
	std::string res = loader.load("http://habrahabr.ru/post/155201/");
	printf(res.c_str());
	return 0;
}
