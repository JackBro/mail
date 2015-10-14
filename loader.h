#include <string>
#include <curl/curl.h>

// http downloader
class Loader {
public:
	// @pipe name - fifo filename to get data from curl wrapper
	Loader(const char *pipe_name);
	~Loader();

	// send content represented by @url to pipe
	void load(const std::string &url);

private:
	CURL *m_curl;
	const char *m_pipe_name;
	FILE *m_fl;
};
