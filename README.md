# media_learn
多媒体学习教程，陆续添加中，Markdown[基础语法参考网站](https://www.markdownguide.org/basic-syntax)。

## Shell脚本编写
[Advanced Bash-Scripting Guide](http://tldp.org/LDP/abs/html/)



---
## GN
GN is a meta-build system that generates build files for Ninja.
### gn编译
git clone https://gn.googlesource.com/gn    
cd gn   
python build/gen.py 
ninja -C out    
To run tests:    
out/gn_unittests



---
## Skia
### Skia编译
bin/gn gen out/Shared --args='extra_cflags=["-w"]'
ninja -C out/Shared

---


## Cpp

---

## SDL2
### SDL2安装
1. 下载源码。
2. ./configure
3. make
4. sudo make install
5. 结合cmake使用  
    `find_package(SDL2 REQUIRED)`  
    `target_link_libraries(${target} SDL2)`

### cleanup res_path
清除内存和文件路径两个函数。

### hello_sdl
加载一张bmp图片，在窗口中显示。

### multi_function
将相关功能从main函数中拆分出多个函数。

### extension_lib
目前还没有找到SDL2_image.h头文件？  
需要单独安装
https://www.libsdl.org/projects/SDL_image/  
运行的时候报错  
error while loading shared libraries: libSDL2_image-2.0.so.0  
vim .bashrc  
`export LD_LIBRARY_PATH="/usr/local/lib"`
source .bashrc
加载png图片进行显示。

### handle_events
处理用户的输入事件，截取png图片的不同部分显示相对应的颜色。

### sdl_font
   

---

## FFmpeg

### ffmpeg安装
1. 下载源码
	`git clone https://github.com/FFmpeg/FFmpeg`	
2. ./configure
	进行配置，可以通过`./configure --help`进行查看。  
	`--enable-shared`编译动态库。
3. `make` && `sudo make install`
	头文件在/usr/local/include/目录下面，生成库文件在/usr/local/lib目录下面。  
	libavcodec.so, libavdevice.so libavfilter.so libavformat.so libavutil.so libswresample.so libswscale.so	

### 知识点
1. libavcodec       Encoding/Decoding Library.
2. libavdevice      Special devices muxing/demuxing library.
3. libavfilter      Graph-based frame editing library.
4. libavformat      I/O and Muxing/Demuxing Library.
5. libavresample    Libavresample (lavr) is a library that handles audio resampling, sample format conversion and mixing.
6. libavutil        Common code shared across all FFmpeg libraries.
7. libpostproc      Video postprocessing library.
8. libswresample    Audio resampling, sample format conversion and mixing library.
9. libswscale       Color conversion and scaling library.
	
### hello_ffmpeg示例
参考[ffmpeg-libav-tutorial](https://github.com/leandromoreira/ffmpeg-libav-tutorial)写得入门示例，取一个视频的
前几帧，保存为.pgm格式的图片。

### avio_reading示例
文件的输入输出操作。

### metadata示例
输出文件的Metadata信息，比如title, artist, composer, genre等。

### AVFormatContext类讲解
介绍AVFormatContext类的成员变量和相关方法。

### write_frames示例
解析视频的前几帧，使用sws_scale转换成为RGB格式，最后保存为.ppm的图片。

### sdl_play_video示例
使用SDL多媒体框架进行视频播放。    

### muxing.c 使用frame合成视频的示例
