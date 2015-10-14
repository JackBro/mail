#include "loader.h"
#include "excp.h"

#include <sstream>
#include <iostream>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

Loader::Loader(const char *pipe_name) : m_curl(NULL), m_pipe_name(pipe_name), m_fl(0)
{
	m_curl = curl_easy_init();
	unlink(m_pipe_name);

	if ((mkfifo(m_pipe_name, 0777)) == -1) {
		fprintf(stderr, "Could not make fifo %s error: %s\n", m_pipe_name, strerror(errno));
		throw MchainException("Could not make fifo\n");
	}
	
	m_fl = fopen(m_pipe_name, "w+");
	if (!m_fl) {
		fprintf(stderr, "Could not open fifo %s error: %s\n", m_pipe_name, strerror(errno));
		throw MchainException("Couln not open fifo\n");
	}
}

Loader::~Loader()
{
	if (m_fl)
		fclose(m_fl);
	if (m_curl)
		curl_easy_cleanup(m_curl);
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(buffer, size, nmemb, (FILE *)stream);
	return written;
}

void Loader::load(const std::string &url)
{
	curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_fl);
	//curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);

	CURLcode res;
	res = curl_easy_perform(m_curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "perform error: %d\n", res);
	}
}
