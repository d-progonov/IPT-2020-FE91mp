/**
 *  Copyright (c) DDE Lab, SUNY Binghamton
 *  Written by Tomas Pevny, e-mail: pevnak@gmail.com
 *
 *  @author Tomas Pevny
 */

#ifndef TEXCEPTION_H
#define TEXCEPTION_H

#include <string>

/** TException is simple class for sending exceptions. */
class TException {
	public:
		TException(std::string s):message(s) {}; ///< Stores the message
		~TException() {};    ///< do nothing

		std::string getMessage() {return message;}
		;   ///< returns message

		std::string what() {return message;}
		; ///< returns message

	private:
		std::string message; ///< message

	};

#endif // TEXCEPTION_H
