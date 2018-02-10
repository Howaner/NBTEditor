#pragma once
#include <string>

namespace Exception {

	class Exception : public std::exception {
		
	public:
		explicit Exception(const char* message): msg(message)
		{
		}

		explicit Exception(const std::string& message): msg(message)
		{
		}

		virtual ~Exception() throw () {};

		const std::string& GetMessage() const { return msg; }

		virtual const char* what() const throw() {
			return msg.c_str();
		}

	protected:
		std::string msg;
	};

}