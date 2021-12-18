#ifndef MNCL_JSON_PARSER_H
#define MNCL_JSON_PARSER_H

#include <vector>
#include <istream>

namespace mncl {
namespace asset {
	
	// Runtime error with an associated stream position the error occurred at.
	class AssetParseException : public std::exception {
		std::streampos position;
	public:
		AssetParseException(const char* message, std::streampos position);
		std::streampos getPosition() const;
	};




	class JsonParser {
	public:
		enum class TokenType : uint32_t {
			is_special    = 0x80000000,
			is_keyword    = 0x00000010,

			eof           = 0x80000000,
			number        = 0x80000001,
			string        = 0x80000002,

			keyword_false = 0x80000010,
			keyword_true  = 0x80000011,
			keyword_null  = 0x80000012,
		};

		enum class ValueType {
			null    = 0,
			number  = 1,
			string  = 2,
			boolean = 3,
			object = 4,
			list = 5
		};

		class Acceptor {
			virtual Acceptor* AcceptArray();
			virtual Acceptor* AcceptKey(uint32_t length, const char* dat);
			virtual void AcceptNumber(uint64_t number, int exponent, bool negative);
			virtual void AcceptString(uint32_t length, const char* dat);
			int status;
		};

		std::istream* input;
		uint64_t charCount;

		char lastChar;
		

		uint64_t numberCoefficient;
		bool numberNegative;
		int numberExponent;
		std::string stringValue;

		// Update lastChar, returns true if eof or some equivalent has been reached.
		bool advance();


		uint32_t nextToken();

	public:
		JsonParser();
		void setStream(std::istream* input, uint64_t charCount = -1);
		void parse();
	};

	







}
}


#endif