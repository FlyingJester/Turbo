#pragma once
#include "c_file.hpp"

namespace Ultra {

class StdCFile : public CFile {
    StdCFile(FILE *);
public:
    StdCFile() = delete;
    virtual ~StdCFile();

    bool seekable() override;

    static StdCFile *getStdin() { return new StdCFile(stdin); }
    static StdCFile *getStdout() { return new StdCFile(stdout); }
    static StdCFile *getStderr() { return new StdCFile(stderr); }

};

} // namespace Ultra
