#include "std_c_file.hpp"

Turbo::StdCFile::~StdCFile(){}

Turbo::StdCFile::StdCFile(FILE *f, bool r, bool w)
  : CFile(f, r, w){}

bool Turbo::StdCFile::seekable(){ return false; }

int Turbo::StdCFile::size() { return 0; }
