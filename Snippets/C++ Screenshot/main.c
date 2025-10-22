#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif // WIN32


#include <Windows.h>
#include <WinUser.h>

#include <stdio.h>


#include "FreeImage.h"


void screenCapture(int x, int y, int w, int h, LPCSTR fname)
{
    HDC hdcSource, hdcMemory;
    int cx, cy;
    HBITMAP hBitMap;
    HWND hwnd;

    hwnd        = GetDesktopWindow();
    hdcSource   = GetDCEx(hwnd, NULL, NULL);
    hdcMemory   = CreateCompatibleDC(hdcSource);
    //cx          = GetSystemMetrics(SM_CXSCREEN);
    //cy          = GetSystemMetrics(SM_CYSCREEN);
    hBitMap     = CreateCompatibleBitmap(hdcSource, w, h);

    SelectObject(hdcMemory, hBitMap);
    BitBlt(hdcMemory, x, y, w, h, hdcSource, 0, 0, SRCCOPY);


    FIBITMAP* dib1 = NULL;
    unsigned char* dib = NULL;
    if (hBitMap) {

        BITMAP bm;
        GetObject(hBitMap, sizeof(BITMAP), (LPSTR)&bm);

        BITMAPINFO bmf = { 0 };
        bmf.bmiHeader.biSize = sizeof(bmf.bmiHeader);
        

        dib1 = FreeImage_Allocate(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel, 0x0000FF, 0xFF0000, 0x00FF00);
        
        HDC dc = GetDC(NULL);
        printf("header get bits: %d\n", GetDIBits(dc, hBitMap, 0, 0, NULL, &bmf, DIB_RGB_COLORS));
        unsigned int image_size = bmf.bmiHeader.biSizeImage;
        dib = malloc(bmf.bmiHeader.biSizeImage);

        bmf.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmf.bmiHeader.biBitCount = 32;
        bmf.bmiHeader.biCompression = BI_RGB;

        printf("image get bits: %d \n\n", GetDIBits(dc, hBitMap, 0, bmf.bmiHeader.biHeight, (LPVOID)dib, &bmf, DIB_RGB_COLORS));
        printf("\nBMP HEADER: %02x\n\n", bmf);

        for (int i = 0; i < 100; i++)
        {
            printf("%02x ", *(dib + i));
        }
        


        //printf("GetInfo: %d\n", FreeImage_GetInfo(dib1));
        // The GetDIBits function clears the biClrUsed and biClrImportant BITMAPINFO members (dont't know why)
        // So we save these infos below. This is needed for palettized images only.
        //int nColors = FreeImage_GetColorsUsed(dib);
        //printf("nColors = %d\n", nColors);
        
        int Success = GetDIBits(dc, hBitMap, 0, FreeImage_GetHeight(dib1),
            FreeImage_GetBits(dib1), FreeImage_GetInfo(dib1), DIB_RGB_COLORS);
        //printf("%d\n\n", Success);
        ReleaseDC(NULL, dc);
        
        //printf("getHeight = %d\n", FreeImage_GetHeight(dib));
        //printf("getBits = %d", FreeImage_GetBits(dib));
       
        // restore BITMAPINFO members
        //FreeImage_GetInfoHeader(dib)->biClrUsed = nColors;
        //FreeImage_GetInfoHeader(dib)->biClrImportant = nColors;


        char s[] = { 0x42, 0x4d, 0x36, 0x20, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x38, 0x04, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0xfd, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        FILE* f;
        f = fopen("test2.bmp", "wb");
        fwrite(s, 1, 54, f);
        fwrite(dib, 1, image_size, f);
        fclose(f);

        FIBITMAP* image = FreeImage_ConvertFromRawBits(dib, w, h, ((((24 * w) + 31) / 32) * 4), 24, 0x0000FF, 0xFF0000, 0x00FF00, 0);
        FreeImage_Save(FIF_BMP, dib1, "new_test_november.bmp", 0);


    }
    else
    {
        printf("BitMap err\n");
    }
    DeleteDC(hdcMemory);
    ReleaseDC(hwnd, hdcSource);
}

int main()
{
    screenCapture(0, 0, 3840, 1080, L"MyFirstScreeshot.bmp");

    return 0;
}
