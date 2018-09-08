#ifndef __MSTRING_HPP__
#define __MSTRING_HPP__

#include <string>

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

	auto	operator+(const MString& other) const -> MString
	{
		MString	ret = MString(true);

		ret.count = count + other.count;
		ret.string = new char[ret.count + 1];
		memcpy(ret.string, string, count);
		memcpy(ret.string + count, other.string, other.count + 1);
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

	auto	Split(MString const& sep, MString* left, MString* right) -> bool
	{
		char* res = strstr(string, sep.string);
		if (!res)
			return false;
		right = &MString(res + sep.count);
		left = &MString(string, (unsigned int)(res - string));
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

	auto	Str() const -> char const* { return string; }
	auto	Count() const -> unsigned int { return count; }

	bool	operator==(const MString& other) { return strcmp(string, other.string) == 0; }
	bool	operator==(char* other) { return strcmp(string, other) == 0; }	

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


#endif /*__MSTRING_HPP__*/