cmake_minimum_required(VERSION 3.22)

project(antihuman_bitcrush VERSION 1.0.0 LANGUAGES C CXX)

include(FetchContent)

# JUCE is fetched automatically during the first CMake configure.
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 9.0.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(JUCE)

juce_add_plugin(AntihumanBitcrush
    COMPANY_NAME "ant!human"
    BUNDLE_ID "audio.antihuman.bitcrush"
    PLUGIN_MANUFACTURER_CODE Anth
    PLUGIN_CODE Bcr1
    PRODUCT_NAME "ant!human Bitcrush"
    DESCRIPTION "Lo-fi bit depth and sample-rate reduction effect"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    FORMATS VST3 Standalone
)

target_sources(AntihumanBitcrush PRIVATE
    PluginProcessor.cpp
    PluginProcessor.h
    PluginEditor.cpp
    PluginEditor.h
)

target_compile_features(AntihumanBitcrush PRIVATE cxx_std_17)

target_compile_definitions(AntihumanBitcrush
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(AntihumanBitcrush
    PRIVATE
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
