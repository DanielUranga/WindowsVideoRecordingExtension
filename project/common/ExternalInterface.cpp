#ifndef STATIC_LINK
#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif

#include <hx/CFFI.h>
#include "WindowsVideoRecording.h"

using namespace windowsvideorecording;

static value windowsvideorecording_init (value path, value videoWidth, value videoHeight, value videoFPS, value videoBitRate) {
    Init(val_string(path), val_int(videoWidth), val_int(videoHeight), val_int(videoFPS), val_int(videoBitRate));
    return alloc_null();
}
DEFINE_PRIM (windowsvideorecording_init, 5);

static value windowsvideorecording_dispose () {
    Dispose();
    return alloc_null();
}
DEFINE_PRIM (windowsvideorecording_dispose, 0);

static value windowsvideorecording_write_frame (value videoFrameBuffer) {
    WriteFrame((BYTE*)buffer_data(val_to_buffer(videoFrameBuffer)));
    return alloc_null();
}
DEFINE_PRIM (windowsvideorecording_write_frame, 1);

extern "C" void windowsvideorecording_main () {
    val_int(0); // Fix Neko init
}
DEFINE_ENTRY_POINT (windowsvideorecording_main);

extern "C" int windowsvideorecording_register_prims () { return 0; }
