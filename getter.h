#include "loader.h"
#include "trainer.h"

class Getter
{
public:
	Getter(const char *pipe_name, unsigned chain_rate);
	~Getter();

	void get(const char *url);
private:
	const char *m_pipe_name;
	int m_fd;
	Loader m_loader;
	Trainer m_trainer;
};
