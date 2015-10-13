#include <map>
#include <string>

// Markov chain creator
class Trainer
{
public:
	Trainer(unsigned chain_rate);
	//~Trainer();
	
	// analyze @buf content for adding into markov chain
	void add_chunk(char *buf, size_t size);

	// save markov chain into file
	//void save(const char *mchain_file);
private:
	// markov chain
	std::multimap<std::string, std::string> m_mchain;
	// chain order - the words' count in key
	unsigned m_chain_rate;
};
