#include "dllapi_p.h"
#include "dllapi.h"
#include "swresample.h" //or "dllapi/swresample/swresample.h" ?

namespace DllAPI {
namespace swresample {

DEFINE_DLL_INSTANCE("swresample")
DEFINE_DLLAPI_ARG(0, const AVClass *, swr_get_class)
DEFINE_DLLAPI_ARG(0, struct SwrContext *, swr_alloc)
DEFINE_DLLAPI_ARG(1, int, swr_init, struct SwrContext *)
DEFINE_DLLAPI_ARG(9, struct SwrContext *, swr_alloc_set_opts,
                  struct SwrContext *,
                  int64_t, enum AVSampleFormat, int,
                  int64_t, enum AVSampleFormat, int,
                  int, void *)
DEFINE_DLLAPI_ARG(1, void, swr_free, struct SwrContext **)
DEFINE_DLLAPI_ARG(5, int, swr_convert,
                  struct SwrContext *, uint8_t **, int,
                  const uint8_t **, int)
DEFINE_DLLAPI_ARG(2, int64_t, swr_next_pts, struct SwrContext *, int64_t)
DEFINE_DLLAPI_ARG(3, int, swr_set_compensation, struct SwrContext *, int, int)
DEFINE_DLLAPI_ARG(2, int, swr_set_channel_mapping, struct SwrContext *, const int *)
DEFINE_DLLAPI_ARG(3, int, swr_set_matrix, struct SwrContext *, const double *, int)
DEFINE_DLLAPI_ARG(2, int, swr_drop_output, struct SwrContext *, int)
DEFINE_DLLAPI_ARG(2, int, swr_inject_silence, struct SwrContext *, int)
DEFINE_DLLAPI_ARG(2, int64_t, swr_get_delay, struct SwrContext *, int64_t)
DEFINE_DLLAPI_ARG(0, unsigned, swresample_version)
DEFINE_DLLAPI_ARG(0, const char *, swresample_configuration)
DEFINE_DLLAPI_ARG(0, const char *, swresample_license)

} //namespace swresample
} //namespace DllAPI
