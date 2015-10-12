#include <stdio.h>
#include <curl/curl.h>

int main()
{
	CURL *curl;

	curl = curl_easy_init();
	if (curl) {
		printf("Hello curl!\n");
		curl_easy_cleanup(curl);
	}
	printf("Hello!\n");
	return 0;
}
