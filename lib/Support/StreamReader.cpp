#include "monocle/Support/StreamReader.h"

#include <cstring>
#include <algorithm>

using namespace mncl;
StreamReader *StreamReader::OpenFile(const char *filename, unsigned int bufferSize) {
	FILE* file = fopen(filename, "rb");
	if (file == nullptr) {
		std::perror("Oh no, file error");
		return nullptr;
	}
	if (bufferSize < 8) {
		
		bufferSize = 8;
	}
	setvbuf(file, nullptr, _IONBF, 0);
	
	return new StreamReader(file, bufferSize);
}

StreamReader::StreamReader(FILE* file, unsigned int bufferSize) : input(file), buffer(new unsigned char[bufferSize]), bufferCapacity(bufferSize), bufferSize(0), bufferPosition(0) {
	reloadBuffer();
}

StreamReader::~StreamReader() {
	fclose(input);
	delete buffer;
}

void StreamReader::fillBuffer() {
	unsigned int transfer = bufferSize - bufferPosition;
	std::memmove(buffer, buffer+bufferPosition, transfer);
	bufferSize = transfer + (unsigned int)fread(buffer + transfer, sizeof(unsigned char), bufferPosition, input);
	bufferPosition = 0;
}

void StreamReader::reloadBuffer() {
	bufferSize = (unsigned int)fread(buffer, sizeof(unsigned char), bufferCapacity, input);
	bufferPosition = 0;
}

bool StreamReader::ensureData(unsigned int data) {
	if ((bufferPosition + data) > bufferSize) {
		fillBuffer();
		return bufferSize > data;
	}
	return true;
}


bool StreamReader::readChar(uint8_t *value) {
	if (ensureData(1)) {
		*value = buffer[bufferPosition++];
		return true;
	}
	return false;
};
bool StreamReader::readShort(uint_least16_t *value) {
	if (ensureData(2)) {
		uint16_t out = (buffer[bufferPosition++] << 8);
		*value = out | buffer[bufferPosition++];
		return true;
	}
	return false;
};
bool StreamReader::readInt(uint32_t *value) {
	if (ensureData(4)) {
		uint32_t out = buffer[bufferPosition++] << 8;
		out = (out | buffer[bufferPosition++]) << 8;
		out = (out | buffer[bufferPosition++]) << 8;
		*value = out | buffer[bufferPosition++];
		return true;
	}
	return false;
};
bool StreamReader::readLong(uint64_t *value) {
	if (ensureData(8)) {
		uint64_t out = buffer[bufferPosition++];
		unsigned int ebp = bufferPosition + 8;
		while (bufferPosition < ebp) {
			out = (out | buffer[bufferPosition++]) << 8;
		}
		*value = out;
		return true;
	}
	return false;
};
bool StreamReader::readFloat(float* value) {
	return readInt((uint32_t*)value);
}
bool StreamReader::readDouble(double* value) {
	return readLong((uint64_t*)value);
}

bool StreamReader::readShortLE(uint_least16_t *value) {
	if (ensureData(2)) {
		unsigned int rPos = bufferPosition += 2;
		uint16_t out = (buffer[rPos--] << 8);
		*value = out | buffer[rPos--];
		return true;
	}
	return false;
};
bool StreamReader::readIntLE(uint32_t *value) {
	if (ensureData(4)) {
		unsigned int rPos = bufferPosition += 4;
		uint32_t out = buffer[--rPos] << 8;
		out = (out | buffer[--rPos]) << 8;
		out = (out | buffer[--rPos]) << 8;
		*value = out | buffer[--rPos];
		return true;
	}
	return false;
};
bool StreamReader::readLongLE(uint64_t *value) {
	if (ensureData(8)) {
		unsigned int rPos = bufferPosition + 8;
		uint64_t out = buffer[--rPos];
		unsigned int ebp = bufferPosition + 8;
		while (rPos > bufferPosition) {
			out = (out | buffer[--rPos]) << 8;
		}
		*value = out;
		bufferPosition += 8;
		return true;
	}
	return false;
};
bool StreamReader::readFloatLE(float* value) {
	return readIntLE((uint32_t*)value);
}
bool StreamReader::readDoubleLE(double* value) {
	return readLong((uint64_t*)value);
}

unsigned int StreamReader::readBlock(unsigned char *block, unsigned int amount) {
	unsigned int transfer = std::min(bufferSize - bufferPosition, amount);
	std::memcpy(block, buffer+bufferPosition, transfer);
	bufferPosition += transfer;
	if (transfer < amount) {
		unsigned int out = transfer + (unsigned int)fread(block+transfer,sizeof(unsigned char), amount - transfer, input);
		reloadBuffer();
		return out;
	}
	return transfer;
}

bool StreamReader::skipBlock(unsigned int amount) {
	unsigned int bufferSkip = std::min(bufferSize - bufferPosition, amount);
	bufferPosition += bufferSkip;
	if (bufferSkip < amount) {
		return jumpRelative(amount - bufferSkip);
	}
	return true;
}

bool StreamReader::jump(long position) {
	if ((fseek(input, position, SEEK_SET) == 0)) {
		if (feof(input) == 0) {
			reloadBuffer();
			return true;
		}
	}
	bufferPosition = bufferCapacity;
	return false;
}

bool StreamReader::jumpRelative(long offset) {
	if ((fseek(input, offset, SEEK_CUR) == 0)) {
		if (feof(input) == 0) {
			reloadBuffer();
			return true;
		}
	}
	bufferPosition = bufferSize;
	return false;
}