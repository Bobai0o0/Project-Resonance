
#pragma once
#include <Utilities/VHUtilities.h>
#include <startendparam.h>
#include <vector>
#include <queue>
#include <string>
#include <VHEffects.h>
#include <memory>

#define MESSAGE_BUFFER_SIZE 1024
#define SERIAL_MESSAGE_LEN 2096
/// @cond HIDDEN_SYMBOL
struct Pairs
{
	char key[32] = {'\0'};
	float value = 0;
	float max = 10000, min = -10000, defVal = 0;

	Pairs() = default;
	Pairs(const char *nam, float val, bool isDefault = false);
	Pairs(const char *nam, float minVal, float maxVal, float def);
	std::string toString() const;
};

/// @endcond

/**
 * @class VHPrimitiveParams
 * @brief Manages the parameters of a haptic primitive.
 *
 * This class provides utilities to define, update, and query parameters
 * (e.g., intensity, duration, frequency) associated with a haptic primitive.
 * Parameters can be set by name, index, or by using the primitive’s type.
 */
class VHPrimitiveParams
{
	float mDuration = 0.0f;
	float mMinDuration = -100000.00;
	float mMaxDuration = 10000.00;
	float mDefaultDuration = 500.00;
	static constexpr size_t MAX_PARAMS = 10;

public:
	/**
	 * @brief Construct a new empty VHPrimitiveParams object.
	 *
	 * @code
	 * VHPrimitiveParams params
	 * @endcode
	 */
	VHPrimitiveParams();

	/**
	 * @brief Construct a new VHPrimitiveParams with a list of parameters.
	 *
	 * @param params List of key–value pairs (parameter name, parameter value).
	 */
	VHPrimitiveParams(std::initializer_list<std::pair<const char *, float>> params);

	/**
	 * @brief Construct a new VHPrimitiveParams by copying from another object.
	 *
	 * @param p Pointer to another VHPrimitiveParams object.
	 */
	VHPrimitiveParams(const VHPrimitiveParams *p);

	/**
	 * @brief Set multiple parameters at once using key–value pairs.
	 *
	 * @param params List of key–value pairs (parameter name, parameter value).
	 */
	void setParameters(std::initializer_list<std::pair<const char *, float>> &params);

	/**
	 * @brief Set a default parameter for the primitive.
	 *
	 * @param nam Parameter name.
	 * @param value Default value to assign.
	 */
	void setDefaultParam(const char *nam, float value);

	/**
	 * @brief Set a parameter by name.
	 *
	 * @param name Parameter name.
	 * @param value Value to assign (default = 0.0).
	 */
	void setParameter(const char *name, float value = 0.0);

	/**
	 * @brief Set a parameter with constraints.
	 *
	 * @param nam Parameter name.
	 * @param defaultVal Default value to assign.
	 * @param min Minimum allowable value.
	 * @param max Maximum allowable value.
	 */
	void setParameter(const char *nam, float defaultVal, float min, float max);

	/**
	 * @brief Set a parameter by index.
	 *
	 * @param idx Parameter index.
	 * @param value Value to assign (default = 0.0).
	 */
	void setParameter(int idx, float value = 0.00);

	/**
	 * @brief Get the parameter name by index.
	 *
	 * @param idx Parameter index.
	 * @return Pointer to the parameter name string.
	 */
	const char *getParameter(int idx) const;

	/**
	 * @brief Set the min, max, and default values for duration.
	 *
	 * @param defaultVal Default duration value.
	 * @param min Minimum allowed duration.
	 * @param max Maximum allowed duration.
	 */
	void setMinMaxDuration(const float defaultVal, const float min, const float max);

	/**
	 * @brief Retrieve the min, max, and default duration values.
	 *
	 * @param defaultVal Reference to store default duration.
	 * @param min Reference to store minimum duration.
	 * @param max Reference to store maximum duration.
	 */
	void getMinMaxDuration(float &defaultVal, float &min, float &max) const;

	/**
	 * @brief Get a parameter value by name.
	 *
	 * @param name Parameter name.
	 * @return Parameter value.
	 */
	float getParameter(const char *name);

	/**
	 * @brief Retrieve min, max, and default values for a parameter by name.
	 *
	 * @param nam Parameter name.
	 * @param defaultVal Reference to store default value.
	 * @param min Reference to store minimum value.
	 * @param max Reference to store maximum value.
	 * @return true if parameter exists, false otherwise.
	 */
	bool getMinMax(const char *nam, float &defaultVal, float &min, float &max);

	/**
	 * @brief Get a parameter value by name.
	 *
	 * @param name Parameter name.
	 * @param val Reference to store the value.
	 * @return true if parameter exists, false otherwise.
	 */
	bool getParameter(const char *name, float &val);

	/**
	 * @brief Clone all parameters from another VHPrimitiveParams object.
	 *
	 * @param source Source object to copy from.
	 */
	void clone(const VHPrimitiveParams *source);

	/**
	 * @brief Remove a parameter by name.
	 *
	 * @param nam Parameter name.
	 * @return true if parameter was removed, false if not found.
	 */
	bool removeParameter(const char *nam);

	/**
	 * @brief Get the number of parameters currently defined.
	 *
	 * @return Number of parameters.
	 */
	size_t getNumParameters() const;

	/**
	 * @brief Serialize all parameters into a string.
	 *
	 * @param strString Reference to string to store serialized result.
	 */
	void getSerilized(std::string &strString);

	/**
	 * @brief Convert all parameters to a human-readable string.
	 *
	 * @return String representation of parameters.
	 */
	std::string toString();

	/**
	 * @brief Get the current duration value.
	 *
	 * @return Duration.
	 */
	float getDur() const { return mDuration; };

	/**
	 * @brief Set the duration value.
	 *
	 * @param dur Duration to set.
	 */
	void setDur(const float dur) { mDuration = dur; };

	/**
	 * @brief Get a parameter value by type.
	 *
	 * @param type Parameter type.
	 * @param defaultVal Default value.
	 * @return Parameter value.
	 */
	float get(VHPrimitive::ParamType type, float defaultVal);

	/**
	 * @brief Get a parameter value by type with constraints.
	 *
	 * @param type Parameter type.
	 * @param defaultVal Default value .
	 * @param min Minimum allowable value.
	 * @param max Maximum allowable value.
	 * @return Parameter value.
	 */
	float get(VHPrimitive::ParamType type, float defaultVal, float min, float max);

	/**
	 * @brief Get a parameter value by name.
	 *
	 * @param name Parameter name.
	 * @param defaultVal Default value.
	 * @return Parameter value.
	 */
	float get(const char *name, float defaultVal);

	/**
	 * @brief Get a parameter value by name with constraints.
	 *
	 * @param name Parameter name.
	 * @param defaultVal Default value.
	 * @param min Minimum allowable value.
	 * @param max Maximum allowable value.
	 * @return Parameter value.
	 */
	float get(const char *name, float defaultVal, float min, float max);

protected:
	/// @cond HIDDEN_SYMBOL
	Pairs mPairs[MAX_PARAMS];
	size_t mNumParams;
	///@endcond
};

/// @cond HIDDEN_SYMBOL
typedef char vhUserMessage[256];

struct HapticParameters
{
	HapticParameters()
	{
		frequency = 0;
		strength = 0;
		duration = 0;
		sharpness = 0;
		subCmd = 0;
	}

	HapticParameters(float freq, float str, float dur, float sharp)
	{
		frequency = freq;
		strength = str;
		duration = dur;
		sharpness = sharp;
		subCmd = 0;
	}
	HapticParameters(float str, float dur, float sharp)
	{
		frequency = 0;
		strength = str;
		duration = dur;
		sharpness = sharp;
		subCmd = 0;
	}
	float frequency;
	float strength;
	float duration;
	float sharpness;
	unsigned char subCmd;
};

struct HapticData
{
	unsigned short dataLength;			  /*!<Length of  arrData member */
	unsigned char Command;				  /*!<Command ID. Check remark section for details*/
	unsigned char Channel;				  /*!<Channel number */
	unsigned char arrData[MAX_DATA_BUFF]; /*!<Binary data */

	HapticData()
	{
		dataLength = 0;
		Command = 0;
		Channel = 0;
		memset(arrData, 0, sizeof(arrData));
	}

	HapticData(unsigned char cmd, unsigned char chnl, HapticParameters &param)
	{
		dataLength = sizeof(HapticParameters);
		Command = cmd;
		Channel = chnl;
		memcpy(arrData, &param, sizeof(HapticParameters));
	}

	HapticData(unsigned char cmd, unsigned char chnl)
	{
		dataLength = 0;
		Command = cmd;
		Channel = chnl;
		memset(arrData, 0, sizeof(arrData));
	}
};

template <typename T>
class CRC
{
public:
	static bool parse(uint8_t *byteArray, size_t size, T *ptr)
	{
		if (size != sizeof(T) + 1) // +1 for CRC byte
			return false;

		uint8_t receivedCRC = byteArray[size - 1];
		uint8_t computedCRC = calculateCRC(byteArray, size - 1); // Exclude last byte

		if (computedCRC != receivedCRC)
			return false;

		memcpy(ptr, byteArray, sizeof(T));
		return true;
	}

	static uint8_t calculateCRC(uint8_t *data, size_t length)
	{
		uint8_t crc = 0;
		for (size_t i = 0; i < length; i++)
		{
			crc ^= data[i];
		}
		return crc;
	}
};

#pragma pack(push, 1)
struct Header
{
	uint8_t cmd;
	uint8_t channel;
	uint8_t dataLength;
};

struct PayLoadPulse
{
	float intensity;
	uint16_t duration;
	float sharpness;

	PayLoadPulse() : intensity(0), duration(0), sharpness(0) {}

	bool parse(uint8_t *byteArray, size_t size)
	{
		return CRC<PayLoadPulse>::parse(byteArray, size, this);
	}
};

struct PayLoadVibr
{
	float frequency;
	float intensity;
	uint16_t duration;
	float sharpness;

	PayLoadVibr() : frequency(0), intensity(0), duration(0), sharpness(0) {}

	bool parse(uint8_t *byteArray, size_t size)
	{
		return CRC<PayLoadVibr>::parse(byteArray, size, this);
	}
};

struct PayLoadPause
{
	uint16_t duration;
	PayLoadPause() : duration(0) {}

	bool parse(uint8_t *byteArray, size_t size)
	{
		return CRC<PayLoadPause>::parse(byteArray, size, this);
	}
};

struct PayLoadErm
{
	float intensity;
	uint16_t duration;

	PayLoadErm() : intensity(0), duration(0) {}

	bool parse(uint8_t *byteArray, size_t size)
	{
		return CRC<PayLoadErm>::parse(byteArray, size, this);
	}
};

struct PayLoadSweep
{
	float duration;
	float startIntensity;
	float startFrequency;
	float startSharpness;
	float endIntensity;
	float endFrequency;
	float endSharpness;
	uint8_t transIntensity;
	uint8_t transFrequency;
	uint8_t transSharpness;

	PayLoadSweep() : duration(0), startIntensity(0), startFrequency(0), startSharpness(0),
					 endIntensity(0), endFrequency(0), endSharpness(0), transIntensity(0),
					 transFrequency(0), transSharpness(0) {}

	bool parse(uint8_t *byteArray, size_t size)
	{
		return CRC<PayLoadSweep>::parse(byteArray, size, this);
	}
};
#pragma pack(pop)

using PayLoadTick = PayLoadPulse;
using PayLoadPcm = PayLoadPulse;
class ICustomPrim;

class CustomPrimInt : public IVhEffect
{
protected:
	ICustomPrim *mCustomPrimPtr;
	VHPrimitiveParams param;
	CustomPrimCb cb;
	float duration;
	char ser[64];

public:
	CustomPrimInt();
	CustomPrimInt(ICustomPrim *cust);
	VHPrimitiveParams *getPrimPtr();
	CustomPrimCb getCallBack();
	float getDur() const;
	void getSerilized(std::string &strSer);
};

class Primitive : public CustomPrimInt
{
	VH_Pcm mVH_Pcm;
	int mPcmSampleRate = PCM_DEFAULT_SAMPLE_RATE;

public:
	float frequency;
	unsigned char strength;
	unsigned long start_time;
	unsigned long duration;
	unsigned long end_time;
	unsigned char channel;
	StartEndParam mSweepIntensityMaped;
	StartEndParam mSweepIntensityOrginal;
	StartEndParam mSweepFrequency;
	StartEndParam mSweepSharpness;
	float sharpness;
	float mIntensity;
	bool played = false;
	int Type;
	double mAccumulatedPhase = 0;
	unsigned long mLastPhaseUpdateTime = 0;
	Primitive();
	Primitive(float freq, float intensity, float dur, float sharp, bool val);
	Primitive(float dur, StartEndParam Intensity, StartEndParam freq, StartEndParam Sharpness);
	Primitive(float intensity, float dur, float sharp, int type);
	Primitive(float intensity, float dur);
	Primitive(float dur);
	Primitive(VH_Pcm *pcm);
	Primitive(ICustomPrim *cust);
	void Update();
	void Update(unsigned long start_timeus);
	float getIntensity() const;
	StartEndParam &getSweepIntensity();
	float getDuration() const;
	int getPcmSampleRate() const;
	VH_Pcm *getPcmPtr();
};

class DeviceInfo
{
	std::string m_strDeviceName;
	std::string m_strManufacture;
	std::string m_strModelNumber;
	std::string m_strSerialNumber;
	std::string m_strVersionNumber;
	MCU_MODE m_Mode;
	ConnType m_Conn;
	CMD_MODE m_CommandMode;

	void setConnType(ConnType connMode, CMD_MODE cmdMode)
	{
		m_Conn = connMode;
		m_CommandMode = cmdMode;
	}
	MCU_MODE getMode()
	{
		return m_Mode;
	}
	ConnType getConnType()
	{
		return m_Conn;
	}
	CMD_MODE getCommandMode()
	{
		return m_CommandMode;
	}

public:
	DeviceInfo()
	{
		m_strDeviceName = "VHDevice";
		m_strManufacture = "UNDEFINED";
		m_strModelNumber = "VH_GENERIC_MODEL";
		m_strSerialNumber = "0000000000";
		m_strVersionNumber = VERSION_STRING;
		m_Mode = MCU_MODE::DEVELOPMENT;
		m_Conn = ConnType::USB_CONN;
		m_CommandMode = CMD_MODE::STRING_MODE;
	}
	DeviceInfo(DeviceInfo *info)
	{
		setDeviceInfo(info);
	}
	void setDeviceInfo(DeviceInfo *info)
	{
		m_strDeviceName = info->getDeviceName();
		m_strManufacture = info->getManufacturer();
		m_strModelNumber = info->getModelNumber();
		m_strSerialNumber = info->getSerialNumber();
		m_strVersionNumber = info->getVersion();
	}
	void setDeviceName(std::string newVal = "VHDevice")
	{
		m_strDeviceName = newVal;
	}
	void setManufacturer(std::string newVal = "")
	{
		m_strManufacture = newVal;
	}
	void setModelNumber(std::string newVal = "")
	{
		m_strModelNumber = newVal;
	}
	void setSerialNumber(std::string newVal = "")
	{
		m_strSerialNumber = newVal;
	}
	void setVersion(std::string newVal = VERSION_STRING)
	{
		m_strVersionNumber = newVal;
	}

	std::string getDeviceName()
	{
		return m_strDeviceName;
	}
	std::string getManufacturer()
	{
		return m_strManufacture;
	}
	std::string getModelNumber()
	{
		return m_strModelNumber;
	}
	std::string getSerialNumber()
	{
		return m_strSerialNumber;
	}
	std::string getVersion()
	{
		return m_strVersionNumber;
	}
};

struct vhName
{
	char name[MESSAGE_BUFFER_SIZE];

	// Constructor
	vhName(const char *str = "")
	{
		strncpy(name, str, sizeof(name));
		name[sizeof(name) - 1] = '\0'; // Ensure null-termination
	}

	// Copy assignment operator
	vhName &operator=(const vhName &other)
	{
		if (this != &other)
		{ // Self-assignment check
			strncpy(name, other.name, sizeof(name));
			name[sizeof(name) - 1] = '\0'; // Ensure null-termination
		}
		return *this;
	}

	std::string toString()
	{
		return std::string(name);
	}
};
class MsgQueue
{
	vhName Msg;
	int msgType;

public:
	MsgQueue()
	{
		msgType = 0;
	}
	MsgQueue(const char *str, int type)
		: Msg(str), msgType(type)
	{
	}
	std::string toString()
	{
		return Msg.toString();
	}
	int getType()
	{
		return msgType;
	}
};

#define INIT_CHECK_FAIL if (!m_pChannelList || !m_pDeviceInfo)

typedef std::queue<Primitive> PrimitiveQueue;
typedef std::queue<vhName> MessageQueue;
typedef std::queue<vhName> ErrorQueue;
typedef void (*BoardIdleCb)(bool IsIdle, uint8_t channelNo);
typedef void (*PcmchunkCb)(std::string &chunk);

/// @endcond
