/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: wifi_constants.proto */

#ifndef PROTOBUF_C_wifi_5fconstants_2eproto__INCLUDED
#define PROTOBUF_C_wifi_5fconstants_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1004000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct WifiConnectedState WifiConnectedState;


/* --- enums --- */

typedef enum _WifiStationState {
    WIFI_STATION_STATE__Connected = 0,
    WIFI_STATION_STATE__Connecting = 1,
    WIFI_STATION_STATE__Disconnected = 2,
    WIFI_STATION_STATE__ConnectionFailed = 3
                                           PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(WIFI_STATION_STATE)
} WifiStationState;
typedef enum _WifiConnectFailedReason {
    WIFI_CONNECT_FAILED_REASON__AuthError = 0,
    WIFI_CONNECT_FAILED_REASON__NetworkNotFound = 1
            PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(WIFI_CONNECT_FAILED_REASON)
} WifiConnectFailedReason;
typedef enum _WifiAuthMode {
    WIFI_AUTH_MODE__Open = 0,
    WIFI_AUTH_MODE__WEP = 1,
    WIFI_AUTH_MODE__WPA_PSK = 2,
    WIFI_AUTH_MODE__WPA2_PSK = 3,
    WIFI_AUTH_MODE__WPA_WPA2_PSK = 4,
    WIFI_AUTH_MODE__WPA2_ENTERPRISE = 5,
    WIFI_AUTH_MODE__WPA3_PSK = 6,
    WIFI_AUTH_MODE__WPA2_WPA3_PSK = 7
                                    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(WIFI_AUTH_MODE)
} WifiAuthMode;

/* --- messages --- */

struct  WifiConnectedState {
    ProtobufCMessage base;
    char *ip4_addr;
    WifiAuthMode auth_mode;
    ProtobufCBinaryData ssid;
    ProtobufCBinaryData bssid;
    int32_t channel;
};
#define WIFI_CONNECTED_STATE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&wifi_connected_state__descriptor) \
    , (char *)protobuf_c_empty_string, WIFI_AUTH_MODE__Open, {0,NULL}, {0,NULL}, 0 }


/* WifiConnectedState methods */
void   wifi_connected_state__init
(WifiConnectedState         *message);
size_t wifi_connected_state__get_packed_size
(const WifiConnectedState   *message);
size_t wifi_connected_state__pack
(const WifiConnectedState   *message,
 uint8_t             *out);
size_t wifi_connected_state__pack_to_buffer
(const WifiConnectedState   *message,
 ProtobufCBuffer     *buffer);
WifiConnectedState *
wifi_connected_state__unpack
(ProtobufCAllocator  *allocator,
 size_t               len,
 const uint8_t       *data);
void   wifi_connected_state__free_unpacked
(WifiConnectedState *message,
 ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*WifiConnectedState_Closure)
(const WifiConnectedState *message,
 void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    wifi_station_state__descriptor;
extern const ProtobufCEnumDescriptor    wifi_connect_failed_reason__descriptor;
extern const ProtobufCEnumDescriptor    wifi_auth_mode__descriptor;
extern const ProtobufCMessageDescriptor wifi_connected_state__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_wifi_5fconstants_2eproto__INCLUDED */
