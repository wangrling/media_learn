//
// Created by wangrl on 19-4-4.
//

extern "C" {
#include <libavcodec/avcodec.h>
}

#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {

    {
        // 测试AVCodecID结构体的数据。
        cout << AVCodecID::AV_CODEC_ID_MPEG4 << endl;
        cout << AVCodecID::AV_CODEC_ID_H264 << endl;
        cout << AVCodecID::AV_CODEC_ID_VP9 << endl;

        AVCodec* encoder = avcodec_find_encoder(AVCodecID::AV_CODEC_ID_H264);


    }

    return 0;
}

