#pragma once
#include <string>
#include <BoardProfile.h>
#include <cstring>
#include "Utilities/VHUtilities.h"
#include <vector>
#include <startendparam.h>
/*! @addtogroup TypeDefinitions
 *  Documentation for Callback type definition
 *  @{
 */

/**
 * @typedef CustomPrimCb
 * @brief Function pointer type for custom primitive callbacks.
 *
 * This typedef defines the signature of a callback function that can be used
 * for handling custom primitives. The callback receives the primitive parameters
 * and a start time, and returns an unsigned char as the status or result.
 *
 * @param param Pointer to a VHPrimitiveParams object containing the primitive parameters.
 * @param startTime The time (in seconds) when the primitive started.
 * @return Unsigned char value representing the result or status of the callback.
 */
typedef unsigned char (*CustomPrimCb)(class VHPrimitiveParams *param, float startTime);
/*! @} */
/// @cond HIDDEN_SYMBOL
class IVhEffect
{
public:
    unsigned char m_ucharTypeId = 0;
    unsigned char toTypeId()
    {
        return m_ucharTypeId;
    }
    void setTypeId(unsigned char id)
    {
        m_ucharTypeId = id;
    }
    virtual ~IVhEffect() {};
};

class VhEffect : public IVhEffect
{
protected:
    int m_iDelay = 0;
    unsigned char mChannel = 0;

public:
    /**
     * @brief Construct a new Vh Effect object
     *
     */
    VhEffect() = default;
    /**
     * @brief Set the Delay
     *
     * @param newVal
     */
    void setDelay(int newVal)
    {
        m_iDelay = newVal;
    }
    /**
     * @brief Get the Delay
     *
     * @return int
     */
    int getDelay()
    {
        return m_iDelay;
    }
    void setChannel(unsigned char channel)
    {
        mChannel = channel;
    }

    unsigned char getChannel()
    {
        return mChannel;
    }
};

class VHPulse : public VhEffect
{
    static float _m_dIntensity;
    static uint16_t _m_dDuration;
    static float _m_MinimumIntensity, _m_MaximumIntensity;
    static uint16_t _m_MinimumDuration, _m_MaximumDuration;
    static float _m_MinSharpness, _m_MaxSharpness;
    static float _m_dSharpness;

public:
    static std::string getName() { return "Pulse"; }
    float m_dIntensity, m_dSharpness;
    uint16_t m_dDuration;
    /**
     * @brief Construct a new VHPulse object
     *
     * @param Intensity
     * @param Duration
     */
    VHPulse(uint16_t Duration, float Intensity, float Sharpness)
    {
        m_dIntensity = Intensity;
        m_dDuration = Duration;
        m_dSharpness = Sharpness;
        m_ucharTypeId = 1;
    }

    VHPulse(uint16_t Duration, float Intensity)
    {
        m_dIntensity = Intensity;
        m_dDuration = Duration;
        m_dSharpness = _m_dSharpness;
        m_ucharTypeId = 1;
    }

    VHPulse(uint16_t Duration)
    {
        m_dIntensity = _m_dIntensity;
        m_dDuration = Duration;
        m_dSharpness = _m_dSharpness;
        m_ucharTypeId = 1;
    }
    /**
     * @brief Construct a new VHPulse object
     *
     */
    VHPulse()
    {
        m_dIntensity = _m_dIntensity;
        m_dDuration = _m_dDuration;
        m_dSharpness = _m_dSharpness;
        m_ucharTypeId = 1;
    }
    /**
     * @brief Construct a new VHPulse object
     *
     * @param p
     */
    VHPulse(VHPulse *p)
    {
        m_dIntensity = p->m_dIntensity;
        m_dDuration = p->m_dDuration;
        m_dSharpness = p->m_dSharpness;
        m_iDelay = p->getDelay();
        m_ucharTypeId = 1;
    }

    /**
     * @brief Set the Default value for intensity up to a specific duration
     *
     * @param Intensity
     * @param Duration
     */
    static void setDefault(float Intensity, uint16_t Duration, float Sharpness)
    {
        _m_dIntensity = vhconstrain(Intensity, PULSE_MIN_INTENSITY, PULSE_MAX_INTENSITY);
        _m_dDuration = vhconstrain(Duration, PULSE_MIN_DURATION, PULSE_MAX_DURATION);
        _m_dSharpness = vhconstrain(Sharpness, PULSE_MIN_SHARPNESS, PULSE_MAX_SHARPNESS);
    }
    static void getDefault(uint16_t &Duration, float &Intensity, float &Sharpness)
    {
        Intensity = _m_dIntensity;
        Duration = _m_dDuration;
        Sharpness = _m_dSharpness;
    }
    /**
     * @brief Set the Minimum value for Intensity
     *
     * @param minIntensity
     */
    static void setMinIntensity(float minIntensity)
    {
        _m_MinimumIntensity = minIntensity;
    }

    static float getMinSharpness()
    {
        return _m_MinSharpness;
    }
    static float getMaxSharpness()
    {
        return _m_MaxSharpness;
    }

    /**
     * @brief Set the Maximum value for Intensity
     *
     * @param maxIntensity
     */
    static void setMaxIntensity(float maxIntensity)
    {
        _m_MaximumIntensity = maxIntensity;
    }
    /**
     * @brief Set the Minimum and Maximum value for Intensity
     *
     * @param minIntensity
     * @param maxIntensity
     */
    static void setMinMaxIntensity(float minIntensity, float maxIntensity)
    {
        _m_MinimumIntensity = minIntensity;
        _m_MaximumIntensity = maxIntensity;
    }
    /**
     * @brief Set the Minimum and Maximum value for Duration
     *
     * @param minDuration
     * @param maxDuration
     */
    static void setMinMaxDuration(uint16_t minDuration, uint16_t maxDuration)
    {
        _m_MinimumDuration = minDuration;
        _m_MaximumDuration = maxDuration;
    }
    /**
     * @brief Set the Minvalue for Duration
     *
     * @param minDuration
     */
    static void setMinDuration(uint16_t minDuration)
    {
        _m_MinimumDuration = minDuration;
    }
    /**
     * @brief Set the Maximum value for Duration
     *
     * @param maxDuration
     */
    static void setMaxDuration(uint16_t maxDuration)
    {
        _m_MaximumDuration = maxDuration;
    }
    /**
     * @brief Get the Minimum Intensity
     *
     * @return float
     */
    static float getMinIntensity()
    {
        return _m_MinimumIntensity;
    }
    /**
     * @brief Get the Maximum  Intensity
     *
     * @return float
     */
    static float getMaxIntensity()
    {
        return _m_MaximumIntensity;
    }
    /**
     * @brief Get the Minimum  Duration
     *
     * @return uint16_t
     */
    static uint16_t getMinDuration()
    {
        return _m_MinimumDuration;
    }
    /**
     * @brief Get the Maximum Duration
     *
     * @return uint16_t
     */
    static uint16_t getMaxDuration()
    {
        return _m_MaximumDuration;
    }

    static void setMinValues(float minIntensity, uint16_t minDuration, float minSharpness)
    {
        _m_MinimumIntensity = vhconstrain(minIntensity, PULSE_MIN_INTENSITY, PULSE_MAX_INTENSITY);
        _m_MinimumDuration = vhconstrain(minDuration, PULSE_MIN_DURATION, PULSE_MAX_DURATION);
        _m_MinSharpness = vhconstrain(minSharpness, PULSE_MIN_SHARPNESS, PULSE_MAX_SHARPNESS);
    }
    static void setMaxValues(float maxIntensity, uint16_t maxDuration, float maxSharpness)
    {
        _m_MaximumIntensity = vhconstrain(maxIntensity, PULSE_MIN_INTENSITY, PULSE_MAX_INTENSITY);
        _m_MaximumDuration = vhconstrain(maxDuration, PULSE_MIN_DURATION, PULSE_MAX_DURATION);
        _m_MaxSharpness = vhconstrain(maxSharpness, PULSE_MIN_SHARPNESS, PULSE_MAX_SHARPNESS);
    }

    static void getMinValues(float &minIntensity, uint16_t &minDuration, float &minSharpness)
    {
        minIntensity = _m_MinimumIntensity;
        minDuration = _m_MinimumDuration;
        minSharpness = _m_MinSharpness;
    }
    static void getMaxValues(float &maxIntensity, uint16_t &maxDuration, float &maxSharpness)
    {
        maxIntensity = _m_MaximumIntensity;
        maxDuration = _m_MaximumDuration;
        maxSharpness = _m_MaxSharpness;
    }
};

class PULSE : public VHPulse
{
};

class VHTick : public VhEffect
{
    static float _m_dIntensity;
    static uint16_t _m_dDuration;
    static float _m_MinimumIntensity, _m_MaximumIntensity;
    static uint16_t _m_MinimumDuration, _m_MaximumDuration;
    static float _m_MinSharpness, _m_MaxSharpness;
    static float _m_dSharpness;

public:
    static std::string getName() { return "Tick"; }
    float m_dIntensity, m_dSharpness;
    uint16_t m_dDuration;
    /**
     * @brief Construct a new VHTick object
     *
     * @param Intensity
     * @param Duration
     */
    VHTick(uint16_t Duration, float Intensity, float sharpness)
    {
        m_dIntensity = Intensity;
        m_dDuration = Duration;
        m_dSharpness = sharpness;
        m_ucharTypeId = 2;
    }

    VHTick(uint16_t Duration, float Intensity)
    {
        m_dIntensity = Intensity;
        m_dDuration = Duration;
        m_dSharpness = _m_dSharpness;
        m_ucharTypeId = 2;
    }

    VHTick(uint16_t Duration)
    {
        m_dIntensity = _m_dIntensity;
        m_dDuration = Duration;
        m_dSharpness = _m_dSharpness;
        m_ucharTypeId = 2;
    }

    /**
     * @brief Construct a new VHTick object
     *
     */
    VHTick()
    {
        m_dIntensity = _m_dIntensity;
        m_dDuration = _m_dDuration;
        m_dSharpness = _m_dSharpness;
        m_ucharTypeId = 2;
    }

    /**
     * @brief Construct a new VHTick object
     *
     * @param p
     */
    VHTick(VHTick *p)
    {
        m_dIntensity = p->m_dIntensity;
        m_dDuration = p->m_dDuration;
        m_dSharpness = p->m_dSharpness;
        m_iDelay = p->getDelay();
        m_ucharTypeId = 2;
    }

    /**
     * @brief Set the Default value for Intensity up to a specific Duration
     *
     * @param Intensity
     * @param Duration
     */
    static void setDefault(float Intensity, uint16_t Duration, float Sharpness)
    {
        _m_dIntensity = vhconstrain(Intensity, TICK_MIN_INTENSITY, TICK_MAX_INTENSITY);
        _m_dDuration = vhconstrain(Duration, TICK_MIN_DURATION, TICK_MAX_DURATION);
        _m_dSharpness = vhconstrain(Sharpness, TICK_MIN_SHARPNESS, TICK_MAX_SHARPNESS);
    }
    static void getDefault(uint16_t &Duration, float &Intensity, float &Sharpness)
    {
        Intensity = _m_dIntensity;
        Duration = _m_dDuration;
        Sharpness = _m_dSharpness;
    }
    static float getMinSharpness()
    {
        return _m_MinSharpness;
    }
    static float getMaxSharpness()
    {
        return _m_MaxSharpness;
    }
    /**
     * @brief Set the Minimum Intensity
     *
     * @param minIntensity
     */
    static void setMinIntensity(float minIntensity)
    {
        _m_MinimumIntensity = minIntensity;
    }
    /**
     * @brief Set the Maximum Intensity
     *
     * @param maxIntensity
     */
    static void setMaxIntensity(float maxIntensity)
    {
        _m_MaximumIntensity = maxIntensity;
    }
    /**
     * @brief Set the Minimum Duration
     *
     * @param minDuration
     */
    static void setMinDuration(uint16_t minDuration)
    {
        _m_MinimumDuration = minDuration;
    }
    /**
     * @brief Set the Maximum Duration
     *
     * @param maxDuration
     */
    static void setMaxDuration(uint16_t maxDuration)
    {
        _m_MaximumDuration = maxDuration;
    }
    /**
     * @brief Get the Minimum Intensity
     *
     * @return float
     */
    static float getMinIntensity()
    {
        return _m_MinimumIntensity;
    }
    /**
     * @brief Get the Maximum Intensity
     *
     * @return float
     */
    static float getMaxIntensity()
    {
        return _m_MaximumIntensity;
    }
    /**
     * @brief Get the Minimum Duration
     *
     * @return uint16_t
     */
    static uint16_t getMinDuration()
    {
        return _m_MinimumDuration;
    }
    /**
     * @brief Get the Maximum Duration
     *
     * @return uint16_t
     */
    static uint16_t getMaxDuration()
    {
        return _m_MaximumDuration;
    }
    /**
     * @brief Set the Minimum and Maximum Intensity
     *
     * @param minIntensity
     * @param maxIntensity
     */
    static void setMinMaxIntensity(float minIntensity, float maxIntensity)
    {
        _m_MinimumIntensity = minIntensity;
        _m_MaximumIntensity = maxIntensity;
    }
    /**
     * @brief Set the Minimum and Maximum Duration
     *
     * @param minDuration
     * @param maxDuration
     */
    static void setMinMaxDuration(uint16_t minDuration, uint16_t maxDuration)
    {
        _m_MinimumDuration = minDuration;
        _m_MaximumDuration = maxDuration;
    }

    static void setMinValues(float minIntensity, uint16_t minDuration, float minSharpness)
    {
        _m_MinimumIntensity = vhconstrain(minIntensity, TICK_MIN_INTENSITY, TICK_MAX_INTENSITY);
        _m_MinimumDuration = vhconstrain(minDuration, TICK_MIN_DURATION, TICK_MAX_DURATION);
        _m_MinSharpness = vhconstrain(minSharpness, TICK_MIN_SHARPNESS, TICK_MAX_SHARPNESS);
    }
    static void setMaxValues(float maxIntensity, uint16_t maxDuration, float maxSharpness)
    {
        _m_MaximumIntensity = vhconstrain(maxIntensity, TICK_MIN_INTENSITY, TICK_MAX_INTENSITY);
        _m_MaximumDuration = vhconstrain(maxDuration, TICK_MIN_DURATION, TICK_MAX_DURATION);
        _m_MaxSharpness = vhconstrain(maxSharpness, TICK_MIN_SHARPNESS, TICK_MAX_SHARPNESS);
    }

    static void getMinValues(float &minIntensity, uint16_t &minDuration, float &minSharpness)
    {
        minIntensity = _m_MinimumIntensity;
        minDuration = _m_MinimumDuration;
        minSharpness = _m_MinSharpness;
    }
    static void getMaxValues(float &maxIntensity, uint16_t &maxDuration, float &maxSharpness)
    {
        maxIntensity = _m_MaximumIntensity;
        maxDuration = _m_MaximumDuration;
        maxSharpness = _m_MaxSharpness;
    }
};

class TICK : public VHTick
{
};

class VHPause : public VhEffect
{
    static uint16_t _m_dDuration, _m_MinimumDuration, _m_MaximumDuration;

public:
    static std::string getName() { return "Pause"; }
    uint16_t m_dDuration;
    /**
     * @brief Construct a new VHPause object
     *
     * @param milliseconds
     */
    VHPause(uint16_t milliseconds)
    {
        m_dDuration = milliseconds;
        m_ucharTypeId = 3;
    }
    /**
     * @brief Construct a new VHPause object
     *
     * @param p
     */
    VHPause(VHPause *p)
    {
        m_dDuration = p->m_dDuration;
        m_ucharTypeId = 3;
    }
    /**
     * @brief Construct a new VHPause object
     *
     */
    VHPause()
    {
        m_dDuration = _m_dDuration;
        m_ucharTypeId = 3;
    }
    /**
     * @brief Set the Default value for Duration
     *
     * @param Duration
     */
    static void setDefault(uint16_t Duration)
    {
        _m_dDuration = vhconstrain(Duration, PAUSE_MIN_DURATION, PAUSE_MAX_DURATION);
    }
    /**
     * @brief Get the Default duration
     *
     * @return uint16_t
     */
    static uint16_t getDefault()
    {
        return _m_dDuration;
    }
    /**
     * @brief Set the Minimum Duration
     *
     * @param minDuration
     */
    static void setMinDuration(uint16_t minDuration)
    {
        _m_MinimumDuration = vhconstrain(minDuration, PAUSE_MIN_DURATION, PAUSE_MAX_DURATION);
    }

    /**
     * @brief Set the Maximum Duration
     *
     * @param maxDuration
     */
    static void setMaxDuration(uint16_t maxDuration)
    {
        _m_MaximumDuration = vhconstrain(maxDuration, PAUSE_MIN_DURATION, PAUSE_MAX_DURATION);
    }
    /**
     * @brief Get the Minimum Duration
     *
     * @return uint16_t
     */
    static uint16_t getMinDuration()
    {
        return _m_MinimumDuration;
    }
    /**
     * @brief Get the Maximum Duration
     *
     * @return uint16_t
     */
    static uint16_t getMaxDuration()
    {
        return _m_MaximumDuration;
    }
    /**
     * @brief Set the Minimum and Maximum Duration
     *
     * @param minDuration
     * @param maxDuration
     */
    static void setMinMaxDuration(uint16_t minDuration, uint16_t maxDuration)
    {
        _m_MinimumDuration = vhconstrain(minDuration, PAUSE_MIN_DURATION, PAUSE_MAX_DURATION);
        _m_MaximumDuration = vhconstrain(maxDuration, PAUSE_MIN_DURATION, PAUSE_MAX_DURATION);
    }
};

class PAUSE : public VHPause
{
};

class VHVibrate : public VhEffect
{
    static float _m_dIntensity;
    static uint16_t _m_dDuration;
    static float _m_dSharpness;
    static float _m_dFrequency;
    static float _m_MinimumIntensity, _m_MaximumIntensity;
    static uint16_t _m_MinimumDuration, _m_MaximumDuration;
    static float _m_MinSharpness, _m_MaxSharpness;
    static float _m_MinimumFreq, _m_MaximumFreq;

public:
    static std::string getName() { return "Vibrate"; }
    float m_frequency, m_intensity, m_sharpness = 0;
    uint16_t m_dDuration;
    std::string Reserved;
    /**
     * @brief Construct a new VHVibrate object
     *
     * @param frequency
     * @param intensity
     * @param duration
     * @param sharpness
     * @param autoOff
     */
    VHVibrate(uint16_t duration, float intensity, float frequency, float sharpness)
    {
        m_frequency = frequency;
        m_intensity = intensity;
        m_dDuration = duration;
        m_sharpness = sharpness;
        m_ucharTypeId = 5;
        Reserved = "";
    }

    VHVibrate(uint16_t duration, float intensity, float frequency)
    {
        m_frequency = frequency;
        m_intensity = intensity;
        m_dDuration = duration;
        m_sharpness = _m_dSharpness;
        m_ucharTypeId = 5;
        Reserved = "";
    }

    VHVibrate(uint16_t duration, float intensity)
    {
        m_frequency = _m_dFrequency;
        m_intensity = intensity;
        m_dDuration = duration;
        m_sharpness = _m_dSharpness;
        m_ucharTypeId = 5;
        Reserved = "";
    }

    VHVibrate(uint16_t duration)
    {
        m_frequency = _m_dFrequency;
        m_intensity = _m_dIntensity;
        m_dDuration = duration;
        m_sharpness = _m_dSharpness;
        m_ucharTypeId = 5;
        Reserved = "";
    }

    /**
     * @brief Construct a new VHVibrate object
     *
     */
    VHVibrate()
    {
        m_frequency = _m_dFrequency;
        m_intensity = _m_dIntensity;
        m_dDuration = _m_dDuration;
        m_sharpness = _m_dSharpness;
        m_ucharTypeId = 5;
        Reserved = "";
    }
    /**
     * @brief Construct a new VHVibrate object
     *
     * @param p
     * @param id
     */
    VHVibrate(VHVibrate *p, unsigned char id)
    {
        m_frequency = p->m_frequency;
        m_intensity = p->m_intensity;
        m_dDuration = p->m_dDuration;
        m_sharpness = p->m_sharpness;
        m_iDelay = p->getDelay();
        m_ucharTypeId = id;
        Reserved = "";
    }

    /**
     * @brief Set the Default values for Intensity,Duration,Frequency,Duty Cycle
     *
     * @param Intensity
     * @param Duration
     * @param Frequency
     */
    static void setDefault(float Frequency, float Intensity, uint16_t Duration, float sharpness)
    {
        _m_dIntensity = vhconstrain(Intensity, VIBRATE_MIN_INTENSITY, VIBRATE_MAX_INTENSITY);
        _m_dDuration = vhconstrain(Duration, VIBRATE_MIN_DURATION, VIBRATE_MAX_DURATION);
        _m_dSharpness = vhconstrain(sharpness, VIBRATE_MIN_SHARPNESS, VIBRATE_MAX_SHARPNESS);

        _m_dFrequency = vhconstrain(Frequency, VIBRATE_MIN_FREQUENCY, VIBRATE_MAX_FREQUENCY);
    }

    static void setDefault(float Intensity, uint16_t Duration, float sharpness)
    {
        _m_dIntensity = vhconstrain(Intensity, VIBRATE_MIN_INTENSITY, VIBRATE_MAX_INTENSITY);
        _m_dDuration = vhconstrain(Duration, VIBRATE_MIN_DURATION, VIBRATE_MAX_DURATION);
        _m_dSharpness = vhconstrain(sharpness, VIBRATE_MIN_SHARPNESS, VIBRATE_MAX_SHARPNESS);
    }

    static void setDefault(float Frequency)
    {
        _m_dFrequency = vhconstrain(Frequency, VIBRATE_MIN_FREQUENCY, VIBRATE_MAX_FREQUENCY);
        _m_dDuration = FRQ_TO_DUR(_m_dFrequency);
    }
    static void getDefault(uint16_t &Duration, float &Intensity, float &Frequency, float &Sharpness)
    {
        Intensity = _m_dIntensity;
        Duration = _m_dDuration;
        Frequency = _m_dFrequency;
        Sharpness = _m_dSharpness;
    }

    static void getDefault(uint16_t &Duration, float &Intensity, float &Sharpness)
    {
        Intensity = _m_dIntensity;
        Duration = _m_dDuration;
        Sharpness = _m_dSharpness;
    }
    /**
     * @brief Set the Minimum Intensity
     *
     * @param minIntensity
     */
    static void setMinIntensity(float minIntensity)
    {
        _m_MinimumIntensity = minIntensity;
    }
    /**
     * @brief Set the Maximum Intensity
     *
     * @param maxIntensity
     */
    static void setMaxIntensity(float maxIntensity)
    {
        _m_MaximumIntensity = maxIntensity;
    }
    /**
     * @brief Set the Minimum Duration
     *
     * @param minDuration
     */
    static void setMinDuration(uint16_t minDuration)
    {
        _m_MinimumDuration = minDuration;
    }
    /**
     * @brief Set the Maximum Duration
     *
     * @param maxDuration
     */
    static void setMaxDuration(uint16_t maxDuration)
    {
        _m_MaximumDuration = maxDuration;
    }
    /**
     * @brief Get the Minimum Intensity
     *
     * @return float
     */
    static float getMinIntensity()
    {
        return _m_MinimumIntensity;
    }
    /**
     * @brief Get the Maximum Intensity
     *
     * @return float
     */
    static float getMaxIntensity()
    {
        return _m_MaximumIntensity;
    }
    /**
     * @brief Get the Minimum Duration
     *
     * @return uint16_t
     */
    static uint16_t getMinDuration()
    {
        return _m_MinimumDuration;
    }
    /**
     * @brief Get the Maximum Duration
     *
     * @return uint16_t
     */
    static uint16_t getMaxDuration()
    {
        return _m_MaximumDuration;
    }

    static float getMinSharpness()
    {
        return _m_MinSharpness;
    }
    static float getMaxSharpness()
    {
        return _m_MaxSharpness;
    }
    /**
     * @brief Set the Minimum and Maximum Intensity
     *
     * @param minIntensity
     * @param maxIntensity
     */
    static void setMinMaxIntensity(float minIntensity, float maxIntensity)
    {
        _m_MinimumIntensity = minIntensity;
        _m_MaximumIntensity = maxIntensity;
    }
    /**
     * @brief Set the Minimum and Maximum Duration
     *
     * @param minDuration
     * @param maxDuration
     */
    static void setMinMaxDuration(uint16_t minDuration, uint16_t maxDuration)
    {
        _m_MinimumDuration = minDuration;
        _m_MaximumDuration = maxDuration;
    }
    static void setMinValues(float minIntensity, uint16_t minDuration, float minSharpness)
    {
        _m_MinimumIntensity = vhconstrain(minIntensity, VIBRATE_MIN_INTENSITY, VIBRATE_MAX_INTENSITY);
        _m_MinimumDuration = vhconstrain(minDuration, VIBRATE_MIN_DURATION, VIBRATE_MAX_DURATION);
        _m_MinSharpness = vhconstrain(minSharpness, VIBRATE_MIN_SHARPNESS, VIBRATE_MAX_SHARPNESS);
    }
    static void setMaxValues(float maxIntensity, uint16_t maxDuration, float maxSharpness)
    {
        _m_MaximumIntensity = vhconstrain(maxIntensity, VIBRATE_MIN_INTENSITY, VIBRATE_MAX_INTENSITY);
        _m_MaximumDuration = vhconstrain(maxDuration, VIBRATE_MIN_DURATION, VIBRATE_MAX_DURATION);
        _m_MaxSharpness = vhconstrain(maxSharpness, VIBRATE_MIN_SHARPNESS, VIBRATE_MAX_SHARPNESS);
    }
    static void getMinValues(float &minIntensity, uint16_t &minDuration, float &minSharpness)
    {
        minIntensity = _m_MinimumIntensity;
        minDuration = _m_MinimumDuration;
        minSharpness = _m_MinSharpness;
    }
    static void getMaxValues(float &maxIntensity, uint16_t &maxDuration, float &maxSharpness)
    {
        maxIntensity = _m_MaximumIntensity;
        maxDuration = _m_MaximumDuration;
        maxSharpness = _m_MaxSharpness;
    }

    static void getMinFreq(float &minFreq)
    {
        minFreq = _m_MinimumFreq;
    }
    static void getMaxFreq(float &maxFreq)
    {
        maxFreq = _m_MaximumFreq;
    }
    static void setMinFreq(float minFreq)
    {
        _m_MinimumFreq = vhconstrain(minFreq, VIBRATE_MIN_FREQUENCY, VIBRATE_MAX_FREQUENCY);
    }
    static void setMaxFreq(float maxFreq)
    {
        _m_MaximumFreq = vhconstrain(maxFreq, VIBRATE_MIN_FREQUENCY, VIBRATE_MAX_FREQUENCY);
        ;
    }
};

class VIBRATE : public VHVibrate
{
};

class VH_Pcm : public VhEffect
{
    static int _mSamplerate;
    const uint8_t *mPcmArray = nullptr;
    uint16_t mLength = 0;

public:
    const uint8_t *getPcmData() const
    {
        return mPcmArray;
    }
    VH_Pcm() = default;

    VH_Pcm(const uint8_t *pcm, const uint16_t size)
        : mPcmArray(pcm), mLength(size)
    {
        m_ucharTypeId = 4;
    }

    VH_Pcm(VH_Pcm *ptr)
        : mPcmArray(ptr->mPcmArray), mLength(ptr->mLength)
    {
        m_ucharTypeId = 4;
    }

    VH_Pcm(const VH_Pcm &other)
        : mPcmArray(other.mPcmArray), mLength(other.mLength)
    {
        m_ucharTypeId = 4;
    }

    VH_Pcm &operator=(const VH_Pcm &other)
    {
        if (this != &other)
        {
            mPcmArray = other.mPcmArray;
            mLength = other.mLength;
            m_ucharTypeId = 4;
        }
        return *this;
    }
    ~VH_Pcm() = default;
    static void setSampleRate(const int val)
    {
        _mSamplerate = val;
    }

    static int getSampleRate()
    {
        return _mSamplerate;
    }

    uint8_t getFrame(unsigned long timeSinceStart) const
    {
        return getFrameAt(timeSinceStart);
    }

    unsigned long getDuration() const
    {
        if (_mSamplerate <= 0)
            return 0;
        return static_cast<unsigned long>(mLength) * (1000000UL / _mSamplerate);
    }

private:
    uint8_t getFrameAt(unsigned long microsecond) const
    {
        if (_mSamplerate <= 0 || mPcmArray == nullptr || mLength == 0)
            return 0;

        unsigned long duration = getDuration();
        unsigned long pos = microsecond % duration; // wrap after full duration
        size_t index = pos / (1000000UL / _mSamplerate);

        if (index < mLength)
        {
            return mPcmArray[index];
        }

        return 0;
    }
};

class PCM : public VH_Pcm
{
};

class VHAudio : public IVhEffect
{
private:
    float mDurationMs = -1.0f; /* -1 or VH_AUDIO_STREAM_INFINITY means infinite */
    float mIntensity = 0.0f;
    uint8_t mAudioChannel = 0; /* 0 = mix all channels, 1 = left, 2 = right. Default 0. */

public:
    VHAudio() = default;
    VHAudio(float durationMs, float intensity = 1.0f, uint8_t audioChannel = 0) : mDurationMs(durationMs), mIntensity(intensity), mAudioChannel(audioChannel) { m_ucharTypeId = 15; }
    VHAudio(const VHAudio &obj) = default;
    bool isPlaying() const { return mIntensity > 0.0f; }
    float getIntensity() const { return mIntensity; }
    float getDurationMs() const { return mDurationMs; }
    uint8_t getAudioChannel() const { return mAudioChannel; }
    bool isInfiniteDuration() const { return mDurationMs < 0.0f; }
};

class VhEffects
{
    std::vector<VhEffect *> m_listEffects;
    unsigned char mChannel;

public:
    /**
     * @brief To Add Effects
     *
     * @param effect
     */
    void Add(VhEffect *effect)
    {
        m_listEffects.push_back(effect);
    }
    /**
     * @brief To Get size of Effects
     *
     * @return int
     */
    int Length()
    {
        return m_listEffects.size();
    }
    /**
     * @brief Get A specific Effect
     *
     * @param i
     * @return VhEffect*
     */
    VhEffect *GetAt(int i)
    {
        return m_listEffects[i];
    }
    void setChannel(unsigned char channel)
    {
        mChannel = channel;
        for (VhEffect *eff : m_listEffects)
        {
            eff->setChannel(mChannel);
        }
    }

    unsigned char getChannel()
    {
        return mChannel;
    }
};

class ICustomPrim : public IVhEffect
{

public:
    virtual void setPrimitive(const char *nam, float duration, VHPrimitiveParams *param, CustomPrimCb cb, int primType = -1) = 0;
    virtual void setName(const char *) = 0;
    virtual void setPrimitive(const VHPrimitiveParams *param) = 0;
    virtual void setPrimitives(std::initializer_list<std::pair<const char *, float>> &params) = 0;
    virtual void setParameter(const char *nam, float defaultVal, float min, float max) = 0;
    virtual void setMinMaxDuration(const float defaultVal, const float min, const float max) = 0;
    virtual void getMinMaxDuration(float &defaultVal, float &min, float &max) const = 0;
    virtual bool getMinMax(const char *nam, float &defaultVal, float &min, float &max) = 0;
    virtual void setCallBack(const CustomPrimCb cb) = 0;
    virtual const char *getName() const = 0;
    virtual VHPrimitiveParams *getPrimPtr() = 0;
    virtual CustomPrimCb getCallBack() = 0;
    virtual float getPrimVal(const char *name) = 0;
    virtual float getDur() const = 0;
    virtual void setDur(const float dur) = 0;
    virtual void getSerilized(std::string &strSer) = 0;
    virtual int getPrimitiveType() const = 0;
    virtual std::string toString() = 0;
    virtual ~ICustomPrim() {};
};

/**
 * @class VHErm
 * @brief Represents an ERM (Eccentric Rotating Mass) haptic effect with Duration and Intensity.
 *
 * Similar to VHPulse but without Sharpness. This class provides controls for:
 *  - Default Intensity & Duration
 *  - Minimum & Maximum limits
 *  - Constructors for various initialization scenarios
 */
class VHErm : public VhEffect
{
    /** Default intensity and duration for ERM effect */
    static float _m_dIntensity;
    static uint16_t _m_dDuration;

    /** Minimum and maximum allowed values */
    static float _m_MinimumIntensity, _m_MaximumIntensity;
    static uint16_t _m_MinimumDuration, _m_MaximumDuration;

public:
    /**
     * @brief Returns the name of this effect.
     *
     * @return "Erm"
     */
    static std::string getName() { return "Erm"; }

    /** Instance intensity and duration */
    float m_dIntensity;
    uint16_t m_dDuration;

    /**
     * @brief Construct a new VHErm object
     *
     * @param Duration Effect duration
     * @param Intensity Effect intensity
     */
    VHErm(uint16_t Duration, float Intensity)
    {
        m_dDuration = Duration;
        m_dIntensity = Intensity;
        m_ucharTypeId = 6; // Unique ID for ERM effect
    }

    /**
     * @brief Construct a new VHErm object with only Duration.
     *
     * Intensity is taken from default value.
     *
     * @param Duration Effect duration
     */
    VHErm(uint16_t Duration)
    {
        m_dDuration = Duration;
        m_dIntensity = _m_dIntensity;
        m_ucharTypeId = 6;
    }

    /**
     * @brief Construct a new VHErm object using default values.
     */
    VHErm()
    {
        m_dIntensity = _m_dIntensity;
        m_dDuration = _m_dDuration;
        m_ucharTypeId = 6;
    }

    /**
     * @brief Copy constructor (from pointer).
     *
     * @param p Pointer to an existing VHErm object
     */
    VHErm(VHErm *p)
    {
        m_dIntensity = p->m_dIntensity;
        m_dDuration = p->m_dDuration;
        m_iDelay = p->getDelay();
        m_ucharTypeId = 6;
    }

    // ------------------------------------------------------------------------
    // DEFAULT VALUE HANDLERS
    // ------------------------------------------------------------------------

    /**
     * @brief Set the default Intensity and Duration values.
     *
     * @param Intensity Default intensity
     * @param Duration  Default duration
     */
    static void setDefault(float Intensity, uint16_t Duration)
    {
        _m_dIntensity = vhconstrain(Intensity, ERM_MIN_INTENSITY, ERM_MAX_INTENSITY);
        _m_dDuration = vhconstrain(Duration, ERM_MIN_DURATION, ERM_MAX_DURATION);
    }

    /**
     * @brief Get the default Intensity and Duration
     *
     * @param Intensity Output intensity
     * @param Duration Output duration
     */
    static void getDefault(float &Intensity, uint16_t &Duration)
    {
        Intensity = _m_dIntensity;
        Duration = _m_dDuration;
    }

    // ------------------------------------------------------------------------
    // MINIMUM / MAXIMUM INTENSITY
    // ------------------------------------------------------------------------

    /**
     * @brief Set the minimum allowed intensity.
     *
     * @param minIntensity Minimum intensity value
     */
    static void setMinIntensity(float minIntensity)
    {
        _m_MinimumIntensity = minIntensity;
    }

    /**
     * @brief Set the maximum allowed intensity.
     *
     * @param maxIntensity Maximum intensity value
     */
    static void setMaxIntensity(float maxIntensity)
    {
        _m_MaximumIntensity = maxIntensity;
    }

    /**
     * @brief Set both minimum and maximum intensity limits.
     *
     * @param minIntensity Minimum allowed intensity
     * @param maxIntensity Maximum allowed intensity
     */
    static void setMinMaxIntensity(float minIntensity, float maxIntensity)
    {
        _m_MinimumIntensity = minIntensity;
        _m_MaximumIntensity = maxIntensity;
    }

    /**
     * @brief Return the minimum allowed intensity.
     *
     * @return float min intensity
     */
    static float getMinIntensity() { return _m_MinimumIntensity; }

    /**
     * @brief Return the maximum allowed intensity.
     *
     * @return float max intensity
     */
    static float getMaxIntensity() { return _m_MaximumIntensity; }

    // ------------------------------------------------------------------------
    // MINIMUM / MAXIMUM DURATION
    // ------------------------------------------------------------------------

    /**
     * @brief Set minimum allowed duration.
     *
     * @param minDuration Minimum duration
     */
    static void setMinDuration(uint16_t minDuration)
    {
        _m_MinimumDuration = minDuration;
    }

    /**
     * @brief Set maximum allowed duration.
     *
     * @param maxDuration Maximum duration
     */
    static void setMaxDuration(float maxDuration)
    {
        _m_MaximumDuration = maxDuration;
    }

    /**
     * @brief Set both minimum and maximum duration values.
     *
     * @param minDuration Minimum duration
     * @param maxDuration Maximum duration
     */
    static void setMinMaxDuration(uint16_t minDuration, uint16_t maxDuration)
    {
        _m_MinimumDuration = minDuration;
        _m_MaximumDuration = maxDuration;
    }

    /**
     * @brief Get minimum allowed duration.
     *
     * @return uint16_t min duration
     */
    static uint16_t getMinDuration() { return _m_MinimumDuration; }

    /**
     * @brief Get maximum allowed duration.
     *
     * @return uint16_t max duration
     */
    static uint16_t getMaxDuration() { return _m_MaximumDuration; }

    // ------------------------------------------------------------------------
    // MIN & MAX COMBINED SETTERS / GETTERS
    // ------------------------------------------------------------------------

    /**
     * @brief Set minimum intensity and duration values.
     *
     * @param minIntensity Min intensity
     * @param minDuration Min duration
     */
    static void setMinValues(float minIntensity, uint16_t minDuration)
    {
        _m_MinimumIntensity = vhconstrain(minIntensity, ERM_MIN_INTENSITY, ERM_MAX_INTENSITY);
        _m_MinimumDuration = vhconstrain(minDuration, ERM_MIN_DURATION, ERM_MAX_DURATION);
    }

    /**
     * @brief Set maximum intensity and duration values.
     *
     * @param maxIntensity Max intensity
     * @param maxDuration Max duration
     */
    static void setMaxValues(float maxIntensity, uint16_t maxDuration)
    {
        _m_MaximumIntensity = vhconstrain(maxIntensity, ERM_MIN_INTENSITY, ERM_MAX_INTENSITY);
        _m_MaximumDuration = vhconstrain(maxDuration, ERM_MIN_DURATION, ERM_MAX_DURATION);
    }

    /**
     * @brief Get minimum intensity and duration values.
     *
     * @param minIntensity Output min intensity
     * @param minDuration Output min duration
     */
    static void getMinValues(float &minIntensity, uint16_t &minDuration)
    {
        minIntensity = _m_MinimumIntensity;
        minDuration = _m_MinimumDuration;
    }

    /**
     * @brief Get maximum intensity and duration values.
     *
     * @param maxIntensity Output max intensity
     * @param maxDuration Output max duration
     */
    static void getMaxValues(float &maxIntensity, uint16_t &maxDuration)
    {
        maxIntensity = _m_MaximumIntensity;
        maxDuration = _m_MaximumDuration;
    }

    static float getMinSharpness()
    {
        return 0.0;
    }
    static float getMaxSharpness()
    {
        return 0.0;
    }
};

class ERM : public VHErm
{
};

/**
 * @class VHSweep
 * @brief Sweep effect: smooth transition of intensity, frequency, and sharpness over duration with optional easing.
 */
class VHSweep : public VhEffect
{
	static float _m_dIntensity;
	static float _m_dDuration;
	static float _m_dSharpness;
	static float _m_dFrequency;
	static float _m_MinimumIntensity, _m_MaximumIntensity;
	static float _m_MinimumDuration, _m_MaximumDuration;
	static float _m_MinSharpness, _m_MaxSharpness;
	static float _m_MinimumFreq, _m_MaximumFreq;

public:
	static std::string getName() { return "Sweep"; }
	StartEndParam m_frequency, m_intensity, m_sharpness;
	float m_dDuration = 0;
	std::string Reserved;

	VHSweep(float duration, float startIntensity, float startFrequency, float startSharpness,
			float endIntensity, float endFrequency, float endSharpness,
			TransitionType transIntensity = TransitionType::Linear,
			TransitionType transFrequency = TransitionType::Linear,
			TransitionType transSharpness = TransitionType::Linear)
		: m_intensity(getIntensity(startIntensity), getIntensity(endIntensity), transIntensity),
		  m_frequency(getFrequency(startFrequency), getFrequency(endFrequency), transFrequency),
		  m_sharpness(getSharpness(startSharpness), getSharpness(endSharpness), transSharpness),
		  m_dDuration(getDuration(duration))
	{
		m_ucharTypeId = 7;
		Reserved = "";
	}

	VHSweep()
	{
		uint16_t d;
		float i, f, s;
		VHVibrate::getDefault(d, i, f, s);
		m_dDuration = (float)d;
		m_intensity = StartEndParam(i, i, TransitionType::Linear);
		m_frequency = StartEndParam(f, f, TransitionType::Linear);
		m_sharpness = StartEndParam(s, s, TransitionType::Linear);
		m_ucharTypeId = 7;
		Reserved = "";
	}

	VHSweep(VHSweep *p, unsigned char id)
	{
		m_dDuration = p->m_dDuration;
		m_intensity = p->m_intensity;
		m_frequency = p->m_frequency;
		m_sharpness = p->m_sharpness;
		m_iDelay = p->getDelay();
		m_ucharTypeId = id;
		Reserved = "";
	}

private:
	float getIntensity(float newVal) const
	{
		if (newVal < 0)
		{
			uint16_t d;
			float i, f, s;
			VHVibrate::getDefault(d, i, f, s);
			return i;
		}
		return newVal;
	}
	float getFrequency(float newVal) const
	{
		if (newVal < 0)
		{
			uint16_t d;
			float i, f, s;
			VHVibrate::getDefault(d, i, f, s);
			return f;
		}
		return newVal;
	}
	float getSharpness(float newVal) const
	{
		if (newVal < 0)
		{
			uint16_t d;
			float i, f, s;
			VHVibrate::getDefault(d, i, f, s);
			return s;
		}
		return newVal;
	}
	float getDuration(float newVal) const
	{
		if (newVal < 0)
		{
			uint16_t d;
			float i, f, s;
			VHVibrate::getDefault(d, i, f, s);
			return (float)d;
		}
		return newVal;
	}
};

class SWEEP : public VHSweep
{
};

/// @endcond