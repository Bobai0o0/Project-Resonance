# VectorHaptics Hackathon Guide

**Build something you can *feel*. This guide gets you from zero to haptic feedback on your ESP32 in minutes.**

---

## What Is This?

VectorHaptics is a firmware library that lets you control haptic actuators (vibration motors, LRAs, voice coils) from an ESP32. You write simple one-liners in C++ and the motor buzzes, pulses, or vibrates exactly how you want.

Think of it like calling `tone()` for sound, but for touch.

---

## What You Need

| Item | Details |
|------|---------|
| **Driver** | Silicon Labs CP210x Windows Driver |
| **Microcontroller** | TITAN Core |
| **Actuator** | LMR (Linear Magnetic Ram), LRA (Linear Resonant Actuator), ERM motor, or voice coil |
| **IDE** | VS Code + PlatformIO, or Arduino IDE |
| **Flash** | 4MB recommended |

### For information on the TITAN Core pinout, refer to the datasheet

> If your board has a DRV motor controller with an enable pin (such as the TITAN Core), you need to turn it on in `setup()`:
> ```cpp
> pinMode(32, OUTPUT);
> digitalWrite(32, HIGH);
> ```

---

## Setup (PlatformIO — Recommended)

### Step 1 — Install Tools

Install **VS Code** and add these extensions:
- PlatformIO IDE
- C/C++ Extension Pack

Install the **Silicon Labs** USB to UART Bridge VCP Driver 
- Included in this folder

### Step 2 — Create a Project

1. Open PlatformIO Home in VS Code
2. Click **New Project**
3. Board: **Espressif ESP32 Dev Module**
4. Framework: **Arduino**
5. Click **Finish**

### Step 3 — Add the Library

Download or clone the VectorHaptics library into your project's `lib/` folder.

### Step 4 — Configure `platformio.ini`

Replace the contents of `platformio.ini` with:

```ini
[platformio]
default_envs = ESP32_PICO_MINI

[env:ESP32_PICO_MINI]
platform = espressif32 @ 6.5.0
board = esp32dev
framework = arduino
monitor_speed = 115200
upload_speed = 512000
build_src_filter = +<*> -<.git/> -<.svn/> -<doc/>
board_build.partitions = no_ota.csv
build_flags =
    -Llib/Vectorhaptics/src/esp32
    -lVectorhaptics
    -Llib/VHAudioStreaming/src/esp32
    -lVHAudioStreaming
    -Llib/VHBoardProfiles/src/esp32
    -lVHBoardProfiles
    -Llib/VHDevTools/src/esp32
    -lVHDevTools
    -Llib/VHEffectReceiver/src/esp32
    -lVHEffectReceiver


```

### Step 5 - Physically configure your Dev Board

The TITAN Core Dev board has 3 Motor Driver Outputs, summarized below: 

| Channels | Device | Max Power |
|---|---|---|
| L+, L-, R+, R- | PAM8403 Dual Audio Amplifier | 5V |
| M+, M- | DRV8212 H-Bridge Motor Driver | 12 | 

The TITAN Core also has a number of GPIO pins, for more details on the pinout, please review the included TITAN Core datasheet. 

To configure the dev kit for flashing, **IO0 must be jumpered to GND**. To get a haptic effect **At least 1 Drake motor must be connected**

### Step 6 — Write Code and Upload

1. Put your code in `src/main.cpp`
2. Build
3. Short IO0 to ground on the TITAN Core 
4. Upload
5. Remove the jumper


### Arduino IDE Alternative

If you prefer the Arduino IDE over PlatformIO, use these steps. **Note:** PlatformIO is still recommended—the library is split into several modules, and PlatformIO’s `platformio.ini` and `lib/` layout handle that with less manual setup.

1. **Install the ESP32 board support**
   - Open **File → Preferences** (or **Arduino IDE → Settings** on macOS).
   - In **Additional boards manager URLs**, add:  
     `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`  
     (If you already have other URLs, add this one separated by a comma.)
   - Go to **Tools → Board → Boards Manager**, search for **“esp32”**, install **Espressif Systems** (use **2.0.14** version)

2. **Install the VectorHaptics libraries**
   - Download this repo as a ZIP and extract it.
   - Copy **all** of these folders into your Arduino **libraries** folder (e.g. `Documents/Arduino/libraries/` or `My Documents/Arduino/libraries/`):
     - `Vectorhaptics`
     - `VHAudioStreaming`
     - `VHBoardProfiles`
     - `VHDevTools`
     - `VHEffectReceiver`
   - Restart the Arduino IDE so it picks up the new libraries.

3. **Optional — for audio streaming**
   - Install **arduino-audio-tools** and **ESP32-A2DP** via **Sketch → Include Library → Manage Libraries**.

4. **Select board and port**
   - **Tools → Board → ESP32 Arduino** → choose **ESP32 Dev Module** (or the specific ESP32 board you use).
   - **Tools → Port** → select the port your ESP32 is on.

---

## Your First Sketch — Single Channel

This is the simplest possible setup. One motor, one pin, one effect.

```cpp
#include "VectorHaptics.h"
#include "ESP32Profiles.h"

VectorHaptics vh;
VHChannels channels;
ESP32Profile myBoard;

void setup() {
    Serial.begin(115200);

    channels.add(1, {25});       // Channel 1 on GPIO pin 25
    vh.init(&myBoard, {&channels});
}

void loop() {
    vh.play({PULSE(20, 1, 1)});  // Short strong pulse
    delay(1000);                  // Wait 1 second
}
```

**That's it.** Upload this, and your motor fires a pulse every second.

---

## Effect Reference — The Fun Part

These are the built-in effects you can use. Each one is a simple macro you pass into `vh.play()`.

### PULSE — Quick Tap

A short transient hit. Great for button presses, notifications, confirmations.

```cpp
PULSE(duration, intensity, sharpness)
```

| Parameter | Type | Range | Default |
|-----------|------|-------|---------|
| `duration` | ms | any | 6.25 ms |
| `intensity` | float | 0.0 - 1.0 | 0.70 |
| `sharpness` | float | 0.0 - 1.0 | 0.50 |

```cpp
vh.play(PULSE(50, 0.8, 0.5));   // 50ms, 80% intensity, medium sharpness
vh.play(PULSE(100));              // 100ms with defaults
vh.play(PULSE());                 // All defaults
```

### TICK — Sharp Click

Like PULSE but with maximum sharpness by default. Perfect for UI clicks and detent feedback.

```cpp
TICK(duration, intensity, sharpness)
```

| Parameter | Type | Range | Default |
|-----------|------|-------|---------|
| `duration` | ms | any | 6.25 ms |
| `intensity` | float | 0.0 - 1.0 | 0.70 |
| `sharpness` | float | 0.0 - 1.0 | 1.00 |

```cpp
vh.play(TICK(30, 0.9, 1.0));    // Sharp 30ms click
```

### VIBRATE — Sustained Vibration

Continuous vibration at a specific frequency. Use for ongoing feedback, alerts, or texture simulation.

```cpp
VIBRATE(duration, intensity, frequency, sharpness)
```

| Parameter | Type | Range | Default |
|-----------|------|-------|---------|
| `duration` | ms | any | ~0.6 ms |
| `intensity` | float | 0.0 - 1.0 | 0.70 |
| `frequency` | Hz | any | 160 Hz |
| `sharpness` | float | 0.0 - 1.0 | 0.00 |

```cpp
vh.play(VIBRATE(300, 0.7, 160.0, 0.3));  // 300ms vibration at 160Hz
vh.play(VIBRATE(1000, 1, 100, 1));        // 1 second, full intensity, 100Hz
```

### ERM — Constant Motor Spin

Simple constant-intensity vibration, like a phone buzzer. If your channel has two pins (H-bridge), negative intensity reverses direction.
Can only be added to M channel
> **Note:** ERM can only be used on a DRV-enabled channel.

```cpp
ERM(duration, intensity)
```

| Parameter | Type | Range | Default |
|-----------|------|-------|---------|
| `duration` | ms | any | 1000 ms |
| `intensity` | float | -1.0 - 1.0 | 1.00 |

```cpp
vh.play(ERM(500, 0.6));   // Buzz for 500ms at 60%
```

### PAUSE — Silent Gap

A timed gap of silence. Only useful inside effect sequences (see below).

```cpp
PAUSE(duration)
```

| Parameter | Type | Default |
|-----------|------|---------|
| `duration` | ms | 500 ms |

### SWEEP — Smooth Transitions

Smoothly ramps intensity, frequency, and sharpness from start to end values over time. Each parameter gets its own easing curve.

```cpp
SWEEP(duration,
      startIntensity, startFrequency, startSharpness,
      endIntensity,   endFrequency,   endSharpness,
      easingIntensity, easingFrequency, easingSharpness)
```

```cpp
// Ramp from quiet/low to loud/high over 1 second
vh.play(SWEEP(
    1000,
    0.3, 80.0, 0.0,                   // Start values
    1.0, 300.0, 0.8,                   // End values
    TransitionType::EaseInOutCubic,    // Intensity easing
    TransitionType::Linear,            // Frequency easing
    TransitionType::EaseOutQuad        // Sharpness easing
));
```

**Available easing types:** `Linear`, `EaseInSine`, `EaseOutSine`, `EaseInOutSine`, `EaseInQuad`, `EaseOutQuad`, `EaseInOutQuad`, `EaseInCubic`, `EaseOutCubic`, `EaseInOutCubic`, `EaseInExpo`, `EaseOutExpo`, `EaseInBounce`, `EaseOutBounce`, `EaseInOutBounce`, and more (30 total).

---

## Playing Multiple Effects in Sequence

Chain effects together by passing them as a list. They play one after another.

```cpp
vh.play({
    PULSE(50, 0.8, 0.5),
    PAUSE(100),
    TICK(30, 1.0, 1.0),
    PAUSE(100),
    VIBRATE(200, 0.6, 160.0, 0.2)
});
```

This creates a "pulse - wait - click - wait - vibrate" pattern.

---

## Multi-Channel — Control Multiple Motors

You can wire up multiple motors on different GPIO pins and control them independently or as groups.

### Setting Up Multiple Channels

```cpp
#include "VectorHaptics.h"
#include "ESP32Profiles.h"

VectorHaptics vh;
VHChannels channels;
ESP32Profile myBoard;

void setup() {
    Serial.begin(115200);
    
    // If your board has a DRV enable pin:
    pinMode(32, OUTPUT);
    digitalWrite(32, HIGH);

    // Define channels with tags
    channels.add(1, {25}, {"Left", "Hand", "All"});   // GPIO 25
    channels.add(2, {26}, {"Right", "Hand", "All"});  // GPIO 26
    channels.add(3, {27, 33}, {"Body", "All"});       // GPIO 27+33 (H-bridge)

    vh.init(&myBoard, {&channels});
}
```

### Targeting Channels

There are three ways to tell VectorHaptics which motor(s) to use:

**1. By channel number:**
```cpp
vh.play(PULSE(50, 0.8, 0.5), 1);   // Channel 1 only
vh.play(PULSE(50, 0.8, 0.5), 2);   // Channel 2 only
```

**2. By tag (most useful):**
```cpp
vh.play(VIBRATE(1000, 1, 100, 1), "Left");  // Only channels tagged "Left"
vh.play(TICK(10, 1, 1), "Hand");              // All channels tagged "Hand" (1 and 2)
vh.play(TICK(10, 1, 1), "All");               // All channels tagged "All" (1, 2, and 3)
```

**3. Tag logic (AND / OR):**
```cpp
vh.play(PULSE(50), "Left | Body");      // Channels tagged "Left" OR "Body"
vh.play(PULSE(50), "Left & Hand");      // Channels tagged "Left" AND "Hand"
```

---

## Non-Blocking Playback (Queue)

`vh.play()` **blocks** — your code waits until the effect finishes. If you need your `loop()` to keep running (reading sensors, handling input, etc.), use `vh.queue()` instead.

```cpp
void setup() {
    // ... init code ...

    // Queue effects (does NOT play yet)
    vh.queue({
        VIBRATE(500, 0.8, 160.0, 0.3),
        PAUSE(200),
        PULSE(50, 1.0, 0.7)
    }, 0, APPEND_EFFECT);    // 0 = channel number, APPEND_EFFECT = add to queue
}

void loop() {
    // Render one step per loop — effects play in background
    if (!vh.queueIsEmpty(0)) {
        vh.renderQueue(0);
    }

    // Your other code keeps running here
    // readSensors();
    // handleButtons();
}
```

`APPEND_EFFECT` adds to the end of the queue. Use `REPLACE_EFFECT` to clear the queue and start fresh.

---

## Audio-to-Haptics (Bluetooth Streaming)

This is one of the coolest features: connect your ESP32 as a Bluetooth speaker, and the audio signal drives your haptic motors in real time.

**Extra libraries needed:** `arduino-audio-tools` and `ESP32-A2DP`

```cpp
#include "VectorHaptics.h"
#include "ESP32Profiles.h"
#include "VHAudioStreaming.h"
#include <AudioTools.h>
#include <BluetoothA2DPSink.h>

#define BUFFER_SIZE 4096

int16_t bData[BUFFER_SIZE] = {0};
VectorHaptics vh;
VHChannels ch;
BluetoothA2DPSink a2dp_sink;
VHAudioStreaming audioStream;
ESP32Profile myBoard;
VHRingBuffer<int16_t> ringBuffer(bData, BUFFER_SIZE);

// Called automatically when Bluetooth audio data arrives
void read_data_stream(const uint8_t *data, uint32_t length) {
    const int16_t *samples = reinterpret_cast<const int16_t *>(data);
    size_t sampleCount = length / sizeof(int16_t);
    audioStream.write(samples, sampleCount);
}

void setup() {
    Serial.begin(115200);

    // Bluetooth audio setup
    a2dp_sink.set_stream_reader(read_data_stream, false);
    a2dp_sink.start("MyHapticDevice", false);  // Bluetooth name

    // Channels that support audio streaming
    ch.add(1, {25}, {"Left"}, {VIBRATE | PCM | AUDIO_STREAM});
    ch.add(2, {26}, {"Right"}, {VIBRATE | PCM | AUDIO_STREAM});

    audioStream.init(&ringBuffer);
    vh.init(&myBoard, {&ch, &audioStream});
    vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY));   // Start streaming
}

void loop() {
    delay(1);
}
```

Pair your phone to the ESP32 via Bluetooth, play music, and your motors vibrate to the beat.

### Audio stream syntax reference

The **`AUDIO_STREAM`** effect is a macro that takes one to three arguments. It controls *when* and *how* the incoming audio stream is played as haptics.

#### Signature

```cpp
AUDIO_STREAM(duration [, intensity [, audioChannel]])
```

| Parameter       | Type   | Meaning |
|----------------|--------|---------|
| `duration`     | float  | How long to play the stream, in **milliseconds**. Use the constant **`VH_AUDIO_STREAM_INFINITY`** (or `-1.0f`) to run until you stop it. |
| `intensity`    | float  | **0.0–1.0**. Strength of the haptic output. Omitted = **1.0**. Use **0.0** to **stop** the stream. |
| `audioChannel` | uint8_t| Which audio channel drives haptics: **0** = mix all (default), **1** = first (e.g. left), **2** = second (e.g. right), etc. |

#### Overloads and examples

| Call | Meaning |
|------|--------|
| `AUDIO_STREAM(duration)` | Play for `duration` ms at full intensity, mixed stereo. |
| `AUDIO_STREAM(duration, intensity)` | Same, but scale haptic strength by `intensity`. |
| `AUDIO_STREAM(duration, intensity, audioChannel)` | Same, but use a specific audio channel (0 = mix, 1 = first, 2 = second, …). |

```cpp
// Run until stopped, full intensity, mixed channels (typical for “music to haptics”)
vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY));

// Run for 5 seconds at 50% intensity
vh.play(AUDIO_STREAM(5000, 0.5));

// Stop streaming (intensity = 0)
vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY, 0.0));

// Left-only / right-only (e.g. for two motors)
vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY, 1.0, 1));  // first channel (e.g. left)
vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY, 1.0, 2));  // second channel (e.g. right)
```

#### Constants

- **`VH_AUDIO_STREAM_INFINITY`** — Use as `duration` to mean “no time limit”; the stream keeps playing until you call `AUDIO_STREAM(..., 0.0)` or otherwise stop it.

#### Channel and buffer setup

1. **Channels** that should receive the audio stream must support it. When adding a channel, include **`AUDIO_STREAM`** (and usually **`VIBRATE`** and **`PCM`**) in the capabilities:

   ```cpp
   ch.add(1, {25}, {"Left"}, {VIBRATE | PCM | AUDIO_STREAM});
   ch.add(2, {26}, {"Right"}, {VIBRATE | PCM | AUDIO_STREAM});
   ```

2. **Ring buffer** — Audio samples are pushed into a ring buffer; the library pulls from it for playback. Typical setup with **int16** samples (e.g. from A2DP):

   ```cpp
   #define BUFFER_SIZE 4096
   int16_t bData[BUFFER_SIZE] = {0};
   VHRingBuffer<int16_t> ringBuffer(bData, BUFFER_SIZE);

   // In setup(), after creating channels and audioStream:
   audioStream.init(&ringBuffer);
   vh.init(&myBoard, {&ch, &audioStream});
   ```

3. **Feeding the stream** — In your Bluetooth (or other) audio callback, push samples into the streaming layer:

   ```cpp
   void read_data_stream(const uint8_t *data, uint32_t length) {
       const int16_t *samples = reinterpret_cast<const int16_t *>(data);
       size_t sampleCount = length / sizeof(int16_t);
       audioStream.write(samples, sampleCount);
   }
   ```

4. **Optional:** **`audioStream.setMaxOutSampleRate(uint32_t sr)`** — Caps the output sample rate (default 16000 Hz); the input stream is resampled if needed. **`audioStream.enableMixing(channel, mixType)`** — Enables mixing of streamed audio with other primitives on a channel (`NO_MIX`, `INTERRUPT_MIX`, `LARGESTVAL_MIX`, `ADDITIVE_MIX`, `CUSTOM_MIX`).

#### Mixing streamed audio with primitives

You can combine **live audio streaming** with **one-shot effects** (PULSE, TICK, VIBRATE, etc.) on the same channel. Call **`audioStream.enableMixing(channelNumber, mixType)`** before starting the stream; then both the stream and any `vh.play()` primitives on that channel are mixed according to `mixType`:

| Mix type | Behavior |
|----------|----------|
| `NO_MIX` | No mixing; stream and primitives don’t combine (default). |
| `INTERRUPT_MIX` | Primitive temporarily overrides or interrupts the stream. |
| `LARGESTVAL_MIX` | Output is the larger of the stream value and the primitive value at each sample. |
| `ADDITIVE_MIX` | Stream + primitive, capped at max (255). Stronger combined effect. |

**Example:** Stream Bluetooth audio on channel 1 and add a button-style click on top using additive mixing:

```cpp
// In setup(), after ch.add(...) and audioStream.init(...):

// Enable additive mixing on channel 1 so stream + primitives are summed
audioStream.enableMixing(1, ADDITIVE_MIX);

vh.init(&myBoard, {&ch, &audioStream});
vh.play(AUDIO_STREAM(VH_AUDIO_STREAM_INFINITY), 1);   // Start stream on channel 1
```

Later, in response to a button press (or in `loop()`), play a primitive on the same channel — it will be mixed with the stream:

```cpp
// User presses a button: add a sharp click on top of the music
vh.play(TICK(30, 0.9, 1.0), 1);   // Channel 1: stream + tick mixed
```

For **left/right** setups, enable mixing per channel:

```cpp
audioStream.enableMixing(1, LARGESTVAL_MIX);   // Left: take louder of stream vs primitive
audioStream.enableMixing(2, ADDITIVE_MIX);     // Right: add stream + primitive
```

---

## Serial Debug Tool (DevTools)

Send effect commands over the serial monitor to test things without re-uploading code.

```cpp
#include "VectorHaptics.h"
#include "ESP32Profiles.h"
#include "VHDevTools.h"

VectorHaptics vh;
VHChannels channels;
VHDevTools devTools;
ESP32Profile myBoard;

void setup() {
    Serial.begin(115200);

    channels.add(1, {25}, {"main"});
    vh.init(&myBoard, {&channels, &devTools});
    vh.setVerbosity(VERBOSE);

    Serial.println("Type commands in the Serial Monitor:");
    Serial.println("  PULSE 50 0.8 0.5");
    Serial.println("  VIBRATE 300 0.7 160 0.3");
    Serial.println("  CHNL 1; TICK 30 1.0 1.0");
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.length() > 0) {
            vh.executeStringCommand(std::string(input.c_str()));
        }
    }
}
```

Open the Serial Monitor (115200 baud), type `PULSE 50 0.8 0.5`, hit enter, and feel the motor respond.

---

## Quick Recipe Book

Here are copy-paste patterns for common hackathon scenarios:

### Button Press Feedback
```cpp
vh.play(TICK(30, 0.9, 1.0));
```

### Success / Confirmation
```cpp
vh.play({PULSE(40, 0.6, 0.5), PAUSE(80), PULSE(40, 1.0, 0.8)});
```

### Error / Warning
```cpp
vh.play({VIBRATE(200, 1.0, 160.0, 0.5), PAUSE(100), VIBRATE(200, 1.0, 160.0, 0.5)});
```

### Heartbeat
```cpp
vh.play({PULSE(60, 0.7, 0.8), PAUSE(120), PULSE(80, 1.0, 0.8), PAUSE(600)});
```

### Ramp Up (alert escalation)
```cpp
vh.play(SWEEP(
    2000,
    0.0, 80.0, 0.0,
    1.0, 300.0, 0.8,
    TransitionType::EaseInCubic,
    TransitionType::Linear,
    TransitionType::Linear
));
```

### Fade Out
```cpp
vh.play(SWEEP(
    1000,
    1.0, 160.0, 0.3,
    0.0, 160.0, 0.3,
    TransitionType::EaseOutQuad,
    TransitionType::Linear,
    TransitionType::Linear
));
```

### Continuous Background Vibration (non-blocking)
```cpp
vh.queue({VIBRATE(5000, 0.4, 120.0, 0.2)}, 0, APPEND_EFFECT);
// Call vh.renderQueue(0) in loop()
```

---

## Logging / Verbosity

Control how much debug output goes to Serial:

```cpp
vh.setVerbosity(SILENT);        // No output
vh.setVerbosity(ERROR_ONLY);    // Errors only
vh.setVerbosity(INFO_ONLY);     // Standard messages
vh.setVerbosity(WARNING_ONLY);  // Warnings only
vh.setVerbosity(VERBOSE);       // Everything (best for debugging)
```

---

## Common Mistakes

| Problem | Fix |
|---------|-----|
| Nothing happens | Check wiring. Make sure the driver enable pin is HIGH if required. |
| `play()` freezes my loop | `play()` is blocking. Use `queue()` + `renderQueue()` for non-blocking. |
| Wrong motor vibrates | Double-check which GPIO pin maps to which channel number. |
| Intensity seems off | Intensity is 0.0 to 1.0 (float), not 0 to 255. |
| Build errors about missing libs | Make sure all library folders are in `lib/` and `platformio.ini` is configured correctly. |
| Audio streaming not working | Install `arduino-audio-tools` and `ESP32-A2DP` separately. |

---

## API Quick Reference

### Core Methods

| Method | What It Does |
|--------|-------------|
| `vh.init(board, {&channels})` | Initialize the library |
| `vh.play(EFFECT(...))` | Play one effect (blocking) |
| `vh.play(EFFECT(...), channel)` | Play on a specific channel number |
| `vh.play(EFFECT(...), "tag")` | Play on channels matching a tag |
| `vh.play({EFFECT1, EFFECT2, ...})` | Play a sequence (blocking) |
| `vh.queue({EFFECTS}, ch, APPEND_EFFECT)` | Queue effects (non-blocking) |
| `vh.renderQueue(ch)` | Render one step of the queue |
| `vh.queueIsEmpty(ch)` | Check if queue is done |
| `vh.stop()` | Stop all playback |
| `vh.playbackStatus()` | Check if anything is playing |
| `vh.setVerbosity(level)` | Set debug output level |
| `vh.executeStringCommand(cmd)` | Run a DevTools text command |

### Effects

| Effect | Signature | Use Case |
|--------|-----------|----------|
| `PULSE` | `(duration, intensity, sharpness)` | Button taps, notifications |
| `TICK` | `(duration, intensity, sharpness)` | UI clicks, detent feedback |
| `VIBRATE` | `(duration, intensity, frequency, sharpness)` | Alerts, sustained feedback |
| `ERM` | `(duration, intensity)` | Simple motor buzz |
| `PAUSE` | `(duration)` | Gap in a sequence |
| `SWEEP` | `(dur, startI, startF, startS, endI, endF, endS, easeI, easeF, easeS)` | Ramps, fades, transitions |
| `AUDIO_STREAM` | `(duration , intensity , audioChannel)` | Live audio to haptics; use duration = `VH_AUDIO_STREAM_INFINITY` for endless stream |
| `PCM` | `(pcm_array, size)` | Raw waveform playback |

All parameters are optional except where noted — sensible defaults are provided.

---

Good luck at the hackathon. Go build something people can feel.
