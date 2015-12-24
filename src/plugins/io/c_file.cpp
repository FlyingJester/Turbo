#include "c_file.hpp"
#include "io_plugin.hpp"

namespace Ultra{

CFile::CFile(FILE *f)
  : file_(f){

}

CFile::~CFile(){}

    // Accepts SEEK_SET, SEEK_CUR, and SEEK_END
bool CFile::seek(unsigned offset, int whence){
    return fseek(file_, offset, whence)==0;
}
    
bool CFile::read(void *to, unsigned n_bytes){
    return fread(to, 1, n_bytes, file_)==n_bytes;
}

bool CFile::write(const void *from, unsigned n_bytes){
    return fwrite(from, 1, n_bytes, file_)==n_bytes;
}
    
bool CFile::writeByte(unsigned char c){
    return fputc(c, file_)==c;
}

unsigned char CFile::readByte(){
    return fgetc(file_);
}

bool CFile::CFileConstructor(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    args.rval().set(JS::ObjectOrNullValue(JS_NewObjectForConstructor(ctx, &IOPlugin::c_file_class, args)));
    return true;
}

void CFile::CFileFinalizer(JSFreeOp *fop, JSObject *obj){
    delete static_cast<CFile *>(JS_GetPrivate(obj));
    // Ensure that the pointer is lost...
    JS_SetPrivate(obj, nullptr);
}


} // namespace Ultra
