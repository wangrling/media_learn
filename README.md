# media_learn
media learn

Markdown[基础语法参考网站](https://www.markdownguide.org/basic-syntax)。

## ffmpeg

### ffmpeg安装
1. 下载源码
	`git clone https://github.com/FFmpeg/FFmpeg`	
2. ./configure
	进行配置，可以通过`./configure --help`进行查看。  
	`--enable-shared`编译动态库。
3. `make` && `sudo make install`
	头文件在/usr/local/include/目录下面，生成库文件在/usr/local/lib目录下面。  
	libavcodec.so, libavdevice.so libavfilter.so libavformat.so libavutil.so libswresample.so libswscale.so	
	
### hello_ffmpeg示例
