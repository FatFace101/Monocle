#include "monocle/Asset/JsonParser.h"

#include <stack>
#include <map>
#include <cctype>

using namespace mncl;
using namespace asset;



const std::map<std::string, JsonParser::TokenType> tokenMap = {
	{"true", JsonParser::TokenType::keyword_true},
	{"false", JsonParser::TokenType::keyword_false},
	{"null", JsonParser::TokenType::keyword_null},
};

AssetParseException::AssetParseException(const char* message, std::streampos position) : std::exception(message), position(position) {}
std::streampos AssetParseException::getPosition() const {return position;}




JsonParser::JsonParser() : lastChar(0), input(nullptr), charCount(0), numberCoefficient(0), numberExponent(0), numberNegative(false), stringValue()  {
	
}


bool JsonParser::advance() {
	int l = input->get();
	lastChar = l;
	return !(--charCount) || (l < 0);
}

// Advances file position and returns if either the end of file or json length is reached.
#define MNCL_ADVANCE_M(v) if (advance()) {return (uint32_t)v;}
// Throws an AssetParseException given an error message.
#define MNCL_ERR_M(t) throw new AssetParseException(t, input->tellg())
//Advances file position and throws an AssetParseException if either the end of file or json length is reached.
#define MNCL_ADVANCE_ERR_M(t) if (advance()) MNCL_ERR_M(t)


uint32_t JsonParser::nextToken() 
{
	if (!charCount || input->rdstate()) {
		return (int)TokenType::eof;
	}
	// Ignore leading whitespace
	while (std::isspace(lastChar)) 
		MNCL_ADVANCE_M(TokenType::eof);

	numberNegative = false;


	if (lastChar == '\"') {
		// Parse string
		stringValue = std::string();
		while (true) {
			MNCL_ADVANCE_ERR_M("Unclosed string");
			if (lastChar == '\"') {
				advance();
				break;
			}
			else if (lastChar == '\\') {
				// Escape sequence
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
					}
					lastChar = v;
					break;
				case '\\':
					lastChar = '\\';
					break;
				case '/':
					lastChar = '/';
					break;
				default:
					MNCL_ERR_M("Invalid escape sequence");
				}
			}
			else if (lastChar < ' ') {
				MNCL_ERR_M("Control character in string");
			}
			stringValue += lastChar;
		}
		return (uint32_t)TokenType::string;
	}
	else if (std::isalpha(lastChar)) {
		stringValue = std::string();
		do {
			stringValue += lastChar;
		} while (!advance() && std::isalpha(lastChar));
		std::map<std::string, TokenType>::const_iterator iter = tokenMap.find(stringValue);

		if (iter != tokenMap.end()) 
			return (uint32_t)iter->second;

		MNCL_ERR_M("Unknown keyword found");
	}

	if (lastChar == '-') {
		MNCL_ADVANCE_ERR_M("Expected digit, EOF found");
		
		numberNegative = true;

		if (!std::isdigit(lastChar))
			MNCL_ERR_M("Expected digit after \"-\"");
	}
	else if (!std::isdigit(lastChar)) {
		// Single character tokens
		char c = lastChar;
		advance();
		return (uint32_t)c;
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
	return (uint32_t)TokenType::number;
}


void JsonParser::setStream(std::istream* input, uint64_t charCount) {
	this->input = input;
	this->charCount = charCount;
	lastChar = input->get();
}





void JsonParser::parse() {

	std::stack<ValueType> values;
	while (true) {
		uint32_t tok = nextToken();
		if (tok & (uint32_t)TokenType::is_special) {
			if (tok & (uint32_t)TokenType::is_keyword) {
				if (tok == (uint32_t)TokenType::keyword_null) {
					
				}
				else {
					// boolean
				}
			}
			else if (tok == (uint32_t)TokenType::string) {

			}
			else if (tok == (uint32_t)TokenType::number) {

			}
			
		}
		else {

		}
	}
}
