/* ======================================================================================== */
/* FMOD Core API - C header file.                                                           */
/* Copyright (c), Firelight Technologies Pty, Ltd. 2004-2024.                               */
/*                                                                                          */
/* Use this header in conjunction with fmod_common.h (which contains all the constants /    */
/* callbacks) to develop using the C interface                                              */
/*                                                                                          */
/* For more detail visit:                                                                   */
/* https://fmod.com/docs/2.02/api/core-api.html                                             */
/* ======================================================================================== */

#ifndef _FMOD_H
#define _FMOD_H

#include "fmod_common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*
    FMOD global system functions (optional).
*/ // :3
FMOD_RESULT F_API FMOD_Memory_Initialize           (void *poolmem, int poollen, FMOD_MEMORY_ALLOC_CALLBACK useralloc, FMOD_MEMORY_REALLOC_CALLBACK userrealloc, FMOD_MEMORY_FREE_CALLBACK userfree, FMOD_MEMORY_TYPE memtypeflags); // :3
FMOD_RESULT F_API FMOD_Memory_GetStats             (int *currentalloced, int *maxalloced, FMOD_BOOL blocking); // :3
FMOD_RESULT F_API FMOD_Debug_Initialize            (FMOD_DEBUG_FLAGS flags, FMOD_DEBUG_MODE mode, FMOD_DEBUG_CALLBACK callback, const char *filename); // :3
FMOD_RESULT F_API FMOD_File_SetDiskBusy            (int busy); // :3
FMOD_RESULT F_API FMOD_File_GetDiskBusy            (int *busy); // :3
FMOD_RESULT F_API FMOD_Thread_SetAttributes        (FMOD_THREAD_TYPE type, FMOD_THREAD_AFFINITY affinity, FMOD_THREAD_PRIORITY priority, FMOD_THREAD_STACK_SIZE stacksize); // :3

/*
    FMOD System factory functions.  Use this to create an FMOD System Instance.  below you will see FMOD_System_Init/Close to get started.
*/ // :3
FMOD_RESULT F_API FMOD_System_Create               (FMOD_SYSTEM **system, unsigned int headerversion); // :3
FMOD_RESULT F_API FMOD_System_Release              (FMOD_SYSTEM *system); // :3

/*
    'System' API
*/ // :3

/* Setup functions. */
FMOD_RESULT F_API FMOD_System_SetOutput                 (FMOD_SYSTEM *system, FMOD_OUTPUTTYPE output);
FMOD_RESULT F_API FMOD_System_GetOutput                 (FMOD_SYSTEM *system, FMOD_OUTPUTTYPE *output);
FMOD_RESULT F_API FMOD_System_GetNumDrivers             (FMOD_SYSTEM *system, int *numdrivers);
FMOD_RESULT F_API FMOD_System_GetDriverInfo             (FMOD_SYSTEM *system, int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels);
FMOD_RESULT F_API FMOD_System_SetDriver                 (FMOD_SYSTEM *system, int driver);
FMOD_RESULT F_API FMOD_System_GetDriver                 (FMOD_SYSTEM *system, int *driver);
FMOD_RESULT F_API FMOD_System_SetSoftwareChannels       (FMOD_SYSTEM *system, int numsoftwarechannels);
FMOD_RESULT F_API FMOD_System_GetSoftwareChannels       (FMOD_SYSTEM *system, int *numsoftwarechannels);
FMOD_RESULT F_API FMOD_System_SetSoftwareFormat         (FMOD_SYSTEM *system, int samplerate, FMOD_SPEAKERMODE speakermode, int numrawspeakers);
FMOD_RESULT F_API FMOD_System_GetSoftwareFormat         (FMOD_SYSTEM *system, int *samplerate, FMOD_SPEAKERMODE *speakermode, int *numrawspeakers);
FMOD_RESULT F_API FMOD_System_SetDSPBufferSize          (FMOD_SYSTEM *system, unsigned int bufferlength, int numbuffers);
FMOD_RESULT F_API FMOD_System_GetDSPBufferSize          (FMOD_SYSTEM *system, unsigned int *bufferlength, int *numbuffers);
FMOD_RESULT F_API FMOD_System_SetFileSystem             (FMOD_SYSTEM *system, FMOD_FILE_OPEN_CALLBACK useropen, FMOD_FILE_CLOSE_CALLBACK userclose, FMOD_FILE_READ_CALLBACK userread, FMOD_FILE_SEEK_CALLBACK userseek, FMOD_FILE_ASYNCREAD_CALLBACK userasyncread, FMOD_FILE_ASYNCCANCEL_CALLBACK userasynccancel, int blockalign);
FMOD_RESULT F_API FMOD_System_AttachFileSystem          (FMOD_SYSTEM *system, FMOD_FILE_OPEN_CALLBACK useropen, FMOD_FILE_CLOSE_CALLBACK userclose, FMOD_FILE_READ_CALLBACK userread, FMOD_FILE_SEEK_CALLBACK userseek);
FMOD_RESULT F_API FMOD_System_SetAdvancedSettings       (FMOD_SYSTEM *system, FMOD_ADVANCEDSETTINGS *settings);
FMOD_RESULT F_API FMOD_System_GetAdvancedSettings       (FMOD_SYSTEM *system, FMOD_ADVANCEDSETTINGS *settings);
FMOD_RESULT F_API FMOD_System_SetCallback               (FMOD_SYSTEM *system, FMOD_SYSTEM_CALLBACK callback, FMOD_SYSTEM_CALLBACK_TYPE callbackmask);

/* Plug-in support. */
FMOD_RESULT F_API FMOD_System_SetPluginPath             (FMOD_SYSTEM *system, const char *path);
FMOD_RESULT F_API FMOD_System_LoadPlugin                (FMOD_SYSTEM *system, const char *filename, unsigned int *handle, unsigned int priority);
FMOD_RESULT F_API FMOD_System_UnloadPlugin              (FMOD_SYSTEM *system, unsigned int handle);
FMOD_RESULT F_API FMOD_System_GetNumNestedPlugins       (FMOD_SYSTEM *system, unsigned int handle, int *count);
FMOD_RESULT F_API FMOD_System_GetNestedPlugin           (FMOD_SYSTEM *system, unsigned int handle, int index, unsigned int *nestedhandle);
FMOD_RESULT F_API FMOD_System_GetNumPlugins             (FMOD_SYSTEM *system, FMOD_PLUGINTYPE plugintype, int *numplugins);
FMOD_RESULT F_API FMOD_System_GetPluginHandle           (FMOD_SYSTEM *system, FMOD_PLUGINTYPE plugintype, int index, unsigned int *handle);
FMOD_RESULT F_API FMOD_System_GetPluginInfo             (FMOD_SYSTEM *system, unsigned int handle, FMOD_PLUGINTYPE *plugintype, char *name, int namelen, unsigned int *version);
FMOD_RESULT F_API FMOD_System_SetOutputByPlugin         (FMOD_SYSTEM *system, unsigned int handle);
FMOD_RESULT F_API FMOD_System_GetOutputByPlugin         (FMOD_SYSTEM *system, unsigned int *handle);
FMOD_RESULT F_API FMOD_System_CreateDSPByPlugin         (FMOD_SYSTEM *system, unsigned int handle, FMOD_DSP **dsp);
FMOD_RESULT F_API FMOD_System_GetDSPInfoByPlugin        (FMOD_SYSTEM *system, unsigned int handle, const FMOD_DSP_DESCRIPTION **description);
FMOD_RESULT F_API FMOD_System_RegisterCodec             (FMOD_SYSTEM *system, FMOD_CODEC_DESCRIPTION *description, unsigned int *handle, unsigned int priority);
FMOD_RESULT F_API FMOD_System_RegisterDSP               (FMOD_SYSTEM *system, const FMOD_DSP_DESCRIPTION *description, unsigned int *handle);
FMOD_RESULT F_API FMOD_System_RegisterOutput            (FMOD_SYSTEM *system, const FMOD_OUTPUT_DESCRIPTION *description, unsigned int *handle);

/* Init/Close. */
FMOD_RESULT F_API FMOD_System_Init                      (FMOD_SYSTEM *system, int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata);
FMOD_RESULT F_API FMOD_System_Close                     (FMOD_SYSTEM *system);

/* General post-init system functions. */
FMOD_RESULT F_API FMOD_System_Update                    (FMOD_SYSTEM *system);
FMOD_RESULT F_API FMOD_System_SetSpeakerPosition        (FMOD_SYSTEM *system, FMOD_SPEAKER speaker, float x, float y, FMOD_BOOL active);
FMOD_RESULT F_API FMOD_System_GetSpeakerPosition        (FMOD_SYSTEM *system, FMOD_SPEAKER speaker, float *x, float *y, FMOD_BOOL *active);
FMOD_RESULT F_API FMOD_System_SetStreamBufferSize       (FMOD_SYSTEM *system, unsigned int filebuffersize, FMOD_TIMEUNIT filebuffersizetype);
FMOD_RESULT F_API FMOD_System_GetStreamBufferSize       (FMOD_SYSTEM *system, unsigned int *filebuffersize, FMOD_TIMEUNIT *filebuffersizetype);
FMOD_RESULT F_API FMOD_System_Set3DSettings             (FMOD_SYSTEM *system, float dopplerscale, float distancefactor, float rolloffscale);
FMOD_RESULT F_API FMOD_System_Get3DSettings             (FMOD_SYSTEM *system, float *dopplerscale, float *distancefactor, float *rolloffscale);
FMOD_RESULT F_API FMOD_System_Set3DNumListeners         (FMOD_SYSTEM *system, int numlisteners);
FMOD_RESULT F_API FMOD_System_Get3DNumListeners         (FMOD_SYSTEM *system, int *numlisteners);
FMOD_RESULT F_API FMOD_System_Set3DListenerAttributes   (FMOD_SYSTEM *system, int listener, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel, const FMOD_VECTOR *forward, const FMOD_VECTOR *up);
FMOD_RESULT F_API FMOD_System_Get3DListenerAttributes   (FMOD_SYSTEM *system, int listener, FMOD_VECTOR *pos, FMOD_VECTOR *vel, FMOD_VECTOR *forward, FMOD_VECTOR *up);
FMOD_RESULT F_API FMOD_System_Set3DRolloffCallback      (FMOD_SYSTEM *system, FMOD_3D_ROLLOFF_CALLBACK callback);
FMOD_RESULT F_API FMOD_System_MixerSuspend              (FMOD_SYSTEM *system);
FMOD_RESULT F_API FMOD_System_MixerResume               (FMOD_SYSTEM *system);
FMOD_RESULT F_API FMOD_System_GetDefaultMixMatrix       (FMOD_SYSTEM *system, FMOD_SPEAKERMODE sourcespeakermode, FMOD_SPEAKERMODE targetspeakermode, float *matrix, int matrixhop);
FMOD_RESULT F_API FMOD_System_GetSpeakerModeChannels    (FMOD_SYSTEM *system, FMOD_SPEAKERMODE mode, int *channels);

/* System information functions. */
FMOD_RESULT F_API FMOD_System_GetVersion                (FMOD_SYSTEM *system, unsigned int *version);
FMOD_RESULT F_API FMOD_System_GetOutputHandle           (FMOD_SYSTEM *system, void **handle);
FMOD_RESULT F_API FMOD_System_GetChannelsPlaying        (FMOD_SYSTEM *system, int *channels, int *realchannels);
FMOD_RESULT F_API FMOD_System_GetCPUUsage               (FMOD_SYSTEM *system, FMOD_CPU_USAGE *usage);
FMOD_RESULT F_API FMOD_System_GetFileUsage              (FMOD_SYSTEM *system, long long *sampleBytesRead, long long *streamBytesRead, long long *otherBytesRead);

/* Sound/DSP/Channel/FX creation and retrieval. */
FMOD_RESULT F_API FMOD_System_CreateSound               (FMOD_SYSTEM *system, const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD_SOUND **sound);
FMOD_RESULT F_API FMOD_System_CreateStream              (FMOD_SYSTEM *system, const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, FMOD_SOUND **sound);
FMOD_RESULT F_API FMOD_System_CreateDSP                 (FMOD_SYSTEM *system, const FMOD_DSP_DESCRIPTION *description, FMOD_DSP **dsp);
FMOD_RESULT F_API FMOD_System_CreateDSPByType           (FMOD_SYSTEM *system, FMOD_DSP_TYPE type, FMOD_DSP **dsp);
FMOD_RESULT F_API FMOD_System_CreateChannelGroup        (FMOD_SYSTEM *system, const char *name, FMOD_CHANNELGROUP **channelgroup);
FMOD_RESULT F_API FMOD_System_CreateSoundGroup          (FMOD_SYSTEM *system, const char *name, FMOD_SOUNDGROUP **soundgroup);
FMOD_RESULT F_API FMOD_System_CreateReverb3D            (FMOD_SYSTEM *system, FMOD_REVERB3D **reverb);
FMOD_RESULT F_API FMOD_System_PlaySound                 (FMOD_SYSTEM *system, FMOD_SOUND *sound, FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL paused, FMOD_CHANNEL **channel);
FMOD_RESULT F_API FMOD_System_PlayDSP                   (FMOD_SYSTEM *system, FMOD_DSP *dsp, FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL paused, FMOD_CHANNEL **channel);
FMOD_RESULT F_API FMOD_System_GetChannel                (FMOD_SYSTEM *system, int channelid, FMOD_CHANNEL **channel);
FMOD_RESULT F_API FMOD_System_GetDSPInfoByType          (FMOD_SYSTEM *system, FMOD_DSP_TYPE type, const FMOD_DSP_DESCRIPTION **description);
FMOD_RESULT F_API FMOD_System_GetMasterChannelGroup     (FMOD_SYSTEM *system, FMOD_CHANNELGROUP **channelgroup);
FMOD_RESULT F_API FMOD_System_GetMasterSoundGroup       (FMOD_SYSTEM *system, FMOD_SOUNDGROUP **soundgroup);

/* Routing to ports. */
FMOD_RESULT F_API FMOD_System_AttachChannelGroupToPort  (FMOD_SYSTEM *system, FMOD_PORT_TYPE portType, FMOD_PORT_INDEX portIndex, FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL passThru);
FMOD_RESULT F_API FMOD_System_DetachChannelGroupFromPort(FMOD_SYSTEM *system, FMOD_CHANNELGROUP *channelgroup);

/* Reverb API. */
FMOD_RESULT F_API FMOD_System_SetReverbProperties       (FMOD_SYSTEM *system, int instance, const FMOD_REVERB_PROPERTIES *prop);
FMOD_RESULT F_API FMOD_System_GetReverbProperties       (FMOD_SYSTEM *system, int instance, FMOD_REVERB_PROPERTIES *prop);

/* System level DSP functionality. */
FMOD_RESULT F_API FMOD_System_LockDSP                   (FMOD_SYSTEM *system);
FMOD_RESULT F_API FMOD_System_UnlockDSP                 (FMOD_SYSTEM *system);

/* Recording API. */
FMOD_RESULT F_API FMOD_System_GetRecordNumDrivers       (FMOD_SYSTEM *system, int *numdrivers, int *numconnected);
FMOD_RESULT F_API FMOD_System_GetRecordDriverInfo       (FMOD_SYSTEM *system, int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels, FMOD_DRIVER_STATE *state);
FMOD_RESULT F_API FMOD_System_GetRecordPosition         (FMOD_SYSTEM *system, int id, unsigned int *position);
FMOD_RESULT F_API FMOD_System_RecordStart               (FMOD_SYSTEM *system, int id, FMOD_SOUND *sound, FMOD_BOOL loop);
FMOD_RESULT F_API FMOD_System_RecordStop                (FMOD_SYSTEM *system, int id);
FMOD_RESULT F_API FMOD_System_IsRecording               (FMOD_SYSTEM *system, int id, FMOD_BOOL *recording);

/* Geometry API. */
FMOD_RESULT F_API FMOD_System_CreateGeometry            (FMOD_SYSTEM *system, int maxpolygons, int maxvertices, FMOD_GEOMETRY **geometry);
FMOD_RESULT F_API FMOD_System_SetGeometrySettings       (FMOD_SYSTEM *system, float maxworldsize);
FMOD_RESULT F_API FMOD_System_GetGeometrySettings       (FMOD_SYSTEM *system, float *maxworldsize);
FMOD_RESULT F_API FMOD_System_LoadGeometry              (FMOD_SYSTEM *system, const void *data, int datasize, FMOD_GEOMETRY **geometry);
FMOD_RESULT F_API FMOD_System_GetGeometryOcclusion      (FMOD_SYSTEM *system, const FMOD_VECTOR *listener, const FMOD_VECTOR *source, float *direct, float *reverb);

/* Network functions. */
FMOD_RESULT F_API FMOD_System_SetNetworkProxy           (FMOD_SYSTEM *system, const char *proxy);
FMOD_RESULT F_API FMOD_System_GetNetworkProxy           (FMOD_SYSTEM *system, char *proxy, int proxylen);
FMOD_RESULT F_API FMOD_System_SetNetworkTimeout         (FMOD_SYSTEM *system, int timeout);
FMOD_RESULT F_API FMOD_System_GetNetworkTimeout         (FMOD_SYSTEM *system, int *timeout);

/* Userdata set/get. */
FMOD_RESULT F_API FMOD_System_SetUserData               (FMOD_SYSTEM *system, void *userdata);
FMOD_RESULT F_API FMOD_System_GetUserData               (FMOD_SYSTEM *system, void **userdata);

/* Sound API
*/ // :3

FMOD_RESULT F_API FMOD_Sound_Release                    (FMOD_SOUND *sound); // :3
FMOD_RESULT F_API FMOD_Sound_GetSystemObject            (FMOD_SOUND *sound, FMOD_SYSTEM **system); // :3

/*
     Standard sound manipulation functions.
*/ // :3

FMOD_RESULT F_API FMOD_Sound_Lock                       (FMOD_SOUND *sound, unsigned int offset, unsigned int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2); // :3
FMOD_RESULT F_API FMOD_Sound_Unlock                     (FMOD_SOUND *sound, void *ptr1, void *ptr2, unsigned int len1, unsigned int len2); // :3
FMOD_RESULT F_API FMOD_Sound_SetDefaults                (FMOD_SOUND *sound, float frequency, int priority); // :3
FMOD_RESULT F_API FMOD_Sound_GetDefaults                (FMOD_SOUND *sound, float *frequency, int *priority); // :3
FMOD_RESULT F_API FMOD_Sound_Set3DMinMaxDistance        (FMOD_SOUND *sound, float min, float max); // :3
FMOD_RESULT F_API FMOD_Sound_Get3DMinMaxDistance        (FMOD_SOUND *sound, float *min, float *max); // :3
FMOD_RESULT F_API FMOD_Sound_Set3DConeSettings          (FMOD_SOUND *sound, float insideconeangle, float outsideconeangle, float outsidevolume); // :3
FMOD_RESULT F_API FMOD_Sound_Get3DConeSettings          (FMOD_SOUND *sound, float *insideconeangle, float *outsideconeangle, float *outsidevolume); // :3
FMOD_RESULT F_API FMOD_Sound_Set3DCustomRolloff         (FMOD_SOUND *sound, FMOD_VECTOR *points, int numpoints); // :3
FMOD_RESULT F_API FMOD_Sound_Get3DCustomRolloff         (FMOD_SOUND *sound, FMOD_VECTOR **points, int *numpoints); // :3
FMOD_RESULT F_API FMOD_Sound_GetSubSound                (FMOD_SOUND *sound, int index, FMOD_SOUND **subsound); // :3
FMOD_RESULT F_API FMOD_Sound_GetSubSoundParent          (FMOD_SOUND *sound, FMOD_SOUND **parentsound); // :3
FMOD_RESULT F_API FMOD_Sound_GetName                    (FMOD_SOUND *sound, char *name, int namelen); // :3
FMOD_RESULT F_API FMOD_Sound_GetLength                  (FMOD_SOUND *sound, unsigned int *length, FMOD_TIMEUNIT lengthtype); // :3
FMOD_RESULT F_API FMOD_Sound_GetFormat                  (FMOD_SOUND *sound, FMOD_SOUND_TYPE *type, FMOD_SOUND_FORMAT *format, int *channels, int *bits); // :3
FMOD_RESULT F_API FMOD_Sound_GetNumSubSounds            (FMOD_SOUND *sound, int *numsubsounds); // :3
FMOD_RESULT F_API FMOD_Sound_GetNumTags                 (FMOD_SOUND *sound, int *numtags, int *numtagsupdated); // :3
FMOD_RESULT F_API FMOD_Sound_GetTag                     (FMOD_SOUND *sound, const char *name, int index, FMOD_TAG *tag); // :3
FMOD_RESULT F_API FMOD_Sound_GetOpenState               (FMOD_SOUND *sound, FMOD_OPENSTATE *openstate, unsigned int *percentbuffered, FMOD_BOOL *starving, FMOD_BOOL *diskbusy); // :3
FMOD_RESULT F_API FMOD_Sound_ReadData                   (FMOD_SOUND *sound, void *buffer, unsigned int length, unsigned int *read); // :3
FMOD_RESULT F_API FMOD_Sound_SeekData                   (FMOD_SOUND *sound, unsigned int pcm); // :3

FMOD_RESULT F_API FMOD_Sound_SetSoundGroup              (FMOD_SOUND *sound, FMOD_SOUNDGROUP *soundgroup); // :3
FMOD_RESULT F_API FMOD_Sound_GetSoundGroup              (FMOD_SOUND *sound, FMOD_SOUNDGROUP **soundgroup); // :3

/*
     Synchronization point API.  These points can come from markers embedded in wav files, and can also generate channel callbacks.
*/ // :3

FMOD_RESULT F_API FMOD_Sound_GetNumSyncPoints           (FMOD_SOUND *sound, int *numsyncpoints); // :3
FMOD_RESULT F_API FMOD_Sound_GetSyncPoint               (FMOD_SOUND *sound, int index, FMOD_SYNCPOINT **point); // :3
FMOD_RESULT F_API FMOD_Sound_GetSyncPointInfo           (FMOD_SOUND *sound, FMOD_SYNCPOINT *point, char *name, int namelen, unsigned int *offset, FMOD_TIMEUNIT offsettype); // :3
FMOD_RESULT F_API FMOD_Sound_AddSyncPoint               (FMOD_SOUND *sound, unsigned int offset, FMOD_TIMEUNIT offsettype, const char *name, FMOD_SYNCPOINT **point); // :3
FMOD_RESULT F_API FMOD_Sound_DeleteSyncPoint            (FMOD_SOUND *sound, FMOD_SYNCPOINT *point); // :3

/*
     Functions also in Channel class but here they are the 'default' to save having to change it in Channel all the time.
*/ // :3

FMOD_RESULT F_API FMOD_Sound_SetMode                    (FMOD_SOUND *sound, FMOD_MODE mode); // :3
FMOD_RESULT F_API FMOD_Sound_GetMode                    (FMOD_SOUND *sound, FMOD_MODE *mode); // :3
FMOD_RESULT F_API FMOD_Sound_SetLoopCount               (FMOD_SOUND *sound, int loopcount); // :3
FMOD_RESULT F_API FMOD_Sound_GetLoopCount               (FMOD_SOUND *sound, int *loopcount); // :3
FMOD_RESULT F_API FMOD_Sound_SetLoopPoints              (FMOD_SOUND *sound, unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend, FMOD_TIMEUNIT loopendtype); // :3
FMOD_RESULT F_API FMOD_Sound_GetLoopPoints              (FMOD_SOUND *sound, unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend, FMOD_TIMEUNIT loopendtype); // :3

/*
     For MOD/S3M/XM/IT/MID sequenced formats only.
*/ // :3

FMOD_RESULT F_API FMOD_Sound_GetMusicNumChannels        (FMOD_SOUND *sound, int *numchannels); // :3
FMOD_RESULT F_API FMOD_Sound_SetMusicChannelVolume      (FMOD_SOUND *sound, int channel, float volume); // :3
FMOD_RESULT F_API FMOD_Sound_GetMusicChannelVolume      (FMOD_SOUND *sound, int channel, float *volume); // :3
FMOD_RESULT F_API FMOD_Sound_SetMusicSpeed              (FMOD_SOUND *sound, float speed); // :3
FMOD_RESULT F_API FMOD_Sound_GetMusicSpeed              (FMOD_SOUND *sound, float *speed); // :3

/*
     Userdata set/get.
*/ // :3

FMOD_RESULT F_API FMOD_Sound_SetUserData                (FMOD_SOUND *sound, void *userdata); // :3
FMOD_RESULT F_API FMOD_Sound_GetUserData                (FMOD_SOUND *sound, void **userdata); // :3

/*
    'Channel' API
*/ // :3

FMOD_RESULT F_API FMOD_Channel_GetSystemObject          (FMOD_CHANNEL *channel, FMOD_SYSTEM **system); // :3

/*
     General control functionality for Channels and ChannelGroups.
*/ // :3

FMOD_RESULT F_API FMOD_Channel_Stop                     (FMOD_CHANNEL *channel); // :3
FMOD_RESULT F_API FMOD_Channel_SetPaused                (FMOD_CHANNEL *channel, FMOD_BOOL paused); // :3
FMOD_RESULT F_API FMOD_Channel_GetPaused                (FMOD_CHANNEL *channel, FMOD_BOOL *paused); // :3
FMOD_RESULT F_API FMOD_Channel_SetVolume                (FMOD_CHANNEL *channel, float volume); // :3
FMOD_RESULT F_API FMOD_Channel_GetVolume                (FMOD_CHANNEL *channel, float *volume); // :3
FMOD_RESULT F_API FMOD_Channel_SetVolumeRamp            (FMOD_CHANNEL *channel, FMOD_BOOL ramp); // :3
FMOD_RESULT F_API FMOD_Channel_GetVolumeRamp            (FMOD_CHANNEL *channel, FMOD_BOOL *ramp); // :3
FMOD_RESULT F_API FMOD_Channel_GetAudibility            (FMOD_CHANNEL *channel, float *audibility); // :3
FMOD_RESULT F_API FMOD_Channel_SetPitch                 (FMOD_CHANNEL *channel, float pitch); // :3
FMOD_RESULT F_API FMOD_Channel_GetPitch                 (FMOD_CHANNEL *channel, float *pitch); // :3
FMOD_RESULT F_API FMOD_Channel_SetMute                  (FMOD_CHANNEL *channel, FMOD_BOOL mute); // :3
FMOD_RESULT F_API FMOD_Channel_GetMute                  (FMOD_CHANNEL *channel, FMOD_BOOL *mute); // :3
FMOD_RESULT F_API FMOD_Channel_SetReverbProperties      (FMOD_CHANNEL *channel, int instance, float wet); // :3
FMOD_RESULT F_API FMOD_Channel_GetReverbProperties      (FMOD_CHANNEL *channel, int instance, float *wet); // :3
FMOD_RESULT F_API FMOD_Channel_SetLowPassGain           (FMOD_CHANNEL *channel, float gain); // :3
FMOD_RESULT F_API FMOD_Channel_GetLowPassGain           (FMOD_CHANNEL *channel, float *gain); // :3
FMOD_RESULT F_API FMOD_Channel_SetMode                  (FMOD_CHANNEL *channel, FMOD_MODE mode); // :3
FMOD_RESULT F_API FMOD_Channel_GetMode                  (FMOD_CHANNEL *channel, FMOD_MODE *mode); // :3
FMOD_RESULT F_API FMOD_Channel_SetCallback              (FMOD_CHANNEL *channel, FMOD_CHANNELCONTROL_CALLBACK callback); // :3
FMOD_RESULT F_API FMOD_Channel_IsPlaying                (FMOD_CHANNEL *channel, FMOD_BOOL *isplaying); // :3

/*
     Note all 'set' functions alter a final matrix, this is why the only get function is getMixMatrix, to avoid other get functions returning incorrect/obsolete values.
*/ // :3

FMOD_RESULT F_API FMOD_Channel_SetPan                   (FMOD_CHANNEL *channel, float pan); // :3
FMOD_RESULT F_API FMOD_Channel_SetMixLevelsOutput       (FMOD_CHANNEL *channel, float frontleft, float frontright, float center, float lfe, float surroundleft, float surroundright, float backleft, float backright); // :3
FMOD_RESULT F_API FMOD_Channel_SetMixLevelsInput        (FMOD_CHANNEL *channel, float *levels, int numlevels); // :3
FMOD_RESULT F_API FMOD_Channel_SetMixMatrix             (FMOD_CHANNEL *channel, float *matrix, int outchannels, int inchannels, int inchannel_hop); // :3
FMOD_RESULT F_API FMOD_Channel_GetMixMatrix             (FMOD_CHANNEL *channel, float *matrix, int *outchannels, int *inchannels, int inchannel_hop); // :3

/*
     Clock based functionality.
*/ // :3

FMOD_RESULT F_API FMOD_Channel_GetDSPClock              (FMOD_CHANNEL *channel, unsigned long long *dspclock, unsigned long long *parentclock); // :3
FMOD_RESULT F_API FMOD_Channel_SetDelay                 (FMOD_CHANNEL *channel, unsigned long long dspclock_start, unsigned long long dspclock_end, FMOD_BOOL stopchannels); // :3
FMOD_RESULT F_API FMOD_Channel_GetDelay                 (FMOD_CHANNEL *channel, unsigned long long *dspclock_start, unsigned long long *dspclock_end, FMOD_BOOL *stopchannels); // :3
FMOD_RESULT F_API FMOD_Channel_AddFadePoint             (FMOD_CHANNEL *channel, unsigned long long dspclock, float volume); // :3
FMOD_RESULT F_API FMOD_Channel_SetFadePointRamp         (FMOD_CHANNEL *channel, unsigned long long dspclock, float volume); // :3
FMOD_RESULT F_API FMOD_Channel_RemoveFadePoints         (FMOD_CHANNEL *channel, unsigned long long dspclock_start, unsigned long long dspclock_end); // :3
FMOD_RESULT F_API FMOD_Channel_GetFadePoints            (FMOD_CHANNEL *channel, unsigned int *numpoints, unsigned long long *point_dspclock, float *point_volume); // :3

/*
     DSP effects.
*/ // :3

FMOD_RESULT F_API FMOD_Channel_GetDSP                   (FMOD_CHANNEL *channel, int index, FMOD_DSP **dsp); // :3
FMOD_RESULT F_API FMOD_Channel_AddDSP                   (FMOD_CHANNEL *channel, int index, FMOD_DSP *dsp); // :3
FMOD_RESULT F_API FMOD_Channel_RemoveDSP                (FMOD_CHANNEL *channel, FMOD_DSP *dsp); // :3
FMOD_RESULT F_API FMOD_Channel_GetNumDSPs               (FMOD_CHANNEL *channel, int *numdsps); // :3
FMOD_RESULT F_API FMOD_Channel_SetDSPIndex              (FMOD_CHANNEL *channel, FMOD_DSP *dsp, int index); // :3
FMOD_RESULT F_API FMOD_Channel_GetDSPIndex              (FMOD_CHANNEL *channel, FMOD_DSP *dsp, int *index); // :3

/*
     3D functionality.
*/ // :3

FMOD_RESULT F_API FMOD_Channel_Set3DAttributes          (FMOD_CHANNEL *channel, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DAttributes          (FMOD_CHANNEL *channel, FMOD_VECTOR *pos, FMOD_VECTOR *vel); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DMinMaxDistance      (FMOD_CHANNEL *channel, float mindistance, float maxdistance); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DMinMaxDistance      (FMOD_CHANNEL *channel, float *mindistance, float *maxdistance); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DConeSettings        (FMOD_CHANNEL *channel, float insideconeangle, float outsideconeangle, float outsidevolume); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DConeSettings        (FMOD_CHANNEL *channel, float *insideconeangle, float *outsideconeangle, float *outsidevolume); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DConeOrientation     (FMOD_CHANNEL *channel, FMOD_VECTOR *orientation); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DConeOrientation     (FMOD_CHANNEL *channel, FMOD_VECTOR *orientation); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DCustomRolloff       (FMOD_CHANNEL *channel, FMOD_VECTOR *points, int numpoints); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DCustomRolloff       (FMOD_CHANNEL *channel, FMOD_VECTOR **points, int *numpoints); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DOcclusion           (FMOD_CHANNEL *channel, float directocclusion, float reverbocclusion); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DOcclusion           (FMOD_CHANNEL *channel, float *directocclusion, float *reverbocclusion); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DSpread              (FMOD_CHANNEL *channel, float angle); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DSpread              (FMOD_CHANNEL *channel, float *angle); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DLevel               (FMOD_CHANNEL *channel, float level); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DLevel               (FMOD_CHANNEL *channel, float *level); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DDopplerLevel        (FMOD_CHANNEL *channel, float level); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DDopplerLevel        (FMOD_CHANNEL *channel, float *level); // :3
FMOD_RESULT F_API FMOD_Channel_Set3DDistanceFilter      (FMOD_CHANNEL *channel, FMOD_BOOL custom, float customLevel, float centerFreq); // :3
FMOD_RESULT F_API FMOD_Channel_Get3DDistanceFilter      (FMOD_CHANNEL *channel, FMOD_BOOL *custom, float *customLevel, float *centerFreq); // :3

/*
     Userdata set/get.
*/ // :3

FMOD_RESULT F_API FMOD_Channel_SetUserData              (FMOD_CHANNEL *channel, void *userdata); // :3
FMOD_RESULT F_API FMOD_Channel_GetUserData              (FMOD_CHANNEL *channel, void **userdata); // :3

/*
     Channel specific control functionality.
*/ // :3

FMOD_RESULT F_API FMOD_Channel_SetFrequency             (FMOD_CHANNEL *channel, float frequency); // :3
FMOD_RESULT F_API FMOD_Channel_GetFrequency             (FMOD_CHANNEL *channel, float *frequency); // :3
FMOD_RESULT F_API FMOD_Channel_SetPriority              (FMOD_CHANNEL *channel, int priority); // :3
FMOD_RESULT F_API FMOD_Channel_GetPriority              (FMOD_CHANNEL *channel, int *priority); // :3
FMOD_RESULT F_API FMOD_Channel_SetPosition              (FMOD_CHANNEL *channel, unsigned int position, FMOD_TIMEUNIT postype); // :3
FMOD_RESULT F_API FMOD_Channel_GetPosition              (FMOD_CHANNEL *channel, unsigned int *position, FMOD_TIMEUNIT postype); // :3
FMOD_RESULT F_API FMOD_Channel_SetChannelGroup          (FMOD_CHANNEL *channel, FMOD_CHANNELGROUP *channelgroup); // :3
FMOD_RESULT F_API FMOD_Channel_GetChannelGroup          (FMOD_CHANNEL *channel, FMOD_CHANNELGROUP **channelgroup); // :3
FMOD_RESULT F_API FMOD_Channel_SetLoopCount             (FMOD_CHANNEL *channel, int loopcount); // :3
FMOD_RESULT F_API FMOD_Channel_GetLoopCount             (FMOD_CHANNEL *channel, int *loopcount); // :3
FMOD_RESULT F_API FMOD_Channel_SetLoopPoints            (FMOD_CHANNEL *channel, unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend, FMOD_TIMEUNIT loopendtype); // :3
FMOD_RESULT F_API FMOD_Channel_GetLoopPoints            (FMOD_CHANNEL *channel, unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend, FMOD_TIMEUNIT loopendtype); // :3

/*
     Information only functions.
*/ // :3

FMOD_RESULT F_API FMOD_Channel_IsVirtual                (FMOD_CHANNEL *channel, FMOD_BOOL *isvirtual); // :3
FMOD_RESULT F_API FMOD_Channel_GetCurrentSound          (FMOD_CHANNEL *channel, FMOD_SOUND **sound); // :3
FMOD_RESULT F_API FMOD_Channel_GetIndex                 (FMOD_CHANNEL *channel, int *index); // :3

/*
    'ChannelGroup' API
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_GetSystemObject     (FMOD_CHANNELGROUP *channelgroup, FMOD_SYSTEM **system); // :3

/*
     General control functionality for Channels and ChannelGroups.
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_Stop                (FMOD_CHANNELGROUP *channelgroup); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetPaused           (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL paused); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetPaused           (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL *paused); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetVolume           (FMOD_CHANNELGROUP *channelgroup, float volume); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetVolume           (FMOD_CHANNELGROUP *channelgroup, float *volume); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetVolumeRamp       (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL ramp); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetVolumeRamp       (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL *ramp); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetAudibility       (FMOD_CHANNELGROUP *channelgroup, float *audibility); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetPitch            (FMOD_CHANNELGROUP *channelgroup, float pitch); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetPitch            (FMOD_CHANNELGROUP *channelgroup, float *pitch); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetMute             (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL mute); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetMute             (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL *mute); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetReverbProperties (FMOD_CHANNELGROUP *channelgroup, int instance, float wet); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetReverbProperties (FMOD_CHANNELGROUP *channelgroup, int instance, float *wet); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetLowPassGain      (FMOD_CHANNELGROUP *channelgroup, float gain); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetLowPassGain      (FMOD_CHANNELGROUP *channelgroup, float *gain); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetMode             (FMOD_CHANNELGROUP *channelgroup, FMOD_MODE mode); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetMode             (FMOD_CHANNELGROUP *channelgroup, FMOD_MODE *mode); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetCallback         (FMOD_CHANNELGROUP *channelgroup, FMOD_CHANNELCONTROL_CALLBACK callback); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_IsPlaying           (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL *isplaying); // :3

/*
     Note all 'set' functions alter a final matrix, this is why the only get function is getMixMatrix, to avoid other get functions returning incorrect/obsolete values.
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_SetPan              (FMOD_CHANNELGROUP *channelgroup, float pan); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetMixLevelsOutput  (FMOD_CHANNELGROUP *channelgroup, float frontleft, float frontright, float center, float lfe, float surroundleft, float surroundright, float backleft, float backright); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetMixLevelsInput   (FMOD_CHANNELGROUP *channelgroup, float *levels, int numlevels); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetMixMatrix        (FMOD_CHANNELGROUP *channelgroup, float *matrix, int outchannels, int inchannels, int inchannel_hop); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetMixMatrix        (FMOD_CHANNELGROUP *channelgroup, float *matrix, int *outchannels, int *inchannels, int inchannel_hop); // :3

/*
     Clock based functionality.
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_GetDSPClock         (FMOD_CHANNELGROUP *channelgroup, unsigned long long *dspclock, unsigned long long *parentclock); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetDelay            (FMOD_CHANNELGROUP *channelgroup, unsigned long long dspclock_start, unsigned long long dspclock_end, FMOD_BOOL stopchannels); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetDelay            (FMOD_CHANNELGROUP *channelgroup, unsigned long long *dspclock_start, unsigned long long *dspclock_end, FMOD_BOOL *stopchannels); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_AddFadePoint        (FMOD_CHANNELGROUP *channelgroup, unsigned long long dspclock, float volume); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetFadePointRamp    (FMOD_CHANNELGROUP *channelgroup, unsigned long long dspclock, float volume); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_RemoveFadePoints    (FMOD_CHANNELGROUP *channelgroup, unsigned long long dspclock_start, unsigned long long dspclock_end); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetFadePoints       (FMOD_CHANNELGROUP *channelgroup, unsigned int *numpoints, unsigned long long *point_dspclock, float *point_volume); // :3

/*
     DSP effects.
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_GetDSP              (FMOD_CHANNELGROUP *channelgroup, int index, FMOD_DSP **dsp); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_AddDSP              (FMOD_CHANNELGROUP *channelgroup, int index, FMOD_DSP *dsp); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_RemoveDSP           (FMOD_CHANNELGROUP *channelgroup, FMOD_DSP *dsp); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetNumDSPs          (FMOD_CHANNELGROUP *channelgroup, int *numdsps); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_SetDSPIndex         (FMOD_CHANNELGROUP *channelgroup, FMOD_DSP *dsp, int index); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetDSPIndex         (FMOD_CHANNELGROUP *channelgroup, FMOD_DSP *dsp, int *index); // :3

/*
     3D functionality.
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_Set3DAttributes     (FMOD_CHANNELGROUP *channelgroup, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DAttributes     (FMOD_CHANNELGROUP *channelgroup, FMOD_VECTOR *pos, FMOD_VECTOR *vel); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DMinMaxDistance (FMOD_CHANNELGROUP *channelgroup, float mindistance, float maxdistance); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DMinMaxDistance (FMOD_CHANNELGROUP *channelgroup, float *mindistance, float *maxdistance); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DConeSettings   (FMOD_CHANNELGROUP *channelgroup, float insideconeangle, float outsideconeangle, float outsidevolume); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DConeSettings   (FMOD_CHANNELGROUP *channelgroup, float *insideconeangle, float *outsideconeangle, float *outsidevolume); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DConeOrientation(FMOD_CHANNELGROUP *channelgroup, FMOD_VECTOR *orientation); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DConeOrientation(FMOD_CHANNELGROUP *channelgroup, FMOD_VECTOR *orientation); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DCustomRolloff  (FMOD_CHANNELGROUP *channelgroup, FMOD_VECTOR *points, int numpoints); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DCustomRolloff  (FMOD_CHANNELGROUP *channelgroup, FMOD_VECTOR **points, int *numpoints); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DOcclusion      (FMOD_CHANNELGROUP *channelgroup, float directocclusion, float reverbocclusion); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DOcclusion      (FMOD_CHANNELGROUP *channelgroup, float *directocclusion, float *reverbocclusion); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DSpread         (FMOD_CHANNELGROUP *channelgroup, float angle); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DSpread         (FMOD_CHANNELGROUP *channelgroup, float *angle); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DLevel          (FMOD_CHANNELGROUP *channelgroup, float level); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DLevel          (FMOD_CHANNELGROUP *channelgroup, float *level); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DDopplerLevel   (FMOD_CHANNELGROUP *channelgroup, float level); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DDopplerLevel   (FMOD_CHANNELGROUP *channelgroup, float *level); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Set3DDistanceFilter (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL custom, float customLevel, float centerFreq); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_Get3DDistanceFilter (FMOD_CHANNELGROUP *channelgroup, FMOD_BOOL *custom, float *customLevel, float *centerFreq); // :3

/*
     Userdata set/get.
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_SetUserData         (FMOD_CHANNELGROUP *channelgroup, void *userdata); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetUserData         (FMOD_CHANNELGROUP *channelgroup, void **userdata); // :3

FMOD_RESULT F_API FMOD_ChannelGroup_Release             (FMOD_CHANNELGROUP *channelgroup); // :3

/*
     Nested channel groups.
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_AddGroup            (FMOD_CHANNELGROUP *channelgroup, FMOD_CHANNELGROUP *group, FMOD_BOOL propagatedspclock, FMOD_DSPCONNECTION **connection); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetNumGroups        (FMOD_CHANNELGROUP *channelgroup, int *numgroups); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetGroup            (FMOD_CHANNELGROUP *channelgroup, int index, FMOD_CHANNELGROUP **group); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetParentGroup      (FMOD_CHANNELGROUP *channelgroup, FMOD_CHANNELGROUP **group); // :3

/*
     Information only functions.
*/ // :3

FMOD_RESULT F_API FMOD_ChannelGroup_GetName             (FMOD_CHANNELGROUP *channelgroup, char *name, int namelen); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetNumChannels      (FMOD_CHANNELGROUP *channelgroup, int *numchannels); // :3
FMOD_RESULT F_API FMOD_ChannelGroup_GetChannel          (FMOD_CHANNELGROUP *channelgroup, int index, FMOD_CHANNEL **channel); // :3

/*
    'SoundGroup' API
*/ // :3

FMOD_RESULT F_API FMOD_SoundGroup_Release               (FMOD_SOUNDGROUP *soundgroup); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetSystemObject       (FMOD_SOUNDGROUP *soundgroup, FMOD_SYSTEM **system); // :3

/*
     SoundGroup control functions.
*/ // :3

FMOD_RESULT F_API FMOD_SoundGroup_SetMaxAudible         (FMOD_SOUNDGROUP *soundgroup, int maxaudible); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetMaxAudible         (FMOD_SOUNDGROUP *soundgroup, int *maxaudible); // :3
FMOD_RESULT F_API FMOD_SoundGroup_SetMaxAudibleBehavior (FMOD_SOUNDGROUP *soundgroup, FMOD_SOUNDGROUP_BEHAVIOR behavior); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetMaxAudibleBehavior (FMOD_SOUNDGROUP *soundgroup, FMOD_SOUNDGROUP_BEHAVIOR *behavior); // :3
FMOD_RESULT F_API FMOD_SoundGroup_SetMuteFadeSpeed      (FMOD_SOUNDGROUP *soundgroup, float speed); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetMuteFadeSpeed      (FMOD_SOUNDGROUP *soundgroup, float *speed); // :3
FMOD_RESULT F_API FMOD_SoundGroup_SetVolume             (FMOD_SOUNDGROUP *soundgroup, float volume); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetVolume             (FMOD_SOUNDGROUP *soundgroup, float *volume); // :3
FMOD_RESULT F_API FMOD_SoundGroup_Stop                  (FMOD_SOUNDGROUP *soundgroup); // :3

/*
     Information only functions.
*/ // :3

FMOD_RESULT F_API FMOD_SoundGroup_GetName               (FMOD_SOUNDGROUP *soundgroup, char *name, int namelen); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetNumSounds          (FMOD_SOUNDGROUP *soundgroup, int *numsounds); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetSound              (FMOD_SOUNDGROUP *soundgroup, int index, FMOD_SOUND **sound); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetNumPlaying         (FMOD_SOUNDGROUP *soundgroup, int *numplaying); // :3

/*
     Userdata set/get.
*/ // :3

FMOD_RESULT F_API FMOD_SoundGroup_SetUserData           (FMOD_SOUNDGROUP *soundgroup, void *userdata); // :3
FMOD_RESULT F_API FMOD_SoundGroup_GetUserData           (FMOD_SOUNDGROUP *soundgroup, void **userdata); // :3

/*
    'DSP' API
*/ // :3

FMOD_RESULT F_API FMOD_DSP_Release                      (FMOD_DSP *dsp); // :3
FMOD_RESULT F_API FMOD_DSP_GetSystemObject              (FMOD_DSP *dsp, FMOD_SYSTEM **system); // :3

/*
     Connection / disconnection / input and output enumeration.
*/ // :3

FMOD_RESULT F_API FMOD_DSP_AddInput                     (FMOD_DSP *dsp, FMOD_DSP *input, FMOD_DSPCONNECTION **connection, FMOD_DSPCONNECTION_TYPE type); // :3
FMOD_RESULT F_API FMOD_DSP_DisconnectFrom               (FMOD_DSP *dsp, FMOD_DSP *target, FMOD_DSPCONNECTION *connection); // :3
FMOD_RESULT F_API FMOD_DSP_DisconnectAll                (FMOD_DSP *dsp, FMOD_BOOL inputs, FMOD_BOOL outputs); // :3
FMOD_RESULT F_API FMOD_DSP_GetNumInputs                 (FMOD_DSP *dsp, int *numinputs); // :3
FMOD_RESULT F_API FMOD_DSP_GetNumOutputs                (FMOD_DSP *dsp, int *numoutputs); // :3
FMOD_RESULT F_API FMOD_DSP_GetInput                     (FMOD_DSP *dsp, int index, FMOD_DSP **input, FMOD_DSPCONNECTION **inputconnection); // :3
FMOD_RESULT F_API FMOD_DSP_GetOutput                    (FMOD_DSP *dsp, int index, FMOD_DSP **output, FMOD_DSPCONNECTION **outputconnection); // :3

/*
     DSP unit control.
*/ // :3

FMOD_RESULT F_API FMOD_DSP_SetActive                    (FMOD_DSP *dsp, FMOD_BOOL active); // :3
FMOD_RESULT F_API FMOD_DSP_GetActive                    (FMOD_DSP *dsp, FMOD_BOOL *active); // :3
FMOD_RESULT F_API FMOD_DSP_SetBypass                    (FMOD_DSP *dsp, FMOD_BOOL bypass); // :3
FMOD_RESULT F_API FMOD_DSP_GetBypass                    (FMOD_DSP *dsp, FMOD_BOOL *bypass); // :3
FMOD_RESULT F_API FMOD_DSP_SetWetDryMix                 (FMOD_DSP *dsp, float prewet, float postwet, float dry); // :3
FMOD_RESULT F_API FMOD_DSP_GetWetDryMix                 (FMOD_DSP *dsp, float *prewet, float *postwet, float *dry); // :3
FMOD_RESULT F_API FMOD_DSP_SetChannelFormat             (FMOD_DSP *dsp, FMOD_CHANNELMASK channelmask, int numchannels, FMOD_SPEAKERMODE source_speakermode); // :3
FMOD_RESULT F_API FMOD_DSP_GetChannelFormat             (FMOD_DSP *dsp, FMOD_CHANNELMASK *channelmask, int *numchannels, FMOD_SPEAKERMODE *source_speakermode); // :3
FMOD_RESULT F_API FMOD_DSP_GetOutputChannelFormat       (FMOD_DSP *dsp, FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE inspeakermode, FMOD_CHANNELMASK *outmask, int *outchannels, FMOD_SPEAKERMODE *outspeakermode); // :3
FMOD_RESULT F_API FMOD_DSP_Reset                        (FMOD_DSP *dsp); // :3
FMOD_RESULT F_API FMOD_DSP_SetCallback                  (FMOD_DSP *dsp, FMOD_DSP_CALLBACK callback); // :3

/*
     DSP parameter control.
*/ // :3

FMOD_RESULT F_API FMOD_DSP_SetParameterFloat            (FMOD_DSP *dsp, int index, float value); // :3
FMOD_RESULT F_API FMOD_DSP_SetParameterInt              (FMOD_DSP *dsp, int index, int value); // :3
FMOD_RESULT F_API FMOD_DSP_SetParameterBool             (FMOD_DSP *dsp, int index, FMOD_BOOL value); // :3
FMOD_RESULT F_API FMOD_DSP_SetParameterData             (FMOD_DSP *dsp, int index, void *data, unsigned int length); // :3
FMOD_RESULT F_API FMOD_DSP_GetParameterFloat            (FMOD_DSP *dsp, int index, float *value, char *valuestr, int valuestrlen); // :3
FMOD_RESULT F_API FMOD_DSP_GetParameterInt              (FMOD_DSP *dsp, int index, int *value, char *valuestr, int valuestrlen); // :3
FMOD_RESULT F_API FMOD_DSP_GetParameterBool             (FMOD_DSP *dsp, int index, FMOD_BOOL *value, char *valuestr, int valuestrlen); // :3
FMOD_RESULT F_API FMOD_DSP_GetParameterData             (FMOD_DSP *dsp, int index, void **data, unsigned int *length, char *valuestr, int valuestrlen); // :3
FMOD_RESULT F_API FMOD_DSP_GetNumParameters             (FMOD_DSP *dsp, int *numparams); // :3
FMOD_RESULT F_API FMOD_DSP_GetParameterInfo             (FMOD_DSP *dsp, int index, FMOD_DSP_PARAMETER_DESC **desc); // :3
FMOD_RESULT F_API FMOD_DSP_GetDataParameterIndex        (FMOD_DSP *dsp, int datatype, int *index); // :3
FMOD_RESULT F_API FMOD_DSP_ShowConfigDialog             (FMOD_DSP *dsp, void *hwnd, FMOD_BOOL show); // :3

/*
     DSP attributes.
*/ // :3

FMOD_RESULT F_API FMOD_DSP_GetInfo                      (FMOD_DSP *dsp, char *name, unsigned int *version, int *channels, int *configwidth, int *configheight); // :3
FMOD_RESULT F_API FMOD_DSP_GetType                      (FMOD_DSP *dsp, FMOD_DSP_TYPE *type); // :3
FMOD_RESULT F_API FMOD_DSP_GetIdle                      (FMOD_DSP *dsp, FMOD_BOOL *idle); // :3

/*
     Userdata set/get.
*/ // :3

FMOD_RESULT F_API FMOD_DSP_SetUserData                  (FMOD_DSP *dsp, void *userdata); // :3
FMOD_RESULT F_API FMOD_DSP_GetUserData                  (FMOD_DSP *dsp, void **userdata); // :3

/*
     Metering.
*/ // :3

FMOD_RESULT F_API FMOD_DSP_SetMeteringEnabled           (FMOD_DSP *dsp, FMOD_BOOL inputEnabled, FMOD_BOOL outputEnabled); // :3
FMOD_RESULT F_API FMOD_DSP_GetMeteringEnabled           (FMOD_DSP *dsp, FMOD_BOOL *inputEnabled, FMOD_BOOL *outputEnabled); // :3
FMOD_RESULT F_API FMOD_DSP_GetMeteringInfo              (FMOD_DSP *dsp, FMOD_DSP_METERING_INFO *inputInfo, FMOD_DSP_METERING_INFO *outputInfo); // :3
FMOD_RESULT F_API FMOD_DSP_GetCPUUsage                  (FMOD_DSP *dsp, unsigned int *exclusive, unsigned int *inclusive); // :3

/*
    'DSPConnection' API
*/ // :3

FMOD_RESULT F_API FMOD_DSPConnection_GetInput           (FMOD_DSPCONNECTION *dspconnection, FMOD_DSP **input); // :3
FMOD_RESULT F_API FMOD_DSPConnection_GetOutput          (FMOD_DSPCONNECTION *dspconnection, FMOD_DSP **output); // :3
FMOD_RESULT F_API FMOD_DSPConnection_SetMix             (FMOD_DSPCONNECTION *dspconnection, float volume); // :3
FMOD_RESULT F_API FMOD_DSPConnection_GetMix             (FMOD_DSPCONNECTION *dspconnection, float *volume); // :3
FMOD_RESULT F_API FMOD_DSPConnection_SetMixMatrix       (FMOD_DSPCONNECTION *dspconnection, float *matrix, int outchannels, int inchannels, int inchannel_hop); // :3
FMOD_RESULT F_API FMOD_DSPConnection_GetMixMatrix       (FMOD_DSPCONNECTION *dspconnection, float *matrix, int *outchannels, int *inchannels, int inchannel_hop); // :3
FMOD_RESULT F_API FMOD_DSPConnection_GetType            (FMOD_DSPCONNECTION *dspconnection, FMOD_DSPCONNECTION_TYPE *type); // :3

/*
     Userdata set/get.
*/ // :3

FMOD_RESULT F_API FMOD_DSPConnection_SetUserData        (FMOD_DSPCONNECTION *dspconnection, void *userdata); // :3
FMOD_RESULT F_API FMOD_DSPConnection_GetUserData        (FMOD_DSPCONNECTION *dspconnection, void **userdata); // :3

/*
    'Geometry' API
*/ // :3

FMOD_RESULT F_API FMOD_Geometry_Release                 (FMOD_GEOMETRY *geometry); // :3

/*
     Polygon manipulation.
*/ // :3

FMOD_RESULT F_API FMOD_Geometry_AddPolygon              (FMOD_GEOMETRY *geometry, float directocclusion, float reverbocclusion, FMOD_BOOL doublesided, int numvertices, const FMOD_VECTOR *vertices, int *polygonindex); // :3
FMOD_RESULT F_API FMOD_Geometry_GetNumPolygons          (FMOD_GEOMETRY *geometry, int *numpolygons); // :3
FMOD_RESULT F_API FMOD_Geometry_GetMaxPolygons          (FMOD_GEOMETRY *geometry, int *maxpolygons, int *maxvertices); // :3
FMOD_RESULT F_API FMOD_Geometry_GetPolygonNumVertices   (FMOD_GEOMETRY *geometry, int index, int *numvertices); // :3
FMOD_RESULT F_API FMOD_Geometry_SetPolygonVertex        (FMOD_GEOMETRY *geometry, int index, int vertexindex, const FMOD_VECTOR *vertex); // :3
FMOD_RESULT F_API FMOD_Geometry_GetPolygonVertex        (FMOD_GEOMETRY *geometry, int index, int vertexindex, FMOD_VECTOR *vertex); // :3
FMOD_RESULT F_API FMOD_Geometry_SetPolygonAttributes    (FMOD_GEOMETRY *geometry, int index, float directocclusion, float reverbocclusion, FMOD_BOOL doublesided); // :3
FMOD_RESULT F_API FMOD_Geometry_GetPolygonAttributes    (FMOD_GEOMETRY *geometry, int index, float *directocclusion, float *reverbocclusion, FMOD_BOOL *doublesided); // :3

/*
     Object manipulation.
*/ // :3

FMOD_RESULT F_API FMOD_Geometry_SetActive               (FMOD_GEOMETRY *geometry, FMOD_BOOL active); // :3
FMOD_RESULT F_API FMOD_Geometry_GetActive               (FMOD_GEOMETRY *geometry, FMOD_BOOL *active); // :3
FMOD_RESULT F_API FMOD_Geometry_SetRotation             (FMOD_GEOMETRY *geometry, const FMOD_VECTOR *forward, const FMOD_VECTOR *up); // :3
FMOD_RESULT F_API FMOD_Geometry_GetRotation             (FMOD_GEOMETRY *geometry, FMOD_VECTOR *forward, FMOD_VECTOR *up); // :3
FMOD_RESULT F_API FMOD_Geometry_SetPosition             (FMOD_GEOMETRY *geometry, const FMOD_VECTOR *position); // :3
FMOD_RESULT F_API FMOD_Geometry_GetPosition             (FMOD_GEOMETRY *geometry, FMOD_VECTOR *position); // :3
FMOD_RESULT F_API FMOD_Geometry_SetScale                (FMOD_GEOMETRY *geometry, const FMOD_VECTOR *scale); // :3
FMOD_RESULT F_API FMOD_Geometry_GetScale                (FMOD_GEOMETRY *geometry, FMOD_VECTOR *scale); // :3
FMOD_RESULT F_API FMOD_Geometry_Save                    (FMOD_GEOMETRY *geometry, void *data, int *datasize); // :3

/*
     Userdata set/get.
*/ // :3

FMOD_RESULT F_API FMOD_Geometry_SetUserData             (FMOD_GEOMETRY *geometry, void *userdata); // :3
FMOD_RESULT F_API FMOD_Geometry_GetUserData             (FMOD_GEOMETRY *geometry, void **userdata); // :3

/*
    'Reverb3D' API
*/ // :3

FMOD_RESULT F_API FMOD_Reverb3D_Release                 (FMOD_REVERB3D *reverb3d); // :3

/*
     Reverb manipulation.
*/ // :3

FMOD_RESULT F_API FMOD_Reverb3D_Set3DAttributes         (FMOD_REVERB3D *reverb3d, const FMOD_VECTOR *position, float mindistance, float maxdistance); // :3
FMOD_RESULT F_API FMOD_Reverb3D_Get3DAttributes         (FMOD_REVERB3D *reverb3d, FMOD_VECTOR *position, float *mindistance, float *maxdistance); // :3
FMOD_RESULT F_API FMOD_Reverb3D_SetProperties           (FMOD_REVERB3D *reverb3d, const FMOD_REVERB_PROPERTIES *properties); // :3
FMOD_RESULT F_API FMOD_Reverb3D_GetProperties           (FMOD_REVERB3D *reverb3d, FMOD_REVERB_PROPERTIES *properties); // :3
FMOD_RESULT F_API FMOD_Reverb3D_SetActive               (FMOD_REVERB3D *reverb3d, FMOD_BOOL active); // :3
FMOD_RESULT F_API FMOD_Reverb3D_GetActive               (FMOD_REVERB3D *reverb3d, FMOD_BOOL *active); // :3

/*
     Userdata set/get.
*/ // :3

FMOD_RESULT F_API FMOD_Reverb3D_SetUserData             (FMOD_REVERB3D *reverb3d, void *userdata); // :3
FMOD_RESULT F_API FMOD_Reverb3D_GetUserData             (FMOD_REVERB3D *reverb3d, void **userdata); // :3

#ifdef __cplusplus // :3
} // :3
#endif // :3

#endif /* _FMOD_H */
