
class Getter
{
public:
	Getter(const char *pipe_name, const char *url);

	void get();
private:
	const char *m_pipe_name;
	const char *m_url;


};
