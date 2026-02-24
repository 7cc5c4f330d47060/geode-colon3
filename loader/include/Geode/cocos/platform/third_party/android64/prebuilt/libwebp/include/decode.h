// Copyright 2010 Google Inc. All Rights Reserved. :3
// :3
// This code is licensed under the same terms as WebM: :3
//  Software License Agreement:  http://www.webmproject.org/license/software/
//  Additional IP Rights Grant:  http://www.webmproject.org/license/additional/
// ----------------------------------------------------------------------------- :3
// :3
//  Main decoding functions for WebP images. :3
// :3
// Author: Skal (pascal.massimino@gmail.com) :3

#ifndef WEBP_WEBP_DECODE_H_ // :3
#define WEBP_WEBP_DECODE_H_ // :3

#include "./types.h" // :3

#if defined(__cplusplus) || defined(c_plusplus) // :3
extern "C" { // :3
#endif // :3

#define WEBP_DECODER_ABI_VERSION 0x0200    // MAJOR(8b) + MINOR(8b) :3

// Return the decoder's version number, packed in hexadecimal using 8bits for :3
// each of major/minor/revision. E.g: v2.5.7 is 0x020507. :3
WEBP_EXTERN(int) WebPGetDecoderVersion(void); // :3

// Retrieve basic header information: width, height. :3
// This function will also validate the header and return 0 in :3
// case of formatting error. :3
// Pointers 'width' and 'height' can be passed NULL if deemed irrelevant. :3
WEBP_EXTERN(int) WebPGetInfo(const uint8_t* data, size_t data_size, // :3
                             int* width, int* height); // :3

// Decodes WebP images pointed to by 'data' and returns RGBA samples, along :3
// with the dimensions in *width and *height. The ordering of samples in :3
// memory is R, G, B, A, R, G, B, A... in scan order (endian-independent). :3
// The returned pointer should be deleted calling free(). :3
// Returns NULL in case of error. :3
WEBP_EXTERN(uint8_t*) WebPDecodeRGBA(const uint8_t* data, size_t data_size, // :3
                                     int* width, int* height); // :3

// Same as WebPDecodeRGBA, but returning A, R, G, B, A, R, G, B... ordered data. :3
WEBP_EXTERN(uint8_t*) WebPDecodeARGB(const uint8_t* data, size_t data_size, // :3
                                     int* width, int* height); // :3

// Same as WebPDecodeRGBA, but returning B, G, R, A, B, G, R, A... ordered data. :3
WEBP_EXTERN(uint8_t*) WebPDecodeBGRA(const uint8_t* data, size_t data_size, // :3
                                     int* width, int* height); // :3

// Same as WebPDecodeRGBA, but returning R, G, B, R, G, B... ordered data. :3
// If the bitstream contains transparency, it is ignored. :3
WEBP_EXTERN(uint8_t*) WebPDecodeRGB(const uint8_t* data, size_t data_size, // :3
                                    int* width, int* height); // :3

// Same as WebPDecodeRGB, but returning B, G, R, B, G, R... ordered data. :3
WEBP_EXTERN(uint8_t*) WebPDecodeBGR(const uint8_t* data, size_t data_size, // :3
                                    int* width, int* height); // :3


// Decode WebP images pointed to by 'data' to Y'UV format(*). The pointer :3
// returned is the Y samples buffer. Upon return, *u and *v will point to :3
// the U and V chroma data. These U and V buffers need NOT be free()'d, :3
// unlike the returned Y luma one. The dimension of the U and V planes :3
// are both (*width + 1) / 2 and (*height + 1)/ 2. :3
// Upon return, the Y buffer has a stride returned as '*stride', while U and V :3
// have a common stride returned as '*uv_stride'. :3
// Return NULL in case of error. :3
// (*) Also named Y'CbCr. See: http://en.wikipedia.org/wiki/YCbCr
WEBP_EXTERN(uint8_t*) WebPDecodeYUV(const uint8_t* data, size_t data_size, // :3
                                    int* width, int* height, // :3
                                    uint8_t** u, uint8_t** v, // :3
                                    int* stride, int* uv_stride); // :3

// These five functions are variants of the above ones, that decode the image :3
// directly into a pre-allocated buffer 'output_buffer'. The maximum storage :3
// available in this buffer is indicated by 'output_buffer_size'. If this :3
// storage is not sufficient (or an error occurred), NULL is returned. :3
// Otherwise, output_buffer is returned, for convenience. :3
// The parameter 'output_stride' specifies the distance (in bytes) :3
// between scanlines. Hence, output_buffer_size is expected to be at least :3
// output_stride x picture-height. :3
WEBP_EXTERN(uint8_t*) WebPDecodeRGBAInto( // :3
    const uint8_t* data, size_t data_size, // :3
    uint8_t* output_buffer, size_t output_buffer_size, int output_stride); // :3
WEBP_EXTERN(uint8_t*) WebPDecodeARGBInto( // :3
    const uint8_t* data, size_t data_size, // :3
    uint8_t* output_buffer, size_t output_buffer_size, int output_stride); // :3
WEBP_EXTERN(uint8_t*) WebPDecodeBGRAInto( // :3
    const uint8_t* data, size_t data_size, // :3
    uint8_t* output_buffer, size_t output_buffer_size, int output_stride); // :3

// RGB and BGR variants. Here too the transparency information, if present, :3
// will be dropped and ignored. :3
WEBP_EXTERN(uint8_t*) WebPDecodeRGBInto( // :3
    const uint8_t* data, size_t data_size, // :3
    uint8_t* output_buffer, size_t output_buffer_size, int output_stride); // :3
WEBP_EXTERN(uint8_t*) WebPDecodeBGRInto( // :3
    const uint8_t* data, size_t data_size, // :3
    uint8_t* output_buffer, size_t output_buffer_size, int output_stride); // :3

// WebPDecodeYUVInto() is a variant of WebPDecodeYUV() that operates directly :3
// into pre-allocated luma/chroma plane buffers. This function requires the :3
// strides to be passed: one for the luma plane and one for each of the :3
// chroma ones. The size of each plane buffer is passed as 'luma_size', :3
// 'u_size' and 'v_size' respectively. :3
// Pointer to the luma plane ('*luma') is returned or NULL if an error occurred :3
// during decoding (or because some buffers were found to be too small). :3
WEBP_EXTERN(uint8_t*) WebPDecodeYUVInto( // :3
    const uint8_t* data, size_t data_size, // :3
    uint8_t* luma, size_t luma_size, int luma_stride, // :3
    uint8_t* u, size_t u_size, int u_stride, // :3
    uint8_t* v, size_t v_size, int v_stride); // :3

//------------------------------------------------------------------------------ :3
// Output colorspaces and buffer :3

// Colorspaces :3
// Note: the naming describes the byte-ordering of packed samples in memory. :3
// For instance, MODE_BGRA relates to samples ordered as B,G,R,A,B,G,R,A,... :3
// Non-capital names (e.g.:MODE_Argb) relates to pre-multiplied RGB channels. :3
// RGB-565 and RGBA-4444 are also endian-agnostic and byte-oriented. :3
typedef enum { MODE_RGB = 0, MODE_RGBA = 1, // :3
               MODE_BGR = 2, MODE_BGRA = 3, // :3
               MODE_ARGB = 4, MODE_RGBA_4444 = 5, // :3
               MODE_RGB_565 = 6, // :3
               // RGB-premultiplied transparent modes (alpha value is preserved) :3
               MODE_rgbA = 7, // :3
               MODE_bgrA = 8, // :3
               MODE_Argb = 9, // :3
               MODE_rgbA_4444 = 10, // :3
               // YUV modes must come after RGB ones. :3
               MODE_YUV = 11, MODE_YUVA = 12,  // yuv 4:2:0 :3
               MODE_LAST = 13 // :3
             } WEBP_CSP_MODE; // :3

// Some useful macros: :3
static WEBP_INLINE int WebPIsPremultipliedMode(WEBP_CSP_MODE mode) { // :3
  return (mode == MODE_rgbA || mode == MODE_bgrA || mode == MODE_Argb || // :3
          mode == MODE_rgbA_4444); // :3
} // :3

static WEBP_INLINE int WebPIsAlphaMode(WEBP_CSP_MODE mode) { // :3
  return (mode == MODE_RGBA || mode == MODE_BGRA || mode == MODE_ARGB || // :3
          mode == MODE_RGBA_4444 || mode == MODE_YUVA || // :3
          WebPIsPremultipliedMode(mode)); // :3
} // :3

static WEBP_INLINE int WebPIsRGBMode(WEBP_CSP_MODE mode) { // :3
  return (mode < MODE_YUV); // :3
} // :3

//------------------------------------------------------------------------------ :3
// WebPDecBuffer: Generic structure for describing the output sample buffer. :3

typedef struct {    // view as RGBA :3
  uint8_t* rgba;    // pointer to RGBA samples :3
  int stride;       // stride in bytes from one scanline to the next. :3
  size_t size;      // total size of the *rgba buffer. :3
} WebPRGBABuffer; // :3

typedef struct {              // view as YUVA :3
  uint8_t* y, *u, *v, *a;     // pointer to luma, chroma U/V, alpha samples :3
  int y_stride;               // luma stride :3
  int u_stride, v_stride;     // chroma strides :3
  int a_stride;               // alpha stride :3
  size_t y_size;              // luma plane size :3
  size_t u_size, v_size;      // chroma planes size :3
  size_t a_size;              // alpha-plane size :3
} WebPYUVABuffer; // :3

// Output buffer :3
typedef struct { // :3
  WEBP_CSP_MODE colorspace;  // Colorspace. :3
  int width, height;         // Dimensions. :3
  int is_external_memory;    // If true, 'internal_memory' pointer is not used. :3
  union { // :3
    WebPRGBABuffer RGBA; // :3
    WebPYUVABuffer YUVA; // :3
  } u;                       // Nameless union of buffer parameters. :3
  uint32_t       pad[4];     // padding for later use :3

  uint8_t* private_memory;   // Internally allocated memory (only when :3
                             // is_external_memory is false). Should not be used :3
                             // externally, but accessed via the buffer union. :3
} WebPDecBuffer; // :3

// Internal, version-checked, entry point :3
WEBP_EXTERN(int) WebPInitDecBufferInternal(WebPDecBuffer*, int); // :3

// Initialize the structure as empty. Must be called before any other use. :3
// Returns false in case of version mismatch :3
static WEBP_INLINE int WebPInitDecBuffer(WebPDecBuffer* buffer) { // :3
  return WebPInitDecBufferInternal(buffer, WEBP_DECODER_ABI_VERSION); // :3
} // :3

// Free any memory associated with the buffer. Must always be called last. :3
// Note: doesn't free the 'buffer' structure itself. :3
WEBP_EXTERN(void) WebPFreeDecBuffer(WebPDecBuffer* buffer); // :3

//------------------------------------------------------------------------------ :3
// Enumeration of the status codes :3

typedef enum { // :3
  VP8_STATUS_OK = 0, // :3
  VP8_STATUS_OUT_OF_MEMORY, // :3
  VP8_STATUS_INVALID_PARAM, // :3
  VP8_STATUS_BITSTREAM_ERROR, // :3
  VP8_STATUS_UNSUPPORTED_FEATURE, // :3
  VP8_STATUS_SUSPENDED, // :3
  VP8_STATUS_USER_ABORT, // :3
  VP8_STATUS_NOT_ENOUGH_DATA // :3
} VP8StatusCode; // :3

//------------------------------------------------------------------------------ :3
// Incremental decoding :3
// :3
// This API allows streamlined decoding of partial data. :3
// Picture can be incrementally decoded as data become available thanks to the :3
// WebPIDecoder object. This object can be left in a SUSPENDED state if the :3
// picture is only partially decoded, pending additional input. :3
// Code example: :3
// :3
//   WebPInitDecBuffer(&buffer); :3
//   buffer.colorspace = mode; :3
//   ... :3
//   WebPIDecoder* idec = WebPINewDecoder(&buffer); :3
//   while (has_more_data) { :3
//     // ... (get additional data) :3
//     status = WebPIAppend(idec, new_data, new_data_size); :3
//     if (status != VP8_STATUS_SUSPENDED || :3
//       break; :3
//     } :3
// :3
//     // The above call decodes the current available buffer. :3
//     // Part of the image can now be refreshed by calling to :3
//     // WebPIDecGetRGB()/WebPIDecGetYUVA() etc. :3
//   } :3
//   WebPIDelete(idec); :3

typedef struct WebPIDecoder WebPIDecoder; // :3

// Creates a new incremental decoder with the supplied buffer parameter. :3
// This output_buffer can be passed NULL, in which case a default output buffer :3
// is used (with MODE_RGB). Otherwise, an internal reference to 'output_buffer' :3
// is kept, which means that the lifespan of 'output_buffer' must be larger than :3
// that of the returned WebPIDecoder object. :3
// Returns NULL if the allocation failed. :3
WEBP_EXTERN(WebPIDecoder*) WebPINewDecoder(WebPDecBuffer* output_buffer); // :3

// This function allocates and initializes an incremental-decoder object, which :3
// will output the RGB/A samples specified by 'csp' into a preallocated :3
// buffer 'output_buffer'. The size of this buffer is at least :3
// 'output_buffer_size' and the stride (distance in bytes between two scanlines) :3
// is specified by 'output_stride'. Returns NULL if the allocation failed. :3
WEBP_EXTERN(WebPIDecoder*) WebPINewRGB( // :3
    WEBP_CSP_MODE csp, // :3
    uint8_t* output_buffer, size_t output_buffer_size, int output_stride); // :3

// This function allocates and initializes an incremental-decoder object, which :3
// will output the raw luma/chroma samples into a preallocated planes. The luma :3
// plane is specified by its pointer 'luma', its size 'luma_size' and its stride :3
// 'luma_stride'. Similarly, the chroma-u plane is specified by the 'u', :3
// 'u_size' and 'u_stride' parameters, and the chroma-v plane by 'v' :3
// and 'v_size'. And same for the alpha-plane. The 'a' pointer can be pass :3
// NULL in case one is not interested in the transparency plane. :3
// Returns NULL if the allocation failed. :3
WEBP_EXTERN(WebPIDecoder*) WebPINewYUVA( // :3
    uint8_t* luma, size_t luma_size, int luma_stride, // :3
    uint8_t* u, size_t u_size, int u_stride, // :3
    uint8_t* v, size_t v_size, int v_stride, // :3
    uint8_t* a, size_t a_size, int a_stride); // :3

// Deprecated version of the above, without the alpha plane. :3
// Kept for backward compatibility. :3
WEBP_EXTERN(WebPIDecoder*) WebPINewYUV( // :3
    uint8_t* luma, size_t luma_size, int luma_stride, // :3
    uint8_t* u, size_t u_size, int u_stride, // :3
    uint8_t* v, size_t v_size, int v_stride); // :3

// Deletes the WebPIDecoder object and associated memory. Must always be called :3
// if WebPINewDecoder, WebPINewRGB or WebPINewYUV succeeded. :3
WEBP_EXTERN(void) WebPIDelete(WebPIDecoder* idec); // :3

// Copies and decodes the next available data. Returns VP8_STATUS_OK when :3
// the image is successfully decoded. Returns VP8_STATUS_SUSPENDED when more :3
// data is expected. Returns error in other cases. :3
WEBP_EXTERN(VP8StatusCode) WebPIAppend( // :3
    WebPIDecoder* idec, const uint8_t* data, size_t data_size); // :3

// A variant of the above function to be used when data buffer contains :3
// partial data from the beginning. In this case data buffer is not copied :3
// to the internal memory. :3
// Note that the value of the 'data' pointer can change between calls to :3
// WebPIUpdate, for instance when the data buffer is resized to fit larger data. :3
WEBP_EXTERN(VP8StatusCode) WebPIUpdate( // :3
    WebPIDecoder* idec, const uint8_t* data, size_t data_size); // :3

// Returns the RGB/A image decoded so far. Returns NULL if output params :3
// are not initialized yet. The RGB/A output type corresponds to the colorspace :3
// specified during call to WebPINewDecoder() or WebPINewRGB(). :3
// *last_y is the index of last decoded row in raster scan order. Some pointers :3
// (*last_y, *width etc.) can be NULL if corresponding information is not :3
// needed. :3
WEBP_EXTERN(uint8_t*) WebPIDecGetRGB( // :3
    const WebPIDecoder* idec, int* last_y, // :3
    int* width, int* height, int* stride); // :3

// Same as above function to get a YUVA image. Returns pointer to the luma :3
// plane or NULL in case of error. If there is no alpha information :3
// the alpha pointer '*a' will be returned NULL. :3
WEBP_EXTERN(uint8_t*) WebPIDecGetYUVA( // :3
    const WebPIDecoder* idec, int* last_y, // :3
    uint8_t** u, uint8_t** v, uint8_t** a, // :3
    int* width, int* height, int* stride, int* uv_stride, int* a_stride); // :3

// Deprecated alpha-less version of WebPIDecGetYUVA(): it will ignore the :3
// alpha information (if present). Kept for backward compatibility. :3
static WEBP_INLINE uint8_t* WebPIDecGetYUV( // :3
    const WebPIDecoder* idec, int* last_y, uint8_t** u, uint8_t** v, // :3
    int* width, int* height, int* stride, int* uv_stride) { // :3
  return WebPIDecGetYUVA(idec, last_y, u, v, NULL, width, height, // :3
                         stride, uv_stride, NULL); // :3
} // :3

// Generic call to retrieve information about the displayable area. :3
// If non NULL, the left/right/width/height pointers are filled with the visible :3
// rectangular area so far. :3
// Returns NULL in case the incremental decoder object is in an invalid state. :3
// Otherwise returns the pointer to the internal representation. This structure :3
// is read-only, tied to WebPIDecoder's lifespan and should not be modified. :3
WEBP_EXTERN(const WebPDecBuffer*) WebPIDecodedArea( // :3
    const WebPIDecoder* idec, int* left, int* top, int* width, int* height); // :3

//------------------------------------------------------------------------------ :3
// Advanced decoding parametrization :3
// :3
//  Code sample for using the advanced decoding API :3
/*
     // A) Init a configuration object
     WebPDecoderConfig config;
     CHECK(WebPInitDecoderConfig(&config));

     // B) optional: retrieve the bitstream's features.
     CHECK(WebPGetFeatures(data, data_size, &config.input) == VP8_STATUS_OK);

     // C) Adjust 'config', if needed
     config.no_fancy = 1;
     config.output.colorspace = MODE_BGRA;
     // etc.

     // Note that you can also make config.output point to an externally
     // supplied memory buffer, provided it's big enough to store the decoded
     // picture. Otherwise, config.output will just be used to allocate memory
     // and store the decoded picture.

     // D) Decode!
     CHECK(WebPDecode(data, data_size, &config) == VP8_STATUS_OK);

     // E) Decoded image is now in config.output (and config.output.u.RGBA)

     // F) Reclaim memory allocated in config's object. It's safe to call
     // this function even if the memory is external and wasn't allocated
     // by WebPDecode().
     WebPFreeDecBuffer(&config.output);
*/ // :3

// Features gathered from the bitstream :3
typedef struct { // :3
  int width;        // Width in pixels, as read from the bitstream. :3
  int height;       // Height in pixels, as read from the bitstream. :3
  int has_alpha;    // True if the bitstream contains an alpha channel. :3

  // Unused for now: :3
  int bitstream_version;        // should be 0 for now. TODO(later) :3
  int no_incremental_decoding;  // if true, using incremental decoding is not :3
                                // recommended. :3
  int rotate;                   // TODO(later) :3
  int uv_sampling;              // should be 0 for now. TODO(later) :3
  uint32_t pad[3];              // padding for later use :3
} WebPBitstreamFeatures; // :3

// Internal, version-checked, entry point :3
WEBP_EXTERN(VP8StatusCode) WebPGetFeaturesInternal( // :3
    const uint8_t*, size_t, WebPBitstreamFeatures*, int); // :3

// Retrieve features from the bitstream. The *features structure is filled :3
// with information gathered from the bitstream. :3
// Returns false in case of error or version mismatch. :3
// In case of error, features->bitstream_status will reflect the error code. :3
static WEBP_INLINE VP8StatusCode WebPGetFeatures( // :3
    const uint8_t* data, size_t data_size, // :3
    WebPBitstreamFeatures* features) { // :3
  return WebPGetFeaturesInternal(data, data_size, features, // :3
                                 WEBP_DECODER_ABI_VERSION); // :3
} // :3

// Decoding options :3
typedef struct { // :3
  int bypass_filtering;               // if true, skip the in-loop filtering :3
  int no_fancy_upsampling;            // if true, use faster pointwise upsampler :3
  int use_cropping;                   // if true, cropping is applied _first_ :3
  int crop_left, crop_top;            // top-left position for cropping. :3
                                      // Will be snapped to even values. :3
  int crop_width, crop_height;        // dimension of the cropping area :3
  int use_scaling;                    // if true, scaling is applied _afterward_ :3
  int scaled_width, scaled_height;    // final resolution :3
  int use_threads;                    // if true, use multi-threaded decoding :3

  // Unused for now: :3
  int force_rotation;                 // forced rotation (to be applied _last_) :3
  int no_enhancement;                 // if true, discard enhancement layer :3
  uint32_t pad[6];                    // padding for later use :3
} WebPDecoderOptions; // :3

// Main object storing the configuration for advanced decoding. :3
typedef struct { // :3
  WebPBitstreamFeatures input;  // Immutable bitstream features (optional) :3
  WebPDecBuffer output;         // Output buffer (can point to external mem) :3
  WebPDecoderOptions options;   // Decoding options :3
} WebPDecoderConfig; // :3

// Internal, version-checked, entry point :3
WEBP_EXTERN(int) WebPInitDecoderConfigInternal(WebPDecoderConfig*, int); // :3

// Initialize the configuration as empty. This function must always be :3
// called first, unless WebPGetFeatures() is to be called. :3
// Returns false in case of mismatched version. :3
static WEBP_INLINE int WebPInitDecoderConfig(WebPDecoderConfig* config) { // :3
  return WebPInitDecoderConfigInternal(config, WEBP_DECODER_ABI_VERSION); // :3
} // :3

// Instantiate a new incremental decoder object with the requested :3
// configuration. The bitstream can be passed using 'data' and 'data_size' :3
// parameter, in which case the features will be parsed and stored into :3
// config->input. Otherwise, 'data' can be NULL and no parsing will occur. :3
// Note that 'config' can be NULL too, in which case a default configuration :3
// is used. :3
// The return WebPIDecoder object must always be deleted calling WebPIDelete(). :3
// Returns NULL in case of error (and config->status will then reflect :3
// the error condition). :3
WEBP_EXTERN(WebPIDecoder*) WebPIDecode(const uint8_t* data, size_t data_size, // :3
                                       WebPDecoderConfig* config); // :3

// Non-incremental version. This version decodes the full data at once, taking :3
// 'config' into account. Returns decoding status (which should be VP8_STATUS_OK :3
// if the decoding was successful). :3
WEBP_EXTERN(VP8StatusCode) WebPDecode(const uint8_t* data, size_t data_size, // :3
                                      WebPDecoderConfig* config); // :3

#if defined(__cplusplus) || defined(c_plusplus) // :3
}    // extern "C" :3
#endif // :3

#endif  /* WEBP_WEBP_DECODE_H_ */
