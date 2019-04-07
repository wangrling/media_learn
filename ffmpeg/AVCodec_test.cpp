//
// Created by wangrl on 19-4-4.
//

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "iostream"

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {

    AVCodec* avCodec = avcodec_find_encoder(AV_CODEC_ID_H264);

    cout << avCodec->name << endl;
    cout << avCodec->long_name << endl;

    // avCodec->type = AVMediaType::AVMEDIA_TYPE_VIDEO;

    // 视频类型
    cout << avCodec->type << endl;
    cout << avCodec->id << endl;

    const AVRational* avRational = avCodec->supported_framerates;
    if (avRational != nullptr) {
        cout << "Numerator " << avRational->num << ", Denominator " << avRational->den << endl;
    }

    const enum AVPixelFormat * avPixelFormat = avCodec->pix_fmts;

    return 0;
}

