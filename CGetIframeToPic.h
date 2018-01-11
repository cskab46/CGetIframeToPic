#if !defined (_C_GET_IFRAME_TO_PIC_H_)  
#define _C_GET_IFRAME_TO_PIC_H_  
  
extern "C"  
{  
#include "libavcodec/avcodec.h"  
#include "libavformat/avformat.h"  
#include "libswscale/swscale.h"  
#include "jpeglib.h"  
}  
  
  
// 取得视频的首个I帧，并转换为图片  
// 图片格式可以为BMP、PPM、JPG  
class CGetIFrameToPic  
{  
public:  
    CGetIFrameToPic(){};  
    virtual ~CGetIFrameToPic(){};  
  
    // 取得视频的首个I帧，并转换为图片  
    virtual int doIFrameToBMP(uint8_t* buffer, int inputWidth, int inputHeight, int inputLinesize, int inputPix_fmt);  
    // 保存帧数据为BMP图片  
    virtual int SaveFrameToBMP(char *pPicFile, uint8_t *pRGBBuffer, int nWidth, int nHeight, int nBitCount);  

private:
	void bufferTransFrameData(uint8_t* buffer, uint8_t *frameData[8]);
	
};  
  
#endif //!defined(_C_GET_IFRAME_TO_PIC_H_)