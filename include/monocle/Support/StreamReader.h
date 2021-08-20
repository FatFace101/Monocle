#ifndef MONOCLE_STREAM_READER_H
#define MONOCLE_STREAM_READER_H

#include <stdio.h>
#include <stdint.h>

namespace mncl {
	
	class StreamReader {
	private:
		FILE *input;
		unsigned char* buffer;
		unsigned int bufferCapacity;
		unsigned int bufferSize;
		unsigned int bufferPosition;

		StreamReader(FILE *input, unsigned int bufferSize);

		~StreamReader();

		void fillBuffer();

		void reloadBuffer();
	public:
		StreamReader(const StreamReader&) = delete;
		StreamReader(StreamReader&&) = delete;



		bool readChar(uint8_t *value);
		bool readShort(uint_least16_t *value);
		bool readInt(uint32_t *value);
		bool readLong(uint64_t *value);
		bool readFloat(float* value);
		bool readDouble(double* value);

		bool readShortLE(uint_least16_t *value);
		bool readIntLE(uint32_t *value);
		bool readLongLE(uint64_t *value);
		bool readFloatLE(float* value);
		bool readDoubleLE(double* value);

		unsigned int readBlock(unsigned char *block, unsigned int amount);

		bool jump(long position);

		bool jumpRelative(long offset);

		bool skipBlock(unsigned int amount);

		bool ensureData(unsigned int data);

		bool isEOF() {return feof(input);}

		operator bool() {return !feof(input);}

		static StreamReader *OpenFile(const char *filename, unsigned int bufferSize);
		
	};

};




#endif