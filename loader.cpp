#include "loader.h"
#include <sstream>
#include <iostream>

Loader::Loader() : m_curl(NULL)
{
	m_curl = curl_easy_init();
}

Loader::~Loader()
{
	if (m_curl)
		curl_easy_cleanup(m_curl);
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *stream)
{
	std::string data((const char*)buffer, size*nmemb);
	*((std::stringstream*)stream) << data << std::endl;
	return size*nmemb;
}

std::string Loader::load(const std::string &url)
{
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	std::stringstream out;
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &out);
	curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);

	CURLcode res;
	res = curl_easy_perform(m_curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "perform error: %d\n", res);
	}
	
	return out.str();
}
