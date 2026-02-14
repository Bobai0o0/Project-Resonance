#pragma once

#include <Interface.h>
#include <memory>

/**
 * @brief Custom primitive class
 *
 * @parblock
 * Each custom primitives are saved on this class
 * @note Please note that duration is added as seperate parameter here. Don't add duration as a seperate parameter in custom parameters.
 *
 *
 * @endparblock
 */
class CustomPrim : public ICustomPrim
{
    char mName[32] = {'\0'};
    VHPrimitiveParams mParam;
    CustomPrimCb mCallBack = nullptr;
    int mPrimitiveType = -1;

public:
    /**
     * @brief Construct a new Custom Prim object
     *
     * @code {.cpp}
     * CustomPrim cust;
     * @endcode
     *
     */
    CustomPrim();
    virtual ~CustomPrim();
    /**
     * @brief Construct a new Custom primitive with given name
     *
     * @param name name of custom primitive
     * @param primType An integer representing primitive
     */
    CustomPrim(const char *name, int primType = -1);
    /**
     * @brief Construct a new Custom Primitive from CustomPrim pointer
     *
     * @param cust pointer to CustomPrim
     */
    CustomPrim(const CustomPrim *cust);
    /**
     * @brief Construct a new Custom Primitive object
     *
     * @param nam name of primitive
     * @param duration Duration of primitive
     * @param param parmeters as VHPrimitiveParams type
     * @param cb Callback need to register for this parameter
     * @param primType an integer representing primitive.
     */
    CustomPrim(const char *nam, float duration, VHPrimitiveParams *param, CustomPrimCb cb, int primType = -1);
    /**
     * @brief Set the primitives
     *
     * @param nam name of primitive
     * @param duration Duration of primitive
     * @param param parmeters as VHPrimitiveParams type
     * @param cb Callback need to register for this parameter
     * @param primType an integer representing primitive.
     */
    void setPrimitive(const char *nam, float duration, VHPrimitiveParams *param, CustomPrimCb cb, int primType = -1) override;
    /**
     * @brief Set the Name object
     * @note Change the name of primitive if needed using this function
     */
    void setName(const char *) override;
    /**
     * @brief Set the Primitive using VHPrimitiveParams pointer
     *
     * @param param  pointer of VHPrimitiveParams
     */
    void setPrimitive(const VHPrimitiveParams *param) override;
    /**
     * @brief Set the Primitives using key value pair
     *
     * @param params an initializer_list of key, value pairs
     */
    void setPrimitives(std::initializer_list<std::pair<const char *, float>> &params) override;
    /**
     * @brief Set the minimum, maximum and default value of Parameters other than duration
     *
     * @param nam name of parameter
     * @param defaultVal its default value
     * @param min its maximum value
     * @param max its minimum value
     */
    void setParameter(const char *nam, float defaultVal, float min, float max) override;
    /**
     * @brief Set the Min Max Duration of the custom primitive
     *
     * @param defaultVal default duration
     * @param min minimum duration
     * @param max maximum duration
     */
    void setMinMaxDuration(const float defaultVal, const float min, const float max) override;
    /**
     * @brief Get the Min Max Duration of the primitive
     *
     * @param defaultVal default duration
     * @param min minimum duration
     * @param max maximum duration
     * Example : <br>
     * @code {.cpp}
     * float default, min,max;
     * getMinMaxDuration(default,min,max);
     * @endcode
     *
     */
    void getMinMaxDuration(float &defaultVal, float &min, float &max) const override;
    /**
     * @brief Get the Min, Max and default values of parameters other than duration
     *
     * @param nam Name of parameter
     * @param defaultVal default duration
     * @param min minimum duration
     * @param max maximum duration
     * Example : <br>
     * @code {.cpp}
     * float default, min,max;
     * getMinMax("temperature",default,min,max);
     * @endcode
     *
     * @return true if success else false
     *
     */
    bool getMinMax(const char *nam, float &defaultVal, float &min, float &max) override;
    /**
     * @brief Refister the Call Back for the primitive
     *
     * @param cb  pointer ro callback function
     */
    void setCallBack(const CustomPrimCb cb) override;
    /**
     * @brief Use this method to inquire the name of primitive from object
     *
     * @return const char* name of the Primitive
     */
    const char *getName() const override;
    /**
     * @brief Get the VHPrimitiveParams pointer
     *
     * @return VHPrimitiveParams* pointer
     */
    VHPrimitiveParams *getPrimPtr() override;
    /**
     * @brief Get the Call Back object
     *
     * @return CustomPrimCb
     */
    CustomPrimCb getCallBack() override;
    /**
     * @brief Get the parameter value of provided parameter
     *
     * @param name  Name of parameter
     * @return float  value of parameter
     *
     * @code {.cpp}
     *  float temperature = obj.getPrimVal("temperature");
     * @endcode
     *
     * @note This function connot be used for getting  duration.
     * @see getDur()
     *
     */
    float getPrimVal(const char *name) override;
    /**
     * @brief Get the duration of primitive
     *
     * @return float duration in milliseconds
     */
    float getDur() const override;
    /**
     * @brief Set the duration of primitive
     *
     * @param dur  duration in milliseconds
     */
    void setDur(const float dur) override;
    /// @cond HIDDEN_SYMBOL
    void getSerilized(std::string &strSer) override;
    int getPrimitiveType() const override;
    std::string toString() override;
    /// @endcond
};

/**
 * @brief Class to manage and create custom haptic primitives.
 * <b>Below example Shows a sample implimentation : </b><br>
 * @ref custom_primitives.ino "custom_primitives.ino"
 */
class VHPrimitives : public IPrimitiveManager, public VHPrimitive
{
    bool mInit = false;

public:
    /**
     * @brief Initialize the VHPrimitives.
     * @code {.cpp}
     * VHPrimitives primitives;
     * @endcode
     */
    void init() override;
    /**
     * @brief register a custom primitive with a name and a callback function.
     * @param name name of the custom primitive.
     * @param cb Callback function that defines the behavior of the custom primitive.
     * @return int returns a new unique ID for the primitive if primitive not exists,
     *             otherwise returns the existing primitive ID.
     * @note You can register up to MAX_PRIMITIVES (50) custom primitives.
     *
     */
    int registerCustom(const char *name, const CustomPrimCb cb);
    /**
     * @brief Add a new custom primitive.
     * @param name Name of the custom primitive.
     * @param duration Duration of the primitive in milliseconds.
     * @param param Parameters for the primitive encapsulated in a VHPrimitiveParams object.
     * @param cb Callback function that defines the behavior of the custom primitive.
     *
     * @note If a primitive with the same name already exists, it will be updated with the new parameters and callback.
     * @note You can add up to MAX_PRIMITIVES (50) custom primitives.
     *
     */
    void addPrimitive(const char *name, float duration, VHPrimitiveParams param, CustomPrimCb cb);
    /**
     * @brief Get a custom primitive by name.
     *
     * @param name Name of the custom primitive.
     * @param duration Duration of the primitive in milliseconds.
     * @param params Parameters for the primitive encapsulated in an initializer list of name-value pairs.
     * @return std::unique_ptr<IVhEffect> A unique pointer to the requested custom primitive effect.
     * @note If the primitive with the specified name does not exist, nullptr is returned.
     *
     */
    std::unique_ptr<IVhEffect> getPrimitive(const char *name, float duration, std::initializer_list<std::pair<const char *, float>> params);
    /**
     * @brief Get the Call Back function pointer for a specific custom primitive by name.
     *
     * @param name  name of the custom primitive.
     * @return CustomPrimCb  Returns the callback function pointer if the primitive exists, otherwise returns nullptr.
     * @note If the primitive with the specified name does not exist, nullptr is returned.
     */
    CustomPrimCb getCallBack(const char *name);
    /**
     * @brief Get the Num custome Primitives added
     *
     * @return size_t returns the number of custom primitives added.
     */
    size_t getNumPrimitives() const;
    /**
     * @brief Get the primitive Name by index
     *
     * @param num Index of the primitive
     * @return std::string name of the primitive at the specified index, or an empty string if the index is out of range.
     */
    std::string getPrimName(size_t num) const override;
    /**
     * @brief Get a string representation of all registered custom primitives.
     *
     * @return std::string A string listing all registered custom primitives and their details.
     */
    std::string toString() override;
    /**
     * @brief Get the pointer to a specific custom primitive by name.
     *
     * @param name Name to be searched
     * @return CustomPrim* Pointer to the CustomPrim object if found, otherwise nullptr.
     * @note If the primitive with the specified name does not exist, nullptr is returned.
     */
    CustomPrim *getPrimitive(const char *name);
    /**
     * @brief Set the Default value for a specific primitive type and parameter type.
     *
     * @param primType  The type of primitive (e.g., PULSE, TICK, PAUSE, VIBRATE, PCM).
     * @param parType   The type of parameter (e.g., INTENSITY, DURATION, SHARPNESS, FREQUENCY, Sampling_rate).
     * @param val       The default value to be set for the specified parameter type.
     * <b>Please refer below example : </b><br>
     * @ref  default_parameters.ino  "default_parameters"
     */
    void setDefault(VHPrimitives::PrimitiveType primType, VHPrimitives::ParamType parType, float val);
    /**
     * @brief Set the minimum range for a specific primitive type and parameter type.
     *
     * @param primType  The type of primitive (e.g., PULSE, TICK, PAUSE, VIBRATE, PCM).
     * @param parType   The type of parameter (e.g., INTENSITY, DURATION, SHARPNESS, FREQUENCY, Sampling_rate).
     * @param val       The default value to be set for the specified parameter type.
     * <b>Please refer below example : </b><br>
     * @ref  default_parameters.ino  "default_parameters"
     */
    void setMin(VHPrimitives::PrimitiveType primType, VHPrimitives::ParamType parType, float val);
    /**
     * @brief Set the maximum range for a specific primitive type and parameter type.
     *
     * @param primType  The type of primitive (e.g., PULSE, TICK, PAUSE, VIBRATE, PCM).
     * @param parType   The type of parameter (e.g., INTENSITY, DURATION, SHARPNESS, FREQUENCY, Sampling_rate).
     * @param val       The default value to be set for the specified parameter type.
     * <b>Please refer below example : </b><br>
     * @ref  default_parameters.ino  "default_parameters"
     */
    void setMax(VHPrimitives::PrimitiveType primType, VHPrimitives::ParamType parType, float val);

private:
    static constexpr size_t MAX_PRIMITIVES = 50;
    size_t mNumCustPrimtives = 0;
    CustomPrim mCustPrimtives[MAX_PRIMITIVES];
    template <class T>
    void setDefaultParam(VHPrimitives::ParamType parType, float val);
    template <class T>
    void setMinMaxPar(VHPrimitives::ParamType parType, float val, bool isMin);
    void setMinMax(VHPrimitives::PrimitiveType primType, VHPrimitives::ParamType parType, float val, bool isMin);
};

using PrimitiveManager = VHPrimitives;

/**
 * @example custom_primitives.ino
 * @example default_parameters.ino
 *
 */