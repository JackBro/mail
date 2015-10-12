#include <string>
#include <curl/curl.h>

class Loader {
public:
	Loader(const char *pipe_name);
	~Loader();
	void load(const std::string &url);

private:
	CURL *m_curl;
	const char *m_pipe_name;
	FILE *m_fl;
};
