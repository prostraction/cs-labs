#ifndef WATERMARKEMBEDDING_H
#define WATERMARKEMBEDDING_H

#include <iostream>
#include "codingmath.h"
#include "watermarkimage.h"

class WatermarkEmbedding {
public:
    /*------------------------------------------------------------------------------*/
    /// ДВП Сангхави
    static bool         EncodeBitmapSanghavi(char                   *Bitmap,
                                             const WatermarkImage   *WImage,
                                             const uint32_t         &ImageX,
                                             const uint32_t         &ImageY,
                                             const uint8_t          &ContainerType);
    /// ДВП Сангхави
    static WatermarkImage* DecodeBitmapSanghavi(const char           *Bitmap,
                                             const WatermarkImage   *WImage,
                                             const uint32_t         &ImageX,
                                             const uint32_t         &ImageY,
                                             const uint8_t          &ContainerType);
    /*------------------------------------------------------------------------------*/
    /// ДВП Сохеили
    static bool         EncodeBitmapSoheili(char                    *Bitmap,
                                            const WatermarkImage    *WImage,
                                            const uint32_t          &ImageX,
                                            const uint32_t          &ImageY,
                                            const int32_t           &Q,
                                            const uint8_t           &ContainerType);
    /// ДВП Сохеили
    static WatermarkImage* DecodeBitmapSoheili(const char              *Bitmap,
                                               const WatermarkImage    *WImage,
                                               const uint32_t          &ImageX,
                                               const uint32_t          &ImageY,
                                               const int32_t           &Q,
                                               const uint8_t           &ContainerType);
    /*------------------------------------------------------------------------------*/
    /// ДКП Коча-Бенхама
    static bool         EncodeBitmapBenham(char                 *Bitmap,
                                           const WatermarkImage *WImage,
                                           const std::string    &PassWord,
                                           const uint32_t       &ImageX,
                                           const uint32_t       &ImageY,
                                           const int32_t        &PositiveMod,
                                           const int32_t        &NegativeMod,
                                           const uint8_t        &ContainerType);
    /// ДКП Коча-Бенхама
    static WatermarkImage*   DecodeBitmapBenham(const char           *Bitmap,
                                                const WatermarkImage *WImage,
                                                const std::string    &PassWord,
                                                const uint32_t       &ImageX,
                                                const uint32_t       &ImageY,
                                                const uint8_t        &ContainerType);

    /*------------------------------------------------------------------------------*/
    static bool         EncodeLSB_Bitmap(char              *Bitmap,
                                         const uint32_t    &BitmapSize,
                                         const std::string &WordToEncode);

    static std::string  DecodeLSB_Bitmap(const char        *Bitmap,
                                         const uint32_t    &BitmapSize);

private:
    /// ДВП Сохеили
    static uint32_t DecodeWaveletSoheiliOneStep(const float     *WaveletL3_HL,
                                                const uint32_t  &WaveletX,
                                                const uint32_t  &WaveletY,
                                                const uint32_t  &Q,
                                                const uint32_t  &WatermarkImageX,
                                                const uint32_t  &WatermarkImageY,
                                                const float     &Coefficient_m_0_25,
                                                const float     &Coefficient_m_0_75,
                                                int32_t         *WatermarkRecovered);

    WatermarkEmbedding();
};

#endif // WATERMARKEMBEDDING_H
