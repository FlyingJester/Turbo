#include "c_file.hpp"
#include "io_plugin.hpp"

namespace Turbo{

CFile::CFile(FILE *f, bool r, bool w)
  : file_(f), r_(r), w_(w){

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

int CFile::size(){
    fpos_t pos;
    fgetpos(file_, &pos);
    
    fseek(file_, 0, SEEK_END);
    const long len = ftell(file_);
    
    fsetpos(file_, &pos);

    return len;
}

int CFile::at(){
    return ftell(file_);
}

bool CFile::readable(){
    return r_;
}

bool CFile::writable(){
    return w_;
}

bool CFile::writeByte(unsigned char c){
    return fputc(c, file_)==c;
}

unsigned char CFile::readByte(){
    return fgetc(file_);
}

bool CFile::CFileConstructor(JSContext *ctx, unsigned argc, JS::Value *vp){
    JS::CallArgs args = CallArgsFromVp(argc, vp);
    
    if(args.length()<1)
        return Plugin::setError(ctx, "CFile::CFileConstructor Error: requires one argument");
    
    if(args[0].isString()){
        const char *const str = JS_EncodeStringToUTF8(ctx, JS::RootedString(ctx, args[0].toString()));
        const char *mode = "rb";
        bool r = true, w = false;
        if(args.length()>1){
            if(!args[1].isInt32())
                return Plugin::setError(ctx, "CFile::CFileConstructor Error: when present, argument 1 must be a read/write constant");
            switch(args[1].toInt32()){
                case IOPlugin::readable:
                    mode = "rb";
                    r = true; w = false;
                    break;
                case IOPlugin::writable:
                    mode = "ab";
                    r = false; w = true;
                    break;
                case IOPlugin::readandwrite:
                    mode = "ab";
                    r = true; w = true;
                    break;
                default:
                return Plugin::setError(ctx, "CFile::CFileConstructor Error: invalid read/write mode");
            }
        }

        FILE *const file = fopen(str, mode);

        if(!file)
            Plugin::setError(ctx, (std::string("CFile::CFileConstructor Error: cannot open file ") + str).c_str());

        JS_free(ctx, (void *)str);

        if(!file)
            return false;
    
        IO *io = new CFile(file, r, w);
        if(r && w)
            io->seek(0, SEEK_SET);

        JS::RootedObject that(ctx, JS_NewObjectForConstructor(ctx, &IOPlugin::io_class, args));
        JS_SetPrivate(that, io);
        args.rval().set(JS::ObjectOrNullValue(that));
    }
    else
        return Plugin::setError(ctx, "CFile::CFileConstructor Error: Invalid type for argument 0");

    return true;
}

void CFile::CFileFinalizer(JSFreeOp *fop, JSObject *obj){
    delete static_cast<CFile *>(JS_GetPrivate(obj));
    // Ensure that the pointer is lost...
    JS_SetPrivate(obj, nullptr);
}


} // namespace Turbo
