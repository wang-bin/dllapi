#include "dllapi.h"
#include "dllapi_p.h"
#include <string>
#include <iostream>
#include <QtCore/QLibrary>
using namespace std;

namespace DllAPI {
namespace test {
DEFINE_DLL_INSTANCE("hello")
DEFINE_DLLAPI_ARG(1, void*, hello, int)
} //namespace test
} //namespace DllAPI
