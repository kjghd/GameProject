#pragma once
#include <ostream>
#include <istream>
#include <string>

class FileWritable
{
protected:
	std::string m_tag;
	virtual void WriteData(std::ostream& os);
public:
	void Write(std::ostream& os);

	static std::string GetNextValue(std::istream& is);
};

