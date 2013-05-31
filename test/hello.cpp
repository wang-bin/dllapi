#include "dllapi.h"
#include <string>
#include <iostream>
#include <QtCore/QLibrary>
using namespace std;

namespace DllAPI {
namespace test {

//TODO: use macro
class dll
{
public:
    static dll& instance() {
        static dll d;
        return d;
    }
    void* resolve(const char* sym) {
        cout << __FUNCTION__ << endl;
        if (!mLoaded)
            return 0;
        return DllAPI::library("hello")->resolve(sym);
    }
private:
    dll():mLoaded(false) {
        if (!DllAPI::load("hello"))
            return;
        mLoaded = true;
    }
    ~dll() {
        unload("hello");
    }
    bool mLoaded;
};

DEFINE_DLLAPI_ARG(1, void*, hello, int)
}
}
