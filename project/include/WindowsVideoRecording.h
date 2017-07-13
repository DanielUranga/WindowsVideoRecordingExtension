#ifndef WINDOSVIDEORECORDING_H
#define WINDOSVIDEORECORDING_H

#include <stdint.h>
#include <string>

namespace windowsvideorecording {

    void Init(std::string outputPath, uint32_t videoWidth, uint32_t videoHeight, uint32_t videoFPS, uint32_t videoBitRate);
    void Dispose();
    void WriteFrame(BYTE* videoFrameBuffer);

}

#endif
