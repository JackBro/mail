#include <string>
#include <curl/curl.h>

class Loader {
public:
	Loader();
	~Loader();
	std::string load(const std::string &url);

private:
	CURL *m_curl;
};
