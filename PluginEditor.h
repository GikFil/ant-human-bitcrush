#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class AntihumanBitcrushAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AntihumanBitcrushAudioProcessorEditor(AntihumanBitcrushAudioProcessor&);
    ~AntihumanBitcrushAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    AntihumanBitcrushAudioProcessor& audioProcessor;

    juce::Slider bitsSlider;
    juce::Slider rateSlider;
    juce::Slider mixSlider;

    juce::Label bitsLabel;
    juce::Label rateLabel;
    juce::Label mixLabel;

    std::unique_ptr<SliderAttachment> bitsAttachment;
    std::unique_ptr<SliderAttachment> rateAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;

    void setupSlider(juce::Slider&, const juce::String& suffix);
    void setupLabel(juce::Label&, const juce::String& text);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AntihumanBitcrushAudioProcessorEditor)
};
