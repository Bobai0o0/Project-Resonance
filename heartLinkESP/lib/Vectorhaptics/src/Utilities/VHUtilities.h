#pragma once
#include <string>
#include <vector>

#define MAX_DATA_BUFF sizeof(HapticParameters) /*!< Maximum data buffer for binary command*/
#define MAX_CMD_LEN 33
#define MAX_CMD_LEN_FUNC_GEN 256
#define SERIAL_TIME_OUT 40
#define PREM_QUEUE_SIZE 20
#define SERIAL_PLOAT_QUEUE 10000
#define USER_MESG_QUEU 10
#define QUEUE_MAX_DELAY 10
#define DEFAULT_PCM_SAMPLE_RATE 8000
#define MAX_PCM_SAMPLE_RATE 22050
#define LUT_SIZE 100
#define RESONANT_FREQ 160.00

#define WIFI_IP 172, 198, 168, 1
#define WIFI_PORT 80
#define WIFI_SUBNET_MASK 255, 255, 255, 0
#define WIFI_GATEWAY 192, 168, 1, 1

#define BUFFFERMAX MAX_DATA_BUFF * 4
#define VH_CHUNK_SIZE MAX_DATA_BUFF
#define TOTAL_ANGLE 360
#define WAVE_DMA_BUFF 1024
#define CHAN_1 25
#define CHAN_2 26
#define CHAN_3 19
#define FG_FB_PCM_REQ 5023
#define FB_MSG_SEPERATOR "*"
#define VERSION_STRING "2.1.0.0"
#define VH_BOARD_NAME "VectorHaptics"
#define DEFAULT_BRD_TIMEOUT 200

// Timers
#define WAVE_GEN_TIMER 0
#define PCM_TIMER 1
#define MIXER_TIMER 2

// Timer scalar values
#define PCM_SCALER 2
#define MIXER_SCALER 2

// Mixer Sample rate
#define MIXER_SAMPLE_RATE 2

#define MIN_INTENSITY 0.000
#define MAX_INTENSITY 1.000

#define MIN_SHARPNESS 0.000
#define MAX_SHARPNESS 1.000

#define MIN_DUTY_CYCLE 0.000
#define MAX_DUTY_CYCLE 1.000

#define MAX_PULSE_DURATION 200
#define MIN_PULSE_DURATION 1

#define MAX_TICK_DURATION 200
#define MIN_TICK_DURATION 1

#define MAX_VIBR_FREQ 1000
#define MIN_VIBR_FREQ 0

#define DEFAULT_MAX_ANGLE_DIV 30

#define BLUETOOTH_NAME "Titan Core"
#define WIFI_SSID "Titan Core"
#define WIFI_PASS "1234567890"

#define BCLK_PIN 7
#define LRC_PIN 8
#define I2S_BIT_PER_SAMPLE 16
#define SD_PIN 20
#define I2S_BOARD "MAX98357A"
#define MAX_DMA_BUFF 1024
#define APP_NAME "FuncGen"
#define PAM_SHDN_PIN 15
#define DRV_SHDN_PIN 32
#define ESP32_PICO_MINI 1
#define CREATE_DS_FROM_STRING 0

#define VH_PULSE 32
#define VH_VIBRATE 33
#define VH_PCM 34
#define VH_TICK 35
#define VH_PAUSE 36
#define VH_ERM 37
#define VH_SWEEP 38

#define VH_HDI 1
#define VH_HDI_D 2
#define VH_HDI_C 3
#define VH_RESTART 4
#define VH_SET_BRD_NAM 10
#define VH_MAX_AUDIOSTREAM_BUFF 1024

#define BOARD_MAX_BUFFER sizeof(HapticData)

#define HALF_PI 1.5707963267948966192313216916398
#define PI 3.1415926535897932384626433832795
#define FRQ_TO_DUR(freq) (100000.0f / (freq))

#define PULSE_MAX_INTENSITY 1.00
#define PULSE_MIN_INTENSITY 0.00
#define PULSE_MAX_SHARPNESS 1.00
#define PULSE_MIN_SHARPNESS 0.00
#define PULSE_MIN_DURATION 1.00
#define PULSE_MAX_DURATION 200

#define TICK_MAX_INTENSITY 1.00
#define TICK_MIN_INTENSITY 0.00
#define TICK_MAX_SHARPNESS 1.00
#define TICK_MIN_SHARPNESS 0.00
#define TICK_MIN_DURATION 1.00
#define TICK_MAX_DURATION 200

#define VIBRATE_MAX_INTENSITY 1.00
#define VIBRATE_MIN_INTENSITY 0.00
#define VIBRATE_MAX_SHARPNESS 1.00
#define VIBRATE_MIN_SHARPNESS 0.00
#define VIBRATE_MIN_DURATION 1.00
#define VIBRATE_MAX_DURATION 10000
#define VIBRATE_MIN_FREQUENCY 0.00
#define VIBRATE_MAX_FREQUENCY 1000

#define PAUSE_MIN_DURATION 1.00
#define PAUSE_MAX_DURATION 10000

#define PULSE_DEFAULT_INTENSITY 0.70
#define PULSE_DEFAULT_DURATION 1000.00 / RESONANT_FREQ
#define PULSE_DEFAULT_SHARPNESS 0.50

#define TICK_DEFAULT_INTENSITY 0.70
#define TICK_DEFAULT_DURATION 1000.00 / RESONANT_FREQ
#define TICK_DEFAULT_SHARPNESS 1.00

#define VIBRATE_DEFAULT_INTENSITY 0.70
#define VIBRATE_DEFAULT_DURATION FRQ_TO_DUR(RESONANT_FREQ)
#define VIBRATE_DEFAULT_SHARPNESS 0.00
#define VIBRATE_DEFAULT_FREQUENCY RESONANT_FREQ

#define ERM_MAX_INTENSITY 1.00f
#define ERM_MIN_INTENSITY -1.00f
#define ERM_MIN_DURATION 1.00f
#define ERM_MAX_DURATION 10000.00f

#define ERM_DEFAULT_INTENSITY 1.00f
#define ERM_DEFAULT_DURATION 1000.00f

#define PAUSE_DEFAULT_DURATION 500.00
#define PCM_DEFAULT_SAMPLE_RATE 8000

// Commands for VH Use

#define COMPILER_PADDING (BOARD_MAX_BUFFER - MAX_DATA_BUFF)
#define DEFAULT_CHANNEL 0

#define MAX_PRIM_ENUMS 8

const int SILENT = 0;				/*!< Turn OFF all type of loggings*/
const int ERROR_ONLY = 1 << 0;		/*!< Turn ON Error Logs*/
const int INFO_ONLY = 1 << 1;		/*!< Turn ON Information Logs*/
const int WARNING_ONLY = 1 << 2;		/*!< Turn ON Warning Logs*/
const int VERBOSE = ERROR_ONLY | INFO_ONLY | WARNING_ONLY; /*!< Turn ON All Logs*/

const int VH_SUCCESS = 1; /*!< SUCCESS*/
const int VH_ERROR = 2;	  /*!< ERROR RETURN*/
const int VH_MESSAGE = 3; /*!< Return Messages*/

/*! @} */

/*! @addtogroup Primitives
 *  Documentation for Primitive Types
 *  @{
 */
const int PULSE = 1 << 0;		 /*!< Pulse */
const int TICK = 1 << 1;		 /*!< Tick */
const int VIBRATE = 1 << 2;		 /*!< Vibration */
const int PAUSE = 1 << 3;		 /*!< Pause */
const int PCM = 1 << 4;			 /*!< Pcm */
const int AUDIO_STREAM = 1 << 5; /*!< Audio Stream */
const int ERM = 1 << 6;			 /*!< Erm */
const int SWEEP = 1 << 7;		 /*!< Sweep */
const int CUSTOM = 1 << 8;		 /*!< CUSTOM */
/*! @} */

/*! @addtogroup Enumerators
 *  Documentation for Enums and Constants
 *  @{
 */
/**
 * @brief Enum for various type of mixing with PCM and Pulse,Tick and Vibration
 *
 */
enum MixingStrategy
{
	NO_MIX = -1,		/*!< No Mixing */
	INTERRUPT_MIX = 0,	/*!< Interrupt mixing */
	LARGESTVAL_MIX = 1, /*!< Output the largest value out of two mixing inputs.*/
	ADDITIVE_MIX = 2,	/*!< Output the sum of two input values,but maximum is 255*/
	CUSTOM_MIX = 3		/*!< Custom mixing */

};

enum class MCU_MODE
{
	PRODUCTION,
	DEVELOPMENT

};
enum class ConnType
{
	USB_CONN,
	BLUETOOTH_CLASSIC,
	BLUETOOTH_LE,
	WIFI_CONN,
	NONE
};

enum class CMD_MODE
{
	STRING_MODE,
	BINARY_MODE
};

enum class OPERATIONS
{
	PULSE,
	TICK,
	VIBRATION,
	PCM,
	I2S,
	WAVEGEN,
	MIXER,
	PAUSE,
	NONE
};

/**
 * @brief Enum for various standerd PCM sample rates
 *
 */
enum class PCMSampleRate
{
	SAMPLE_8K = 5000,  /*!< Sample Rate of 8000 Hz*/
	SAMPLE_11K = 3628, /*!< Sample Rate of 11000 Hz*/
	SAMPLE_16K = 2500, /*!< Sample Rate of 16000 Hz*/
	SAMPLE_20k = 2000, /*!< Sample Rate of 2000 Hz*/
	NONE,
};

enum LOG_TYPE
{
	ERROR_MSG,
	INFO_MSG,
	PCM_CHUNK_REQ,
	WARNING_MSG,
	/** Command response (e.g. HDI/DevTools). Intended to be shown regardless of verbosity. */
	RESPONSE_MSG,
};
enum class WaveType
{
	SINE,
	SQUARE,
	HYBRID,
	TRIANGULAR,
	NONE
};

enum IType
{
	idev_tools,
	ieffect_receiver,
	ichannel_list,
	ii2s,
	iprimitive_man,
	iaudio_streaming,
	iaudio_streaming_mixer
};

enum CHANNEL_STATUS
{
	CHANNEL_IDLE,
	CHANNEL_WAKE,
	MAX_CHANNEL_STATUS
};

enum class VHPlaybackState
{
	VH_IDLE,	 /*!< Playback is idle */
	VH_PLAYING,	 /*!< Playback is in progress */
	VH_DISABLED, /*!< Playback is disabled */
	VH_UNKNOWN	 /*!< Unknown state */
};

class VHPrimitive
{
public:
	enum class PrimitiveType
	{
		PULSE = 0,
		TICK = 1,
		PAUSE = 2,
		VIBRATE = 3,
		PCM = 4
	};

	enum class ParamType
	{
		INTENSITY = 0,
		DURATION = 1,
		SHARPNESS = 2,
		FREQUENCY = 3,
		Sampling_rate = 4
	};
};
enum QueueOperType
{
	APPEND_EFFECT,
	REPLACE_EFFECT
};

class BoardProfile;
class VectorHaptics;

class VHBaseType
{
	IType m_enumType;

protected:
	VHBaseType() = default;
	VHBaseType(IType type)
	{
		m_enumType = type;
	}
	BoardProfile *m_pBoard = nullptr;
	VectorHaptics *mVhPtr = nullptr;

public:
	~VHBaseType() {}
	IType getType() { return m_enumType; };
	void init(BoardProfile *pBoard, VectorHaptics *vh)
	{
		m_pBoard = pBoard;
		mVhPtr = vh;
	}
};

class DataBufferManager
{
	std::vector<uint8_t> mBuffer;

public:
	void clear();

	void write(const uint8_t *data, size_t length);

	int read(uint8_t *data, size_t length);

	size_t getSize() const;
};

#define UNUSED(x) (void)(x)