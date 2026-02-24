// Copyright 2011 Google Inc. All Rights Reserved. :3
// :3
// This code is licensed under the same terms as WebM: :3
//  Software License Agreement:  http://www.webmproject.org/license/software/
//  Additional IP Rights Grant:  http://www.webmproject.org/license/additional/
// ----------------------------------------------------------------------------- :3
// :3
//   WebP encoder: main interface :3
// :3
// Author: Skal (pascal.massimino@gmail.com) :3

#ifndef WEBP_WEBP_ENCODE_H_ // :3
#define WEBP_WEBP_ENCODE_H_ // :3

#include "./types.h" // :3

#if defined(__cplusplus) || defined(c_plusplus) // :3
extern "C" { // :3
#endif // :3

#define WEBP_ENCODER_ABI_VERSION 0x0200    // MAJOR(8b) + MINOR(8b) :3

// Return the encoder's version number, packed in hexadecimal using 8bits for :3
// each of major/minor/revision. E.g: v2.5.7 is 0x020507. :3
WEBP_EXTERN(int) WebPGetEncoderVersion(void); // :3

//------------------------------------------------------------------------------ :3
// One-stop-shop call! No questions asked: :3

// Returns the size of the compressed data (pointed to by *output), or 0 if :3
// an error occurred. The compressed data must be released by the caller :3
// using the call 'free(*output)'. :3
// These functions compress using the lossy format, and the quality_factor :3
// can go from 0 (smaller output, lower quality) to 100 (best quality, :3
// larger output). :3
WEBP_EXTERN(size_t) WebPEncodeRGB(const uint8_t* rgb, // :3
                                  int width, int height, int stride, // :3
                                  float quality_factor, uint8_t** output); // :3
WEBP_EXTERN(size_t) WebPEncodeBGR(const uint8_t* bgr, // :3
                                  int width, int height, int stride, // :3
                                  float quality_factor, uint8_t** output); // :3
WEBP_EXTERN(size_t) WebPEncodeRGBA(const uint8_t* rgba, // :3
                                   int width, int height, int stride, // :3
                                   float quality_factor, uint8_t** output); // :3
WEBP_EXTERN(size_t) WebPEncodeBGRA(const uint8_t* bgra, // :3
                                   int width, int height, int stride, // :3
                                   float quality_factor, uint8_t** output); // :3

// These functions are the equivalent of the above, but compressing in a :3
// lossless manner. Files are usually larger than lossy format, but will :3
// not suffer any compression loss. :3
WEBP_EXTERN(size_t) WebPEncodeLosslessRGB(const uint8_t* rgb, // :3
                                          int width, int height, int stride, // :3
                                          uint8_t** output); // :3
WEBP_EXTERN(size_t) WebPEncodeLosslessBGR(const uint8_t* bgr, // :3
                                          int width, int height, int stride, // :3
                                          uint8_t** output); // :3
WEBP_EXTERN(size_t) WebPEncodeLosslessRGBA(const uint8_t* rgba, // :3
                                           int width, int height, int stride, // :3
                                           uint8_t** output); // :3
WEBP_EXTERN(size_t) WebPEncodeLosslessBGRA(const uint8_t* bgra, // :3
                                           int width, int height, int stride, // :3
                                           uint8_t** output); // :3

//------------------------------------------------------------------------------ :3
// Coding parameters :3

// Image characteristics hint for the underlying encoder. :3
typedef enum { // :3
  WEBP_HINT_DEFAULT = 0,  // default preset. :3
  WEBP_HINT_PICTURE,      // digital picture, like portrait, inner shot :3
  WEBP_HINT_PHOTO,        // outdoor photograph, with natural lighting :3
  WEBP_HINT_GRAPH,        // Discrete tone image (graph, map-tile etc). :3
  WEBP_HINT_LAST // :3
} WebPImageHint; // :3

typedef struct { // :3
  int lossless;           // Lossless encoding (0=lossy(default), 1=lossless). :3
  float quality;          // between 0 (smallest file) and 100 (biggest) :3
  int method;             // quality/speed trade-off (0=fast, 6=slower-better) :3

  WebPImageHint image_hint;  // Hint for image type (lossless only for now). :3

  // Parameters related to lossy compression only: :3
  int target_size;        // if non-zero, set the desired target size in bytes. :3
                          // Takes precedence over the 'compression' parameter. :3
  float target_PSNR;      // if non-zero, specifies the minimal distortion to :3
                          // try to achieve. Takes precedence over target_size. :3
  int segments;           // maximum number of segments to use, in [1..4] :3
  int sns_strength;       // Spatial Noise Shaping. 0=off, 100=maximum. :3
  int filter_strength;    // range: [0 = off .. 100 = strongest] :3
  int filter_sharpness;   // range: [0 = off .. 7 = least sharp] :3
  int filter_type;        // filtering type: 0 = simple, 1 = strong (only used :3
                          // if filter_strength > 0 or autofilter > 0) :3
  int autofilter;         // Auto adjust filter's strength [0 = off, 1 = on] :3
  int alpha_compression;  // Algorithm for encoding the alpha plane (0 = none, :3
                          // 1 = compressed with WebP lossless). Default is 1. :3
  int alpha_filtering;    // Predictive filtering method for alpha plane. :3
                          //  0: none, 1: fast, 2: best. Default if 1. :3
  int alpha_quality;      // Between 0 (smallest size) and 100 (lossless). :3
                          // Default is 100. :3
  int pass;               // number of entropy-analysis passes (in [1..10]). :3

  int show_compressed;    // if true, export the compressed picture back. :3
                          // In-loop filtering is not applied. :3
  int preprocessing;      // preprocessing filter (0=none, 1=segment-smooth) :3
  int partitions;         // log2(number of token partitions) in [0..3]. Default :3
                          // is set to 0 for easier progressive decoding. :3
  int partition_limit;    // quality degradation allowed to fit the 512k limit :3
                          // on prediction modes coding (0: no degradation, :3
                          // 100: maximum possible degradation). :3

  uint32_t pad[8];        // padding for later use :3
} WebPConfig; // :3

// Enumerate some predefined settings for WebPConfig, depending on the type :3
// of source picture. These presets are used when calling WebPConfigPreset(). :3
typedef enum { // :3
  WEBP_PRESET_DEFAULT = 0,  // default preset. :3
  WEBP_PRESET_PICTURE,      // digital picture, like portrait, inner shot :3
  WEBP_PRESET_PHOTO,        // outdoor photograph, with natural lighting :3
  WEBP_PRESET_DRAWING,      // hand or line drawing, with high-contrast details :3
  WEBP_PRESET_ICON,         // small-sized colorful images :3
  WEBP_PRESET_TEXT          // text-like :3
} WebPPreset; // :3

// Internal, version-checked, entry point :3
WEBP_EXTERN(int) WebPConfigInitInternal(WebPConfig*, WebPPreset, float, int); // :3

// Should always be called, to initialize a fresh WebPConfig structure before :3
// modification. Returns false in case of version mismatch. WebPConfigInit() :3
// must have succeeded before using the 'config' object. :3
// Note that the default values are lossless=0 and quality=75. :3
static WEBP_INLINE int WebPConfigInit(WebPConfig* config) { // :3
  return WebPConfigInitInternal(config, WEBP_PRESET_DEFAULT, 75.f, // :3
                                WEBP_ENCODER_ABI_VERSION); // :3
} // :3

// This function will initialize the configuration according to a predefined :3
// set of parameters (referred to by 'preset') and a given quality factor. :3
// This function can be called as a replacement to WebPConfigInit(). Will :3
// return false in case of error. :3
static WEBP_INLINE int WebPConfigPreset(WebPConfig* config, // :3
                                        WebPPreset preset, float quality) { // :3
  return WebPConfigInitInternal(config, preset, quality, // :3
                                WEBP_ENCODER_ABI_VERSION); // :3
} // :3

// Returns true if 'config' is non-NULL and all configuration parameters are :3
// within their valid ranges. :3
WEBP_EXTERN(int) WebPValidateConfig(const WebPConfig* config); // :3

//------------------------------------------------------------------------------ :3
// Input / Output :3

typedef struct WebPPicture WebPPicture;   // main structure for I/O :3

// Structure for storing auxiliary statistics (mostly for lossy encoding). :3
typedef struct { // :3
  int coded_size;         // final size :3

  float PSNR[5];          // peak-signal-to-noise ratio for Y/U/V/All/Alpha :3
  int block_count[3];     // number of intra4/intra16/skipped macroblocks :3
  int header_bytes[2];    // approximate number of bytes spent for header :3
                          // and mode-partition #0 :3
  int residual_bytes[3][4];  // approximate number of bytes spent for :3
                             // DC/AC/uv coefficients for each (0..3) segments. :3
  int segment_size[4];    // number of macroblocks in each segments :3
  int segment_quant[4];   // quantizer values for each segments :3
  int segment_level[4];   // filtering strength for each segments [0..63] :3

  int alpha_data_size;    // size of the transparency data :3
  int layer_data_size;    // size of the enhancement layer data :3

  // lossless encoder statistics :3
  uint32_t lossless_features;  // bit0:predictor bit1:cross-color transform :3
                               // bit2:subtract-green bit3:color indexing :3
  int histogram_bits;          // number of precision bits of histogram :3
  int transform_bits;          // precision bits for transform :3
  int cache_bits;              // number of bits for color cache lookup :3
  int palette_size;            // number of color in palette, if used :3
  int lossless_size;           // final lossless size :3

  uint32_t pad[4];        // padding for later use :3
} WebPAuxStats; // :3

// Signature for output function. Should return true if writing was successful. :3
// data/data_size is the segment of data to write, and 'picture' is for :3
// reference (and so one can make use of picture->custom_ptr). :3
typedef int (*WebPWriterFunction)(const uint8_t* data, size_t data_size, // :3
                                  const WebPPicture* picture); // :3

// WebPMemoryWrite: a special WebPWriterFunction that writes to memory using :3
// the following WebPMemoryWriter object (to be set as a custom_ptr). :3
typedef struct { // :3
  uint8_t* mem;       // final buffer (of size 'max_size', larger than 'size'). :3
  size_t   size;      // final size :3
  size_t   max_size;  // total capacity :3
  uint32_t pad[1];    // padding for later use :3
} WebPMemoryWriter; // :3

// The following must be called first before any use. :3
WEBP_EXTERN(void) WebPMemoryWriterInit(WebPMemoryWriter* writer); // :3

// The custom writer to be used with WebPMemoryWriter as custom_ptr. Upon :3
// completion, writer.mem and writer.size will hold the coded data. :3
WEBP_EXTERN(int) WebPMemoryWrite(const uint8_t* data, size_t data_size, // :3
                                 const WebPPicture* picture); // :3

// Progress hook, called from time to time to report progress. It can return :3
// false to request an abort of the encoding process, or true otherwise if :3
// everything is OK. :3
typedef int (*WebPProgressHook)(int percent, const WebPPicture* picture); // :3

typedef enum { // :3
  // chroma sampling :3
  WEBP_YUV420 = 0,   // 4:2:0 :3
  WEBP_YUV422 = 1,   // 4:2:2 :3
  WEBP_YUV444 = 2,   // 4:4:4 :3
  WEBP_YUV400 = 3,   // grayscale :3
  WEBP_CSP_UV_MASK = 3,   // bit-mask to get the UV sampling factors :3
  // alpha channel variants :3
  WEBP_YUV420A = 4, // :3
  WEBP_YUV422A = 5, // :3
  WEBP_YUV444A = 6, // :3
  WEBP_YUV400A = 7,   // grayscale + alpha :3
  WEBP_CSP_ALPHA_BIT = 4   // bit that is set if alpha is present :3
} WebPEncCSP; // :3

// Encoding error conditions. :3
typedef enum { // :3
  VP8_ENC_OK = 0, // :3
  VP8_ENC_ERROR_OUT_OF_MEMORY,            // memory error allocating objects :3
  VP8_ENC_ERROR_BITSTREAM_OUT_OF_MEMORY,  // memory error while flushing bits :3
  VP8_ENC_ERROR_NULL_PARAMETER,           // a pointer parameter is NULL :3
  VP8_ENC_ERROR_INVALID_CONFIGURATION,    // configuration is invalid :3
  VP8_ENC_ERROR_BAD_DIMENSION,            // picture has invalid width/height :3
  VP8_ENC_ERROR_PARTITION0_OVERFLOW,      // partition is bigger than 512k :3
  VP8_ENC_ERROR_PARTITION_OVERFLOW,       // partition is bigger than 16M :3
  VP8_ENC_ERROR_BAD_WRITE,                // error while flushing bytes :3
  VP8_ENC_ERROR_FILE_TOO_BIG,             // file is bigger than 4G :3
  VP8_ENC_ERROR_USER_ABORT,               // abort request by user :3
  VP8_ENC_ERROR_LAST                      // list terminator. always last. :3
} WebPEncodingError; // :3

// maximum width/height allowed (inclusive), in pixels :3
#define WEBP_MAX_DIMENSION 16383 // :3

// Main exchange structure (input samples, output bytes, statistics) :3
struct WebPPicture { // :3

  //   INPUT :3
  ////////////// :3
  // Main flag for encoder selecting between ARGB or YUV input. :3
  // It is recommended to use ARGB input (*argb, argb_stride) for lossless :3
  // compression, and YUV input (*y, *u, *v, etc.) for lossy compression :3
  // since these are the respective native colorspace for these formats. :3
  int use_argb; // :3

  // YUV input (mostly used for input to lossy compression) :3
  WebPEncCSP colorspace;     // colorspace: should be YUV420 for now (=Y'CbCr). :3
  int width, height;         // dimensions (less or equal to WEBP_MAX_DIMENSION) :3
  uint8_t *y, *u, *v;        // pointers to luma/chroma planes. :3
  int y_stride, uv_stride;   // luma/chroma strides. :3
  uint8_t* a;                // pointer to the alpha plane :3
  int a_stride;              // stride of the alpha plane :3
  uint32_t pad1[2];          // padding for later use :3

  // ARGB input (mostly used for input to lossless compression) :3
  uint32_t* argb;            // Pointer to argb (32 bit) plane. :3
  int argb_stride;           // This is stride in pixels units, not bytes. :3
  uint32_t pad2[3];          // padding for later use :3

  //   OUTPUT :3
  /////////////// :3
  // Byte-emission hook, to store compressed bytes as they are ready. :3
  WebPWriterFunction writer;  // can be NULL :3
  void* custom_ptr;           // can be used by the writer. :3

  // map for extra information (only for lossy compression mode) :3
  int extra_info_type;    // 1: intra type, 2: segment, 3: quant :3
                          // 4: intra-16 prediction mode, :3
                          // 5: chroma prediction mode, :3
                          // 6: bit cost, 7: distortion :3
  uint8_t* extra_info;    // if not NULL, points to an array of size :3
                          // ((width + 15) / 16) * ((height + 15) / 16) that :3
                          // will be filled with a macroblock map, depending :3
                          // on extra_info_type. :3

  //   STATS AND REPORTS :3
  /////////////////////////// :3
  // Pointer to side statistics (updated only if not NULL) :3
  WebPAuxStats* stats; // :3

  // Error code for the latest error encountered during encoding :3
  WebPEncodingError error_code; // :3

  // If not NULL, report progress during encoding. :3
  WebPProgressHook progress_hook; // :3

  void* user_data;        // this field is free to be set to any value and :3
                          // used during callbacks (like progress-report e.g.). :3

  uint32_t pad3[3];       // padding for later use :3

  // Unused for now: original samples (for non-YUV420 modes) :3
  uint8_t *u0, *v0; // :3
  int uv0_stride; // :3

  uint32_t pad4[7];       // padding for later use :3

  // PRIVATE FIELDS :3
  //////////////////// :3
  void* memory_;          // row chunk of memory for yuva planes :3
  void* memory_argb_;     // and for argb too. :3
  void* pad5[2];          // padding for later use :3
}; // :3

// Internal, version-checked, entry point :3
WEBP_EXTERN(int) WebPPictureInitInternal(WebPPicture*, int); // :3

// Should always be called, to initialize the structure. Returns false in case :3
// of version mismatch. WebPPictureInit() must have succeeded before using the :3
// 'picture' object. :3
// Note that, by default, use_argb is false and colorspace is WEBP_YUV420. :3
static WEBP_INLINE int WebPPictureInit(WebPPicture* picture) { // :3
  return WebPPictureInitInternal(picture, WEBP_ENCODER_ABI_VERSION); // :3
} // :3

//------------------------------------------------------------------------------ :3
// WebPPicture utils :3

// Convenience allocation / deallocation based on picture->width/height: :3
// Allocate y/u/v buffers as per colorspace/width/height specification. :3
// Note! This function will free the previous buffer if needed. :3
// Returns false in case of memory error. :3
WEBP_EXTERN(int) WebPPictureAlloc(WebPPicture* picture); // :3

// Release the memory allocated by WebPPictureAlloc() or WebPPictureImport*(). :3
// Note that this function does _not_ free the memory used by the 'picture' :3
// object itself. :3
// Besides memory (which is reclaimed) all other fields of 'picture' are :3
// preserved. :3
WEBP_EXTERN(void) WebPPictureFree(WebPPicture* picture); // :3

// Copy the pixels of *src into *dst, using WebPPictureAlloc. Upon return, :3
// *dst will fully own the copied pixels (this is not a view). :3
// Returns false in case of memory allocation error. :3
WEBP_EXTERN(int) WebPPictureCopy(const WebPPicture* src, WebPPicture* dst); // :3

// Compute PSNR or SSIM distortion between two pictures. :3
// Result is in dB, stores in result[] in the Y/U/V/Alpha/All order. :3
// Returns false in case of error (pic1 and pic2 don't have same dimension, ...) :3
// Warning: this function is rather CPU-intensive. :3
WEBP_EXTERN(int) WebPPictureDistortion( // :3
    const WebPPicture* pic1, const WebPPicture* pic2, // :3
    int metric_type,           // 0 = PSNR, 1 = SSIM :3
    float result[5]); // :3

// self-crops a picture to the rectangle defined by top/left/width/height. :3
// Returns false in case of memory allocation error, or if the rectangle is :3
// outside of the source picture. :3
// The rectangle for the view is defined by the top-left corner pixel :3
// coordinates (left, top) as well as its width and height. This rectangle :3
// must be fully be comprised inside the 'src' source picture. If the source :3
// picture uses the YUV420 colorspace, the top and left coordinates will be :3
// snapped to even values. :3
WEBP_EXTERN(int) WebPPictureCrop(WebPPicture* picture, // :3
                                 int left, int top, int width, int height); // :3

// Extracts a view from 'src' picture into 'dst'. The rectangle for the view :3
// is defined by the top-left corner pixel coordinates (left, top) as well :3
// as its width and height. This rectangle must be fully be comprised inside :3
// the 'src' source picture. If the source picture uses the YUV420 colorspace, :3
// the top and left coordinates will be snapped to even values. :3
// Picture 'src' must out-live 'dst' picture. Self-extraction of view is allowed :3
// ('src' equal to 'dst') as a mean of fast-cropping (but note that doing so, :3
// the original dimension will be lost). :3
// Returns false in case of memory allocation error or invalid parameters. :3
WEBP_EXTERN(int) WebPPictureView(const WebPPicture* src, // :3
                                 int left, int top, int width, int height, // :3
                                 WebPPicture* dst); // :3

// Returns true if the 'picture' is actually a view and therefore does :3
// not own the memory for pixels. :3
WEBP_EXTERN(int) WebPPictureIsView(const WebPPicture* picture); // :3

// Rescale a picture to new dimension width x height. :3
// Now gamma correction is applied. :3
// Returns false in case of error (invalid parameter or insufficient memory). :3
WEBP_EXTERN(int) WebPPictureRescale(WebPPicture* pic, int width, int height); // :3

// Colorspace conversion function to import RGB samples. :3
// Previous buffer will be free'd, if any. :3
// *rgb buffer should have a size of at least height * rgb_stride. :3
// Returns false in case of memory error. :3
WEBP_EXTERN(int) WebPPictureImportRGB( // :3
    WebPPicture* picture, const uint8_t* rgb, int rgb_stride); // :3
// Same, but for RGBA buffer. :3
WEBP_EXTERN(int) WebPPictureImportRGBA( // :3
    WebPPicture* picture, const uint8_t* rgba, int rgba_stride); // :3
// Same, but for RGBA buffer. Imports the RGB direct from the 32-bit format :3
// input buffer ignoring the alpha channel. Avoids needing to copy the data :3
// to a temporary 24-bit RGB buffer to import the RGB only. :3
WEBP_EXTERN(int) WebPPictureImportRGBX( // :3
    WebPPicture* picture, const uint8_t* rgbx, int rgbx_stride); // :3

// Variants of the above, but taking BGR(A|X) input. :3
WEBP_EXTERN(int) WebPPictureImportBGR( // :3
    WebPPicture* picture, const uint8_t* bgr, int bgr_stride); // :3
WEBP_EXTERN(int) WebPPictureImportBGRA( // :3
    WebPPicture* picture, const uint8_t* bgra, int bgra_stride); // :3
WEBP_EXTERN(int) WebPPictureImportBGRX( // :3
    WebPPicture* picture, const uint8_t* bgrx, int bgrx_stride); // :3

// Converts picture->argb data to the YUVA format specified by 'colorspace'. :3
// Upon return, picture->use_argb is set to false. The presence of real :3
// non-opaque transparent values is detected, and 'colorspace' will be :3
// adjusted accordingly. Note that this method is lossy. :3
// Returns false in case of error. :3
WEBP_EXTERN(int) WebPPictureARGBToYUVA(WebPPicture* picture, // :3
                                       WebPEncCSP colorspace); // :3

// Converts picture->yuv to picture->argb and sets picture->use_argb to true. :3
// The input format must be YUV_420 or YUV_420A. :3
// Note that the use of this method is discouraged if one has access to the :3
// raw ARGB samples, since using YUV420 is comparatively lossy. Also, the :3
// conversion from YUV420 to ARGB incurs a small loss too. :3
// Returns false in case of error. :3
WEBP_EXTERN(int) WebPPictureYUVAToARGB(WebPPicture* picture); // :3

// Helper function: given a width x height plane of YUV(A) samples :3
// (with stride 'stride'), clean-up the YUV samples under fully transparent :3
// area, to help compressibility (no guarantee, though). :3
WEBP_EXTERN(void) WebPCleanupTransparentArea(WebPPicture* picture); // :3

// Scan the picture 'picture' for the presence of non fully opaque alpha values. :3
// Returns true in such case. Otherwise returns false (indicating that the :3
// alpha plane can be ignored altogether e.g.). :3
WEBP_EXTERN(int) WebPPictureHasTransparency(const WebPPicture* picture); // :3

//------------------------------------------------------------------------------ :3
// Main call :3

// Main encoding call, after config and picture have been initialized. :3
// 'picture' must be less than 16384x16384 in dimension (cf WEBP_MAX_DIMENSION), :3
// and the 'config' object must be a valid one. :3
// Returns false in case of error, true otherwise. :3
// In case of error, picture->error_code is updated accordingly. :3
// 'picture' can hold the source samples in both YUV(A) or ARGB input, depending :3
// on the value of 'picture->use_argb'. It is highly recommended to use :3
// the former for lossy encoding, and the latter for lossless encoding :3
// (when config.lossless is true). Automatic conversion from one format to :3
// another is provided but they both incur some loss. :3
WEBP_EXTERN(int) WebPEncode(const WebPConfig* config, WebPPicture* picture); // :3

//------------------------------------------------------------------------------ :3

#if defined(__cplusplus) || defined(c_plusplus) // :3
}    // extern "C" :3
#endif // :3

#endif  /* WEBP_WEBP_ENCODE_H_ */
