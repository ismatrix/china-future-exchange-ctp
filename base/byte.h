/*
 * ut_byte.h
 *
 *  Created on: 2013-9-22
 *      Author: echo
 *       Brief: some byte operations and data structures
 */

#ifndef UT_BYTE_H_
#define UT_BYTE_H_

///bit operations
/*#define ut_bitset(VAL,INDEX)	((VAL)|=(1<<INDEX))
#define ut_bittst(VAL,INDEX)	((VAL)&(1<<INDEX))
#define ut_bitclr(VAL,INDEX)	((VAL)&=(0xFFFFFFFE << INDEX))*/

#include <string.h>
#include <string>
#include <stdint.h>
///a structure which keeps a pointer pointing to a bytes block
struct ut_bytes
{
	uint8_t* _data;
	uint32_t _len;

	inline ut_bytes():_data(NULL), _len(0) { }
	inline ut_bytes(const void* data, uint32_t len):_data(const_cast<uint8_t*>((const uint8_t*)data)), _len(len) { }
	inline ut_bytes(const ut_bytes& bytes):_data(bytes._data), _len(bytes._len) { }
	inline void operator=(const ut_bytes& bytes) { _data = bytes._data; _len = bytes._len; }
	inline void set(const void* data, uint32_t len) { _data = const_cast<uint8_t*>((const uint8_t*)data); _len = len; }
	inline std::string str() { return std::string(reinterpret_cast<char*>(_data), static_cast<size_t>(_len)); }
	inline bool operator==(const ut_bytes& rhs) const { return _len==rhs._len && 0==memcmp(_data, rhs._data, _len); }
	inline bool operator!=(const ut_bytes& rhs) const { return _len!=rhs._len || 0!=memcmp(_data, rhs._data, _len); }
};

/**
 * a memory block with fixed length
 * this template has 5 specification implements.
 */
template<int bytes_len>
struct ut_block
{
	typedef ut_block<bytes_len> self_t;
	static const int _len = bytes_len;
	inline ut_block() { }
	inline ut_block(const void* v) { memcpy(_data, v, bytes_len); }
	inline ut_block(const self_t& block) { memcpy(_data, block._data, _len); }
	inline void operator=(const self_t& block) { memcpy(_data, block._data, bytes_len); }
	inline void operator=(const std::string& str) { memcpy(_data, str.c_str(), bytes_len); }
	inline void operator=(const void* v) { memcpy(_data, v, bytes_len); }
	inline bool operator==(const self_t& rhs) const { return 0==memcmp(_data, rhs._data, bytes_len); }
	inline bool operator!=(const self_t& rhs) const { return 0!=memcmp(_data, rhs._data, bytes_len); }
	inline bool operator==(const char* v) const { if(strlen(v)!=bytes_len) return false; return 0==memcmp(_data,v,bytes_len); }
	inline bool operator!=(const char* v) const { if(strlen(v)!=bytes_len) return true; return 0!=memcmp(_data,v,bytes_len); }
	inline uint8_t* data() { return _data; }
	inline const uint8_t* data() const { return _data; }
	inline void fill(const int8_t c) { memset(_data, c, bytes_len); }
	inline std::string str() const { return std::string(reinterpret_cast<const char*>(_data),bytes_len); }
private:
	uint8_t	_data[bytes_len];
};
template<>
struct ut_block<1>
{
	typedef ut_block<1> self_t;
	static const int _len = 1;
	inline ut_block():_data(0) { }
	inline ut_block(const void* v):_data( ((const uint8_t*)v)[0] ) { }
	inline ut_block(const self_t& block):_data(block._data) { }
	inline void operator=(const self_t& block) { _data = block._data; }
	inline void operator=(const std::string& str) { _data = str.c_str()[0]; }
	inline void operator=(const void* v) { _data = ((const uint8_t*)v)[0]; }
	inline bool operator==(const self_t& rhs) const { return _data==rhs._data; }
	inline bool operator!=(const self_t& rhs) const { return _data!=rhs._data; }
	inline bool operator==(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return false; return _data==v[0]; }
	inline bool operator!=(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return true; return _data!=v[0]; }
	inline uint8_t* data() { return &_data; }
	inline const uint8_t* data() const { return &_data; }
	inline uint8_t u8() const { return _data; }
	inline void fill(const int8_t c) { _data = c; }
	inline std::string str() const { return std::string(reinterpret_cast<const char*>(&_data),_len); }
private:
	uint8_t	_data;
};
template<>
struct ut_block<2>
{
	typedef ut_block<2> self_t;
	static const int _len = 2;
	inline ut_block() { }
	inline ut_block(const void* v) { _data._u16 = *reinterpret_cast<const uint16_t*>(v); }
	inline ut_block(const self_t& block) { _data._u16 = block._data._u16; }
	inline void operator=(const self_t& block) { _data._u16 = block._data._u16; }
	inline void operator=(const std::string& str) { _data._u16 = *reinterpret_cast<const uint16_t*>(str.c_str()); }
	inline void operator=(const uint8_t* v) { _data._u16 = *reinterpret_cast<const uint16_t*>(v); }
	inline bool operator==(const self_t& rhs) const { return _data._u16==rhs._data._u16; }
	inline bool operator!=(const self_t& rhs) const { return _data._u16!=rhs._data._u16; }
	inline bool operator==(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return false; return _data._u16==*reinterpret_cast<const uint16_t*>(v); }
	inline bool operator!=(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return true; return _data._u16!=*reinterpret_cast<const uint16_t*>(v); }
	inline uint8_t* data() { return _data._u8; }
	inline const uint8_t* data() const { return _data._u8; }
	inline uint16_t u16() const { return _data._u16; }
	inline void fill(const int8_t c) { _data._u8[0] = _data._u8[1] = c; }
	inline std::string str() const { return std::string(reinterpret_cast<const char*>(_data._u8),_len); }
private:
	union
	{
		uint16_t _u16;
		uint8_t	_u8[2];
	}_data;
};
template<>
struct ut_block<3>
{
	typedef ut_block<3> self_t;
	static const int _len = 3;
	inline ut_block() { }
	inline ut_block(const void* v) { _data._s._u16 = *reinterpret_cast<const uint16_t*>(v); _data._s._u8 = ((const uint8_t*)v)[2]; }
	inline ut_block(const self_t& block) { _data._s._u16 = block._data._s._u16; _data._s._u8 = block._data._s._u8; }
	inline void operator=(const self_t& block) { _data._s._u16 = block._data._s._u16; _data._s._u8 = block._data._s._u8; }
	inline void operator=(const std::string& str) { const char* v = str.c_str(); _data._s._u16 = *reinterpret_cast<const uint16_t*>(v); _data._s._u8 = v[2]; }
	inline void operator=(const uint8_t* v) { _data._s._u16 = *reinterpret_cast<const uint16_t*>(v); _data._s._u8 = v[2]; }
	inline bool operator==(const self_t& rhs) const { return _data._s._u16==rhs._data._s._u16 && _data._s._u8==rhs._data._s._u8; }
	inline bool operator!=(const self_t& rhs) const { return _data._s._u16!=rhs._data._s._u16 && _data._s._u8==rhs._data._s._u8; }
	inline bool operator==(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return false; return _data._s._u16==*reinterpret_cast<const uint16_t*>(v) && _data._s._u8==v[2]; }
	inline bool operator!=(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return true; return _data._s._u16!=*reinterpret_cast<const uint16_t*>(v) || _data._s._u8!=v[2]; }
	inline uint8_t* data() { return _data._u8; }
	inline const uint8_t* data() const { return _data._u8; }
	inline void fill(const int8_t c) { _data._u8[0] = _data._u8[1] = _data._u8[2] = c; }
	inline std::string str() const { return std::string(reinterpret_cast<const char*>(_data._u8),_len); }
private:
	union
	{
#pragma pack(1)
		struct
		{
			uint16_t _u16;
			uint8_t _u8;
		}_s;
#pragma pack()
		uint8_t	_u8[3];
	}_data;
};
template<>
struct ut_block<4>
{
	typedef ut_block<4> self_t;
	static const int _len = 4;
	inline ut_block() { }
	inline ut_block(const void* v) { _data._u32 = *reinterpret_cast<const uint32_t*>(v); }
	inline ut_block(const self_t& block) { _data._u32 = block._data._u32; }
	inline void operator=(const self_t& block) { _data._u32 = block._data._u32; }
	inline void operator=(const std::string& str) { _data._u32 = *reinterpret_cast<const uint32_t*>(str.c_str()); }
	inline void operator=(const uint8_t* v) { _data._u32 = *reinterpret_cast<const uint32_t*>(v); }
	inline bool operator==(const self_t& rhs) const { return _data._u32==rhs._data._u32; }
	inline bool operator!=(const self_t& rhs) const { return _data._u32!=rhs._data._u32; }
	inline bool operator==(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return false; return _data._u32==*reinterpret_cast<const uint32_t*>(v); }
	inline bool operator!=(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return true; return _data._u32!=*reinterpret_cast<const uint32_t*>(v); }
	inline uint8_t* data() { return _data._u8; }
	inline const uint8_t* data() const { return _data._u8; }
	inline uint32_t u32() const { return _data._u32; }
	inline void fill(const int8_t c) { _data._u8[0] = _data._u8[1] = _data._u8[2] = _data._u8[3] = c; }
	inline std::string str() const { return std::string(reinterpret_cast<const char*>(_data._u8),_len); }
private:
	union
	{
		uint32_t _u32;
		uint8_t	_u8[4];
	}_data;
};
template<>
struct ut_block<8>
{
	typedef ut_block<8> self_t;
	static const int _len = 8;
	inline ut_block() { }
	inline ut_block(const void* v) { _data._u64 = *reinterpret_cast<const uint64_t*>(v); }
	inline ut_block(const self_t& block) { _data._u64 = block._data._u64; }
	inline void operator=(const self_t& block) { _data._u64 = block._data._u64; }
	inline void operator=(const std::string& str) { _data._u64 = *reinterpret_cast<const uint64_t*>(str.c_str()); }
	inline void operator=(const uint8_t* v) { _data._u64 = *reinterpret_cast<const uint64_t*>(v); }
	inline bool operator==(const self_t& rhs) const { return _data._u64==rhs._data._u64; }
	inline bool operator!=(const self_t& rhs) const { return _data._u64!=rhs._data._u64; }
	inline bool operator==(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return false; return _data._u64==*reinterpret_cast<const uint64_t*>(v); }
	inline bool operator!=(const char* v) const { if(static_cast<int>(strlen(v))!=_len) return true; return _data._u64!=*reinterpret_cast<const uint64_t*>(v); }
	inline uint8_t* data() { return _data._u8; }
	inline const uint8_t* data() const { return _data._u8; }
	inline uint64_t u64() const { return _data._u64; }
	inline void fill(const int8_t c) { _data._u8[0] = _data._u8[1] = _data._u8[2] = _data._u8[3] = _data._u8[4] = _data._u8[5] = _data._u8[6] = _data._u8[7] = c; }
	inline std::string str() const { return std::string(reinterpret_cast<const char*>(_data._u8),_len); }
private:
	union
	{
		uint64_t _u64;
		uint8_t	_u8[8];
	}_data;
};

/**
 * enhanced memory block whose value can be set via an uint32
 * this template has 5 specification implements.
 */
template<int bytes_len>
struct ut_block_uint32 : public ut_block<bytes_len>
{
	typedef ut_block_uint32<bytes_len> self_t;
	typedef ut_block<bytes_len> base_t;
	inline void operator=(const std::string& str) { *reinterpret_cast<base_t*>(this) = str;/*memcpy(base_t::_data, str.c_str(), bytes_len);*/ }
	inline void operator=(const void* v) { *reinterpret_cast<base_t*>(this) = (const uint8_t*)v;/*memcpy(base_t::_data, v, bytes_len);*/ }
	inline void operator=(const self_t& block) { *reinterpret_cast<base_t*>(this) = *reinterpret_cast<const base_t*>(&block);/*memcpy(base_t::_data, block._data, bytes_len);*/ }
	void operator=(const uint32_t v) { util_u322n(v, base_t::data(), bytes_len); }
	inline uint32_t uint32_val() const { return util_n2u32(base_t::data(), bytes_len); }
};

/**
 * buffer with fixed length
 */
template<uint32_t max_bytes_len>
struct ut_buf
{
	uint8_t _data[max_bytes_len];
	uint32_t _len;
	static const uint32_t _max_len = max_bytes_len;

	inline ut_buf():_len(0) { }
	inline ut_buf(uint32_t len):_len(len) { }
	inline ut_buf(const void* data, uint32_t len) { set(data, len); }
	inline ut_buf(const ut_buf& buf):_len(buf._len) { memcpy(_data, buf._data, _len); }
	inline uint32_t set(const void* data, uint32_t len)
	{
		memcpy(_data, data, _len=std::min(max_bytes_len, len));
		return _len;
	}
	inline void operator=(const ut_buf& buf) { memcpy(_data, buf._data, _len=buf._len); }
	inline void operator=(const std::string& str) { memcpy(_data, str.c_str(), _len=str.size()); }
	inline bool operator==(const ut_buf& buf) const
	{
		if(_len==buf._len) return 0==memcmp(_data, buf._data, _len);
		return false;
	}
	inline bool equal(const void* data, uint32_t len) const
	{
		if(_len==len) return 0==memcmp(_data, data, len);
		return false;
	}
	inline bool operator!=(const ut_buf& buf) const { return !(*this==buf); }
	inline void fill(const int8_t c) { memset(_data, c, max_bytes_len); }
	inline std::string str() const { return std::string(reinterpret_cast<const char*>(_data), static_cast<size_t>(_len)); }
	inline uint32_t str(const std::string& s) { return set(reinterpret_cast<const uint8_t*>(s.c_str()), static_cast<uint32_t>(s.size())); }
};

#endif /* UT_BYTE_H_ */
