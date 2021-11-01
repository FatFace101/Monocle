#include "monocle/Asset/JsonParser.h"
#include <cctype>

using namespace mncl;
using namespace json;



AssetParseException::AssetParseException(const char* message, std::streampos position) : std::exception(message), position(position) {}
std::streampos AssetParseException::getPosition() const {return position;}




JsonParser::JsonParser() : input(nullptr), charCount(0), numberCoefficient(0), numberExponent(0), numberNegative(false), stringValue()  {
	lastChar = input->get();
}


bool JsonParser::advance() {
	int l = input->get();
	lastChar = l;
	return !(--charCount) || (l < 0);
}

// Advances file position and returns if either the end of file or json length is reached.
#define MNCL_ADVANCE_M(v) if (advance()) {return (int)v;}
// Throws an AssetParseException given an error message.
#define MNCL_ERR_M(t) throw new AssetParseException(t, input->tellg())
//Advances file position and throws an AssetParseException if either the end of file or json length is reached.
#define MNCL_ADVANCE_ERR_M(t) if (advance()) MNCL_ERR_M(t)


int JsonParser::nextToken() 
{
	if (!charCount || input->rdstate()) {
		return (int)TokenType::eof;
	}
	// Ignore leading whitespace
	while (std::isspace(lastChar)) 
		MNCL_ADVANCE_M(TokenType::eof);

	numberNegative = false;
	switch (lastChar) 
	{
	case '"':
		stringValue.clear();

		while (true) {
			MNCL_ADVANCE_ERR_M("Unclosed string");
			if (lastChar == '\"') {
				advance();
				break;
			}
			else if (lastChar == '\\') {
				MNCL_ADVANCE_ERR_M("Unclosed string");
				char v;
				switch (lastChar) {
				case 't':
					lastChar = '\t';
					break;
				case 'n':
					lastChar = '\n';
					break;
				case 'f':
					lastChar = '\f';
					break;
				case 'r':
					lastChar = '\r';
					break;
				case 'b':
					lastChar = '\b';
					break;
				case 'u':
					v = 0;
					for (int i = 0; i < 4; i++) {
						MNCL_ADVANCE_ERR_M("Unclosed string");
						unsigned char sub = lastChar - '0';
						if (sub >= 10) {
							sub -= ('A' - '0');
							if (sub >= 6) {
								sub -= ('A' - 'a');
								if (sub >= 6) {
									MNCL_ERR_M("Invalid hex character");
								}
							}
							sub += 10;
						}
						v = v << 4 | sub;
						printf("hex digit %i, total %i\n", sub, v);
					}
					lastChar = v;
					break;
				case '\\':
					lastChar = '\\';
					break;
				case '\/':
					lastChar = '/';
					break;
				default:
					MNCL_ERR_M("Invalid escape sequence");
				}
			}
			else if (lastChar < ' ') {
				MNCL_ERR_M("Control character in string");
			}
			stringValue.push_back(lastChar);
		}
		stringValue.push_back(0);
		return (int)TokenType::string;
	case '-':
		MNCL_ADVANCE_ERR_M("Expected digit, EOF found");
		
		numberNegative = true;

		if (!std::isdigit(lastChar))
			MNCL_ERR_M("Expected digit after \"-\"");
	default:

		// Single character tokens
		if (!std::isdigit(lastChar)) {
			char c = lastChar;
			advance();
			return c;
		}

		// ---- Parse Number ----

		numberExponent = 0;

		uint64_t tempv;

		if (lastChar != '0') 
		{
			tempv = lastChar - '0';
			// Parse whole part
			do {
				numberCoefficient = tempv;

				MNCL_ADVANCE_M(TokenType::number);

				unsigned char sub = lastChar - '0';

				if (sub >= 10) 
					break;

				tempv = numberCoefficient * 10 + sub;

			} while (tempv >= numberCoefficient);

			// Precision has possibly been exceeded, keep track of extra places in numberExponent
			while (std::isdigit(lastChar)) 
			{
				numberExponent++;
				MNCL_ADVANCE_M(TokenType::number);
			}
		}
		else
		{
			MNCL_ADVANCE_M(TokenType::number);
			if (std::isdigit(lastChar)) {
				MNCL_ERR_M("Leading zero(s) not allowed.");
			}
		}
		if (lastChar == '.')
		{
			MNCL_ADVANCE_ERR_M("D");
			if (!std::isdigit(lastChar))
			{
				MNCL_ERR_M("Expected digit after decimal point");
			}

			// Parse decimal part

			while (tempv >= numberCoefficient) {

				unsigned char sub = lastChar - '0';
				if (sub >= 10)
					break;

				numberExponent--;
				numberCoefficient = numberCoefficient * 10 + sub;

				MNCL_ADVANCE_M(TokenType::number);
			};

			// Discard extra decimal places
			while (std::isdigit(lastChar))
				MNCL_ADVANCE_M(TokenType::number);

		}
		if (lastChar == 'E' || lastChar == 'e')
		{
			MNCL_ADVANCE_ERR_M("Expected digit or sign, EOF found");
			bool expNegative = lastChar == '-';
			if (lastChar == '+' || expNegative) {
				MNCL_ADVANCE_ERR_M("Expected digit, EOF found");
			}
			if (!std::isdigit(lastChar)) {
				MNCL_ERR_M("Malformed exponent");
			}
			int ex = 0;
			do {
				unsigned char sub = lastChar - '0';
				if (sub >= 10)
					break;
				ex = ex * 10 + sub;
			} while (!advance());
			numberExponent += expNegative ? -ex : ex;
		}
		return (int)TokenType::number;
	}
}




void JsonParser::setStream(std::istream* input, uint64_t charCount = -1) {
	
}
