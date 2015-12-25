#include "std_c_file.hpp"

Ultra::StdCFile::~StdCFile(){}

Ultra::StdCFile::StdCFile(FILE *f, bool r, bool w)
  : CFile(f, r, w){}

bool Ultra::StdCFile::seekable(){ return false; }

int Ultra::StdCFile::size() { return 0; }
