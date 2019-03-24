//
// Created by wangrl on 19-3-23.
//


extern "C" {
#include <libavformat/avformat.h>
#include <stdio.h>
}

#include <string>



const static int64_t MICRO_PER_SECOND = 1000000;    // 秒转换成为微秒。

int main(int argc, char* argv[]) {

    const char input_file[] = "/home/wangrl/Videos/small_bunny_1080p_30fps.mp4";

    {
        // 必须初始化。
        AVFormatContext *avFormatContext = NULL;

        avformat_open_input(&avFormatContext, input_file, NULL, NULL);


        // 打印视频的时间，转换成为hh:mm:ss的形式。
        int64_t total_seconds = avFormatContext->duration / MICRO_PER_SECOND;
        int64_t hours = total_seconds / 3600;
        int64_t minutes = (total_seconds % 3600) / 60;
        int64_t seconds = total_seconds % 60;

        printf("%02ld:%02ld:%02ld\n", hours, minutes, seconds);

        // 输出视频的元数据。
        AVDictionaryEntry *tag = NULL;

        // 获取片源的名字，如果传递""就是遍历元数据。
        while ((tag = av_dict_get(avFormatContext->metadata, "title", tag, AV_DICT_IGNORE_SUFFIX))) {
            printf("%s=%s\n", tag->key, tag->value);
        }





        avformat_close_input(&avFormatContext);
    }
    return 0;
}
