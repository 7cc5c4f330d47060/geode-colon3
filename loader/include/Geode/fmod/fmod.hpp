/* ======================================================================================== */
/* FMOD Core API - C++ header file.                                                         */
/* Copyright (c), Firelight Technologies Pty, Ltd. 2004-2024.                               */
/*                                                                                          */
/* Use this header in conjunction with fmod_common.h (which contains all the constants /    */
/* callbacks) to develop using the C++ language.                                            */
/*                                                                                          */
/* For more detail visit:                                                                   */
/* https://fmod.com/docs/2.02/api/core-api.html                                             */
/* ======================================================================================== */
#ifndef _FMOD_HPP
#define _FMOD_HPP

#include "fmod_common.h"
#include "fmod.h"

/*
    FMOD Namespace
*/ // :3
namespace FMOD // :3
{ // :3
    class System; // :3
    class Sound; // :3
    class ChannelControl; // :3
    class Channel; // :3
    class ChannelGroup; // :3
    class SoundGroup; // :3
    class DSP; // :3
    class DSPConnection; // :3
    class Geometry; // :3
    class Reverb3D; // :3

    /*
        FMOD global system functions (optional).
    */ // :3
    inline FMOD_RESULT Memory_Initialize    (void *poolmem, int poollen, FMOD_MEMORY_ALLOC_CALLBACK useralloc, FMOD_MEMORY_REALLOC_CALLBACK userrealloc, FMOD_MEMORY_FREE_CALLBACK userfree, FMOD_MEMORY_TYPE memtypeflags = FMOD_MEMORY_ALL) { return FMOD_Memory_Initialize(poolmem, poollen, useralloc, userrealloc, userfree, memtypeflags); } // :3
    inline FMOD_RESULT Memory_GetStats      (int *currentalloced, int *maxalloced, bool blocking = true) { return FMOD_Memory_GetStats(currentalloced, maxalloced, blocking); } // :3
    inline FMOD_RESULT Debug_Initialize     (FMOD_DEBUG_FLAGS flags, FMOD_DEBUG_MODE mode = FMOD_DEBUG_MODE_TTY, FMOD_DEBUG_CALLBACK callback = 0, const char *filename = 0) { return FMOD_Debug_Initialize(flags, mode, callback, filename); } // :3
    inline FMOD_RESULT File_SetDiskBusy     (int busy) { return FMOD_File_SetDiskBusy(busy); } // :3
    inline FMOD_RESULT File_GetDiskBusy     (int *busy) { return FMOD_File_GetDiskBusy(busy); } // :3
    inline FMOD_RESULT Thread_SetAttributes (FMOD_THREAD_TYPE type, FMOD_THREAD_AFFINITY affinity = FMOD_THREAD_AFFINITY_GROUP_DEFAULT, FMOD_THREAD_PRIORITY priority = FMOD_THREAD_PRIORITY_DEFAULT, FMOD_THREAD_STACK_SIZE stacksize = FMOD_THREAD_STACK_SIZE_DEFAULT) { return FMOD_Thread_SetAttributes(type, affinity, priority, stacksize); } // :3

    /*
        FMOD System factory functions.
    */ // :3
    inline FMOD_RESULT System_Create        (System **system, unsigned int headerversion = FMOD_VERSION) { return FMOD_System_Create((FMOD_SYSTEM **)system, headerversion); } // :3

    /*
       'System' API
    */ // :3
    class System // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a System class.  System_Create must be used. :3
        System(); // :3
        System(const System &); // :3

      public: // :3

        FMOD_RESULT F_API release                 (); // :3

        // Setup functions. :3
        FMOD_RESULT F_API setOutput               (FMOD_OUTPUTTYPE output); // :3
        FMOD_RESULT F_API getOutput               (FMOD_OUTPUTTYPE *output); // :3
        FMOD_RESULT F_API getNumDrivers           (int *numdrivers); // :3
        FMOD_RESULT F_API getDriverInfo           (int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels); // :3
        FMOD_RESULT F_API setDriver               (int driver); // :3
        FMOD_RESULT F_API getDriver               (int *driver); // :3
        FMOD_RESULT F_API setSoftwareChannels     (int numsoftwarechannels); // :3
        FMOD_RESULT F_API getSoftwareChannels     (int *numsoftwarechannels); // :3
        FMOD_RESULT F_API setSoftwareFormat       (int samplerate, FMOD_SPEAKERMODE speakermode, int numrawspeakers); // :3
        FMOD_RESULT F_API getSoftwareFormat       (int *samplerate, FMOD_SPEAKERMODE *speakermode, int *numrawspeakers); // :3
        FMOD_RESULT F_API setDSPBufferSize        (unsigned int bufferlength, int numbuffers); // :3
        FMOD_RESULT F_API getDSPBufferSize        (unsigned int *bufferlength, int *numbuffers); // :3
        FMOD_RESULT F_API setFileSystem           (FMOD_FILE_OPEN_CALLBACK useropen, FMOD_FILE_CLOSE_CALLBACK userclose, FMOD_FILE_READ_CALLBACK userread, FMOD_FILE_SEEK_CALLBACK userseek, FMOD_FILE_ASYNCREAD_CALLBACK userasyncread, FMOD_FILE_ASYNCCANCEL_CALLBACK userasynccancel, int blockalign); // :3
        FMOD_RESULT F_API attachFileSystem        (FMOD_FILE_OPEN_CALLBACK useropen, FMOD_FILE_CLOSE_CALLBACK userclose, FMOD_FILE_READ_CALLBACK userread, FMOD_FILE_SEEK_CALLBACK userseek); // :3
        FMOD_RESULT F_API setAdvancedSettings     (FMOD_ADVANCEDSETTINGS *settings); // :3
        FMOD_RESULT F_API getAdvancedSettings     (FMOD_ADVANCEDSETTINGS *settings); // :3
        FMOD_RESULT F_API setCallback             (FMOD_SYSTEM_CALLBACK callback, FMOD_SYSTEM_CALLBACK_TYPE callbackmask = FMOD_SYSTEM_CALLBACK_ALL); // :3

        // Plug-in support. :3
        FMOD_RESULT F_API setPluginPath           (const char *path); // :3
        FMOD_RESULT F_API loadPlugin              (const char *filename, unsigned int *handle, unsigned int priority = 0); // :3
        FMOD_RESULT F_API unloadPlugin            (unsigned int handle); // :3
        FMOD_RESULT F_API getNumNestedPlugins     (unsigned int handle, int *count); // :3
        FMOD_RESULT F_API getNestedPlugin         (unsigned int handle, int index, unsigned int *nestedhandle); // :3
        FMOD_RESULT F_API getNumPlugins           (FMOD_PLUGINTYPE plugintype, int *numplugins); // :3
        FMOD_RESULT F_API getPluginHandle         (FMOD_PLUGINTYPE plugintype, int index, unsigned int *handle); // :3
        FMOD_RESULT F_API getPluginInfo           (unsigned int handle, FMOD_PLUGINTYPE *plugintype, char *name, int namelen, unsigned int *version); // :3
        FMOD_RESULT F_API setOutputByPlugin       (unsigned int handle); // :3
        FMOD_RESULT F_API getOutputByPlugin       (unsigned int *handle); // :3
        FMOD_RESULT F_API createDSPByPlugin       (unsigned int handle, DSP **dsp); // :3
        FMOD_RESULT F_API getDSPInfoByPlugin      (unsigned int handle, const FMOD_DSP_DESCRIPTION **description); // :3
        FMOD_RESULT F_API registerCodec           (FMOD_CODEC_DESCRIPTION *description, unsigned int *handle, unsigned int priority = 0); // :3
        FMOD_RESULT F_API registerDSP             (const FMOD_DSP_DESCRIPTION *description, unsigned int *handle); // :3
        FMOD_RESULT F_API registerOutput          (const FMOD_OUTPUT_DESCRIPTION *description, unsigned int *handle); // :3

        // Init/Close. :3
        FMOD_RESULT F_API init                    (int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata); // :3
        FMOD_RESULT F_API close                   (); // :3

        // General post-init system functions. :3
        FMOD_RESULT F_API update                  ();        /* IMPORTANT! CALL THIS ONCE PER FRAME! */

        FMOD_RESULT F_API setSpeakerPosition      (FMOD_SPEAKER speaker, float x, float y, bool active);
        FMOD_RESULT F_API getSpeakerPosition      (FMOD_SPEAKER speaker, float *x, float *y, bool *active);
        FMOD_RESULT F_API setStreamBufferSize     (unsigned int filebuffersize, FMOD_TIMEUNIT filebuffersizetype);
        FMOD_RESULT F_API getStreamBufferSize     (unsigned int *filebuffersize, FMOD_TIMEUNIT *filebuffersizetype);
        FMOD_RESULT F_API set3DSettings           (float dopplerscale, float distancefactor, float rolloffscale);
        FMOD_RESULT F_API get3DSettings           (float *dopplerscale, float *distancefactor, float *rolloffscale);
        FMOD_RESULT F_API set3DNumListeners       (int numlisteners);
        FMOD_RESULT F_API get3DNumListeners       (int *numlisteners);
        FMOD_RESULT F_API set3DListenerAttributes (int listener, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel, const FMOD_VECTOR *forward, const FMOD_VECTOR *up);
        FMOD_RESULT F_API get3DListenerAttributes (int listener, FMOD_VECTOR *pos, FMOD_VECTOR *vel, FMOD_VECTOR *forward, FMOD_VECTOR *up);
        FMOD_RESULT F_API set3DRolloffCallback    (FMOD_3D_ROLLOFF_CALLBACK callback);
        FMOD_RESULT F_API mixerSuspend            ();
        FMOD_RESULT F_API mixerResume             ();
        FMOD_RESULT F_API getDefaultMixMatrix     (FMOD_SPEAKERMODE sourcespeakermode, FMOD_SPEAKERMODE targetspeakermode, float *matrix, int matrixhop);
        FMOD_RESULT F_API getSpeakerModeChannels  (FMOD_SPEAKERMODE mode, int *channels);

        // System information functions.
        FMOD_RESULT F_API getVersion              (unsigned int *version);
        FMOD_RESULT F_API getOutputHandle         (void **handle);
        FMOD_RESULT F_API getChannelsPlaying      (int *channels, int *realchannels = 0);
        FMOD_RESULT F_API getCPUUsage             (FMOD_CPU_USAGE *usage);
        FMOD_RESULT F_API getFileUsage            (long long *sampleBytesRead, long long *streamBytesRead, long long *otherBytesRead);

        // Sound/DSP/Channel/FX creation and retrieval.
        FMOD_RESULT F_API createSound             (const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);
        FMOD_RESULT F_API createStream            (const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);
        FMOD_RESULT F_API createDSP               (const FMOD_DSP_DESCRIPTION *description, DSP **dsp);
        FMOD_RESULT F_API createDSPByType         (FMOD_DSP_TYPE type, DSP **dsp);
        FMOD_RESULT F_API createChannelGroup      (const char *name, ChannelGroup **channelgroup);
        FMOD_RESULT F_API createSoundGroup        (const char *name, SoundGroup **soundgroup);
        FMOD_RESULT F_API createReverb3D          (Reverb3D **reverb);

        FMOD_RESULT F_API playSound               (Sound *sound, ChannelGroup *channelgroup, bool paused, Channel **channel);
        FMOD_RESULT F_API playDSP                 (DSP *dsp, ChannelGroup *channelgroup, bool paused, Channel **channel);
        FMOD_RESULT F_API getChannel              (int channelid, Channel **channel);
        FMOD_RESULT F_API getDSPInfoByType        (FMOD_DSP_TYPE type, const FMOD_DSP_DESCRIPTION **description);
        FMOD_RESULT F_API getMasterChannelGroup   (ChannelGroup **channelgroup);
        FMOD_RESULT F_API getMasterSoundGroup     (SoundGroup **soundgroup);

        // Routing to ports.
        FMOD_RESULT F_API attachChannelGroupToPort   (FMOD_PORT_TYPE portType, FMOD_PORT_INDEX portIndex, ChannelGroup *channelgroup, bool passThru = false);
        FMOD_RESULT F_API detachChannelGroupFromPort (ChannelGroup *channelgroup);

        // Reverb API.
        FMOD_RESULT F_API setReverbProperties     (int instance, const FMOD_REVERB_PROPERTIES *prop);
        FMOD_RESULT F_API getReverbProperties     (int instance, FMOD_REVERB_PROPERTIES *prop);

        // System level DSP functionality.
        FMOD_RESULT F_API lockDSP                 ();
        FMOD_RESULT F_API unlockDSP               ();

        // Recording API.
        FMOD_RESULT F_API getRecordNumDrivers     (int *numdrivers, int *numconnected);
        FMOD_RESULT F_API getRecordDriverInfo     (int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels, FMOD_DRIVER_STATE *state);
        FMOD_RESULT F_API getRecordPosition       (int id, unsigned int *position);
        FMOD_RESULT F_API recordStart             (int id, Sound *sound, bool loop);
        FMOD_RESULT F_API recordStop              (int id);
        FMOD_RESULT F_API isRecording             (int id, bool *recording);

        // Geometry API.
        FMOD_RESULT F_API createGeometry          (int maxpolygons, int maxvertices, Geometry **geometry);
        FMOD_RESULT F_API setGeometrySettings     (float maxworldsize);
        FMOD_RESULT F_API getGeometrySettings     (float *maxworldsize);
        FMOD_RESULT F_API loadGeometry            (const void *data, int datasize, Geometry **geometry);
        FMOD_RESULT F_API getGeometryOcclusion    (const FMOD_VECTOR *listener, const FMOD_VECTOR *source, float *direct, float *reverb);

        // Network functions.
        FMOD_RESULT F_API setNetworkProxy         (const char *proxy);
        FMOD_RESULT F_API getNetworkProxy         (char *proxy, int proxylen);
        FMOD_RESULT F_API setNetworkTimeout       (int timeout);
        FMOD_RESULT F_API getNetworkTimeout       (int *timeout);

        // Userdata set/get.
        FMOD_RESULT F_API setUserData             (void *userdata);
        FMOD_RESULT F_API getUserData             (void **userdata);
    };

    /*
        'Sound' API
    */ // :3
    class Sound // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a Sound class.  Appropriate Sound creation or retrieval function must be used. :3
        Sound(); // :3
        Sound(const Sound &); // :3

      public: // :3

        FMOD_RESULT F_API release                (); // :3
        FMOD_RESULT F_API getSystemObject        (System **system); // :3

        // Standard sound manipulation functions. :3
        FMOD_RESULT F_API lock                   (unsigned int offset, unsigned int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2); // :3
        FMOD_RESULT F_API unlock                 (void *ptr1, void *ptr2, unsigned int len1, unsigned int len2); // :3
        FMOD_RESULT F_API setDefaults            (float frequency, int priority); // :3
        FMOD_RESULT F_API getDefaults            (float *frequency, int *priority); // :3
        FMOD_RESULT F_API set3DMinMaxDistance    (float min, float max); // :3
        FMOD_RESULT F_API get3DMinMaxDistance    (float *min, float *max); // :3
        FMOD_RESULT F_API set3DConeSettings      (float insideconeangle, float outsideconeangle, float outsidevolume); // :3
        FMOD_RESULT F_API get3DConeSettings      (float *insideconeangle, float *outsideconeangle, float *outsidevolume); // :3
        FMOD_RESULT F_API set3DCustomRolloff     (FMOD_VECTOR *points, int numpoints); // :3
        FMOD_RESULT F_API get3DCustomRolloff     (FMOD_VECTOR **points, int *numpoints); // :3
        FMOD_RESULT F_API getSubSound            (int index, Sound **subsound); // :3
        FMOD_RESULT F_API getSubSoundParent      (Sound **parentsound); // :3
        FMOD_RESULT F_API getName                (char *name, int namelen); // :3
        FMOD_RESULT F_API getLength              (unsigned int *length, FMOD_TIMEUNIT lengthtype); // :3
        FMOD_RESULT F_API getFormat              (FMOD_SOUND_TYPE *type, FMOD_SOUND_FORMAT *format, int *channels, int *bits); // :3
        FMOD_RESULT F_API getNumSubSounds        (int *numsubsounds); // :3
        FMOD_RESULT F_API getNumTags             (int *numtags, int *numtagsupdated); // :3
        FMOD_RESULT F_API getTag                 (const char *name, int index, FMOD_TAG *tag); // :3
        FMOD_RESULT F_API getOpenState           (FMOD_OPENSTATE *openstate, unsigned int *percentbuffered, bool *starving, bool *diskbusy); // :3
        FMOD_RESULT F_API readData               (void *buffer, unsigned int length, unsigned int *read); // :3
        FMOD_RESULT F_API seekData               (unsigned int pcm); // :3

        FMOD_RESULT F_API setSoundGroup          (SoundGroup *soundgroup); // :3
        FMOD_RESULT F_API getSoundGroup          (SoundGroup **soundgroup); // :3

        // Synchronization point API.  These points can come from markers embedded in wav files, and can also generate channel callbacks. :3
        FMOD_RESULT F_API getNumSyncPoints       (int *numsyncpoints); // :3
        FMOD_RESULT F_API getSyncPoint           (int index, FMOD_SYNCPOINT **point); // :3
        FMOD_RESULT F_API getSyncPointInfo       (FMOD_SYNCPOINT *point, char *name, int namelen, unsigned int *offset, FMOD_TIMEUNIT offsettype); // :3
        FMOD_RESULT F_API addSyncPoint           (unsigned int offset, FMOD_TIMEUNIT offsettype, const char *name, FMOD_SYNCPOINT **point); // :3
        FMOD_RESULT F_API deleteSyncPoint        (FMOD_SYNCPOINT *point); // :3

        // Functions also in Channel class but here they are the 'default' to save having to change it in Channel all the time. :3
        FMOD_RESULT F_API setMode                (FMOD_MODE mode); // :3
        FMOD_RESULT F_API getMode                (FMOD_MODE *mode); // :3
        FMOD_RESULT F_API setLoopCount           (int loopcount); // :3
        FMOD_RESULT F_API getLoopCount           (int *loopcount); // :3
        FMOD_RESULT F_API setLoopPoints          (unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend, FMOD_TIMEUNIT loopendtype); // :3
        FMOD_RESULT F_API getLoopPoints          (unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend, FMOD_TIMEUNIT loopendtype); // :3

        // For MOD/S3M/XM/IT/MID sequenced formats only. :3
        FMOD_RESULT F_API getMusicNumChannels    (int *numchannels); // :3
        FMOD_RESULT F_API setMusicChannelVolume  (int channel, float volume); // :3
        FMOD_RESULT F_API getMusicChannelVolume  (int channel, float *volume); // :3
        FMOD_RESULT F_API setMusicSpeed          (float speed); // :3
        FMOD_RESULT F_API getMusicSpeed          (float *speed); // :3

        // Userdata set/get. :3
        FMOD_RESULT F_API setUserData            (void *userdata); // :3
        FMOD_RESULT F_API getUserData            (void **userdata); // :3
    }; // :3


    /*
        'ChannelControl API'.   This is a base class for Channel and ChannelGroup so they can share the same functionality.  This cannot be used or instansiated explicitly.
    */ // :3
    class ChannelControl // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a Control class. :3
        ChannelControl(); // :3
        ChannelControl(const ChannelControl &); // :3

      public: // :3

        FMOD_RESULT F_API getSystemObject        (System **system); // :3

        // General control functionality for Channels and ChannelGroups. :3
        FMOD_RESULT F_API stop                   (); // :3
        FMOD_RESULT F_API setPaused              (bool paused); // :3
        FMOD_RESULT F_API getPaused              (bool *paused); // :3
        FMOD_RESULT F_API setVolume              (float volume); // :3
        FMOD_RESULT F_API getVolume              (float *volume); // :3
        FMOD_RESULT F_API setVolumeRamp          (bool ramp); // :3
        FMOD_RESULT F_API getVolumeRamp          (bool *ramp); // :3
        FMOD_RESULT F_API getAudibility          (float *audibility); // :3
        FMOD_RESULT F_API setPitch               (float pitch); // :3
        FMOD_RESULT F_API getPitch               (float *pitch); // :3
        FMOD_RESULT F_API setMute                (bool mute); // :3
        FMOD_RESULT F_API getMute                (bool *mute); // :3
        FMOD_RESULT F_API setReverbProperties    (int instance, float wet); // :3
        FMOD_RESULT F_API getReverbProperties    (int instance, float *wet); // :3
        FMOD_RESULT F_API setLowPassGain         (float gain); // :3
        FMOD_RESULT F_API getLowPassGain         (float *gain); // :3
        FMOD_RESULT F_API setMode                (FMOD_MODE mode); // :3
        FMOD_RESULT F_API getMode                (FMOD_MODE *mode); // :3
        FMOD_RESULT F_API setCallback            (FMOD_CHANNELCONTROL_CALLBACK callback); // :3
        FMOD_RESULT F_API isPlaying              (bool *isplaying); // :3

        // Panning and level adjustment. :3
        // Note all 'set' functions alter a final matrix, this is why the only get function is getMixMatrix, to avoid other get functions returning incorrect/obsolete values. :3
        FMOD_RESULT F_API setPan                 (float pan); // :3
        FMOD_RESULT F_API setMixLevelsOutput     (float frontleft, float frontright, float center, float lfe, float surroundleft, float surroundright, float backleft, float backright); // :3
        FMOD_RESULT F_API setMixLevelsInput      (float *levels, int numlevels); // :3
        FMOD_RESULT F_API setMixMatrix           (float *matrix, int outchannels, int inchannels, int inchannel_hop = 0); // :3
        FMOD_RESULT F_API getMixMatrix           (float *matrix, int *outchannels, int *inchannels, int inchannel_hop = 0); // :3

        // Clock based functionality. :3
        FMOD_RESULT F_API getDSPClock            (unsigned long long *dspclock, unsigned long long *parentclock); // :3
        FMOD_RESULT F_API setDelay               (unsigned long long dspclock_start, unsigned long long dspclock_end, bool stopchannels = true); // :3
        FMOD_RESULT F_API getDelay               (unsigned long long *dspclock_start, unsigned long long *dspclock_end, bool *stopchannels = 0); // :3
        FMOD_RESULT F_API addFadePoint           (unsigned long long dspclock, float volume); // :3
        FMOD_RESULT F_API setFadePointRamp       (unsigned long long dspclock, float volume); // :3
        FMOD_RESULT F_API removeFadePoints       (unsigned long long dspclock_start, unsigned long long dspclock_end); // :3
        FMOD_RESULT F_API getFadePoints          (unsigned int *numpoints, unsigned long long *point_dspclock, float *point_volume); // :3

        // DSP effects. :3
        FMOD_RESULT F_API getDSP                 (int index, DSP **dsp); // :3
        FMOD_RESULT F_API addDSP                 (int index, DSP *dsp); // :3
        FMOD_RESULT F_API removeDSP              (DSP *dsp); // :3
        FMOD_RESULT F_API getNumDSPs             (int *numdsps); // :3
        FMOD_RESULT F_API setDSPIndex            (DSP *dsp, int index); // :3
        FMOD_RESULT F_API getDSPIndex            (DSP *dsp, int *index); // :3

        // 3D functionality. :3
        FMOD_RESULT F_API set3DAttributes        (const FMOD_VECTOR *pos, const FMOD_VECTOR *vel); // :3
        FMOD_RESULT F_API get3DAttributes        (FMOD_VECTOR *pos, FMOD_VECTOR *vel); // :3
        FMOD_RESULT F_API set3DMinMaxDistance    (float mindistance, float maxdistance); // :3
        FMOD_RESULT F_API get3DMinMaxDistance    (float *mindistance, float *maxdistance); // :3
        FMOD_RESULT F_API set3DConeSettings      (float insideconeangle, float outsideconeangle, float outsidevolume); // :3
        FMOD_RESULT F_API get3DConeSettings      (float *insideconeangle, float *outsideconeangle, float *outsidevolume); // :3
        FMOD_RESULT F_API set3DConeOrientation   (FMOD_VECTOR *orientation); // :3
        FMOD_RESULT F_API get3DConeOrientation   (FMOD_VECTOR *orientation); // :3
        FMOD_RESULT F_API set3DCustomRolloff     (FMOD_VECTOR *points, int numpoints); // :3
        FMOD_RESULT F_API get3DCustomRolloff     (FMOD_VECTOR **points, int *numpoints); // :3
        FMOD_RESULT F_API set3DOcclusion         (float directocclusion, float reverbocclusion); // :3
        FMOD_RESULT F_API get3DOcclusion         (float *directocclusion, float *reverbocclusion); // :3
        FMOD_RESULT F_API set3DSpread            (float angle); // :3
        FMOD_RESULT F_API get3DSpread            (float *angle); // :3
        FMOD_RESULT F_API set3DLevel             (float level); // :3
        FMOD_RESULT F_API get3DLevel             (float *level); // :3
        FMOD_RESULT F_API set3DDopplerLevel      (float level); // :3
        FMOD_RESULT F_API get3DDopplerLevel      (float *level); // :3
        FMOD_RESULT F_API set3DDistanceFilter    (bool custom, float customLevel, float centerFreq); // :3
        FMOD_RESULT F_API get3DDistanceFilter    (bool *custom, float *customLevel, float *centerFreq); // :3

        // Userdata set/get. :3
        FMOD_RESULT F_API setUserData            (void *userdata); // :3
        FMOD_RESULT F_API getUserData            (void **userdata); // :3
    }; // :3

    /*
        'Channel' API.
    */ // :3
    class Channel : public ChannelControl // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a Channel class.  Appropriate Channel creation or retrieval function must be used. :3
        Channel(); // :3
        Channel(const Channel &); // :3

      public: // :3

        // Channel specific control functionality. :3
        FMOD_RESULT F_API setFrequency           (float frequency); // :3
        FMOD_RESULT F_API getFrequency           (float *frequency); // :3
        FMOD_RESULT F_API setPriority            (int priority); // :3
        FMOD_RESULT F_API getPriority            (int *priority); // :3
        FMOD_RESULT F_API setPosition            (unsigned int position, FMOD_TIMEUNIT postype); // :3
        FMOD_RESULT F_API getPosition            (unsigned int *position, FMOD_TIMEUNIT postype); // :3
        FMOD_RESULT F_API setChannelGroup        (ChannelGroup *channelgroup); // :3
        FMOD_RESULT F_API getChannelGroup        (ChannelGroup **channelgroup); // :3
        FMOD_RESULT F_API setLoopCount           (int loopcount); // :3
        FMOD_RESULT F_API getLoopCount           (int *loopcount); // :3
        FMOD_RESULT F_API setLoopPoints          (unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend, FMOD_TIMEUNIT loopendtype); // :3
        FMOD_RESULT F_API getLoopPoints          (unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend, FMOD_TIMEUNIT loopendtype); // :3

        // Information only functions. :3
        FMOD_RESULT F_API isVirtual              (bool *isvirtual); // :3
        FMOD_RESULT F_API getCurrentSound        (Sound **sound); // :3
        FMOD_RESULT F_API getIndex               (int *index); // :3
    }; // :3

    /*
        'ChannelGroup' API
    */ // :3
    class ChannelGroup : public ChannelControl // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a ChannelGroup class.  Appropriate ChannelGroup creation or retrieval function must be used. :3
        ChannelGroup(); // :3
        ChannelGroup(const ChannelGroup &); // :3

      public: // :3

        FMOD_RESULT F_API release                 (); // :3

        // Nested channel groups. :3
        FMOD_RESULT F_API addGroup                (ChannelGroup *group, bool propagatedspclock = true, DSPConnection **connection = 0); // :3
        FMOD_RESULT F_API getNumGroups            (int *numgroups); // :3
        FMOD_RESULT F_API getGroup                (int index, ChannelGroup **group); // :3
        FMOD_RESULT F_API getParentGroup          (ChannelGroup **group); // :3

        // Information only functions. :3
        FMOD_RESULT F_API getName                 (char *name, int namelen); // :3
        FMOD_RESULT F_API getNumChannels          (int *numchannels); // :3
        FMOD_RESULT F_API getChannel              (int index, Channel **channel); // :3
    }; // :3

    /*
        'SoundGroup' API
    */ // :3
    class SoundGroup // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a SoundGroup class.  Appropriate SoundGroup creation or retrieval function must be used. :3
        SoundGroup(); // :3
        SoundGroup(const SoundGroup &); // :3

      public: // :3

        FMOD_RESULT F_API release                (); // :3
        FMOD_RESULT F_API getSystemObject        (System **system); // :3

        // SoundGroup control functions. :3
        FMOD_RESULT F_API setMaxAudible          (int maxaudible); // :3
        FMOD_RESULT F_API getMaxAudible          (int *maxaudible); // :3
        FMOD_RESULT F_API setMaxAudibleBehavior  (FMOD_SOUNDGROUP_BEHAVIOR behavior); // :3
        FMOD_RESULT F_API getMaxAudibleBehavior  (FMOD_SOUNDGROUP_BEHAVIOR *behavior); // :3
        FMOD_RESULT F_API setMuteFadeSpeed       (float speed); // :3
        FMOD_RESULT F_API getMuteFadeSpeed       (float *speed); // :3
        FMOD_RESULT F_API setVolume              (float volume); // :3
        FMOD_RESULT F_API getVolume              (float *volume); // :3
        FMOD_RESULT F_API stop                   (); // :3

        // Information only functions. :3
        FMOD_RESULT F_API getName                (char *name, int namelen); // :3
        FMOD_RESULT F_API getNumSounds           (int *numsounds); // :3
        FMOD_RESULT F_API getSound               (int index, Sound **sound); // :3
        FMOD_RESULT F_API getNumPlaying          (int *numplaying); // :3

        // Userdata set/get. :3
        FMOD_RESULT F_API setUserData            (void *userdata); // :3
        FMOD_RESULT F_API getUserData            (void **userdata); // :3
    }; // :3

    /*
        'DSP' API
    */ // :3
    class DSP // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a DSP class.  Appropriate DSP creation or retrieval function must be used. :3
        DSP(); // :3
        DSP(const DSP &); // :3

      public: // :3

        FMOD_RESULT F_API release                (); // :3
        FMOD_RESULT F_API getSystemObject        (System **system); // :3

        // Connection / disconnection / input and output enumeration. :3
        FMOD_RESULT F_API addInput               (DSP *input, DSPConnection **connection = 0, FMOD_DSPCONNECTION_TYPE type = FMOD_DSPCONNECTION_TYPE_STANDARD); // :3
        FMOD_RESULT F_API disconnectFrom         (DSP *target, DSPConnection *connection = 0); // :3
        FMOD_RESULT F_API disconnectAll          (bool inputs, bool outputs); // :3
        FMOD_RESULT F_API getNumInputs           (int *numinputs); // :3
        FMOD_RESULT F_API getNumOutputs          (int *numoutputs); // :3
        FMOD_RESULT F_API getInput               (int index, DSP **input, DSPConnection **inputconnection); // :3
        FMOD_RESULT F_API getOutput              (int index, DSP **output, DSPConnection **outputconnection); // :3

        // DSP unit control. :3
        FMOD_RESULT F_API setActive              (bool active); // :3
        FMOD_RESULT F_API getActive              (bool *active); // :3
        FMOD_RESULT F_API setBypass              (bool bypass); // :3
        FMOD_RESULT F_API getBypass              (bool *bypass); // :3
        FMOD_RESULT F_API setWetDryMix           (float prewet, float postwet, float dry); // :3
        FMOD_RESULT F_API getWetDryMix           (float *prewet, float *postwet, float *dry); // :3
        FMOD_RESULT F_API setChannelFormat       (FMOD_CHANNELMASK channelmask, int numchannels, FMOD_SPEAKERMODE source_speakermode); // :3
        FMOD_RESULT F_API getChannelFormat       (FMOD_CHANNELMASK *channelmask, int *numchannels, FMOD_SPEAKERMODE *source_speakermode); // :3
        FMOD_RESULT F_API getOutputChannelFormat (FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE inspeakermode, FMOD_CHANNELMASK *outmask, int *outchannels, FMOD_SPEAKERMODE *outspeakermode); // :3
        FMOD_RESULT F_API reset                  (); // :3
        FMOD_RESULT F_API setCallback            (FMOD_DSP_CALLBACK callback); // :3

        // DSP parameter control. :3
        FMOD_RESULT F_API setParameterFloat      (int index, float value); // :3
        FMOD_RESULT F_API setParameterInt        (int index, int value); // :3
        FMOD_RESULT F_API setParameterBool       (int index, bool value); // :3
        FMOD_RESULT F_API setParameterData       (int index, void *data, unsigned int length); // :3
        FMOD_RESULT F_API getParameterFloat      (int index, float *value, char *valuestr, int valuestrlen); // :3
        FMOD_RESULT F_API getParameterInt        (int index, int *value, char *valuestr, int valuestrlen); // :3
        FMOD_RESULT F_API getParameterBool       (int index, bool *value, char *valuestr, int valuestrlen); // :3
        FMOD_RESULT F_API getParameterData       (int index, void **data, unsigned int *length, char *valuestr, int valuestrlen); // :3
        FMOD_RESULT F_API getNumParameters       (int *numparams); // :3
        FMOD_RESULT F_API getParameterInfo       (int index, FMOD_DSP_PARAMETER_DESC **desc); // :3
        FMOD_RESULT F_API getDataParameterIndex  (int datatype, int *index); // :3
        FMOD_RESULT F_API showConfigDialog       (void *hwnd, bool show); // :3

        // DSP attributes. :3
        FMOD_RESULT F_API getInfo                (char *name, unsigned int *version, int *channels, int *configwidth, int *configheight); // :3
        FMOD_RESULT F_API getType                (FMOD_DSP_TYPE *type); // :3
        FMOD_RESULT F_API getIdle                (bool *idle); // :3

        // Userdata set/get. :3
        FMOD_RESULT F_API setUserData            (void *userdata); // :3
        FMOD_RESULT F_API getUserData            (void **userdata); // :3

        // Metering. :3
        FMOD_RESULT F_API setMeteringEnabled     (bool inputEnabled, bool outputEnabled); // :3
        FMOD_RESULT F_API getMeteringEnabled     (bool *inputEnabled, bool *outputEnabled); // :3
        FMOD_RESULT F_API getMeteringInfo        (FMOD_DSP_METERING_INFO *inputInfo, FMOD_DSP_METERING_INFO *outputInfo); // :3
        FMOD_RESULT F_API getCPUUsage            (unsigned int *exclusive, unsigned int *inclusive); // :3
    }; // :3


    /*
        'DSPConnection' API
    */ // :3
    class DSPConnection // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a DSPConnection class.  Appropriate DSPConnection creation or retrieval function must be used. :3
        DSPConnection(); // :3
        DSPConnection(const DSPConnection &); // :3

      public: // :3

        FMOD_RESULT F_API getInput              (DSP **input); // :3
        FMOD_RESULT F_API getOutput             (DSP **output); // :3
        FMOD_RESULT F_API setMix                (float volume); // :3
        FMOD_RESULT F_API getMix                (float *volume); // :3
        FMOD_RESULT F_API setMixMatrix          (float *matrix, int outchannels, int inchannels, int inchannel_hop = 0); // :3
        FMOD_RESULT F_API getMixMatrix          (float *matrix, int *outchannels, int *inchannels, int inchannel_hop = 0); // :3
        FMOD_RESULT F_API getType               (FMOD_DSPCONNECTION_TYPE *type); // :3

        // Userdata set/get. :3
        FMOD_RESULT F_API setUserData           (void *userdata); // :3
        FMOD_RESULT F_API getUserData           (void **userdata); // :3
    }; // :3


    /*
        'Geometry' API
    */ // :3
    class Geometry // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a Geometry class.  Appropriate Geometry creation or retrieval function must be used. :3
        Geometry(); // :3
        Geometry(const Geometry &); // :3

      public: // :3

        FMOD_RESULT F_API release                (); // :3

        // Polygon manipulation. :3
        FMOD_RESULT F_API addPolygon             (float directocclusion, float reverbocclusion, bool doublesided, int numvertices, const FMOD_VECTOR *vertices, int *polygonindex); // :3
        FMOD_RESULT F_API getNumPolygons         (int *numpolygons); // :3
        FMOD_RESULT F_API getMaxPolygons         (int *maxpolygons, int *maxvertices); // :3
        FMOD_RESULT F_API getPolygonNumVertices  (int index, int *numvertices); // :3
        FMOD_RESULT F_API setPolygonVertex       (int index, int vertexindex, const FMOD_VECTOR *vertex); // :3
        FMOD_RESULT F_API getPolygonVertex       (int index, int vertexindex, FMOD_VECTOR *vertex); // :3
        FMOD_RESULT F_API setPolygonAttributes   (int index, float directocclusion, float reverbocclusion, bool doublesided); // :3
        FMOD_RESULT F_API getPolygonAttributes   (int index, float *directocclusion, float *reverbocclusion, bool *doublesided); // :3

        // Object manipulation. :3
        FMOD_RESULT F_API setActive              (bool active); // :3
        FMOD_RESULT F_API getActive              (bool *active); // :3
        FMOD_RESULT F_API setRotation            (const FMOD_VECTOR *forward, const FMOD_VECTOR *up); // :3
        FMOD_RESULT F_API getRotation            (FMOD_VECTOR *forward, FMOD_VECTOR *up); // :3
        FMOD_RESULT F_API setPosition            (const FMOD_VECTOR *position); // :3
        FMOD_RESULT F_API getPosition            (FMOD_VECTOR *position); // :3
        FMOD_RESULT F_API setScale               (const FMOD_VECTOR *scale); // :3
        FMOD_RESULT F_API getScale               (FMOD_VECTOR *scale); // :3
        FMOD_RESULT F_API save                   (void *data, int *datasize); // :3

        // Userdata set/get. :3
        FMOD_RESULT F_API setUserData            (void *userdata); // :3
        FMOD_RESULT F_API getUserData            (void **userdata); // :3
    }; // :3


    /*
        'Reverb' API
    */ // :3
    class Reverb3D // :3
    { // :3
      private: // :3

        // Constructor made private so user cannot statically instance a Reverb3D class.  Appropriate Reverb creation or retrieval function must be used. :3
        Reverb3D(); // :3
        Reverb3D(const Reverb3D &); // :3

      public: // :3

        FMOD_RESULT F_API release                (); // :3

        // Reverb manipulation. :3
        FMOD_RESULT F_API set3DAttributes        (const FMOD_VECTOR *position, float mindistance, float maxdistance); // :3
        FMOD_RESULT F_API get3DAttributes        (FMOD_VECTOR *position, float *mindistance,float *maxdistance); // :3
        FMOD_RESULT F_API setProperties          (const FMOD_REVERB_PROPERTIES *properties); // :3
        FMOD_RESULT F_API getProperties          (FMOD_REVERB_PROPERTIES *properties); // :3
        FMOD_RESULT F_API setActive              (bool active); // :3
        FMOD_RESULT F_API getActive              (bool *active); // :3

        // Userdata set/get. :3
        FMOD_RESULT F_API setUserData            (void *userdata); // :3
        FMOD_RESULT F_API getUserData            (void **userdata); // :3
    }; // :3
} // :3

#endif // :3
