#include <stdint.h>
#include "wrapper/wrapper.h"
#include "wrapper/verbose.h"

// https://developer.android.com/ndk/guides/audio/opensl/opensl-for-android
// https://www.khronos.org/registry/OpenSL-ES/specs/OpenSL_ES_Specification_1.1.pdf

typedef uint8_t SLuint8;
typedef uint16_t SLuint16;
typedef uint32_t SLuint32;
typedef SLuint8 SLchar;
typedef SLuint32 SLboolean;
typedef SLuint32 SLresult;

#define SL_BOOLEAN_FALSE            ((SLboolean) 0x00000000)
#define SL_BOOLEAN_TRUE             ((SLboolean) 0x00000001)

#define SL_RESULT_SUCCESS			((SLuint32) 0x00000000)
#define SL_RESULT_PRECONDITIONS_VIOLATED	((SLuint32) 0x00000001)
#define SL_RESULT_PARAMETER_INVALID		((SLuint32) 0x00000002)
#define SL_RESULT_MEMORY_FAILURE		((SLuint32) 0x00000003)
#define SL_RESULT_RESOURCE_ERROR		((SLuint32) 0x00000004)
#define SL_RESULT_RESOURCE_LOST			((SLuint32) 0x00000005)
#define SL_RESULT_IO_ERROR			((SLuint32) 0x00000006)
#define SL_RESULT_BUFFER_INSUFFICIENT		((SLuint32) 0x00000007)
#define SL_RESULT_CONTENT_CORRUPTED		((SLuint32) 0x00000008)
#define SL_RESULT_CONTENT_UNSUPPORTED		((SLuint32) 0x00000009)
#define SL_RESULT_CONTENT_NOT_FOUND		((SLuint32) 0x0000000A)
#define SL_RESULT_PERMISSION_DENIED		((SLuint32) 0x0000000B)
#define SL_RESULT_FEATURE_UNSUPPORTED		((SLuint32) 0x0000000C)
#define SL_RESULT_INTERNAL_ERROR		((SLuint32) 0x0000000D)
#define SL_RESULT_UNKNOWN_ERROR			((SLuint32) 0x0000000E)
#define SL_RESULT_OPERATION_ABORTED		((SLuint32) 0x0000000F)
#define SL_RESULT_CONTROL_LOST			((SLuint32) 0x00000010)

#define SL_OBJECT_STATE_UNREALIZED	((SLuint32) 0x00000001)
#define SL_OBJECT_STATE_REALIZED	((SLuint32) 0x00000002)
#define SL_OBJECT_STATE_SUSPENDED	((SLuint32) 0x00000003)

#define SL_DEFAULTDEVICEID_LED          ((SLuint32) 0xFFFFFFFD)
#define SL_DEFAULTDEVICEID_VIBRA        ((SLuint32) 0xFFFFFFFC)

struct SLInterfaceID {
   SLuint32 time_low;
   SLuint16 time_mid;
   SLuint16 time_hi_and_version;
   SLuint16 clock_seq;
   uint8_t node[6];
};

struct SLInterfaceID *SL_IID_BUFFERQUEUE;
struct SLInterfaceID *SL_IID_ENGINE;
struct SLInterfaceID *SL_IID_PLAY;
struct SLInterfaceID *SL_IID_VOLUME;

struct SLObjectItf {
   SLresult (*Realize)(struct SLObjectItf**, SLboolean);
   SLresult (*Resume)(struct SLObjectItf**, SLboolean);
   SLresult (*GetState)(struct SLObjectItf**, SLuint32*);
   SLresult (*GetInterface)(struct SLObjectItf**, const struct SLInterfaceID*, struct SLObjectItf***);
   SLresult (*RegisterCallback)(struct SLObjectItf**, void*, void*);
   SLresult (*AbortAsyncOperation)(struct SLObjectItf**);
   SLresult (*Destroy)(struct SLObjectItf**);
   SLresult (*SetPriority)(struct SLObjectItf**, SLuint32, SLboolean);
   SLresult (*GetPriority)(struct SLObjectItf**, SLuint32*, SLboolean*);
   SLresult (*SetLossOfControlInterfaces)(struct SLObjectItf**, SLuint16, const struct SLInterfaceID**, SLboolean);
};

static struct SLObjectItf**
sl_object(void);

struct SLEngineItf {
   SLresult (*CreateLEDDevice)(struct SLEngineItf**, struct SLObjectItf***, SLuint32, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*CreateVibraDevice)(struct SLEngineItf**, struct SLObjectItf***, SLuint32, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*CreateAudioPlayer)(struct SLEngineItf**, struct SLObjectItf***, void*, void*, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*CreateAudioRecorder)(struct SLEngineItf**, struct SLObjectItf***, void*, void*, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*CreateMidiPlayer)(struct SLEngineItf**, struct SLObjectItf***, void*, void*, void*, void*, void*, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*CreateListener)(struct SLEngineItf**, struct SLObjectItf***, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*Create3DGroup)(struct SLEngineItf**, struct SLObjectItf***, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*CreateOutputMix)(struct SLEngineItf**, struct SLObjectItf***, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*CreateMetadataExtractor)(struct SLEngineItf**, struct SLObjectItf***, void*, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*CreateExtensionObject)(struct SLEngineItf**, struct SLObjectItf***, void*, SLuint32, SLuint32, const struct SLInterfaceID**, const SLboolean*);
   SLresult (*QueryNumSupportedInterfaces)(struct SLEngineItf**, SLuint32, SLuint32*);
   SLresult (*QuerySupportedInterfaces)(struct SLEngineItf**, SLuint32, SLuint32, const struct SLInterfaceID**);
   SLresult (*QueryNumSupportedExtensions)(struct SLEngineItf**, SLuint32, SLuint32*);
   SLresult (*QuerySupportedExtensions)(struct SLEngineItf**, SLuint32, const char*, SLuint16*);
   SLresult (*IsExtensionSupported)(struct SLEngineItf**, const char*, SLboolean*);
};

static SLresult
sl_engine_create_led_device(struct SLEngineItf **engine, struct SLObjectItf ***device, SLuint32 device_id, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %u, %u %p, %p", (void*)engine, (void*)device, device_id, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device || device_id != SL_DEFAULTDEVICEID_LED)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_vibra_device(struct SLEngineItf **engine, struct SLObjectItf ***device, SLuint32 device_id, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %u, %u %p, %p", (void*)engine, (void*)device, device_id, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device || device_id != SL_DEFAULTDEVICEID_VIBRA)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_audio_player(struct SLEngineItf **engine, struct SLObjectItf ***device, void *src, void *snk, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %p, %p, %u %p, %p", (void*)engine, (void*)device, src, snk, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_audio_recorder(struct SLEngineItf **engine, struct SLObjectItf ***device, void *src, void *snk, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %p, %p, %u %p, %p", (void*)engine, (void*)device, src, snk, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_midi_player(struct SLEngineItf **engine, struct SLObjectItf ***device, void *midi_src, void *bank_src, void *audio_out, void *vibra, void *led_array, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %p, %p, %p, %p, %p, %u %p, %p", (void*)engine, (void*)device, midi_src, bank_src, audio_out, vibra, led_array, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_listener(struct SLEngineItf **engine, struct SLObjectItf ***device, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %u, %p, %p", (void*)engine, (void*)device, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_3d_group(struct SLEngineItf **engine, struct SLObjectItf ***device, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %u, %p, %p", (void*)engine, (void*)device, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_output_mix(struct SLEngineItf **engine, struct SLObjectItf ***device, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %u, %p, %p", (void*)engine, (void*)device, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_metadata_extractor(struct SLEngineItf **engine, struct SLObjectItf ***device, void *source, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %p, %u, %p, %p", (void*)engine, (void*)device, source, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device || !source)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_create_extension_object(struct SLEngineItf **engine, struct SLObjectItf ***device, void *params, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %p, %p, %u, %p, %p", (void*)engine, (void*)device, params, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!device || !params)
      return SL_RESULT_PARAMETER_INVALID;

   *device = sl_object();
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_query_num_supported_interfaces(struct SLEngineItf **engine, SLuint32 *out_num_supported)
{
   verbose("%p, %p", (void*)engine, (void*)out_num_supported);

   if (!out_num_supported)
      return SL_RESULT_PARAMETER_INVALID;

   *out_num_supported = 0;
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_query_supported_interfaces(struct SLEngineItf **engine, SLuint32 index, const SLchar *name, SLuint16 *out_name_len)
{
   verbose("%p, %u, %s, %p", (void*)engine, index, name, (void*)out_name_len);

   if (!name || !out_name_len)
      return SL_RESULT_PARAMETER_INVALID;

   *out_name_len = 0;
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_query_num_supported_extensions(struct SLEngineItf **engine, SLuint32 *out_num_supported)
{
   verbose("%p, %p", (void*)engine, (void*)out_num_supported);

   if (!out_num_supported)
      return SL_RESULT_PARAMETER_INVALID;

   *out_num_supported = 0;
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_query_supported_extensions(struct SLEngineItf **engine, SLuint32 index, const SLchar *name, SLuint16 *out_name_len)
{
   verbose("%p, %u, %s, %p", (void*)engine, index, name, (void*)out_name_len);

   if (!name || !out_name_len)
      return SL_RESULT_PARAMETER_INVALID;

   *out_name_len = 0;
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_engine_is_extension_supported(struct SLEngineItf **engine, const SLchar *name, SLboolean *out_supported)
{
   verbose("%p, %s, %p", (void*)engine, name, (void*)out_supported);

   if (!name || !out_supported)
      return SL_RESULT_PARAMETER_INVALID;

   *out_supported = SL_BOOLEAN_FALSE;
   return SL_RESULT_SUCCESS;
}

static struct SLEngineItf**
sl_engine(void)
{
   static struct SLEngineItf itf, *itfp;
   if (!itfp) {
#define WRAP(x) wrapper_create(#x, x)
      itf.CreateLEDDevice = WRAP(sl_engine_create_led_device);
      itf.CreateVibraDevice = WRAP(sl_engine_create_vibra_device);
      itf.CreateAudioPlayer = WRAP(sl_engine_create_audio_player);
      itf.CreateAudioRecorder = WRAP(sl_engine_create_audio_recorder);
      itf.CreateMidiPlayer = WRAP(sl_engine_create_midi_player);
      itf.CreateListener = WRAP(sl_engine_create_listener);
      itf.Create3DGroup = WRAP(sl_engine_create_3d_group);
      itf.CreateOutputMix = WRAP(sl_engine_create_output_mix);
      itf.CreateMetadataExtractor = WRAP(sl_engine_create_metadata_extractor);
      itf.CreateExtensionObject = WRAP(sl_engine_create_extension_object);
      itf.QueryNumSupportedInterfaces = WRAP(sl_engine_query_num_supported_interfaces);
      itf.QuerySupportedInterfaces = WRAP(sl_engine_query_supported_interfaces);
      itf.QueryNumSupportedExtensions = WRAP(sl_engine_query_num_supported_extensions);
      itf.QuerySupportedExtensions = WRAP(sl_engine_query_supported_extensions);
      itf.IsExtensionSupported = WRAP(sl_engine_is_extension_supported);
#undef WRAP
      itfp = &itf;
   }
   return &itfp;
}

static SLresult
sl_object_realize(struct SLObjectItf **object, SLboolean async)
{
   verbose("%p, %u", (void*)object, async);
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_resume(struct SLObjectItf **object, SLboolean async)
{
   verbose("%p, %u", (void*)object, async);
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_get_state(struct SLObjectItf **object, SLuint32 *out_state)
{
   verbose("%p, %p", (void*)object, (void*)out_state);

   if (!out_state)
      return SL_RESULT_PARAMETER_INVALID;

   *out_state = SL_OBJECT_STATE_REALIZED;
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_get_interface(struct SLObjectItf **object, const struct SLInterfaceID *iid, struct SLObjectItf ***out_interface)
{
   verbose("%p, %p", (void*)object, (void*)out_interface);

   if (iid == SL_IID_ENGINE) {
      *out_interface = (struct SLObjectItf**)sl_engine();
   } else {
      return SL_RESULT_PARAMETER_INVALID;
   }

   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_register_callback(struct SLObjectItf **object, void *cb, void *ctx)
{
   verbose("%p, %p, %p", (void*)object, cb, ctx);
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_abort_async_operation(struct SLObjectItf **object)
{
   verbose("%p", (void*)object);
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_destroy(struct SLObjectItf **object)
{
   verbose("%p", (void*)object);
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_set_priority(struct SLObjectItf **object, SLuint32 priority, SLboolean preemptable)
{
   verbose("%p, %u, %u", (void*)object, priority, preemptable);
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_get_priority(struct SLObjectItf **object, SLuint32 *out_priority, SLboolean *out_preemptable)
{
   verbose("%p, %p, %p", (void*)object, (void*)out_priority, (void*)out_preemptable);

   if (!out_priority || !out_preemptable)
      return SL_RESULT_PARAMETER_INVALID;

   *out_priority = 0;
   *out_preemptable = SL_BOOLEAN_FALSE;
   return SL_RESULT_SUCCESS;
}

static SLresult
sl_object_set_loss_of_control_interfaces(struct SLObjectItf **object, SLuint16 num_interfaces, struct SLInterfaceID **interface_ids, SLboolean enabled)
{
   verbose("%p, %u, %p, %u", (void*)object, num_interfaces, (void*)interface_ids, enabled);
   return SL_RESULT_SUCCESS;
}

static struct SLObjectItf**
sl_object(void)
{
   static struct SLObjectItf itf, *itfp;
   if (!itfp) {
#define WRAP(x) wrapper_create(#x, x)
      itf.Realize = WRAP(sl_object_realize);
      itf.Resume = WRAP(sl_object_resume);
      itf.GetState = WRAP(sl_object_get_state);
      itf.GetInterface = WRAP(sl_object_get_interface);
      itf.RegisterCallback = WRAP(sl_object_register_callback);
      itf.AbortAsyncOperation = WRAP(sl_object_abort_async_operation);
      itf.Destroy = WRAP(sl_object_destroy);
      itf.SetPriority = WRAP(sl_object_set_priority);
      itf.GetPriority = WRAP(sl_object_get_priority);
      itf.SetLossOfControlInterfaces = WRAP(sl_object_set_loss_of_control_interfaces);
#undef WRAP
      itfp = &itf;
   }
   return &itfp;
}

SLresult
slCreateEngine(struct SLObjectItf ***engine, SLuint32 num_options, void *options, SLuint32 num_interfaces, const struct SLInterfaceID **interface_ids, const SLboolean *interface_required)
{
   verbose("%p, %u, %p, %u, %p, %p", (void*)engine, num_options, options, num_interfaces, (void*)interface_ids, (void*)interface_required);

   if (!engine)
      return SL_RESULT_PARAMETER_INVALID;

   *engine = sl_object();
   return SL_RESULT_SUCCESS;
}
