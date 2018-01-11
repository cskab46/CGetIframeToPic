#include "CGetIFrameToPic.h"

void CGetIFrameToPic::bufferTransFrameData(uint8_t* buffer, uint8_t *frameData[8])
{
	frameData[0] = buffer;
}

//AVCodecContext *pCodecCtxDec = NULL;
//{    
/** 
     * Pixel format, see AV_PIX_FMT_xxx. 
     * May be set by the demuxer if known from headers. 
     * May be overridden by the decoder if it knows better. 
     * - encoding: Set by user. 
     * - decoding: Set by user if known, overridden by libavcodec if known 
     */  
//	 enum AVPixelFormat pix_fmt; http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html
//	}
//pCodecCtxDec->pix_fmt

//AVFrame *pFrameDec = NULL;  
//{#define AV_NUM_DATA_POINTERS 8... int linesize[AV_NUM_DATA_POINTERS]; ...}
//pFrameDec->linesize
// int linesize[8] = {0}; linesize = inputWidth * 3;
int CGetIFrameToPic::doIFrameToBMP(uint8_t* buffer, int inputWidth, int inputHeight, int inputLinesize, int inputPix_fmt)
{
	char szPicFile[256];  
	char * pPicFile = "/tmp/myIFrame";
	uint8_t * frameData[8];
	
	// 因为我们准备输出保存24位RGB色的PPM文件，我们必需把帧的格式从原来的转换为RGB。  
    // FFMPEG将为我们做这些转换。  
    // 在大多数项目中（包括我们的这个）我们都想把原始的帧转换成一个特定的格式。  
    // 让我们先为转换来申请一帧的内存  
    AVFrame *pFrameRGB = NULL;  
    // Allocate an AVFrame structure  
    pFrameRGB = avcodec_alloc_frame();  
    if(pFrameRGB == NULL)  
    {  
        printf("Allocate an AVFrame structure error.\n");  
        return -1;  
    }  
	
	// ------------frame to bmp start------------  
	memset(szPicFile, 0, sizeof(szPicFile));  
	strncpy(szPicFile, pPicFile, sizeof(szPicFile));  
	strncat(szPicFile, ".bmp", sizeof(szPicFile));  
	// Convert the image from its native format to RGB  
	img_convert_ctx = sws_getContext(  
		inputWidth, inputHeight, inputPix_fmt,  
		inputWidth, inputHeight,   
		AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL);  
	bufferTransFrameData(buffer, frameData);
	sws_scale(img_convert_ctx, frameData, inputLinesize, 0,   
		inputHeight, pFrameRGB->data, pFrameRGB->linesize);  
	SaveFrameToBMP(szPicFile, pFrameRGB->data[0],   
		inputWidth, inputHeight, 24);  
	sws_freeContext(img_convert_ctx);  
}

// 保存帧数据为BMP图片  
int CGetIFrameToPic::SaveFrameToBMP(char *pPicFile, uint8_t *pRGBBuffer, int nWidth, int nHeight, int nBitCount)  
{  
    BITMAPFILEHEADER bmpheader;  
    BITMAPINFOHEADER bmpinfo;  
    memset(&bmpheader, 0, sizeof(BITMAPFILEHEADER));  
    memset(&bmpinfo, 0, sizeof(BITMAPINFOHEADER));  
  
    FILE *fp = NULL;  
    fp = fopen(pPicFile, "wb");  
    if(NULL == fp)  
    {  
        printf("file open error %s\n", pPicFile);  
        return -1;  
    }  
    // set BITMAPFILEHEADER value  
    bmpheader.bfType = ('M' << 8) | 'B';  
    bmpheader.bfReserved1 = 0;  
    bmpheader.bfReserved2 = 0;  
    bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
    bmpheader.bfSize = bmpheader.bfOffBits + nWidth * nHeight * nBitCount / 8;  
    // set BITMAPINFOHEADER value  
    bmpinfo.biSize = sizeof(BITMAPINFOHEADER);  
    bmpinfo.biWidth = nWidth;  
    bmpinfo.biHeight = 0 - nHeight;  
    bmpinfo.biPlanes = 1;  
    bmpinfo.biBitCount = nBitCount;  
    bmpinfo.biCompression = BI_RGB;  
    bmpinfo.biSizeImage = 0;  
    bmpinfo.biXPelsPerMeter = 100;  
    bmpinfo.biYPelsPerMeter = 100;  
    bmpinfo.biClrUsed = 0;  
    bmpinfo.biClrImportant = 0;  
    // write pic file  
    fwrite(&bmpheader, sizeof(BITMAPFILEHEADER), 1, fp);  
    fwrite(&bmpinfo, sizeof(BITMAPINFOHEADER), 1, fp);  
    fwrite(pRGBBuffer, nWidth * nHeight * nBitCount / 8, 1, fp);  
    fclose(fp);  
    return 0;  
}  

