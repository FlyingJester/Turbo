#include "std_c_file.hpp"

Ultra::StdCFile::~StdCFile(){}

Ultra::StdCFile::StdCFile(FILE *f)
  : CFile(f){}

bool Ultra::StdCFile::seekable(){ return false; }
