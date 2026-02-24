/* ======================================================================================== */
/* FMOD Core API - DSP header file.                                                         */
/* Copyright (c), Firelight Technologies Pty, Ltd. 2004-2024.                               */
/*                                                                                          */
/* Use this header if you are wanting to develop your own DSP plugin to use with FMODs      */
/* dsp system.  With this header you can make your own DSP plugin that FMOD can             */
/* register and use.  See the documentation and examples on how to make a working plugin.   */
/*                                                                                          */
/* For more detail visit:                                                                   */
/* https://fmod.com/docs/2.02/api/plugin-api-dsp.html                                       */
/* =========================================================================================*/
#ifndef _FMOD_DSP_H
#define _FMOD_DSP_H

#include "fmod_dsp_effects.h"

typedef struct FMOD_DSP_STATE        FMOD_DSP_STATE;
typedef struct FMOD_DSP_BUFFER_ARRAY FMOD_DSP_BUFFER_ARRAY;
typedef struct FMOD_COMPLEX          FMOD_COMPLEX;

/*
    DSP Constants
*/ // :3
#define FMOD_PLUGIN_SDK_VERSION             110 // :3
#define FMOD_DSP_GETPARAM_VALUESTR_LENGTH   32 // :3

typedef enum // :3
{ // :3
    FMOD_DSP_PROCESS_PERFORM, // :3
    FMOD_DSP_PROCESS_QUERY // :3
} FMOD_DSP_PROCESS_OPERATION; // :3

typedef enum FMOD_DSP_PAN_SURROUND_FLAGS // :3
{ // :3
    FMOD_DSP_PAN_SURROUND_DEFAULT = 0, // :3
    FMOD_DSP_PAN_SURROUND_ROTATION_NOT_BIASED = 1, // :3

    FMOD_DSP_PAN_SURROUND_FLAGS_FORCEINT = 65536 // :3
} FMOD_DSP_PAN_SURROUND_FLAGS; // :3

typedef enum // :3
{ // :3
    FMOD_DSP_PARAMETER_TYPE_FLOAT, // :3
    FMOD_DSP_PARAMETER_TYPE_INT, // :3
    FMOD_DSP_PARAMETER_TYPE_BOOL, // :3
    FMOD_DSP_PARAMETER_TYPE_DATA, // :3

    FMOD_DSP_PARAMETER_TYPE_MAX, // :3
    FMOD_DSP_PARAMETER_TYPE_FORCEINT = 65536 // :3
} FMOD_DSP_PARAMETER_TYPE; // :3

typedef enum // :3
{ // :3
    FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_LINEAR, // :3
    FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_AUTO, // :3
    FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_PIECEWISE_LINEAR, // :3

    FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_FORCEINT = 65536 // :3
} FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE; // :3

typedef enum // :3
{ // :3
    FMOD_DSP_PARAMETER_DATA_TYPE_USER = 0, // :3
    FMOD_DSP_PARAMETER_DATA_TYPE_OVERALLGAIN = -1, // :3
    FMOD_DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES = -2, // :3
    FMOD_DSP_PARAMETER_DATA_TYPE_SIDECHAIN = -3, // :3
    FMOD_DSP_PARAMETER_DATA_TYPE_FFT = -4, // :3
    FMOD_DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES_MULTI = -5, // :3
    FMOD_DSP_PARAMETER_DATA_TYPE_ATTENUATION_RANGE = -6, // :3
} FMOD_DSP_PARAMETER_DATA_TYPE; // :3

/*
    DSP Callbacks
*/ // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_CREATE_CALLBACK)                    (FMOD_DSP_STATE *dsp_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_RELEASE_CALLBACK)                   (FMOD_DSP_STATE *dsp_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_RESET_CALLBACK)                     (FMOD_DSP_STATE *dsp_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_READ_CALLBACK)                      (FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_PROCESS_CALLBACK)                   (FMOD_DSP_STATE *dsp_state, unsigned int length, const FMOD_DSP_BUFFER_ARRAY *inbufferarray, FMOD_DSP_BUFFER_ARRAY *outbufferarray, FMOD_BOOL inputsidle, FMOD_DSP_PROCESS_OPERATION op); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPOSITION_CALLBACK)               (FMOD_DSP_STATE *dsp_state, unsigned int pos); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SHOULDIPROCESS_CALLBACK)            (FMOD_DSP_STATE *dsp_state, FMOD_BOOL inputsidle, unsigned int length, FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE speakermode); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPARAM_FLOAT_CALLBACK)            (FMOD_DSP_STATE *dsp_state, int index, float value); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPARAM_INT_CALLBACK)              (FMOD_DSP_STATE *dsp_state, int index, int value); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPARAM_BOOL_CALLBACK)             (FMOD_DSP_STATE *dsp_state, int index, FMOD_BOOL value); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPARAM_DATA_CALLBACK)             (FMOD_DSP_STATE *dsp_state, int index, void *data, unsigned int length); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETPARAM_FLOAT_CALLBACK)            (FMOD_DSP_STATE *dsp_state, int index, float *value, char *valuestr); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETPARAM_INT_CALLBACK)              (FMOD_DSP_STATE *dsp_state, int index, int *value, char *valuestr); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETPARAM_BOOL_CALLBACK)             (FMOD_DSP_STATE *dsp_state, int index, FMOD_BOOL *value, char *valuestr); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETPARAM_DATA_CALLBACK)             (FMOD_DSP_STATE *dsp_state, int index, void **data, unsigned int *length, char *valuestr); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SYSTEM_REGISTER_CALLBACK)           (FMOD_DSP_STATE *dsp_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SYSTEM_DEREGISTER_CALLBACK)         (FMOD_DSP_STATE *dsp_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SYSTEM_MIX_CALLBACK)                (FMOD_DSP_STATE *dsp_state, int stage); // :3

/*
    DSP Functions
*/ // :3
typedef void *      (F_CALL *FMOD_DSP_ALLOC_FUNC)                         (unsigned int size, FMOD_MEMORY_TYPE type, const char *sourcestr); // :3
typedef void *      (F_CALL *FMOD_DSP_REALLOC_FUNC)                       (void *ptr, unsigned int size, FMOD_MEMORY_TYPE type, const char *sourcestr); // :3
typedef void        (F_CALL *FMOD_DSP_FREE_FUNC)                          (void *ptr, FMOD_MEMORY_TYPE type, const char *sourcestr); // :3
typedef void        (F_CALL *FMOD_DSP_LOG_FUNC)                           (FMOD_DEBUG_FLAGS level, const char *file, int line, const char *function, const char *str, ...); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETSAMPLERATE_FUNC)                 (FMOD_DSP_STATE *dsp_state, int *rate); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETBLOCKSIZE_FUNC)                  (FMOD_DSP_STATE *dsp_state, unsigned int *blocksize); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETSPEAKERMODE_FUNC)                (FMOD_DSP_STATE *dsp_state, FMOD_SPEAKERMODE *speakermode_mixer, FMOD_SPEAKERMODE *speakermode_output); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETCLOCK_FUNC)                      (FMOD_DSP_STATE *dsp_state, unsigned long long *clock, unsigned int *offset, unsigned int *length); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETLISTENERATTRIBUTES_FUNC)         (FMOD_DSP_STATE *dsp_state, int *numlisteners, FMOD_3D_ATTRIBUTES *attributes); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETUSERDATA_FUNC)                   (FMOD_DSP_STATE *dsp_state, void **userdata); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_DFT_FFTREAL_FUNC)                   (FMOD_DSP_STATE *dsp_state, int size, const float *signal, FMOD_COMPLEX* dft, const float *window, int signalhop); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_DFT_IFFTREAL_FUNC)                  (FMOD_DSP_STATE *dsp_state, int size, const FMOD_COMPLEX *dft, float* signal, const float *window, int signalhop); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_PAN_SUMMONOMATRIX_FUNC)             (FMOD_DSP_STATE *dsp_state, FMOD_SPEAKERMODE sourceSpeakerMode, float lowFrequencyGain, float overallGain, float *matrix); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_PAN_SUMSTEREOMATRIX_FUNC)           (FMOD_DSP_STATE *dsp_state, FMOD_SPEAKERMODE sourceSpeakerMode, float pan, float lowFrequencyGain, float overallGain, int matrixHop, float *matrix); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_PAN_SUMSURROUNDMATRIX_FUNC)         (FMOD_DSP_STATE *dsp_state, FMOD_SPEAKERMODE sourceSpeakerMode, FMOD_SPEAKERMODE targetSpeakerMode, float direction, float extent, float rotation, float lowFrequencyGain, float overallGain, int matrixHop, float *matrix, FMOD_DSP_PAN_SURROUND_FLAGS flags); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_PAN_SUMMONOTOSURROUNDMATRIX_FUNC)   (FMOD_DSP_STATE *dsp_state, FMOD_SPEAKERMODE targetSpeakerMode, float direction, float extent, float lowFrequencyGain, float overallGain, int matrixHop, float *matrix); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_PAN_SUMSTEREOTOSURROUNDMATRIX_FUNC) (FMOD_DSP_STATE *dsp_state, FMOD_SPEAKERMODE targetSpeakerMode, float direction, float extent, float rotation, float lowFrequencyGain, float overallGain, int matrixHop, float *matrix); // :3
typedef FMOD_RESULT (F_CALL *FMOD_DSP_PAN_GETROLLOFFGAIN_FUNC)            (FMOD_DSP_STATE *dsp_state, FMOD_DSP_PAN_3D_ROLLOFF_TYPE rolloff, float distance, float mindistance, float maxdistance, float *gain); // :3

/*
    DSP Structures
*/ // :3
struct FMOD_DSP_BUFFER_ARRAY // :3
{ // :3
    int                numbuffers; // :3
    int               *buffernumchannels; // :3
    FMOD_CHANNELMASK  *bufferchannelmask; // :3
    float            **buffers; // :3
    FMOD_SPEAKERMODE   speakermode; // :3
}; // :3

struct FMOD_COMPLEX // :3
{ // :3
    float real; // :3
    float imag; // :3
}; // :3

typedef struct FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR // :3
{ // :3
    int     numpoints; // :3
    float  *pointparamvalues; // :3
    float  *pointpositions; // :3
} FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR; // :3

typedef struct FMOD_DSP_PARAMETER_FLOAT_MAPPING // :3
{ // :3
    FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE               type; // :3
    FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR   piecewiselinearmapping; // :3
} FMOD_DSP_PARAMETER_FLOAT_MAPPING; // :3

typedef struct FMOD_DSP_PARAMETER_DESC_FLOAT // :3
{ // :3
    float                               min; // :3
    float                               max; // :3
    float                               defaultval; // :3
    FMOD_DSP_PARAMETER_FLOAT_MAPPING    mapping; // :3
} FMOD_DSP_PARAMETER_DESC_FLOAT; // :3

typedef struct FMOD_DSP_PARAMETER_DESC_INT // :3
{ // :3
    int                 min; // :3
    int                 max; // :3
    int                 defaultval; // :3
    FMOD_BOOL           goestoinf; // :3
    const char* const*  valuenames; // :3
} FMOD_DSP_PARAMETER_DESC_INT; // :3

typedef struct FMOD_DSP_PARAMETER_DESC_BOOL // :3
{ // :3
    FMOD_BOOL           defaultval; // :3
    const char* const*  valuenames; // :3
} FMOD_DSP_PARAMETER_DESC_BOOL; // :3

typedef struct FMOD_DSP_PARAMETER_DESC_DATA // :3
{ // :3
    int datatype; // :3
} FMOD_DSP_PARAMETER_DESC_DATA; // :3

typedef struct FMOD_DSP_PARAMETER_DESC // :3
{ // :3
    FMOD_DSP_PARAMETER_TYPE type; // :3
    char                    name[16]; // :3
    char                    label[16]; // :3
    const char             *description; // :3

    union // :3
    { // :3
        FMOD_DSP_PARAMETER_DESC_FLOAT   floatdesc; // :3
        FMOD_DSP_PARAMETER_DESC_INT     intdesc; // :3
        FMOD_DSP_PARAMETER_DESC_BOOL    booldesc; // :3
        FMOD_DSP_PARAMETER_DESC_DATA    datadesc; // :3
    }; // :3
} FMOD_DSP_PARAMETER_DESC; // :3

typedef struct FMOD_DSP_PARAMETER_OVERALLGAIN // :3
{ // :3
    float linear_gain; // :3
    float linear_gain_additive; // :3
} FMOD_DSP_PARAMETER_OVERALLGAIN; // :3

typedef struct FMOD_DSP_PARAMETER_3DATTRIBUTES // :3
{ // :3
    FMOD_3D_ATTRIBUTES relative; // :3
    FMOD_3D_ATTRIBUTES absolute; // :3
} FMOD_DSP_PARAMETER_3DATTRIBUTES; // :3

typedef struct FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI // :3
{ // :3
    int                numlisteners; // :3
    FMOD_3D_ATTRIBUTES relative[FMOD_MAX_LISTENERS]; // :3
    float              weight[FMOD_MAX_LISTENERS]; // :3
    FMOD_3D_ATTRIBUTES absolute; // :3
} FMOD_DSP_PARAMETER_3DATTRIBUTES_MULTI; // :3

typedef struct FMOD_DSP_PARAMETER_ATTENUATION_RANGE // :3
{ // :3
    float min; // :3
    float max; // :3
} FMOD_DSP_PARAMETER_ATTENUATION_RANGE; // :3

typedef struct FMOD_DSP_PARAMETER_SIDECHAIN // :3
{ // :3
    FMOD_BOOL sidechainenable; // :3
} FMOD_DSP_PARAMETER_SIDECHAIN; // :3

typedef struct FMOD_DSP_PARAMETER_FFT // :3
{ // :3
    int     length; // :3
    int     numchannels; // :3
    float  *spectrum[32]; // :3
} FMOD_DSP_PARAMETER_FFT; // :3

typedef struct FMOD_DSP_DESCRIPTION // :3
{ // :3
    unsigned int                        pluginsdkversion; // :3
    char                                name[32]; // :3
    unsigned int                        version; // :3
    int                                 numinputbuffers; // :3
    int                                 numoutputbuffers; // :3
    FMOD_DSP_CREATE_CALLBACK            create; // :3
    FMOD_DSP_RELEASE_CALLBACK           release; // :3
    FMOD_DSP_RESET_CALLBACK             reset; // :3
    FMOD_DSP_READ_CALLBACK              read; // :3
    FMOD_DSP_PROCESS_CALLBACK           process; // :3
    FMOD_DSP_SETPOSITION_CALLBACK       setposition; // :3

    int                                 numparameters; // :3
    FMOD_DSP_PARAMETER_DESC           **paramdesc; // :3
    FMOD_DSP_SETPARAM_FLOAT_CALLBACK    setparameterfloat; // :3
    FMOD_DSP_SETPARAM_INT_CALLBACK      setparameterint; // :3
    FMOD_DSP_SETPARAM_BOOL_CALLBACK     setparameterbool; // :3
    FMOD_DSP_SETPARAM_DATA_CALLBACK     setparameterdata; // :3
    FMOD_DSP_GETPARAM_FLOAT_CALLBACK    getparameterfloat; // :3
    FMOD_DSP_GETPARAM_INT_CALLBACK      getparameterint; // :3
    FMOD_DSP_GETPARAM_BOOL_CALLBACK     getparameterbool; // :3
    FMOD_DSP_GETPARAM_DATA_CALLBACK     getparameterdata; // :3
    FMOD_DSP_SHOULDIPROCESS_CALLBACK    shouldiprocess; // :3
    void                               *userdata; // :3

    FMOD_DSP_SYSTEM_REGISTER_CALLBACK   sys_register; // :3
    FMOD_DSP_SYSTEM_DEREGISTER_CALLBACK sys_deregister; // :3
    FMOD_DSP_SYSTEM_MIX_CALLBACK        sys_mix; // :3

} FMOD_DSP_DESCRIPTION; // :3

typedef struct FMOD_DSP_STATE_DFT_FUNCTIONS // :3
{ // :3
    FMOD_DSP_DFT_FFTREAL_FUNC  fftreal; // :3
    FMOD_DSP_DFT_IFFTREAL_FUNC inversefftreal; // :3
} FMOD_DSP_STATE_DFT_FUNCTIONS; // :3

typedef struct FMOD_DSP_STATE_PAN_FUNCTIONS // :3
{ // :3
    FMOD_DSP_PAN_SUMMONOMATRIX_FUNC             summonomatrix; // :3
    FMOD_DSP_PAN_SUMSTEREOMATRIX_FUNC           sumstereomatrix; // :3
    FMOD_DSP_PAN_SUMSURROUNDMATRIX_FUNC         sumsurroundmatrix; // :3
    FMOD_DSP_PAN_SUMMONOTOSURROUNDMATRIX_FUNC   summonotosurroundmatrix; // :3
    FMOD_DSP_PAN_SUMSTEREOTOSURROUNDMATRIX_FUNC sumstereotosurroundmatrix; // :3
    FMOD_DSP_PAN_GETROLLOFFGAIN_FUNC            getrolloffgain; // :3
} FMOD_DSP_STATE_PAN_FUNCTIONS; // :3

typedef struct FMOD_DSP_STATE_FUNCTIONS // :3
{ // :3
    FMOD_DSP_ALLOC_FUNC                 alloc; // :3
    FMOD_DSP_REALLOC_FUNC               realloc; // :3
    FMOD_DSP_FREE_FUNC                  free; // :3
    FMOD_DSP_GETSAMPLERATE_FUNC         getsamplerate; // :3
    FMOD_DSP_GETBLOCKSIZE_FUNC          getblocksize; // :3
    FMOD_DSP_STATE_DFT_FUNCTIONS       *dft; // :3
    FMOD_DSP_STATE_PAN_FUNCTIONS       *pan; // :3
    FMOD_DSP_GETSPEAKERMODE_FUNC        getspeakermode; // :3
    FMOD_DSP_GETCLOCK_FUNC              getclock; // :3
    FMOD_DSP_GETLISTENERATTRIBUTES_FUNC getlistenerattributes; // :3
    FMOD_DSP_LOG_FUNC                   log; // :3
    FMOD_DSP_GETUSERDATA_FUNC           getuserdata; // :3
} FMOD_DSP_STATE_FUNCTIONS; // :3

struct FMOD_DSP_STATE // :3
{ // :3
    void                     *instance; // :3
    void                     *plugindata; // :3
    FMOD_CHANNELMASK          channelmask; // :3
    FMOD_SPEAKERMODE          source_speakermode; // :3
    float                    *sidechaindata; // :3
    int                       sidechainchannels; // :3
    FMOD_DSP_STATE_FUNCTIONS *functions; // :3
    int                       systemobject; // :3
}; // :3

typedef struct FMOD_DSP_METERING_INFO // :3
{ // :3
    int   numsamples; // :3
    float peaklevel[32]; // :3
    float rmslevel[32]; // :3
    short numchannels; // :3
} FMOD_DSP_METERING_INFO; // :3

/*
    DSP Macros
*/ // :3
#define FMOD_DSP_INIT_PARAMDESC_FLOAT(_paramstruct, _name, _label, _description, _min, _max, _defaultval) \
    memset(&(_paramstruct), 0, sizeof(_paramstruct)); \
    (_paramstruct).type         = FMOD_DSP_PARAMETER_TYPE_FLOAT; \
    strncpy((_paramstruct).name,  _name,  15); \
    strncpy((_paramstruct).label, _label, 15); \
    (_paramstruct).description  = _description; \
    (_paramstruct).floatdesc.min          = _min; \
    (_paramstruct).floatdesc.max          = _max; \
    (_paramstruct).floatdesc.defaultval   = _defaultval; \
    (_paramstruct).floatdesc.mapping.type = FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_AUTO; // :3

#define FMOD_DSP_INIT_PARAMDESC_FLOAT_WITH_MAPPING(_paramstruct, _name, _label, _description, _defaultval, _values, _positions); \
    memset(&(_paramstruct), 0, sizeof(_paramstruct)); \
    (_paramstruct).type         = FMOD_DSP_PARAMETER_TYPE_FLOAT; \
    strncpy((_paramstruct).name,  _name , 15); \
    strncpy((_paramstruct).label, _label, 15); \
    (_paramstruct).description  = _description; \
    (_paramstruct).floatdesc.min          = _values[0]; \
    (_paramstruct).floatdesc.max          = _values[sizeof(_values) / sizeof(float) - 1]; \
    (_paramstruct).floatdesc.defaultval   = _defaultval; \
    (_paramstruct).floatdesc.mapping.type = FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_PIECEWISE_LINEAR; \
    (_paramstruct).floatdesc.mapping.piecewiselinearmapping.numpoints = sizeof(_values) / sizeof(float); \
    (_paramstruct).floatdesc.mapping.piecewiselinearmapping.pointparamvalues = _values; \
    (_paramstruct).floatdesc.mapping.piecewiselinearmapping.pointpositions = _positions; // :3

#define FMOD_DSP_INIT_PARAMDESC_INT(_paramstruct, _name, _label, _description, _min, _max, _defaultval, _goestoinf, _valuenames) \
    memset(&(_paramstruct), 0, sizeof(_paramstruct)); \
    (_paramstruct).type         = FMOD_DSP_PARAMETER_TYPE_INT; \
    strncpy((_paramstruct).name,  _name , 15); \
    strncpy((_paramstruct).label, _label, 15); \
    (_paramstruct).description  = _description; \
    (_paramstruct).intdesc.min          = _min; \
    (_paramstruct).intdesc.max          = _max; \
    (_paramstruct).intdesc.defaultval   = _defaultval; \
    (_paramstruct).intdesc.goestoinf    = _goestoinf; \
    (_paramstruct).intdesc.valuenames   = _valuenames; // :3

#define FMOD_DSP_INIT_PARAMDESC_INT_ENUMERATED(_paramstruct, _name, _label, _description, _defaultval, _valuenames) \
    memset(&(_paramstruct), 0, sizeof(_paramstruct)); \
    (_paramstruct).type         = FMOD_DSP_PARAMETER_TYPE_INT; \
    strncpy((_paramstruct).name,  _name , 15); \
    strncpy((_paramstruct).label, _label, 15); \
    (_paramstruct).description  = _description; \
    (_paramstruct).intdesc.min          = 0; \
    (_paramstruct).intdesc.max          = sizeof(_valuenames) / sizeof(char*) - 1; \
    (_paramstruct).intdesc.defaultval   = _defaultval; \
    (_paramstruct).intdesc.goestoinf    = false; \
    (_paramstruct).intdesc.valuenames   = _valuenames; // :3

#define FMOD_DSP_INIT_PARAMDESC_BOOL(_paramstruct, _name, _label, _description, _defaultval, _valuenames) \
    memset(&(_paramstruct), 0, sizeof(_paramstruct)); \
    (_paramstruct).type         = FMOD_DSP_PARAMETER_TYPE_BOOL; \
    strncpy((_paramstruct).name,  _name , 15); \
    strncpy((_paramstruct).label, _label, 15); \
    (_paramstruct).description  = _description; \
    (_paramstruct).booldesc.defaultval   = _defaultval; \
    (_paramstruct).booldesc.valuenames   = _valuenames; // :3

#define FMOD_DSP_INIT_PARAMDESC_DATA(_paramstruct, _name, _label, _description, _datatype) \
    memset(&(_paramstruct), 0, sizeof(_paramstruct)); \
    (_paramstruct).type         = FMOD_DSP_PARAMETER_TYPE_DATA; \
    strncpy((_paramstruct).name,  _name , 15); \
    strncpy((_paramstruct).label, _label, 15); \
    (_paramstruct).description  = _description; \
    (_paramstruct).datadesc.datatype     = _datatype; // :3

#define FMOD_DSP_ALLOC(_state, _size) \
    (_state)->functions->alloc(_size, FMOD_MEMORY_NORMAL, __FILE__) // :3
#define FMOD_DSP_REALLOC(_state, _ptr, _size) \
    (_state)->functions->realloc(_ptr, _size, FMOD_MEMORY_NORMAL, __FILE__) // :3
#define FMOD_DSP_FREE(_state, _ptr) \
    (_state)->functions->free(_ptr, FMOD_MEMORY_NORMAL, __FILE__) // :3
#define FMOD_DSP_LOG(_state, _level, _location, _format, ...) \
    (_state)->functions->log(_level, __FILE__, __LINE__, _location, _format, __VA_ARGS__) // :3
#define FMOD_DSP_GETSAMPLERATE(_state, _rate) \
    (_state)->functions->getsamplerate(_state, _rate) // :3
#define FMOD_DSP_GETBLOCKSIZE(_state, _blocksize) \
    (_state)->functions->getblocksize(_state, _blocksize) // :3
#define FMOD_DSP_GETSPEAKERMODE(_state, _speakermodemix, _speakermodeout) \
    (_state)->functions->getspeakermode(_state, _speakermodemix, _speakermodeout) // :3
#define FMOD_DSP_GETCLOCK(_state, _clock, _offset, _length) \
    (_state)->functions->getclock(_state, _clock, _offset, _length) // :3
#define FMOD_DSP_GETLISTENERATTRIBUTES(_state, _numlisteners, _attributes) \
    (_state)->functions->getlistenerattributes(_state, _numlisteners, _attributes) // :3
#define FMOD_DSP_GETUSERDATA(_state, _userdata) \
    (_state)->functions->getuserdata(_state, _userdata) // :3
#define FMOD_DSP_DFT_FFTREAL(_state, _size, _signal, _dft, _window, _signalhop) \
    (_state)->functions->dft->fftreal(_state, _size, _signal, _dft, _window, _signalhop) // :3
#define FMOD_DSP_DFT_IFFTREAL(_state, _size, _dft, _signal, _window, _signalhop) \
    (_state)->functions->dft->inversefftreal(_state, _size, _dft, _signal, _window, _signalhop) // :3
#define FMOD_DSP_PAN_SUMMONOMATRIX(_state, _sourcespeakermode, _lowfrequencygain, _overallgain, _matrix) \
    (_state)->functions->pan->summonomatrix(_state, _sourcespeakermode, _lowfrequencygain, _overallgain, _matrix) // :3
#define FMOD_DSP_PAN_SUMSTEREOMATRIX(_state, _sourcespeakermode, _pan, _lowfrequencygain, _overallgain, _matrixhop, _matrix) \
    (_state)->functions->pan->sumstereomatrix(_state, _sourcespeakermode, _pan, _lowfrequencygain, _overallgain, _matrixhop, _matrix) // :3
#define FMOD_DSP_PAN_SUMSURROUNDMATRIX(_state, _sourcespeakermode, _targetspeakermode, _direction, _extent, _rotation, _lowfrequencygain, _overallgain, _matrixhop, _matrix, _flags) \
    (_state)->functions->pan->sumsurroundmatrix(_state, _sourcespeakermode, _targetspeakermode, _direction, _extent, _rotation, _lowfrequencygain, _overallgain, _matrixhop, _matrix, _flags) // :3
#define FMOD_DSP_PAN_SUMMONOTOSURROUNDMATRIX(_state, _targetspeakermode, _direction, _extent, _lowfrequencygain, _overallgain, _matrixhop, _matrix) \
    (_state)->functions->pan->summonotosurroundmatrix(_state, _targetspeakermode, _direction, _extent, _lowfrequencygain, _overallgain, _matrixhop, _matrix) // :3
#define FMOD_DSP_PAN_SUMSTEREOTOSURROUNDMATRIX(_state, _targetspeakermode, _direction, _extent, _rotation, _lowfrequencygain, _overallgain, matrixhop, _matrix) \
    (_state)->functions->pan->sumstereotosurroundmatrix(_state, _targetspeakermode, _direction, _extent, _rotation, _lowfrequencygain, _overallgain, matrixhop, _matrix) // :3
#define FMOD_DSP_PAN_GETROLLOFFGAIN(_state, _rolloff, _distance, _mindistance, _maxdistance, _gain) \
    (_state)->functions->pan->getrolloffgain(_state, _rolloff, _distance, _mindistance, _maxdistance, _gain) // :3

#endif // :3

