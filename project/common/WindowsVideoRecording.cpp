#include "IMFObjectWrapper.h"
#include "WindowsVideoRecording.h"

#include <Windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Mfreadwrite.h>
#include <mferror.h>

#include <memory>
#include <random>
#include <string>
#include <utility>

#pragma comment(lib, "mfreadwrite")
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid")

namespace windowsvideorecording {

    static std::unique_ptr<IMFWrappers::IMFSinkWriterWrapper> sSinkWriter;
    static DWORD sStreamIndex;
    static UINT32 sVideoWidth;
    static UINT32 sVideoHeight;
    static UINT32 sVideoFPS;
    static UINT32 sVideoBitRate;
    static LONGLONG sRtStart;

    void InitializeSinkWriter(std::string outputPath)
    {
        const GUID   VIDEO_ENCODING_FORMAT = MFVideoFormat_WMV3;
        const GUID   VIDEO_INPUT_FORMAT = MFVideoFormat_RGB32;

        sSinkWriter = std::make_unique<IMFWrappers::IMFSinkWriterWrapper>(outputPath.c_str(), nullptr, nullptr);

        // Set the output media type.
        IMFWrappers::IMFMediaTypeWrapper pMediaTypeOut;

        pMediaTypeOut.setGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
        pMediaTypeOut.setGUID(MF_MT_SUBTYPE, VIDEO_ENCODING_FORMAT);
        pMediaTypeOut.setUINT32(MF_MT_AVG_BITRATE, sVideoBitRate);
        pMediaTypeOut.setUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
        pMediaTypeOut.setAttributeSize(MF_MT_FRAME_SIZE, sVideoWidth, sVideoHeight);
        pMediaTypeOut.setAttributeRatio(MF_MT_FRAME_RATE, sVideoFPS, 1);
        pMediaTypeOut.setAttributeRatio(MF_MT_PIXEL_ASPECT_RATIO, 1, 1);

        // Set the input media type.
        IMFWrappers::IMFMediaTypeWrapper pMediaTypeIn;

        pMediaTypeIn.setGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
        pMediaTypeIn.setGUID(MF_MT_SUBTYPE, VIDEO_INPUT_FORMAT);
        pMediaTypeIn.setUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
        pMediaTypeIn.setAttributeSize(MF_MT_FRAME_SIZE, sVideoWidth, sVideoHeight);
        pMediaTypeIn.setAttributeRatio(MF_MT_FRAME_RATE, sVideoFPS, 1);
        pMediaTypeIn.setAttributeRatio(MF_MT_PIXEL_ASPECT_RATIO, 1, 1);

        sStreamIndex = sSinkWriter->AddStream(pMediaTypeOut);
        sSinkWriter->setInputMediaType(sStreamIndex, pMediaTypeIn, NULL);
        sSinkWriter->beginWritting();

        sSinkWriter->addRef();
    }

    void Init(std::string outputPath, uint32_t videoWidth, uint32_t videoHeight, uint32_t videoFPS, uint32_t videoBitRate)
    {
        std::cout << "Init : videoWidth=" << videoWidth << ", videoHeight=" << videoHeight << ", videoFPS=" << videoFPS << ", videoBitRate=" << videoBitRate << std::endl;
        sRtStart = 0;
        try
        {
            IMFWrappers::CoInitializeExWrapper(NULL, COINIT_APARTMENTTHREADED);
            IMFWrappers::MFStartupWrapper(MF_VERSION);

            sVideoWidth = videoWidth;
            sVideoHeight = videoHeight;
            sVideoFPS = videoFPS;
            sVideoBitRate = videoBitRate;

            InitializeSinkWriter(outputPath);
        }
        catch (const WindowsError& err)
        {
            std::cout << "Catched exception - " << err.toString() << std::endl;
        }
    }

    void Dispose()
    {
        std::cout << "Dispose" << std::endl;
        sSinkWriter.reset();
        MFShutdown();
        CoUninitialize();
    }

    void WriteFrame(BYTE* videoFrameBuffer)
    {
        const int videoFrameDuration = 10 * 1000 * 1000 / sVideoFPS;
        const LONG cbWidth = 4 * sVideoWidth;
        const DWORD cbBuffer = cbWidth * sVideoHeight;
        BYTE *pData = NULL;
        sRtStart += videoFrameDuration;

        IMFWrappers::IMFMediaBufferWrapper pBuffer(cbBuffer);
        pBuffer.lock(&pData);
        pBuffer.copyImage(pData, cbWidth, videoFrameBuffer, cbWidth, cbWidth, sVideoHeight);
        pBuffer.unlock();
        pBuffer.setCurrentLength(cbBuffer);

        IMFWrappers::IMFSampleWrapper pSample;
        pSample.addBuffer(pBuffer);
        pSample.setSampleTime(sRtStart);

        pSample.setSampleDuration(videoFrameDuration);

        sSinkWriter->writeSample(sStreamIndex, pSample);

        pSample.release();
        pBuffer.release();
    }

}
