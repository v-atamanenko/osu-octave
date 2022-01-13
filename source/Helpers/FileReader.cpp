#include "FileReader.h"

u32 FileReader::BUFFERSIZE = 1024;

FileReader::FileReader(u8* source)
{
	Init(NULL, source);
}

FileReader::FileReader(std::string& filename)
{
	FILE* handle = fopen(filename.c_str(), "rb");
	Init(handle, NULL);
}

FileReader::FileReader(const char* filename)
{
	FILE* handle = fopen(filename, "rb");
	Init(handle, NULL);
}

void FileReader::Init(FILE* handle, u8* buffer)
{
	if (buffer == NULL && handle == NULL)
	{
		iprintf("\x1b[0;0Hno source");
		mHandle = NULL;
		mBuffer = NULL;
		
		fReady = false;
		return;
	}
	
	mBuffer = buffer;
	if (buffer == NULL)
		mBuffer = new u8[BUFFERSIZE];
	
	mHandle = handle;
	
	Reset();
	fReady = true;
}

FileReader::~FileReader()
{
	if (mHandle != NULL)
	{
		fclose(mHandle);
	}
	
	if (mBuffer != NULL)
	{
		delete[] mBuffer;
	}
}

int FileReader::FillBuffer() const
{
	if (mHandle == NULL || feof(mHandle))
		return -1;
	if (pos == 0)
		return 0;
	
	//shift remaining buffer
	memcpy(mBuffer, mBuffer+pos, BUFFERSIZE-pos);
	int bytesread = fread(mBuffer+BUFFERSIZE-pos, 1, pos, mHandle);
	pos = 0;
	return bytesread;
}

void FileReader::PrepareBuffer(u8 datasize) const
{
	//todo: handle eof
	if (mHandle != NULL && BUFFERSIZE - datasize < pos)
		FillBuffer();
}

u8 FileReader::ReadInt8() const
{
	PrepareBuffer(1);
	return mBuffer[pos++];
}

u16 FileReader::ReadInt16() const
{
	PrepareBuffer(2);
	u16 t = mBuffer[pos++];
	t += mBuffer[pos++] << 8;
	return t;
}

u32 FileReader::ReadInt32() const
{
	PrepareBuffer(4);
	u32 t = mBuffer[pos++];
	t += mBuffer[pos++] << 8;
	t += mBuffer[pos++] << 16;
	t += mBuffer[pos++] << 24;
	return t;
}

float FileReader::ReadFloat() const
{
	PrepareBuffer(4);
	u8 c[4];
	c[0] = mBuffer[pos++];
	c[1] = mBuffer[pos++];
	c[2] = mBuffer[pos++];
	c[3] = mBuffer[pos++];
	return *reinterpret_cast<float*>(c);
}

// Read Integers of variable size, kind of like in midi
u32 FileReader::ReadVarInt() const
{
	PrepareBuffer(4);
	u32 value = 0;
	u8 i = 0;
	u8 b;
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
	u32 l = ReadVarInt();
	if (l == 0)
		return NULL;
	
	PrepareBuffer(l);
	
	char* c = new char[l+1];
	for (u32 i=0; i<l; ++i)
		c[i] = mBuffer[pos++];
	c[l] = '\0';
	
	std::string s(c);
	return s;
}

void FileReader::Reset() const
{
	if (mHandle == NULL)
	{
		pos = 0;
	}
	else
	{
		pos = BUFFERSIZE;
		rewind(mHandle);
	}
}

void FileReader::Skip(u32 count) const
{
	PrepareBuffer(count);
	pos += count;
}


