#include <unistd.h>
#include "FileReader.h"

uint32_t FileReader::BUFFERSIZE = 1024;

FileReader::FileReader(uint8_t* source)
{
	Init(nullptr, source);
}

FileReader::FileReader(std::string& filename)
{
	FILE* handle = fopen(filename.c_str(), "rb");
    if (handle == nullptr) {
        char temp[1024];
        getcwd(temp, sizeof(temp));
        fprintf(stderr, "failed to open handle for filename: %s. cwd: %s. return: %i\n", filename.c_str(), temp, errno);
    }
	Init(handle, nullptr);
}

FileReader::FileReader(const char* filename)
{
	FILE* handle = fopen(filename, "rb");
    if (handle == nullptr) {
        char temp[1024];
        getcwd(temp, sizeof(temp));
        fprintf(stderr, "failed to open handle for filename: %s. cwd: %s. return: %i\n", filename, temp, errno);
    }
	Init(handle, nullptr);
}

void FileReader::Init(FILE* handle, uint8_t* buffer)
{
	if (buffer == nullptr && handle == nullptr)
	{
		printf("\x1b[0;0Hno source");
		mHandle = nullptr;
		mBuffer = nullptr;
		
		fReady = false;
		return;
	}
	
	mBuffer = buffer;
	if (buffer == nullptr)
		mBuffer = new uint8_t[BUFFERSIZE];
	
	mHandle = handle;
	
	Reset();
	fReady = true;
}

FileReader::~FileReader()
{
	if (mHandle != nullptr)
	{
		fclose(mHandle);
	}
	
	delete[] mBuffer;
}

size_t FileReader::FillBuffer() const
{
	if (mHandle == nullptr || feof(mHandle))
		return -1;
	if (pos == 0)
		return 0;
	
	//shift remaining buffer
	memcpy(mBuffer, mBuffer+pos, BUFFERSIZE-pos);
	int bytesread = fread(mBuffer+BUFFERSIZE-pos, 1, pos, mHandle);
	pos = 0;
	return bytesread;
}

void FileReader::PrepareBuffer(uint8_t datasize) const
{
	//todo: handle eof
	if (mHandle != nullptr && BUFFERSIZE - datasize < pos)
		FillBuffer();
}

uint8_t FileReader::ReadInt8() const
{
	PrepareBuffer(1);
	return mBuffer[pos++];
}

uint16_t FileReader::ReadInt16() const
{
	PrepareBuffer(2);
	uint16_t t = mBuffer[pos++];
	t += mBuffer[pos++] << 8;
	return t;
}

uint32_t FileReader::ReadInt32() const
{
	PrepareBuffer(4);
	uint32_t t = mBuffer[pos++];
	t += mBuffer[pos++] << 8;
	t += mBuffer[pos++] << 16;
	t += mBuffer[pos++] << 24;
	return t;
}

float FileReader::ReadFloat() const
{
	PrepareBuffer(4);
	uint8_t c[4];
	c[0] = mBuffer[pos++];
	c[1] = mBuffer[pos++];
	c[2] = mBuffer[pos++];
	c[3] = mBuffer[pos++];
	return *reinterpret_cast<float*>(c);
}

// Read Integers of variable size, kind of like in midi
uint32_t FileReader::ReadVarInt() const
{
	PrepareBuffer(4);
	uint32_t value = 0;
	uint8_t i = 0;
	uint8_t b;
	do                                      // Bits in the file:            10010101 11000101 00100010
	{                                       // Bits in the Output: 00000000  0010101  1000101  0100010
		b = mBuffer[pos++];
		value += (b & 0x7F) << (7 * i);
		++i;
	} while ((b & 0x80) > 0);

	return value;
}


std::string FileReader::ReadString() const
{
	uint32_t l = ReadVarInt();
	if (l == 0)
		return "";
	
	PrepareBuffer(l);
	
	char* c = new char[l+1];
	for (uint32_t i=0; i<l; ++i)
		c[i] = mBuffer[pos++];
	c[l] = '\0';
	
	std::string s(c);
	return s;
}

void FileReader::ReadData(void* ptr, uint32_t size) const
{
    memcpy(ptr, &mBuffer[pos], size);
	pos += size;
}

void FileReader::Reset() const
{
	if (mHandle == nullptr)
	{
		pos = 0;
	}
	else
	{
		pos = BUFFERSIZE;
		rewind(mHandle);
	}
}

void FileReader::Skip(uint32_t count) const
{
	PrepareBuffer(count);
	pos += count;
}


