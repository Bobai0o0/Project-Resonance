#pragma once
#include <string>
#include <Utilities/VHUtilities.h>
#define PWM_FREQ 30000

typedef void (*TimerCb)(void *param);

#define vhconstrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

/**
 * @brief This structure holds the i2s pin configuration information
 * @note This is useful when creating new board APIs
 *
 */
typedef struct
{
    int mck_io_num = -1;        /*!< MCK in out pin. Note that ESP32 supports setting MCK on GPIO0/GPIO1/GPIO3 only*/
    int bck_io_num = -1;        /*!< BCK in out pin*/
    int ws_io_num = -1;         /*!< WS in out pin*/
    int data_out_num = -1;      /*!< DATA out pin*/
    int data_in_num = -1;       /*!< DATA in pin*/
    uint32_t samplerate = 8000; /*!< Audio sample rate*/
    uint8_t bitpersample = 16;  /*!< Audio bit per sample*/
} I2sInfo;

/**
 * @struct TaskConfig
 * @brief Holds thread/task configuration for a board.
 *
 * This structure defines the properties required to create or configure
 * a task/thread on the board, such as its name, stack size, priority,
 * core affinity, and the entry function.
 *
 * @note Useful when creating new board APIs that need to manage tasks.
 */
struct TaskConfig
{
    const char *name = nullptr;     /*!< Name of the task (for identification). */
    int stackSize = 0;              /*!< Task stack size in bytes. */
    void *param = nullptr;          /*!< Pointer to task parameter data. */
    int priority = 0;               /*!< Task priority level. */
    int8_t coreId = -1;             /*!< Core affinity:
                                         -1 → no pinning (any core),
                                          0 → pin to core 0,
                                          1 → pin to core 1. */
    void (*func)(void *) = nullptr; /*!< Task entry function pointer. */

    /**
     * @brief Construct a new TaskConfig object with specific settings.
     *
     * @param name Name of the task.
     * @param stackSize Task stack size in bytes.
     * @param param Pointer to task parameter data.
     * @param priority Task priority level.
     * @param func Pointer to the task function (entry point).
     * @param coreId Core affinity (-1 = no pinning, 0 or 1 = specific core).
     */
    TaskConfig(const char *name, int stackSize, void *param, int priority,
               void (*func)(void *), int8_t coreId = -1)
        : name(name), stackSize(stackSize), param(param), priority(priority),
          coreId(coreId), func(func) {}

    /**
     * @brief Construct a new default TaskConfig object.
     *
     * Initializes all members to default values:
     * - name = nullptr
     * - stackSize = 0
     * - param = nullptr
     * - priority = 0
     * - coreId = -1
     * - func = nullptr
     */
    TaskConfig() = default;

    /**
     * @brief Clone configuration values from another TaskConfig object.
     *
     * @param other Pointer to the source TaskConfig to copy from.
     */
    void clone(const TaskConfig *other)
    {
        name = other->name;
        stackSize = other->stackSize;
        param = other->param;
        priority = other->priority;
        coreId = other->coreId;
        func = other->func;
    }
};

/**
 * @brief Interface for board-specific task management.
 *
 * Inherit this class when implementing APIs for creating and managing
 * board threads or tasks. Provides a standard abstraction layer for
 * different board RTOS implementations.
 */
class ITaskManager
{
protected:
    TaskConfig mTaskConfig; /*!< Holds configuration details for the task. */

public:
    virtual ~ITaskManager() {}

    /**
     * @brief Create a new task using the given configuration.
     *
     * @param config Pointer to a TaskConfig object with task details.
     * @return int Implementation-specific status code (0 = success, negative = failure).
     */
    virtual int createTask(TaskConfig *config) = 0;

    /**
     * @brief Delete the currently running task.
     *
     * Cleans up task resources. Behavior depends on the board RTOS.
     */
    virtual void deleteTask() = 0;
};

/**
 * @brief Interface for board-specific mutex management.
 *
 * Inherit this class when implementing APIs for creating and managing
 * mutexes to ensure thread-safe access to shared resources.
 */
class IBoardMutex
{
public:
    IBoardMutex() {}
    virtual ~IBoardMutex() {}

    /**
     * @brief Create a new mutex object.
     *
     * @return void* Handle/pointer to the created mutex.
     */
    virtual void *createMutex() = 0;

    /**
     * @brief Acquire (lock) the mutex.
     *
     * @param xBlockTime Maximum time (in ticks) to wait for the mutex.
     *                   0 means non-blocking.
     * @return true if the mutex was successfully acquired, false otherwise.
     */
    virtual bool take(uint32_t xBlockTime = 0) = 0;

    /**
     * @brief Release (unlock) the mutex.
     */
    virtual void give() = 0;
};

/**
 * @brief Interface for board-specific queue management.
 *
 * Inherit this class when implementing APIs for creating and managing
 * inter-task queues for message passing.
 */
class IBoardQueue
{
public:
    IBoardQueue() {}
    virtual ~IBoardQueue() {}

    /**
     * @brief Create a new queue.
     *
     * @param queueSize Maximum number of items in the queue.
     * @param size Size (in bytes) of each item in the queue.
     */
    virtual void createQueue(const unsigned int queueSize, const unsigned int size) = 0;

    /**
     * @brief Receive an item from the queue.
     *
     * @param pvBuffer Pointer to buffer to store received item.
     * @param xTicksToWait Max time (in ticks) to wait for an item.
     * @return int Status code (implementation-specific).
     */
    virtual int receive(void *const pvBuffer, unsigned int xTicksToWait) = 0;

    /**
     * @brief Send an item to the queue.
     *
     * @param pvItemToQueue Pointer to the item to send.
     * @param xTicksToWait Max time (in ticks) to wait if queue is full.
     * @return int Status code (implementation-specific).
     */
    virtual int send(const void *const pvItemToQueue, unsigned int xTicksToWait) = 0;

    /**
     * @brief ISR-safe version of receive().
     *
     * @param pvBuffer Pointer to buffer to store received item.
     * @param pxTaskWoken Set to non-zero if a higher priority task was woken.
     * @return int Status code.
     */
    virtual int receiveISR(void *const pvBuffer, int *pxTaskWoken) = 0;

    /**
     * @brief ISR-safe version of send().
     *
     * @param pvItemToQueue Pointer to the item to send.
     * @param pxTaskWoken Set to non-zero if a higher priority task was woken.
     * @return int Status code.
     */
    virtual int sendISR(const void *const pvItemToQueue, int *pxTaskWoken) = 0;

    /**
     * @brief Check if the queue has items available.
     *
     * @return int Number of items in the queue (or >0 if not empty).
     */
    virtual int hasItems() = 0;
};

/**
 * @brief Interface for board-specific I2S management.
 *
 * Inherit this class when implementing APIs for initializing and
 * managing I2S peripherals (audio streaming).
 */
class IBoardI2sMan
{
public:
    IBoardI2sMan() {}

    /**
     * @brief Initialize the I2S interface with given settings.
     *
     * @param info Reference to an I2sInfo configuration object.
     */
    virtual void begin(I2sInfo &info) = 0;

    /**
     * @brief Stop the I2S interface and release resources.
     */
    virtual void stop() = 0;

    /**
     * @brief Start the I2S interface for data transfer.
     */
    virtual void start() = 0;

    /**
     * @brief Write a buffer of audio data to the I2S peripheral.
     *
     * @param data Pointer to the data buffer.
     * @param size Size of the data buffer in bytes.
     */
    virtual void write(void *data, size_t size) = 0;

    /**
     * @brief Write a single byte to the I2S peripheral.
     *
     * @param val Byte value to write.
     */
    virtual void write(uint8_t val) = 0;

    /**
     * @brief Check if the I2S interface is currently running.
     *
     * @return true if running, false otherwise.
     */
    virtual bool IsRunning() = 0;
};

/**
 * @brief Interface for board-specific timer management.
 *
 * Inherit this class when implementing APIs for creating and controlling timers.
 */
class IBoardTimer
{
public:
    /**
     * @brief Set the callback function for the timer.
     *
     * @param cb Timer callback function.
     * @param param Optional user parameter passed to the callback.
     */
    virtual void setCallback(TimerCb cb, void *param = nullptr) = 0;

    /**
     * @brief Start the timer.
     */
    virtual void startTimer() = 0;

    /**
     * @brief Stop the timer.
     */
    virtual void stopTimer() = 0;
};

/**
 * @brief Base class for all embedded board profiles.
 *
 * This class must be inherited and implemented for each supported
 * embedded board. Once implemented, its pointer should be passed
 * to the `loadBoard()` function for initialization.
 *
 * @par Implementation Note
 * You must also implement the function `loadActiveBoard()` in your
 * source (.cpp) file to return an instance of your board profile.
 *
 * @code{.cpp}
 * BoardProfile *loadActiveBoard()
 * {
 *     if (!YourBoard::getInstance())
 *     {
 *         static YourBoard yourBoardProfile;
 *         return &yourBoardProfile;
 *     }
 *     return YourBoard::getInstance();
 * }
 * @endcode
 *
 * Please check the Example @ref BoardProfile_example.ino "BoardProfile_example"
 */
class BoardProfile

{
    IBoardI2sMan *mBoardI2sMan;

protected:
    int m_iErrorLogger;           /*!< Saves the logging type */
    bool mIsBasic = false;        /*!< This not using currently */
    static BoardProfile *_mBoard; /*!< instance of Board profile */

public:
    /**
     * @brief Construct a new Board Profile object
     * This constructor will be called when using loadBoard("boardname") API
     * ref loadBoard
     */
    BoardProfile();
    /**
     * @brief Destroy the Board Profile object
     *
     */
    virtual ~BoardProfile();
    /**
     * @brief set the log type
     *
     * @param vals  Loag type
     */
    void log(int vals);
    /**
     * @brief initialize the board with given gpio pin
     *
     * @param pin gpio number
     */
    virtual void init(unsigned char pin = 0) = 0;
    /**
     * @brief initialize the board with given gpio pins. Uses in case of dual channel
     *
     * @param pin1 gpio number
     * @param pin2 gpio number
     * @param ctrlPin Any control pin
     * @parblock
     *  Control pin means , the pin that control the peripheral which is connected to the channel
     * @endparblock
     *
     * @note You can modify the logic when implementing
     */
    virtual void init(unsigned char pin1, unsigned char pin2, unsigned char ctrlPin = 0) = 0;
    /**
     * @brief Set the Output pin
     * This is used when using board basic profile
     * @param pin gpio pin
     */
    virtual void setOutput(unsigned char pin) = 0;
    /**
     * @brief Delays the operation for a specific period of time in milliseconds
     *
     * @param ms time in miliseconds
     */
    virtual void delay(unsigned int ms) = 0;
    /**
     * @brief Delays the operation for a specific period of time in microseconds
     *
     * @param us time in microseconds
     */
    virtual void delayMicroseconds(unsigned int us) = 0;
    /**
     * @brief write an 8 bit value to the gpio pin
     *
     * @param val 8-bit value
     *
     * @note Value write to the gpio pin which is set using setOutput
     *
     */
    virtual void write(unsigned char val) = 0;
    /**
     * @brief write an 8 bit value to the gpio pin
     *
     * @param pin gpio pin
     * @param value 8-bit value
     */
    virtual void writeSinglePin(unsigned char pin, unsigned char value) = 0;
    /**
     * @brief write  8 bit values to two gpio pins
     *
     * @param pin1 gpio pin1
     * @param pin2 gpio pin2
     * @param val1 8-bit value1
     * @param val2 8-bit value2
     */
    virtual void writeDualPin(unsigned char pin1, unsigned char pin2, unsigned char val1, unsigned char val2) = 0;
    /**
     * @brief returns unique id for the specific board as a string
     *
     * @return std::string unique id
     */
    virtual std::string GetMacIdEndChars() = 0;
    /**
     * @brief logs the messages.
     *
     * @param msg message
     * @param type message type
     */
    virtual void logMessages(std::string msg, LOG_TYPE type = ERROR_MSG) = 0;
    /**
     * @brief log the error using boards logging api
     *
     * @param line line number of code
     * @param file file name of code
     * @param msg message
     */
    virtual void logError(int line, const char *file, const char *msg) = 0;
    /**
     * @brief Set if board is in basic configuration or not
     *
     * @param IsBasic true/false
     */
    virtual void setBasic(bool IsBasic) { mIsBasic = IsBasic; };
    /**
     * @brief Put the pin to high value
     *
     * @param pin gpio number
     */
    virtual void high(unsigned char pin) = 0;
    /**
     * @brief Put the pin to Low value
     *
     * @param pin gpio number
     */
    virtual void low(unsigned char pin) = 0;
    /**
     * @brief Create and returns the board mutex handle
     *
     * @return IBoardMutex*
     */
    virtual IBoardMutex *createMutexHandle() = 0;
    /**
     * @brief Create and returns the board queue handle
     *
     * @return IBoardQueue*
     */
    virtual IBoardQueue *createQueueHandle() = 0;
    /**
     * @brief Get the time elapsed in microseconds since the board starts
     *
     * @return unsigned long
     */
    virtual unsigned long getTimeMicroseconds() = 0;
    /**
     * @brief Get the time elapsed in milliseconds since the board starts
     *
     * @return unsigned long
     */
    virtual unsigned long millis() = 0;
    /**
     * @brief Create a new task.
     *
     * @param func       Pointer to the task function.
     * @param name       Name of the task.
     * @param stackSize  Task stack size in bytes.
     * @param param      Parameter to pass to the task function.
     * @param priority   Task priority level.
     * @param taskHandle Handle to the created task (output).
     * @return int       Status code (0 = success, negative = error).
     */
    virtual int createTask(void (*func)(void *), const char *name,
                           int stackSize, void *param,
                           int priority, void *taskHandle) = 0;

    /**
     * @brief Create a new task using configuration.
     *
     * @param config Task configuration object.
     * @return ITaskManager* Pointer to the created task manager instance.
     */
    virtual ITaskManager *createTask(TaskConfig *config) = 0;

    /**
     * @brief Get singleton instance of BoardProfile.
     *
     * @return BoardProfile* Pointer to the board profile instance.
     */
    static BoardProfile *getInstance();

    /**
     * @brief Create a timer event for periodic or count-up operation.
     *
     * @param timer   Timer index.
     * @param freq    Frequency in Hz.
     * @param countup True for count-up mode, false for count-down mode.
     * @return IBoardTimer* Pointer to the timer object.
     */
    virtual IBoardTimer *createTimerEvents(uint8_t timer, uint32_t freq, bool countup = true) = 0;

    /**
     * @brief Get the I2S manager.
     *
     * @return IBoardI2sMan* Pointer to the I2S manager.
     */
    virtual IBoardI2sMan *getI2sMan() = 0;

    /**
     * @brief Restart the board.
     */
    virtual void restart() = 0;

    /**
     * @brief Send data using DMA.
     *
     * @param data Pointer to the data buffer.
     * @param len  Length of the data buffer in bytes.
     * @return bool True if successful, false otherwise.
     */
    virtual bool sendDataDMA(uint8_t *data, size_t len) { return false; };

    /**
     * @brief Send data over I2S channel.
     *
     * @param channel I2S channel index.
     * @param data    Pointer to the data buffer.
     * @param len     Length of the data buffer in bytes.
     */
    virtual void sendI2S(int channel, uint8_t *data, size_t len) {};

    /**
     * @brief Enable or disable DAC output.
     *
     * @param enable True to enable DAC, false to disable.
     */
    virtual void enableDac(bool enable) {};

    /**
     * @brief Save a key-value pair.
     * Example :
     *
     * Name is saved in flash memory as a key-value pair like example below
     *
     * Name  |  Age
     * ------|------
     * John  |  25
     * Leo   |  35
     * Above information we can save like
     *
     * esp32_Setup->save("John","25");
     *
     * esp32_Setup->save("Leo","35");
     *
     * This pure virtual function saves a key-value pair to the board.
     *
     * @param key The key associated with the value to be saved.
     * @param value The value to be saved.
     *
     */
    virtual void save(const char *key, const char *value) = 0;
    /**
     * @brief Reads a value associated with the given key.
     * Example:
     * We can read like below [continue from save example]
     *
     * std::string JohnAge = esp32_Setup->read("John", "20"); returns 25
     *
     * std::string LeoAge = esp32_Setup->read("Leo", "20"); returns 35
     *
     * std::string MartinAge = esp32_Setup->read("Martin", "20"); returns 20 because Martin's age is not in memory.
     *
     * This function retrieves the value associated with the specified key.
     *
     * If the key does not exist, the provided default value is returned.
     * @param key The key to look up.
     * @param defaultVal The default value to return if the key is not found.
     * @return The value associated with the key, or the default value if the key is not found.
     */
    virtual std::string read(const char *key, const char *defaultVal) = 0;
    /**
     * @brief Configure a pin with the specified mode.
     *
     * @param pin  GPIO pin number.
     * @param mode Pin mode (e.g., INPUT, OUTPUT, etc.).
     */
    virtual void pinMode(int pin, int mode) = 0;

    /**
     * @brief Configure a pin as OUTPUT
     *
     * @param pin GPIO pin number.
     */
    virtual void pinMode(int pin) = 0;

    /**
     * @brief Write a digital value to a pin.
     *
     * @param pin GPIO pin number.
     * @param val Value
     */
    virtual void digitalWrite(int pin, int val) = 0;

    /**
     * @brief Read the digital value from a pin.
     *
     * @param pin GPIO pin number.
     * @return int
     */
    virtual int digitalRead(int pin) = 0;
    /**
     * @brief Change the PWM frequency for a specific pin.
     *
     * @param pin  GPIO pin number.
     * @param freq New frequency in Hz.
     */
    virtual void changePwmFrequency(const unsigned char pin, const unsigned long freq) {};
};

namespace VH
{
    std::string to_string(float value, int decimalPoints);
    std::string to_string(float value);
    std::string to_string(int value);
    void ltrim(std::string &s);
    void rtrim(std::string &s);
    void trim(std::string &s);
    int to_Int(std::string &s);
    bool isInteger(const std::string &str);
    float to_Float(std::string &s);
    void toCharArray(std::string &s, char *arr, int len);
    std::string toLower(std::string s);
    bool equalsIgnoreCase(std::string s1, std::string s2);
    long map(long x, long in_min, long in_max, long out_min, long out_max);

};
/// @cond HIDDEN_SYMBOL
class BoardBasic
{
public:
    virtual void init() = 0;
    virtual void setOutput(unsigned char pin) = 0;
    virtual void delay(unsigned int ms) = 0;
    virtual void write(unsigned char val) = 0;
    virtual unsigned long micros() = 0;
    virtual unsigned long millis() = 0;
};
/// @endcond
class VHBoard : public BoardBasic
{
    unsigned char mPin;
    BoardProfile *board = nullptr;

public:
    /**
     * @brief Construct a new VHBoard object
     *@param pBoard pointer to the board profile
     */
    VHBoard(BoardProfile *pboard);
    /**
     * @brief initialize the VHBoard library
     *
     * @note This function will be called when you calling vh.init.
     */
    void init() override;
    /**
     * @brief Set the Output pin
     *
     * @param pin gpio pin
     */
    virtual void setOutput(unsigned char pin) override;
    /**
     * @brief Delays the operation for a specific period of time in milliseconds
     *
     * @param ms time in milliseconds
     */
    void delay(unsigned int ms) override;
    /**
     * @brief  write an 8 bit value to the gpio pin.
     *
     * @param val 8 bit value
     */
    void write(unsigned char val) override;
    /**
     * @brief Get the time elapsed in microseconds since the board starts
     *
     * @return unsigned long
     */
    unsigned long micros() override;
    /**
     * @brief Get the time elapsed in microseconds since the board starts
     *
     * @return unsigned long
     */
    unsigned long millis() override;
};