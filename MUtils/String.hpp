﻿#ifndef __MSTRING_HPP__
#define __MSTRING_HPP__

#include <string>
#include <cwchar>
#include <stdlib.h>
#include <codecvt>

class MString
{

public:
	MString(bool empty = false)
	{
		if (empty)
			return;
		string = new char;
		*string = '\0';
	}

	MString(const char* other) { copy(other); }
	MString(const char* other, unsigned int length) { copy(other, length); }
	MString(const MString& other) { copy(other); }

	MString(MString&& other)
	{
		string = other.string;
		other.string = nullptr;
		count = other.count;
		other.count = 0;
	}

	~MString()
	{
		if (string != nullptr)
		{
			delete string;
			string = nullptr;
		}
		count = 0;
	}

	static auto	FromWString(const wchar_t* wideStr) -> MString
	{
		MString ret(true);
		ret.count = wcslen(wideStr);
		ret.string = new char[ret.count + 1];
		ret.string[ret.count] = '\0';

		size_t charsConverted = 0;
		wcstombs_s(&charsConverted, ret.string, ret.count + 1, wideStr, ret.count);
		return ret;
	}

	auto	operator+(const MString& other) const -> MString
	{
		MString	ret = MString(true);

		ret.count = count + other.count;
		ret.string = new char[ret.count + 1];
		memcpy(ret.string, string, count);
		memcpy(ret.string + count, other.string, other.count + 1);
		return ret;
	}

	auto	operator+(const char charValue) const -> MString
	{
		MString	ret = MString(true);

		ret.count = count + 1;
		ret.string = new char[ret.count + 1];
		memcpy(ret.string, string, count);
		ret[count] = charValue;
		ret[count + 1] = '\0';
		return ret;
	}

	auto	operator+(const char* other) const -> MString
	{
		MString	ret = MString(true);

		size_t len = strlen(other);

		ret.count = (unsigned int)len + count;
		ret.string = new char[ret.count + 1];
		memcpy(ret.string, string, count);
		memcpy(ret.string + count, other, len + 1);
		return ret;
	}

	friend MString	operator+(const char* value, MString const& other)
	{
		return MString(value) + other;
	}

	auto	operator+=(MString const& other) -> MString&
	{
		Append(other);
		return *this;
	}

	auto	operator+=(const char* other) -> MString&
	{
		Append(other);
		return *this;
	}

	auto	operator+=(char other) -> MString&
	{
		Append(other);
		return *this;
	}

	auto	operator=(const char* other) -> MString&
	{
		delete string;
		copy(other);
		return *this;
	}

	auto	operator=(const MString& other) -> MString&
	{
		delete string;
		copy(other);
		return *this;
	}

	auto	operator=(MString&& other) -> MString&
	{
		delete string;
		string = other.string;
		other.string = nullptr;
		count = other.count;
		other.count = 0;
		return *this;
	}

	auto	operator[](unsigned int idx) -> char& { return string[idx]; }

	auto	Contains(MString const& sub) -> bool { return strstr(string, sub.string) != nullptr; }
	auto	Contains(char* const sub) const -> bool { return strstr(string, sub) != nullptr; }

	auto	Append(MString const& other) -> void
	{
		char* temp = new char[count + other.count + 1];
		memcpy(temp, string, count);
		memcpy(temp + count, other.string, other.count + 1);
		delete string;
		string = temp;
		count = count + other.count;
	}

	auto	Append(char* const other) -> void
	{
		size_t len = strlen(other);
		char* temp = new char[count + len + 1];
		memcpy(temp, string, count);
		memcpy(temp + count, other, len + 1);
		delete string;
		string = temp;
		count = count + (unsigned int)len;
	}

	auto	Append(char other) -> void
	{
		char* temp = new char[count + 1 + 1];
		memcpy(temp, string, count);
		temp[count] = other;
		temp[count + 1] = '\0';
		delete string;
		string = temp;
		count += 1;
	}

	auto	Empty() -> void 
	{
		if (!string)
			return;
		delete string;
		string = new char;
		*string = '\0';
		count = 0;
	}

	auto	InsertAt(unsigned int idx, char* const other) -> void
	{
		if (idx > count)
			return;
		size_t len = strlen(other);
		char* temp = new char[count + len + 1];
		memcpy(temp, string, idx);
		memcpy(temp + idx, other, len);	
		memcpy(temp + idx + len, string + idx, count - idx);
		count += len;
		delete string;
		temp[count] = '\0';
		string = temp;
	}

	auto	InsertAt(unsigned int idx, MString const& other) -> void
	{
		if (idx > count)
			return;
		char* temp = new char[count + other.count + 1];
		memcpy(temp, string, idx);
		memcpy(temp + idx, other.string, other.count);
		memcpy(temp + idx + other.count, string + idx, count - idx);
		count += other.count;
		delete string;
		temp[count] = '\0';
		string = temp;
	}

	auto	ValidIndex(unsigned int idx) -> bool { return string && count > idx; }

	auto	RemoveAt(unsigned int idx, unsigned int size) -> void
	{
		if (idx + size > count)
			return;
		char* temp = new char[count - size + 1];
		memcpy(temp, string, idx);
		memcpy(temp + idx, string + idx + size, count - idx + size);
		count -= size;
		temp[count] = '\0';
		delete string;
		string = temp;
	}

	auto	Split(MString const& sep, MString& left, MString& right) const -> bool
	{
		char* res = strstr(string, sep.string);
		if (!res)
			return false;
		right = MString(res + sep.count);
		left = MString(string, (unsigned int)(res - string));
		return true;
	}

	auto	ToLower() const -> MString
	{
		MString	ret = *this;
		for (char* str = ret.string; *str != '\0'; ++str)
			*str = tolower(*str);
		return ret;
	}

	auto	ToUpper() const -> MString
	{
		MString ret = *this;
		for (char* str = ret.string; *str != '\0'; ++str)
			*str = toupper(*str);
		return ret;
	}

	static MString FromInt(int const& value)
	{
		char buffer[50];
		int n = 0;
		n = sprintf_s(buffer, 50, "%d", value);
		return MString(buffer, n);
	}

	static MString FromUInt(unsigned int const& value)
	{
		char buffer[50];
		int n = 0;
		n = sprintf_s(buffer, 50,"%d", value);
		return MString(buffer, n);
	}

	static MString FromFloat(float const& value)
	{
		char buffer[50];
		int n = 0;
		n = sprintf_s(buffer, 50, "%f", value);
		return MString(buffer, n);
	}

	auto	Str() const -> char const* { return string; }
	auto	Count() const -> unsigned int { return count; }

	bool	operator==(const MString& other) { return strcmp(string, other.string) == 0; }
	bool	operator==(char* other) { return strcmp(string, other) == 0; }	

	bool	operator <(const MString& other) const { return strcmp(string, other.string) < 0; }

	//ITERATOR SHIT

protected:

private:
	auto	copy(const char* other) -> void
	{
		size_t size = strlen(other);
		string = new char[size + 1];
		memcpy(string, other, size + 1);
		count = size;
	}

	auto	copy(MString const& other) -> void
	{
		count = other.count;
		string = new char[count + 1];
		memcpy(string, other.string, count + 1);
	}

	auto	copy(const char* other, unsigned int length) -> void
	{
		size_t otherLen = strlen(other);
 		count = length > otherLen ? otherLen: length;
		string = new char[count + 1];
		memcpy(string, other, count);
		string[count] = '\0';
	}

	char*			string = nullptr;
	unsigned int	count = 0;
};

class MWString
{
public:
	MWString(bool empty = false)
	{
		if (empty)
			return;
		wstring = new wchar_t;
		*wstring = '\0';
	}

	MWString(const wchar_t* other) { copy(other); }
	MWString(const wchar_t* other, unsigned int length) { copy(other, length); }
	MWString(const MWString& other) { copy(other); }

	MWString(MWString&& other)
	{
		wstring = other.wstring;
		other.wstring = nullptr;
		count = other.count;
		other.count = 0;
	}

	~MWString()
	{
		if (wstring != nullptr)
		{
			delete wstring;
			wstring = nullptr;
		}
		count = 0;
	}

	auto	Append(wchar_t* const other) -> void
	{
		size_t len = wcslen(other);
		wchar_t* temp = new wchar_t[count + len + 1];
		memcpy(temp, wstring, count * sizeof(wchar_t));
		memcpy(temp + count * sizeof(wchar_t), other, (len + 1) * sizeof(wchar_t));
		delete wstring;
		wstring = temp;
		count = count + (unsigned int)len;
	}

	auto	Append(wchar_t other) -> void
	{
		wchar_t* temp = new wchar_t[count + 1 + 1];
		memcpy(temp, wstring, count * sizeof(wchar_t));
		temp[count] = other;
		temp[count + 1] = L'\0';
		delete wstring;
		wstring = temp;
		count += 1;
	}

	auto	ToUTF8String() const -> MString
	{
		if (count == 0)
			return MString();
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		return MString(conv.to_bytes(wstring).c_str());
	}

	auto	operator=(const wchar_t* other) -> MWString&
	{
		delete wstring;
		copy(other);
		return *this;
	}

	auto	operator=(const MWString& other) -> MWString&
	{
		delete wstring;
		copy(other);
		return *this;
	}

	auto	operator=(MWString&& other) -> MWString&
	{
		delete wstring;
		wstring = other.wstring;
		other.wstring = nullptr;
		count = other.count;
		other.count = 0;
		return *this;
	}
	
	auto	operator[](unsigned int idx) -> wchar_t& { return wstring[idx]; }

	static auto FromUTF8(const char* str) -> MWString
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16conv;
		return MWString(utf16conv.from_bytes(str).c_str());
	}

	static auto	FromString(MString const& string) -> MWString
	{
		MWString	ret(true);
		ret.wstring = new wchar_t[string.Count() + 1];
		size_t sizePtr;
		mbstowcs_s(&sizePtr, ret.wstring, string.Count() + 1, string.Str(), string.Count() + 1);
		//mbstowcs(ret.wstring, string.Str(), string.Count() + 1);
		ret.count = string.Count();
		ret.wstring[ret.count] = '\0';
		return ret;
	}

	auto	Str() const -> wchar_t* { return wstring; }
	auto	Count() const -> unsigned int { return count; }

private:
	auto	copy(const wchar_t* other) -> void
	{
		size_t size = wcslen(other);
		wstring = new wchar_t[size + 1];
		wmemcpy(wstring, other, size);
		count = size;
		wstring[count] = '\0';
	}

	auto	copy(MWString const& other) -> void
	{
		wstring = new wchar_t[other.count + 1];
		wmemcpy(wstring, other.wstring, other.count);
		count = other.count;
		wstring[count] = '\0';
	}

	auto copy(const wchar_t* other, unsigned int length) -> void
	{
		size_t otherLen = wcslen(other);
		count = length > otherLen ? otherLen : length;
		wstring = new wchar_t[count + 1];
		wmemcpy(wstring, other, count);
		wstring[count] = '\0';
	}

	wchar_t*		wstring = nullptr;
	unsigned int	count = 0;
};

#endif /*__MSTRING_HPP__*/