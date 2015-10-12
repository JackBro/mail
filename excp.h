#include <string>

class MchainException
{
public:
	MchainException(const std::string &msg)
	{
		m_msg = msg;
	}

	const char *message() const
	{
		return m_msg.c_str();
	}
private:
	std::string m_msg;
};
