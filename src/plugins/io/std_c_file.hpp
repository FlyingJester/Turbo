#pragma once
#include "c_file.hpp"

namespace Turbo {

class StdCFile : public CFile {
    StdCFile(FILE *, bool r, bool w);
public:
    StdCFile() = delete;
    virtual ~StdCFile();

    bool seekable() override;
    int size() override;

    static StdCFile *getStdin() { return new StdCFile(stdin, true, false); }
    static StdCFile *getStdout() { return new StdCFile(stdout, false, true); }
    static StdCFile *getStderr() { return new StdCFile(stderr, false, true); }

};

} // namespace Turbo
