#pragma once
#include <functional>

using namespace std;

namespace HashFunction
{
	typedef function<unsigned int(const unsigned char*, unsigned int)> HashFunc;

	/// <summary>
	/// Basic hashing.
	/// </summary>
	/// <param name="data">Data to be hashed.</param>
	/// <param name="length">Length of the data.</param>
	/// <returns>Hash value.</returns>
	unsigned int BadHash(const unsigned char* data, unsigned int length)
	{
		unsigned int hash = 0;
		for (unsigned int i = 0; i < length; ++i)
			hash += data[i];
		return hash;
	}

	/// <summary>
	/// Hash function written by Brian K and Dennis R.
	/// </summary>
	/// <param name="data">Data to be hashed.</param>
	/// <param name="length">Length of the data.</param>
	/// <returns>Hash value.</returns>
	unsigned int BKDRHash(const unsigned char* data, unsigned int length)
	{
		unsigned int hash = 0;
		for (unsigned int i = 0; i < length; ++i)
			hash = (hash * 1313) + data[i];
		return (hash & 0x7FFFFFFF);
	}

	/// <summary>
	/// Hash function commonly used on UNIX-based systems.
	/// </summary>
	/// <param name="data">Data to be hashed.</param>
	/// <param name="length">Length of the data.</param>
	/// <returns>Hash value.</returns>
	unsigned int ELFHash(const unsigned char* data, unsigned int length)
	{
		unsigned int hash = 0;
		unsigned int x = 0;

		for (unsigned int i = 0; i < length; ++i)
		{
			hash = (hash << 4) + data[i];
			if ((x = hash & 0xF0000000L) != 0)
			{
				hash ^= (x >> 24);
				hash &= ~x;
			}
		}
		
		return (hash & 0x7FFFFFFF);
	}
	
	static HashFunc default = BKDRHash;
}

