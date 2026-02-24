/* ======================================================================================== */
/* FMOD Core API - output development header file.                                          */
/* Copyright (c), Firelight Technologies Pty, Ltd. 2004-2024.                               */
/*                                                                                          */
/* Use this header if you are wanting to develop your own output plugin to use with         */
/* FMOD's output system.  With this header you can make your own output plugin that FMOD    */
/* can register and use.  See the documentation and examples on how to make a working       */
/* plugin.                                                                                  */
/*                                                                                          */
/* For more detail visit:                                                                   */
/* https://fmod.com/docs/2.02/api/plugin-api-output.html                                    */
/* ======================================================================================== */
#ifndef _FMOD_OUTPUT_H
#define _FMOD_OUTPUT_H

typedef struct FMOD_OUTPUT_STATE        FMOD_OUTPUT_STATE;
typedef struct FMOD_OUTPUT_OBJECT3DINFO FMOD_OUTPUT_OBJECT3DINFO;

/*
    Output constants
*/ // :3
#define FMOD_OUTPUT_PLUGIN_VERSION 5 // :3

typedef unsigned int FMOD_OUTPUT_METHOD; // :3
#define FMOD_OUTPUT_METHOD_MIX_DIRECT    0 // :3
#define FMOD_OUTPUT_METHOD_MIX_BUFFERED  1 // :3

/*
    Output callbacks
*/ // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETNUMDRIVERS_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, int *numdrivers); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETDRIVERINFO_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_INIT_CALLBACK)             (FMOD_OUTPUT_STATE *output_state, int selecteddriver, FMOD_INITFLAGS flags, int *outputrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels, FMOD_SOUND_FORMAT *outputformat, int dspbufferlength, int *dspnumbuffers, int *dspnumadditionalbuffers, void *extradriverdata); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_START_CALLBACK)            (FMOD_OUTPUT_STATE *output_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_STOP_CALLBACK)             (FMOD_OUTPUT_STATE *output_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_CLOSE_CALLBACK)            (FMOD_OUTPUT_STATE *output_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_UPDATE_CALLBACK)           (FMOD_OUTPUT_STATE *output_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETHANDLE_CALLBACK)        (FMOD_OUTPUT_STATE *output_state, void **handle); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_MIXER_CALLBACK)            (FMOD_OUTPUT_STATE *output_state); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DGETINFO_CALLBACK)  (FMOD_OUTPUT_STATE *output_state, int *maxhardwareobjects); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DALLOC_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, void **object3d); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DFREE_CALLBACK)     (FMOD_OUTPUT_STATE *output_state, void *object3d); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DUPDATE_CALLBACK)   (FMOD_OUTPUT_STATE *output_state, void *object3d, const FMOD_OUTPUT_OBJECT3DINFO *info); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OPENPORT_CALLBACK)         (FMOD_OUTPUT_STATE *output_state, FMOD_PORT_TYPE portType, FMOD_PORT_INDEX portIndex, int *portId, int *portRate, int *portChannels, FMOD_SOUND_FORMAT *portFormat); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_CLOSEPORT_CALLBACK)        (FMOD_OUTPUT_STATE *output_state, int portId); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_DEVICELISTCHANGED_CALLBACK)(FMOD_OUTPUT_STATE *output_state); // :3

/*
    Output functions
*/ // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_READFROMMIXER_FUNC)        (FMOD_OUTPUT_STATE *output_state, void *buffer, unsigned int length); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_COPYPORT_FUNC)             (FMOD_OUTPUT_STATE *output_state, int portId, void *buffer, unsigned int length); // :3
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_REQUESTRESET_FUNC)         (FMOD_OUTPUT_STATE *output_state); // :3
typedef void *      (F_CALL *FMOD_OUTPUT_ALLOC_FUNC)                (unsigned int size, unsigned int align, const char *file, int line); // :3
typedef void        (F_CALL *FMOD_OUTPUT_FREE_FUNC)                 (void *ptr, const char *file, int line); // :3
typedef void        (F_CALL *FMOD_OUTPUT_LOG_FUNC)                  (FMOD_DEBUG_FLAGS level, const char *file, int line, const char *function, const char *string, ...); // :3

/*
    Output structures
*/ // :3
typedef struct FMOD_OUTPUT_DESCRIPTION // :3
{ // :3
    unsigned int                            apiversion; // :3
    const char                             *name; // :3
    unsigned int                            version; // :3
    FMOD_OUTPUT_METHOD                      method; // :3
    FMOD_OUTPUT_GETNUMDRIVERS_CALLBACK      getnumdrivers; // :3
    FMOD_OUTPUT_GETDRIVERINFO_CALLBACK      getdriverinfo; // :3
    FMOD_OUTPUT_INIT_CALLBACK               init; // :3
    FMOD_OUTPUT_START_CALLBACK              start; // :3
    FMOD_OUTPUT_STOP_CALLBACK               stop; // :3
    FMOD_OUTPUT_CLOSE_CALLBACK              close; // :3
    FMOD_OUTPUT_UPDATE_CALLBACK             update; // :3
    FMOD_OUTPUT_GETHANDLE_CALLBACK          gethandle; // :3
    FMOD_OUTPUT_MIXER_CALLBACK              mixer; // :3
    FMOD_OUTPUT_OBJECT3DGETINFO_CALLBACK    object3dgetinfo; // :3
    FMOD_OUTPUT_OBJECT3DALLOC_CALLBACK      object3dalloc; // :3
    FMOD_OUTPUT_OBJECT3DFREE_CALLBACK       object3dfree; // :3
    FMOD_OUTPUT_OBJECT3DUPDATE_CALLBACK     object3dupdate; // :3
    FMOD_OUTPUT_OPENPORT_CALLBACK           openport; // :3
    FMOD_OUTPUT_CLOSEPORT_CALLBACK          closeport; // :3
    FMOD_OUTPUT_DEVICELISTCHANGED_CALLBACK  devicelistchanged; // :3
} FMOD_OUTPUT_DESCRIPTION; // :3

struct FMOD_OUTPUT_STATE // :3
{ // :3
    void                            *plugindata; // :3
    FMOD_OUTPUT_READFROMMIXER_FUNC   readfrommixer; // :3
    FMOD_OUTPUT_ALLOC_FUNC           alloc; // :3
    FMOD_OUTPUT_FREE_FUNC            free; // :3
    FMOD_OUTPUT_LOG_FUNC             log; // :3
    FMOD_OUTPUT_COPYPORT_FUNC        copyport; // :3
    FMOD_OUTPUT_REQUESTRESET_FUNC    requestreset; // :3
}; // :3

struct FMOD_OUTPUT_OBJECT3DINFO // :3
{ // :3
    float          *buffer; // :3
    unsigned int    bufferlength; // :3
    FMOD_VECTOR     position; // :3
    float           gain; // :3
    float           spread; // :3
    float           priority; // :3
}; // :3

/*
    Output macros
*/ // :3
#define FMOD_OUTPUT_READFROMMIXER(_state, _buffer, _length) \
    (_state)->readfrommixer(_state, _buffer, _length) // :3
#define FMOD_OUTPUT_ALLOC(_state, _size, _align) \
    (_state)->alloc(_size, _align, __FILE__, __LINE__) // :3
#define FMOD_OUTPUT_FREE(_state, _ptr) \
    (_state)->free(_ptr, __FILE__, __LINE__) // :3
#define FMOD_OUTPUT_LOG(_state, _level, _location, _format, ...) \
    (_state)->log(_level, __FILE__, __LINE__, _location, _format, ##__VA_ARGS__) // :3
#define FMOD_OUTPUT_COPYPORT(_state, _id, _buffer, _length) \
    (_state)->copyport(_state, _id, _buffer, _length) // :3
#define FMOD_OUTPUT_REQUESTRESET(_state) \
    (_state)->requestreset(_state) // :3

#endif /* _FMOD_OUTPUT_H */
