#include <cstdio>
#include <cstring>
#include <string>

#ifndef __FILEREADER_H__
#define __FILEREADER_H__

class FileReader
{
	public:
		FileReader(uint8_t* source);
		FileReader(std::string& filename);
		FileReader(const char* filename);
		~FileReader();
		
		uint8_t ReadInt8() const;
		uint16_t ReadInt16() const;
		uint32_t ReadInt32() const;
		float ReadFloat() const;
		uint32_t ReadVarInt() const;
		std::string ReadString() const;
		void ReadData(void* ptr, uint32_t size) const;
		
		bool Ready() const { return fReady; }
		
		void Skip(uint32_t count) const;
		
		void Reset() const;
	
	protected:
		uint8_t* mBuffer;
		mutable uint32_t pos;
		
		static uint32_t BUFFERSIZE;
		
		FILE* mHandle;
		
		int FillBuffer() const;
		void PrepareBuffer(uint8_t datasize) const;
		
		bool fReady;
	
	private:
		void Init(FILE* handle, uint8_t* buffer);
};

#endif