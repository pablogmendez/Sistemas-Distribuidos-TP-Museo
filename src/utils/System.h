#ifndef SYSTEM_H
#define SYSTEM_H

#include <ctime>
#include <exception>
#include <vector>
#include <errno.h>
#include <string>
#include <string.h>
#include <unistd.h>

class SystemErrorException : public std::exception
{
	private:
		int _number;

	public:
		SystemErrorException () throw ();
		SystemErrorException (int err) throw ();

		virtual ~SystemErrorException () throw () { }

		virtual const char *what() const throw()
		{
			return strerror (this->_number);
		}

		int number () const { return _number; }
};

class System
{
	public:
		static pid_t spawn (const char *file, char* const argv[]);
		static pid_t spawn (const char* file, std::vector<const char*>& args);

		static void semrm (const std::string& archivo, char letra);
		static void shmrm (const std::string& archivo, char letra);
		static void mqrm (const std::string& archivo, char letra);

		template <typename T>
		static void check (T err)
		{
			if (err == (T)-1) throw SystemErrorException ();
		}

		static void millisleep (unsigned long millis)
		{
			struct timespec per;
			per.tv_sec = millis / 1000;
			per.tv_nsec = (millis % 1000) * 1000000;
			nanosleep (&per, NULL);
		}
};

#endif
