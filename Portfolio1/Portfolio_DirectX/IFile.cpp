#include "IFile.h"

IFile::IFile(
	const std::string& strFilePathIn,
	const std::string& strFileNameIn
)
	: strFilePath(strFilePathIn), strFileName(strFileNameIn)
{
}

IFile::~IFile()
{
}
