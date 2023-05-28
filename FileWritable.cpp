#include "FileWritable.h"

void FileWritable::WriteData(std::ostream& os)
{
	// To be overridden.
}

void FileWritable::Write(std::ostream& os)
{
	os << m_tag << ',';
	WriteData(os);
}

std::string FileWritable::GetNextValue(std::istream& is)
{
	std::string buffer;
	while (is.peek() != ',' && is.peek() != '\n' && is.peek() != EOF)
	{
		buffer.push_back(is.get());
	}
	if (is.peek() != ',' || is.peek() != '\n')
	{
		is.ignore(1);
	}
	return buffer;
}
