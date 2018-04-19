/**** User Selection *********/

/** Network setting **/

#define USE_DHCP				(0)					/* Select  0(static configuration) or 1(use DHCP) */
#if (USE_DHCP == 0)
//	#define IP_ADDRESS			("192.168.10.100")
//	#define SUBNET_MASK			("255.255.255.0")
//	#define DEFAULT_GATEWAY		("192.168.10.1")
	#define IP_ADDRESS			("192.168.43.235")
	#define SUBNET_MASK			("255.255.255.0")
	#define DEFAULT_GATEWAY		("192.168.43.1")
	#define SERVER_ADDRESS 		("192.168.43.230")
	#define SERVER_PORT 8000
#endif

#define NETWORK_TYPE			(1)					/* Select  0(EthernetInterface) or 1(GR_PEACH_WlanBP3595) */
#if (NETWORK_TYPE == 1)

	#define WLAN_SECURITY		NSAPI_SECURITY_WPA2	/* NSAPI_SECURITY_NONE, NSAPI_SECURITY_WEP, NSAPI_SECURITY_WPA or NSAPI_SECURITY_WPA2 */
#endif

/** Camera setting **/

#if 0
#define VIDEO_INPUT_METHOD		(VIDEO_CMOS_CAMERA)	/* Select  VIDEO_CVBS or VIDEO_CMOS_CAMERA */
#else
#define VIDEO_INPUT_METHOD		(VIDEO_CVBS)		/* Select  VIDEO_CVBS or VIDEO_CMOS_CAMERA */
#endif

#define VIDEO_INPUT_FORMAT		(VIDEO_YCBCR422)	/* Select  VIDEO_YCBCR422 or VIDEO_RGB888 or VIDEO_RGB565 */
#define USE_VIDEO_CH			(0)					/* Select  0 or 1 If selecting VIDEO_CMOS_CAMERA, should be 0.) */
#define VIDEO_PAL				(1)					/* Select  0(NTSC) or 1(PAL) If selecting VIDEO_CVBS, this parameter is not referenced.) */

/** SD card spi channel setting **/

#define SD_SPICH			(2)

/** WiFi settings **/
/** Milkcocoa settings **/
#define WLAN_SSID             ("aterm-1c08e0-g")                // SSID
#define WLAN_PSK              ("493689785890f")                 // PSK(Pre-Shared Key)

/** Milkcocoa settings **/
//#define MILKCOCOA_APP_ID        "postizpayuc0"
//#define MILKCOCOA_DATASTORE     "zumo"
//#define MILKCOCOA_SERVERPORT    1883
//const char MQTT_SERVER[] = MILKCOCOA_APP_ID ".mlkcca.com";
//const char MQTT_CLIENTID[] = __TIME__ MILKCOCOA_APP_ID;

/*****************************/
