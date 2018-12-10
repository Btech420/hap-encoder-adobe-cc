#ifndef FFMPEG_HELPERS_HPP
#define FFMPEG_HELPERS_HPP

#include <array>
#include <functional>
#include <memory>

extern"C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}

typedef std::array<char, 4> FileFormat;
typedef std::array<char, 4> VideoFormat;
typedef std::function<size_t(uint8_t*, int size)> MovieReadCallback;            // return 0 on success, -ve on failure
typedef std::function<size_t(const uint8_t*, int size)> MovieWriteCallback;     // return 0 on success, -ve on failure
typedef std::function<int (int64_t offset, int whence)> MovieSeekCallback;      // return 0 on success, -ve on failure
typedef std::function<int ()> MovieCloseCallback;                               // return 0 on success, -ve on failure
typedef std::function<void (const char *)> MovieErrorCallback;                  // must not throw

// wrappers for libav-* objects

struct FormatContextDeleter {
    void operator()(AVFormatContext *context)
    {
        avformat_free_context(context);
    }
};
typedef std::unique_ptr<AVFormatContext, FormatContextDeleter> FormatContext;

struct IOContextDeleter {
    void operator()(AVIOContext *ioContext)
    {
        avio_flush(ioContext);
        av_freep(&ioContext->buffer);
        avio_context_free(&ioContext);
    }
};
typedef std::unique_ptr<AVIOContext, IOContextDeleter> IOContext;

#endif