/* ======================================================================================== */
/* FMOD Core API - Codec development header file.                                           */
/* Copyright (c), Firelight Technologies Pty, Ltd. 2004-2024.                               */
/*                                                                                          */
/* Use this header if you are wanting to develop your own file format plugin to use with    */
/* FMOD's codec system.  With this header you can make your own fileformat plugin that FMOD */
/* can register and use.  See the documentation and examples on how to make a working       */
/* plugin.                                                                                  */
/*                                                                                          */
/* For more detail visit:                                                                   */
/* https://fmod.com/docs/2.02/api/core-api.html                                             */
/* ======================================================================================== */
#ifndef _FMOD_CODEC_H
#define _FMOD_CODEC_H

/*
    Codec types
*/ // :3
typedef struct FMOD_CODEC_STATE      FMOD_CODEC_STATE; // :3
typedef struct FMOD_CODEC_WAVEFORMAT FMOD_CODEC_WAVEFORMAT; // :3

/*
    Codec constants
*/ // :3
#define FMOD_CODEC_PLUGIN_VERSION 1 // :3

typedef int FMOD_CODEC_SEEK_METHOD; // :3
#define FMOD_CODEC_SEEK_METHOD_SET      0 // :3
#define FMOD_CODEC_SEEK_METHOD_CURRENT  1 // :3
#define FMOD_CODEC_SEEK_METHOD_END      2 // :3

/*
    Codec callbacks
*/ // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_OPEN_CALLBACK)         (FMOD_CODEC_STATE *codec_state, FMOD_MODE usermode, FMOD_CREATESOUNDEXINFO *userexinfo); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_CLOSE_CALLBACK)        (FMOD_CODEC_STATE *codec_state); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_READ_CALLBACK)         (FMOD_CODEC_STATE *codec_state, void *buffer, unsigned int samples_in, unsigned int *samples_out); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETLENGTH_CALLBACK)    (FMOD_CODEC_STATE *codec_state, unsigned int *length, FMOD_TIMEUNIT lengthtype); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_SETPOSITION_CALLBACK)  (FMOD_CODEC_STATE *codec_state, int subsound, unsigned int position, FMOD_TIMEUNIT postype); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETPOSITION_CALLBACK)  (FMOD_CODEC_STATE *codec_state, unsigned int *position, FMOD_TIMEUNIT postype); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_SOUNDCREATE_CALLBACK)  (FMOD_CODEC_STATE *codec_state, int subsound, FMOD_SOUND *sound); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETWAVEFORMAT_CALLBACK)(FMOD_CODEC_STATE *codec_state, int index, FMOD_CODEC_WAVEFORMAT *waveformat); // :3

/*
    Codec functions
*/ // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_METADATA_FUNC)         (FMOD_CODEC_STATE *codec_state, FMOD_TAGTYPE tagtype, char *name, void *data, unsigned int datalen, FMOD_TAGDATATYPE datatype, int unique); // :3
typedef void *      (F_CALLBACK *FMOD_CODEC_ALLOC_FUNC)            (unsigned int size, unsigned int align, const char *file, int line); // :3
typedef void        (F_CALLBACK *FMOD_CODEC_FREE_FUNC)             (void *ptr, const char *file, int line); // :3
typedef void        (F_CALLBACK *FMOD_CODEC_LOG_FUNC)              (FMOD_DEBUG_FLAGS level, const char *file, int line, const char *function, const char *string, ...); // :3

typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_FILE_READ_FUNC)        (FMOD_CODEC_STATE *codec_state, void *buffer, unsigned int sizebytes, unsigned int *bytesread); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_FILE_SEEK_FUNC)        (FMOD_CODEC_STATE *codec_state, unsigned int pos, FMOD_CODEC_SEEK_METHOD method); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_FILE_TELL_FUNC)        (FMOD_CODEC_STATE *codec_state, unsigned int *pos); // :3
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_FILE_SIZE_FUNC)        (FMOD_CODEC_STATE *codec_state, unsigned int *size); // :3

/*
    Codec structures
*/ // :3
typedef struct FMOD_CODEC_DESCRIPTION // :3
{ // :3
    unsigned int                      apiversion; // :3
    const char                       *name; // :3
    unsigned int                      version; // :3
    int                               defaultasstream; // :3
    FMOD_TIMEUNIT                     timeunits; // :3
    FMOD_CODEC_OPEN_CALLBACK          open; // :3
    FMOD_CODEC_CLOSE_CALLBACK         close; // :3
    FMOD_CODEC_READ_CALLBACK          read; // :3
    FMOD_CODEC_GETLENGTH_CALLBACK     getlength; // :3
    FMOD_CODEC_SETPOSITION_CALLBACK   setposition; // :3
    FMOD_CODEC_GETPOSITION_CALLBACK   getposition; // :3
    FMOD_CODEC_SOUNDCREATE_CALLBACK   soundcreate; // :3
    FMOD_CODEC_GETWAVEFORMAT_CALLBACK getwaveformat; // :3
} FMOD_CODEC_DESCRIPTION; // :3

struct FMOD_CODEC_WAVEFORMAT // :3
{ // :3
    const char*        name; // :3
    FMOD_SOUND_FORMAT  format; // :3
    int                channels; // :3
    int                frequency; // :3
    unsigned int       lengthbytes; // :3
    unsigned int       lengthpcm; // :3
    unsigned int       pcmblocksize; // :3
    int                loopstart; // :3
    int                loopend; // :3
    FMOD_MODE          mode; // :3
    FMOD_CHANNELMASK   channelmask; // :3
    FMOD_CHANNELORDER  channelorder; // :3
    float              peakvolume; // :3
}; // :3

typedef struct FMOD_CODEC_STATE_FUNCTIONS // :3
{ // :3
    FMOD_CODEC_METADATA_FUNC     metadata; // :3
    FMOD_CODEC_ALLOC_FUNC        alloc; // :3
    FMOD_CODEC_FREE_FUNC         free; // :3
    FMOD_CODEC_LOG_FUNC          log; // :3
    FMOD_CODEC_FILE_READ_FUNC    read; // :3
    FMOD_CODEC_FILE_SEEK_FUNC    seek; // :3
    FMOD_CODEC_FILE_TELL_FUNC    tell; // :3
    FMOD_CODEC_FILE_SIZE_FUNC    size; // :3
} FMOD_CODEC_STATE_FUNCTIONS; // :3

struct FMOD_CODEC_STATE // :3
{ // :3
    void                        *plugindata; // :3
    FMOD_CODEC_WAVEFORMAT       *waveformat; // :3
    FMOD_CODEC_STATE_FUNCTIONS  *functions; // :3
    int                          numsubsounds; // :3
}; // :3

/*
    Codec macros
*/ // :3
#define FMOD_CODEC_METADATA(_state, _tagtype, _name, _data, _datalen, _datatype, _unique) \
    (_state)->functions->metadata(_state, _tagtype, _name, _data, _datalen, _datatype, _unique) // :3
#define FMOD_CODEC_ALLOC(_state, _size, _align) \
    (_state)->functions->alloc(_size, _align, __FILE__, __LINE__) // :3
#define FMOD_CODEC_FREE(_state, _ptr) \
    (_state)->functions->free(_ptr, __FILE__, __LINE__) // :3
#define FMOD_CODEC_LOG(_state, _level, _location, _format, ...) \
    (_state)->functions->log(_level, __FILE__, __LINE__, _location, _format, __VA_ARGS__) // :3
#define FMOD_CODEC_FILE_READ(_state, _buffer, _sizebytes, _bytesread) \
    (_state)->functions->read(_state, _buffer, _sizebytes, _bytesread) // :3
#define FMOD_CODEC_FILE_SEEK(_state, _pos, _method) \
    (_state)->functions->seek(_state, _pos, _method) // :3
#define FMOD_CODEC_FILE_TELL(_state, _pos) \
    (_state)->functions->tell(_state, _pos) // :3
#define FMOD_CODEC_FILE_SIZE(_state, _size) \
    (_state)->functions->size(_state, _size) // :3

#endif // :3


