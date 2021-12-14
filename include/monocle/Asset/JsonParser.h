#ifndef MNCL_JSON_PARSER_H
#define MNCL_JSON_PARSER_H

#include <vector>
#include <istream>

namespace mncl {
namespace json {
	
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
			is_type       = 0x80000000,
			is_keyword    = 0x00000010,

			eof           = 0x80000000,
			number        = 0x80000001,
			string        = 0x80000002,

			keyword_true  = 0x80000010,
			keyword_false = 0x80000011,
			keyword_null  = 0x80000012,
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
	};

	







}
}


#endif