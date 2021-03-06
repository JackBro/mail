#include "trainer.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <cstdlib>

bool is_letter(char var)
{
	return ((var >= 65 && var <= 90) || (var >= 97 && var <= 122));
}

bool is_uppercase(char var)
{
	return (var >= 65 && var <=90);
}

char to_lowercase(char var)
{
	return var+32;
}

char *ltrim(char* ptr, unsigned size)
{
	while(ptr < ptr+size) {
		if (*ptr != ' ') {
			return ptr;		
		}
		ptr++;
	}
	return ptr;
}

Trainer::Trainer(unsigned chain_rate) : m_chain_rate(chain_rate) {};

void Trainer::add_chunk(char *buf, size_t size)
{
	char *ptr = buf;
	unsigned cnt = size;
	while(cnt--) {
		if(!is_letter(*ptr))
			*ptr = ' ';
		if (is_uppercase(*ptr)) {
			*ptr = to_lowercase(*ptr);			
		}
		ptr++;
	}

	ptr = buf;
	char *key_start = 0;
	char *key_end = 0;
	char *val_start = 0;
	char *next_key_start = 0;
	char *prev = 0;

	unsigned order = 0;
	cnt = 0;
	bool cnt_change = false;

	while (ptr < buf+size) {
		if (!key_start && *ptr != ' ') {
			key_start = ptr;
			cnt_change= true;
		}
		if (prev && *prev != ' ' && *ptr == ' '){
			order++;
			cnt++;
		}
		if (cnt_change && cnt == 1) {
			next_key_start = ptr;
			cnt = 0;
			cnt_change = false;
		}
		if (order == m_chain_rate) {
			key_start = ltrim(key_start, ptr-key_start);
			std::string str(key_start, ptr);
			// printf("key: %s ", str.c_str());
			order = 0;
			cnt = 0;

			char *val = ptr;

			while (val < buf + size) {
				if (!val_start && *val != ' ') {
					val_start = val;
				}
				if (val_start && *val == ' ') {
					std::string value(val_start, val);

					m_mchain.insert(std::pair<std::string, std::string>(str, value));

			//		printf(" val: %s \n", value.c_str());
					val_start = 0;
					break;
				}
				val++;
			}

			key_start = next_key_start;
			cnt_change= true;
			ptr = key_start;

		}

		prev = ptr;
		ptr++;

	}
	
	//print_mchain();
}

void Trainer::print_mchain()
{
	for (std::multimap<std::string, std::string>::iterator it = m_mchain.begin(); it != m_mchain.end(); ++it)
	printf("key: %s \tvalue: %s\n", (*it).first.c_str(), (*it).second.c_str());
}

void Trainer::save(const char *mchain_file)
{
	std::ofstream file;
	file.open(mchain_file);
	if (file.is_open()) {
		for (std::multimap<std::string, std::string>::iterator it = m_mchain.begin(); it != m_mchain.end(); ++it) {
		file << (*it).first << ":" << (*it).second << std::endl;
		}
	file.close();
	} else {
		fprintf(stderr, "Could not open file %s\n", mchain_file);
	}

	//get(/*mchain_file*/"/tmp/trainer.cpp");

}

void Trainer::get(const char *mchain_file)
{
	//printf("%s\n", __func__);
	std::ifstream file;
	file.open(mchain_file);

	char key[1024];
	char value[255];
	if (file.is_open()) {
		//printf("%s\n", mchain_file);
		m_mchain.clear();
		
		while(file.getline(key, sizeof(key), ':')) {
			//printf("##key: %s\n", key);
			file.getline(value, sizeof(value));
			//printf("##value: %s\n", value);

			m_mchain.insert(std::pair<std::string, std::string>(key, value));
		}
		//print_mchain();

		file.close();
	} else {
		fprintf(stderr, "Could not open file %s\n", mchain_file);
	}

}

bool Trainer::complete(const char *str, unsigned words_cnt, std::string &out)
{
	std::string key(str);
	unsigned count, shift;

	std::pair <std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> ret;
	std::multimap<std::string, std::string>::iterator it; 

	bool is_first = true;

	while(words_cnt--) {

		count = m_mchain.count(key.c_str());

		if (!count)
			return false;


		srand(time(NULL));
		shift = rand() % count;
	
		if (count == 1)
			shift = 0;	

		//printf("shift: %d\n", shift);

		ret = m_mchain.equal_range(key);

		unsigned delta = 0;

		for (it = ret.first; it != ret.second; ++it, delta++) {
			if (delta == shift) {
			//	printf("first: %s second: %s\n", it->first.c_str(), it->second.c_str());
				break;
			}
		}

		unsigned space_pos = 0;
		while(space_pos < it->first.size() - 1) {
			if (*(it->first.begin()+space_pos) == ' ')
				break;
			space_pos++;
		}

		//printf("space_pos: %d\n", space_pos);	
		key.clear();
		if (m_chain_rate > 1) {
			key.append(it->first.begin() + space_pos + 1, it->first.end());
			key.append(" ");
		}
		key.append(it->second);


		if (is_first)
			out.append(it->first);

		out.append(" ");
		out.append(it->second);

		//printf("out: %s\n", out.c_str());

		is_first = false;

		//printf("~~~ new key: %s\n\n", key.c_str());
	}

	return true;
}
