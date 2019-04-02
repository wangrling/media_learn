//
// Created by wangrl on 19-3-23.
//


extern "C" {
#include <libavformat/avformat.h>
#include <stdio.h>
}

#include <string>
#include <iostream>



const static int64_t MICRO_PER_SECOND = 1000000;    // 秒转换成为微秒。

using std::cout;
using std::endl;

int main(int argc, char* argv[]) {

    const char input_file[] = "/home/wangrl/Videos/small_bunny_1080p_30fps.mp4";

    {
        AVFormatContext *avFormatContext = NULL;
        avformat_open_input(&avFormatContext, input_file, NULL, NULL);

        // The input container format.
        AVInputFormat* avInputFormat = avFormatContext->iformat;
        cout << avInputFormat->name << endl;
        cout << avInputFormat->long_name << endl;
        // cout << avInputFormat->extensions << endl;
        // cout << avInputFormat->mime_type << endl;

        // Read the format header and initialize the AVFormatContext structure.
        avInputFormat->read_header(avFormatContext);

        AVIOContext* avioContext = avFormatContext->pb;

        cout << avFormatContext->nb_streams << endl;

        // cout << avFormatContext->bit_rate << endl;

        AVDictionary* avDictionary = avFormatContext->metadata;

        AVDictionaryEntry *tag = NULL;
        av_dict_get(avDictionary, "", tag, AV_DICT_IGNORE_SUFFIX);

    }

    {
        // Field Documentation
        AVFormatContext *avFormatContext = NULL;
        avformat_open_input(&avFormatContext, input_file, NULL, NULL);

        const AVClass* avClass = avFormatContext->av_class;

        // 打印类的名字。
        std::cout << avClass->class_name << std::endl;

        // const struct AVOption* avOption = avClass->option;

        cout << avClass->version << endl;
        cout << (50 << 16 | 15 << 8 | 2) << endl;

        cout << avClass->log_level_offset_offset << endl;
    }




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
