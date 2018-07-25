#include "../MUtils/String.hpp"

#include <iostream>

#define LOG(str) std::cout << str << std::endl;

void main()
{
	MString	test("Test");
	MString	test2(test);
	test2.Append("2");
	MString	test3(test2.Str(), 4);
	test2.Append(test);
	
	MString test4 = test2;

	test4 += "Try";
	char* comp = "Test2TestTry";
	bool res = test2 == comp;

	LOG(test4[3]);
	LOG(test4.Contains(MString("Try")));
	test4.Empty();
	LOG(test4.Count());
	MString finalStr = "Test2TestTry";
	LOG(finalStr.ToLower().Str());
	LOG(finalStr.ToUpper().Str());

	finalStr.InsertAt(1, "ry");
	MString finalCopy = finalStr;
	finalCopy.RemoveAt(1, 2);
	LOG(finalCopy.Str());
	finalStr.InsertAt(5, MString("Margoulin"));
	LOG(finalStr.Str());

	while (true)
	{ }
}