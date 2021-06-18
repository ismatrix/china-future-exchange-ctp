#include <fstream>
#include "conf.h"



void  Trim(char *szString)
{
    int iLen;
    int n, iLeftStart;

    iLen = strlen(szString);
    if (iLen == 0)
        return;

    /* Trim right first */
    for (n = iLen - 1; n >= 0; --n)
    {
        // Trim space and tab
        if (szString[n] == ' ' || szString[n] == '\t' || szString[n] == '\r' || szString[n] == '\n')
            szString[n] = 0;
        else
            break;
    }

    iLen = strlen(szString);
    if (iLen == 0)
        return;

    /* Trim left */
    for (n = 0; n < iLen; ++n)
    {
        if (szString[n] != ' ' && szString[n] != '\t' && szString[n] != '\r' && szString[n] != '\n')
            break;
    }

    /* no space at the left */
    if (n == 0)
        return;

    iLeftStart = n;

    /* plus NULL */
    memmove(szString, &(szString[iLeftStart]), iLen - iLeftStart + 1);
}

void TrimStr(string&  in)
{
	char* p = new char[in.size() + 1];
	
	strncpy(p, in.c_str(), in.size());
	p[in.size()] = 0;
	Trim(p);
	in = p;

	delete p;
}

Conf::Conf(void)
{
}

Conf::~Conf(void)
{
}

bool Conf::load(const std::string& file_name)
{
	std::ifstream ifs;
	ifs.open(file_name.c_str(), std::ios::in);
	if (ifs.fail())
		return false;
	return load_data_from_stream(ifs);
}
bool Conf::parse(const std::string& data)
{
	std::stringstream ss;
	ss.str(data);
	return load_data_from_stream(ss);
}
std::string& Conf::get(const std::string& section, const std::string& name, std::string& value, const std::string& def)
{
	bool using_default = true;
	section_map_t::iterator it = config.find(section);
	if (it != config.end())
	{
		cnf_section_t::iterator subit = it->second.find(name);
		if (subit != it->second.end())
		{
			value = subit->second;
			using_default = false;
		}
	}
	if(using_default)
	{
		cout << "[" << section << "]\"" << name << "\" not found, default \"" << def << "\" used." << endl;
		value = def;
	}
	return value;
}
char* Conf::get(const std::string& section, const std::string& name, char* value, const std::string& def)
{
	bool using_default = true;
	section_map_t::iterator it = config.find(section);
	if (it != config.end())
	{
		cnf_section_t::iterator subit = it->second.find(name);
		if (subit != it->second.end())
		{
			memcpy(value, subit->second.c_str(), subit->second.size());
			value[subit->second.size()] = '\0';
			using_default = false;
		}
	}
	if(using_default)
	{
		cout << "[" << section << "]\"" << name << "\" not found, default \"" << def << "\" used." << endl;
		memcpy(value, def.c_str(), def.size());
		value[def.size()] = '\0';
	}
	return value;
}
bool Conf::load_data_from_stream(std::basic_istream<char>& ifs)
{
	std::string app;
	std::string name;
	std::string value;
	std::string line;
	while (getline(ifs,line))
	{
		if (line.empty())
		{
			continue;
		}

		unsigned int i = 0;
		TrimStr(line);

		char ch = line[i];
		if('#'!=ch && ';'!=ch)
		{
			bool good = true;
			if('['==ch)
			{
				std::string::size_type  j = line.find(']', i);
				if(std::string::npos != j)
				{
					app = line.substr(i+1, j-i-1);
					TrimStr(app);
					if (app.empty())
						good = false;
				}
				else
					good = false;
			}
			if(good)
			{
				unsigned int j = (unsigned int)line.find('=', i);
				if(j > i)
				{
					name = line.substr(i, j-i);
					TrimStr(name);
					if(!name.empty())
					{
						value = line.substr(j+1);
						TrimStr(value);
						config[app][name] = value;
					}
				}
			}
		}
	}
	return true;
}
