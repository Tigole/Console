#include "Console.h"

#include <iostream>
#include <sstream>

#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#include <Wincon.h>
#endif // _WIN32


namespace tigole
{

std::ostream& operator<<(std::ostream& o, ConsoleAttribute const& ca)
{
	std::map<std::string, ConsoleAttribute> const& m = Console::getMap();
	std::map<std::string, ConsoleAttribute>::const_iterator it;

	for (it = m.begin(); it != m.end(); it++)
	{
		if (it->second == ca)
		{
			o << it->first;
			return o;
		}
	}
	return o;
}

std::istream& operator>>(std::istream& i, ConsoleAttribute& ca)
{

	std::map<std::string, ConsoleAttribute> const& m = Console::getMap();
	std::map<std::string, ConsoleAttribute>::const_iterator it;
	std::string str;

	i >> str;

	it = m.find(str);
	if (it != m.end())
	{
		ca = it->second;
	}

	return i;
}

bool Console::_already_exist(false);
std::map<std::string, ConsoleAttribute> Console::_map;

Console::Console() : _out(std::cout), _color(CONSOLE_WHITE), _delay(0)
{
	/*_out << "[Console::Console] " << this << "\n";
	_out << "[Console::Console] " << &_out << "\n";*/
	if (!_already_exist)
	{
		_map["CONSOLE_BLACK"] = CONSOLE_BLACK;
		_map["CONSOLE_BLUE"] = CONSOLE_BLUE;
		_map["CONSOLE_CYAN"] = CONSOLE_CYAN;
		_map["CONSOLE_GREEN"] = CONSOLE_GREEN;
		_map["CONSOLE_INTENSE"] = CONSOLE_INTENSE;
		_map["CONSOLE_MAGENTA"] = CONSOLE_MAGENTA;
		_map["CONSOLE_NO_INTENSE"] = CONSOLE_NO_INTENSE;
		_map["CONSOLE_RED"] = CONSOLE_RED;
		_map["CONSOLE_WHITE"] = CONSOLE_WHITE;
		_map["CONSOLE_YELLOW"] = CONSOLE_YELLOW;
		_already_exist = true;
	}
#ifdef _WIN32
    initialize();
#endif // _WIN32
}

Console::Console(Console const& c) : _out(c._out), _color(c._color), _delay(c._delay)
{
}

#ifdef _WIN32
void Console::initialize(void)
{
    CONSOLE_SCREEN_BUFFER_INFO infos;
    _handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(_handle, &infos);
    _default_attrib = infos.wAttributes;
}
#endif // _WIN32

Console::~Console()
{
	reset();
}

Console& Console::reset()
{
#ifndef _WIN32
	_out << "\033[0m";
#else

    SetConsoleTextAttribute(_handle, _default_attrib);

#endif // _WIN32
	return *this;
}

Console& Console::setAttribute(ConsoleAttribute ca)
{
	if (ca == CONSOLE_INTENSE)
		setIntensity(true);
	else if (ca == CONSOLE_NO_INTENSE)
		setIntensity(false);
	else
		setColor(ca);
	return *this;
}

Console& Console::setIntensity(bool b)
{
	if (b)
	{
#ifndef _WIN32
		_out << "\033[1m";
#else
		SetConsoleTextAttribute(_handle, 0);
#endif // _WIN32
	}
	else
	{
		reset();
		setColor(_color);
	}
	return *this;
}

std::map<std::string, ConsoleAttribute> const& Console::getMap()
{
	return _map;
}

Console& Console::write(std::string const& str)
{
	if (_delay <= 0)
		_out << str;
	else
		for (unsigned int i = 0; i < str.size(); i++)
		{
			_out << str[i] << std::flush;
#ifndef _WIN32
			usleep((unsigned int)_delay*1000);
#else
			Sleep(_delay);
#endif // _WIN32
		}
	return *this;
}

Console& Console::setDelay(int millisec)
{
	_delay = millisec;
	return *this;
}

void Console::demonstrate(std::string const& str)
{
	unsigned int delay[4] = {100, 50, 25, 10};
	Console& c(*this);

	for (unsigned int i = 0; i < 4; i++)
	{
		c.setDelay(delay[i]);
		reset();
		c << "Delay of " << delay[i] << "ms\n";
		c << CONSOLE_BLACK << str << "\t";
		c << CONSOLE_BLUE << str << "\t";
		c << CONSOLE_CYAN << str << "\n";
		c << CONSOLE_GREEN << str << "\t";
		c << CONSOLE_MAGENTA << str << "\t";
		c << CONSOLE_RED << str << "\n";
		c << CONSOLE_YELLOW << str << "\t";
		c << CONSOLE_WHITE << str << "\n";
		c << CONSOLE_INTENSE;
		c << CONSOLE_BLACK << str << "\t";
		c << CONSOLE_BLUE << str << "\t";
		c << CONSOLE_CYAN << str << "\n";
		c << CONSOLE_GREEN << str << "\t";
		c << CONSOLE_MAGENTA << str << "\t";
		c << CONSOLE_RED << str << "\n";
		c << CONSOLE_YELLOW << str << "\t";
		c << CONSOLE_WHITE << str << "\t";
		c << CONSOLE_NO_INTENSE << "\n";
	}
	c.reset();
}

Console& Console::setColor(ConsoleAttribute ca)
{

#ifndef _WIN32
	std::string str(colorToString(ca));

	_color = ca;
	_out << str;
#else
	WORD word(colorToWORD(ca));

	SetConsoleTextAttribute(_handle, word);
#endif // _WIN32

	return *this;
}

#ifndef _WIN32
std::string Console::colorToString(ConsoleAttribute ca)
{
	std::string str("\033[");
	switch (ca)
	{
	case CONSOLE_BLACK:
		str += "30";
		break;
	case CONSOLE_WHITE:
		str += "37";
		break;
	case CONSOLE_RED:
		str += "31";
		break;
	case CONSOLE_GREEN:
		str += "32";
		break;
	case CONSOLE_BLUE:
		str += "34";
		break;
	case CONSOLE_YELLOW:
		str += "33";
		break;
	case CONSOLE_MAGENTA:
		str += "35";
		break;
	case CONSOLE_CYAN:
		str += "36";
		break;
	}

	str += "m";
	return str;
}

#else

WORD Console::colorToWORD(ConsoleAttribute ca)
{
    CONSOLE_SCREEN_BUFFER_INFO screen_infos;
	WORD word, flag_instensity;
	GetConsoleScreenBufferInfo(_handle, &screen_infos);

	word = screen_infos.wAttributes;

	flag_instensity = word & FOREGROUND_INTENSITY;

	switch(ca)
	{
	case CONSOLE_BLACK:
		word = 0;
		break;
	case CONSOLE_WHITE:
		word = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	case CONSOLE_RED:
		word = FOREGROUND_RED;
		break;
	case CONSOLE_GREEN:
		word = FOREGROUND_GREEN;
		break;
	case CONSOLE_BLUE:
		word = FOREGROUND_BLUE;
		break;
	case CONSOLE_YELLOW:
		word = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case CONSOLE_MAGENTA:
		word = FOREGROUND_RED | FOREGROUND_BLUE;
		break;
	case CONSOLE_CYAN:
		word = FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	}

	word |= flag_instensity;

	return word;
}

#endif // _WIN32

Console& operator<<(Console& c, std::string const& str)
{
	return c.write(str);
}

Console& operator<<(Console& c, ConsoleAttribute const& ca)
{
	return c.setAttribute(ca);
}

Console& operator<<(Console& c, char const* str)
{
	c << std::string(str);
	return c;
}

Console& operator<<(Console& c, int i)
{
	std::stringstream ss;
	ss << i;
	return c.write(ss.str());
}

Console& operator<<(Console& c, unsigned int i)
{
	std::stringstream ss;
	ss << i;
	return c.write(ss.str());
}

Console& operator<<(Console& c, short i)
{
	std::stringstream ss;
	ss << i;
	return c.write(ss.str());
}

Console& operator<<(Console& c, unsigned short i)
{
	std::stringstream ss;
	ss << i;
	return c.write(ss.str());
}

Console& operator<<(Console& c, long i)
{
	std::stringstream ss;
	ss << i;
	return c.write(ss.str());
}

Console& operator<<(Console& c, unsigned long i)
{
	std::stringstream ss;
	ss << i;
	return c.write(ss.str());
}

Console& operator<<(Console& c, char i)
{
	std::stringstream ss;
	ss << i;
	return c.write(ss.str());
}

Console& operator<<(Console& c, unsigned char i)
{
	std::stringstream ss;
	ss << i;
	return c.write(ss.str());
}

}
