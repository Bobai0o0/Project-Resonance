/**
 * Linker stub: ensures Arduino IDE links the ESP32 core's Preferences and I2S
 * when using the precompiled libVHBoardProfiles.a. Do not remove from dist.
 */
#include <Preferences.h>
#include <I2S.h>

#ifndef ARDUINO
#error "LinkerDeps.cpp is for Arduino/ESP32 core only."
#endif

// Reference core types so the linker pulls in Preferences and I2S from the core.
// This file is compiled by the IDE; the resulting .o triggers linking of core libs.
void vhboardprofiles_ensure_preferences_i2s_linkage(void) {
    Preferences prefs;
    (void)prefs;
    (void)&I2S;
}
