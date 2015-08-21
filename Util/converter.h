#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>

using namespace std;

class Convert
{
public:
	static inline wstring ToWchar_t(long data)
	{
	    wostringstream os;
	    os << data;
	    return os.str();
	}
	
	static inline wstring ToWchar_t(int data)
	{
	    wostringstream os;
	    os << data;
	    return os.str();
	}
	
	static inline wstring ToWchar_t(unsigned int data)
	{
	    wostringstream os;
	    os << data;
	    return os.str();
	}
	
	static inline wstring ToWchar_t(char data)
	{
	    wostringstream os;
	    os << data;
	    return os.str();
	}
	
	static inline wstring ToWchar_t(double data)
	{
	    wostringstream os;
	    os << data;
	    return os.str();
	}
	
	static inline wstring ToWchar_t(float data)
	{
	    wostringstream os;
	    os << data;
	    return os.str();
	}
};