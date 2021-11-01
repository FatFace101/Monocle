#ifndef MNCL_JSON_PARSER_H
#define MNCL_JSON_PARSER_H

#include <vector>
#include <map>
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



	class JsonValue {
		virtual void write(void* dest) = 0;
	};


	class JsonValueObject : public JsonValue {
		std::map<std::string, JsonValue*> values;
	};

	class JsonValueArray : public JsonValue {
		std::vector<JsonValue*> values;
	};

	class JsonValueNumber : public JsonValue {
		uint64_t numberCoefficient;
		bool numberNegative;
		int numberExponent;
	};

	class JsonValueString : public JsonValue {
		std::string value;
	};


	class JsonType {
		enum class RawType {
			boolean = 0,
			number = 1,
			string = 2,
			object = 3,
			array = 4,
			collection = 8,
		};
	};


	class JsonObjectType : public JsonValue {
		std::map<std::string, size_t> memberOffsets;
		size_t objectSize;
		void(*complete)(void* destination, void* out);
	};

	class JsonArrayType : public JsonValue {
		size_t elementSize;
	};





	class JsonParser {
	public:
		enum class TokenType {
			eof = -1,
			number = -2,
			string = -3
		};

		JsonType* expected;
		std::istream* input;
		uint64_t charCount;


		char lastChar;
		

		uint64_t numberCoefficient;
		bool numberNegative;
		int numberExponent;
		std::vector<char> stringValue;

		// Update lastChar, returns true if eof or some equivalent has been reached.
		bool advance();


		int nextToken();

	public:
		JsonParser();
		void setStream(std::istream* input, uint64_t charCount = -1);
	};

	







}
}


#endif