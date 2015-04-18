#ifndef TIGOLE_CONSOLE_H
#define TIGOLE_CONSOLE_H

#include <ostream>
#include <string>
#include <map>

#ifdef __WIN32
#include <Windows.h>
#endif // __WIN32

namespace tigole
{
enum ConsoleAttribute
{
	CONSOLE_INTENSE,
	CONSOLE_NO_INTENSE,
	CONSOLE_BLACK,
	CONSOLE_WHITE,
	CONSOLE_RED,
	CONSOLE_GREEN,
	CONSOLE_BLUE,
	CONSOLE_YELLOW,
	CONSOLE_MAGENTA,
	CONSOLE_CYAN
};

std::ostream& operator<<(std::ostream& o, ConsoleAttribute const& ca);
std::istream& operator>>(std::istream& i, ConsoleAttribute& ca);

class Console
{
public:
	Console();
	Console(Console const& c);
	~Console();

	Console& reset();
	Console& setAttribute(ConsoleAttribute ca);
	/** 10, 50 or 100 are good**/
	Console& setDelay(int millisec);

	template<typename T>
	Console& write(T const& t)
	{
		_out << t;
		return *this;
	}

	Console& write(std::string const& str);

	void demonstrate(std::string const& str = "This is an example.");

	static std::map<std::string, ConsoleAttribute> const& getMap();

private:

	Console& setColor(ConsoleAttribute ca);
	Console& setIntensity(bool b);
#ifndef _WIN32
	std::string colorToString(ConsoleAttribute ca);
#else
	WORD colorToWORD(ConsoleAttribute ca);
	void initialize(void);
#endif // _WIN32

	std::ostream& _out;
	ConsoleAttribute _color;
	int _delay;
#ifdef _WIN32
	void* _handle;
	WORD _default_attrib;
#endif // _WIN32

	static std::map<std::string, ConsoleAttribute> _map;
	static bool _already_exist;
};

template<typename T>
Console& operator<<(Console& c, T const& t)
{
	return c.write(t);
}

Console& operator<<(Console& c, std::string const& str);
Console& operator<<(Console& c, char const* str);
Console& operator<<(Console& c, int i);
Console& operator<<(Console& c, unsigned int i);
Console& operator<<(Console& c, short i);
Console& operator<<(Console& c, unsigned short i);
Console& operator<<(Console& c, long i);
Console& operator<<(Console& c, unsigned long i);
Console& operator<<(Console& c, char i);
Console& operator<<(Console& c, unsigned char i);

Console& operator<<(Console& c, ConsoleAttribute const& ca);

}


#endif // JAJA_CONSOLE_H
