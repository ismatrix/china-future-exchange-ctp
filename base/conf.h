/*
 * config.h
 *
 *  Created on: 2009-3-18
 *      Author: echo
 */

#ifndef UTIL_CONFIG_H_
#define UTIL_CONFIG_H_
#include <iostream>
#include <string.h>
#include <sstream>
#include <map>


using namespace std;
/**
 * @class config
 *
 * @brief load config information from a *.ini file.
 */
class Conf
{
public:
	/// constructor
	Conf(void);

	/// destructor
	~Conf(void);

	/**
	* load an config file.
	*
	* @param file_name  Path and name of the config file.
	*/
	bool load(const std::string& file_name);

	//parse configuration data
	bool parse(const std::string& data);

	/**
	* get a value in the specified field.
	*
	* @param section	from which field range you want read the name.
	* @param name		name of the value.
	* @param value		reference to the object in which you want store the value.
	* @param def		default value.
	*
	* @retval T&  The reference you pass in by @a Value.
	*/
	template<class T>
	T& get(const std::string& section, const std::string& name, T& value, const std::string& def = "")
	{
		bool using_default = true;
		std::string val;
		section_map_t::iterator it = config.find(section);
		if (it != config.end())
		{
			cnf_section_t::iterator subit = it->second.find(name);
			if (subit != it->second.end())
			{
				val = subit->second;
				using_default = false;
			}
		}
		if(using_default)
		{
			val = def;
		}
		std::stringstream sstream(val);
		sstream >> value;

		return value;
	}
	std::string& get(const std::string& section, const std::string& name, std::string& value, const std::string& def = "");
	char* get(const std::string& section, const std::string& name, char* value, const std::string& def = "");

private:
	bool load_data_from_stream(std::basic_istream<char>& stream);
private:
	typedef std::map< std::string, std::string > cnf_section_t;
	typedef std::map< std::string, cnf_section_t > section_map_t;
	section_map_t config;
};


#endif /* UTIL_CONFIG_H_ */
