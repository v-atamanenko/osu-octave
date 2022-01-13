#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <string>

#ifndef __FILEREADER_H__
#define __FILEREADER_H__



class FileReader
{
	public:
		FileReader(u8* source);
		FileReader(std::string& filename);
		FileReader(const char* filename);
		~FileReader();
		
		u8 ReadInt8() const;
		u16 ReadInt16() const;
		u32 ReadInt32() const;
		float ReadFloat() const;
		u32 ReadVarInt() const;
		std::string ReadString() const;
		void ReadData(void* ptr, u32 size) const;
		
		bool Ready() const { return fReady; }
		
		void Skip(u32 count) const;
		
		void Reset() const;
	
	protected:
		u8* mBuffer;
		mutable u32 pos;
		
		static u32 BUFFERSIZE;
		
		FILE* mHandle;
		
		int FillBuffer() const;
		void PrepareBuffer(u8 datasize) const;
		
		bool fReady;
	
	private:
		void Init(FILE* handle, u8* buffer);
};

#endif