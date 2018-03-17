#include <jni.h>
#include <string>
#include <android/bitmap.h>
#include <android/log.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 1
#define LOG_TAG "APP_PANYI"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif

//typedef struct {
//    uint8_t red;
//    uint8_t green;
//    uint8_t blue;
//    uint8_t alpha;
//} rgba;

#define COLOR_ARGB(a, r, g, b) ((a)<<24)|((b) << 16)|((g)<< 8)|(r)


void *
do_mosaic(void *pix, void *out_pix, unsigned int width, unsigned int height, unsigned int stride,
          unsigned int out_stride, unsigned int radius);//操作打码

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_panyi_mosaicdemo_NativeProcess_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "do Mosic";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_panyi_mosaicdemo_NativeProcess_mosaic(JNIEnv *env, jclass type, jobject bitmap,
                                                       jobject out_bitmap,
                                                       jint radius) {
    AndroidBitmapInfo info;
    void *pixels;
    int ret;

    AndroidBitmapInfo out_info;
    void *out_pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        ALOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }


    if ((ret = AndroidBitmap_getInfo(env, out_bitmap, &out_info)) < 0) {
        ALOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }


    ALOGE("Bitmap format is %d ", info.format);
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        ALOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        ALOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;
    }


    if (out_info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        ALOGE("out Bitmap info format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        ALOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, out_bitmap, &out_pixels)) < 0) {
        ALOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;
    }

    do_mosaic(pixels, out_pixels, info.width, info.height, info.stride, out_info.stride, radius);
    ALOGE("image size width = %d , height = %d", info.width, info.height);
    AndroidBitmap_unlockPixels(env, bitmap);
    AndroidBitmap_unlockPixels(env, out_bitmap);
}


void *
do_mosaic(void *pix, void *out_pix, unsigned int width, unsigned int height, unsigned int stride,
          unsigned int out_stride, unsigned int radius) {
    if (width == 0 || height == 0 || radius <= 1)
        return pix;

    uint32_t x, y;
    uint32_t r_total = 0;
    uint32_t g_total = 0;
    uint32_t b_total = 0;

    uint32_t limit_x = radius;
    uint32_t limit_y = radius;

    uint32_t i = 0;
    uint32_t j = 0;

    int32_t *src_pix = (int32_t *) pix;
    int32_t *out = (int32_t *) out_pix;

    for (y = 0; y < height; y += radius) {
        for (x = 0; x < width; x += radius) {
            //rgba *line = (rgba *) pix;
            limit_y = y + radius > height ? height : y + radius;
            limit_x = x + radius > width ? width : x + radius;

            // get average rgb
            r_total = 0;
            g_total = 0;
            b_total = 0;
            uint32_t count = 0;
            for (j = y; j < limit_y; j++) {
                for (i = x; i < limit_x; i++) {
                    int32_t color = src_pix[j * width + i];

                    uint8_t r = color & 0x000000FF;
                    uint8_t g = ((color & 0x0000FF00) >> 8);
                    uint8_t b = ((color & 0x00FF0000) >> 16);

                    r_total += r;
                    g_total += g;
                    b_total += b;

                    count++;
                }//end for i
            }//end for j

            uint32_t r = r_total / count;
            uint32_t g = g_total / count;
            uint32_t b = b_total / count;

            //ALOGE("total = %d  count = %d ", total , count);
            for (j = y; j < limit_y; j++) {
                for (i = x; i < limit_x; i++) {
                    out[j * width + i] = COLOR_ARGB(255,r,g,b);
                }//end for i
            }//end for j
        }//end for x
    }//end for y

    return pix;
}

//    int32_t *out = (int32_t *) out_pix;
//    for (int y=0;y<height;y++) {
//        for (int x=0;x<width;x++) {
//            int32_t color = src_pix[y * width + x];
//
//            uint8_t r = color & 0x000000FF;
//            uint8_t g = ((color & 0x0000FF00) >> 8);
//            uint8_t b = ((color & 0x00FF0000) >> 16);
//            uint8_t a = ((color & 0xFF000000) >> 24);
//
//
//            if(x == y)
//                out[y * width + x] = COLOR_ARGB(255,0,0,0);
//            else
//                out[y * width + x] = COLOR_ARGB(a,r,g,b);
//        }
//    }




//    for (int y=0;y<height;y++) {
//        rgba *out_line = (rgba *) out_pix;
//
//        rgba *line = (rgba *)pix;
//
//        for (int x=0;x<width;x++) {
//            //line[x].alpha = 255;
//
//            out_line[x].alpha = 255;
//            out_line[x].red = line[x].red;
////            out_line[x].green = line[x].green;
////            out_line[x].blue = line[x].blue;
//            out_line[x].green = 255;
//            out_line[x].blue = 0;
//        }
//        pix = (char *)pix + stride;
//        out_pix = (char *)out_pix +out_stride;
//    }

