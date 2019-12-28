#include <string>

// When we get a bad token, we assign it a non-negative
// number. I know this is somewhat backwards, but this
// works very well for enums.
enum Token {
	tok_eof = -1,

	// Commands. def is for functions we define inside
	// a kaleidescope file, and extern for those defined
	// in the standard library.
	tok_def = -2,
	tok_extern = -3,

	// primary token types
	tok_identifier = -4,
	tok_number = -5,
};

// Filled in if the token we read is an identifier.
static std::string identifier;
// Filled in if the token is a number. All numbers are
// doubles in this language.
static double numVal;
