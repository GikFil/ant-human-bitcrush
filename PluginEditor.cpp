
#include "PluginEditor.h"

namespace
{
    const juce::Colour background{ 0xff101014 };
    const juce::Colour panel{ 0xff191820 };
    const juce::Colour accent{ 0xffb66cff };
    const juce::Colour mainText{ 0xfff4effa };
    const juce::Colour mutedText{ 0xff928a9d };
    const juce::Colour outline{ 0xff393342 };
}

AntihumanBitcrushAudioProcessorEditor::AntihumanBitcrushAudioProcessorEditor(
    AntihumanBitcrushAudioProcessor& processor)
    : AudioProcessorEditor(&processor), audioProcessor(processor)
{
    setSize(510, 330);

    setupSlider(bitsSlider, " bit");
    setupSlider(rateSlider, "x");
    setupSlider(mixSlider, "%");

    bitsSlider.setRange(2.0, 16.0, 1.0);
    rateSlider.setRange(1.0, 64.0, 1.0);
    mixSlider.setRange(0.0, 100.0, 0.1);

    setupLabel(bitsLabel, "BIT DEPTH");
    setupLabel(rateLabel, "RATE REDUCTION");
    setupLabel(mixLabel, "DRY / WET");

    bitsAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.parameters, "bits", bitsSlider);
    rateAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.parameters, "rate", rateSlider);
    mixAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.parameters, "mix", mixSlider);
}

void AntihumanBitcrushAudioProcessorEditor::setupSlider(
    juce::Slider& slider, const juce::String& suffix)
{
    slider.setSliderStyle(
        juce::Slider::RotaryHorizontalVerticalDrag);

    slider.setTextBoxStyle(
        juce::Slider::TextBoxBelow, false, 88, 22);

    slider.setTextValueSuffix(suffix);

    slider.setColour(juce::Slider::rotarySliderFillColourId, accent);
    slider.setColour(juce::Slider::rotarySliderOutlineColourId, outline);
    slider.setColour(juce::Slider::thumbColourId, mainText);
    slider.setColour(juce::Slider::textBoxTextColourId, mainText);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, panel);
    slider.setColour(
        juce::Slider::textBoxOutlineColourId,
        juce::Colour(0x00000000));

    addAndMakeVisible(slider);
}

void AntihumanBitcrushAudioProcessorEditor::setupLabel(
    juce::Label& label, const juce::String& text)
{
    label.setText(text, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);

    label.setColour(juce::Label::textColourId, mutedText);

    label.setFont(
        juce::Font(juce::FontOptions(12.0f).withStyle("Bold")));

    addAndMakeVisible(label);
}

void AntihumanBitcrushAudioProcessorEditor::paint(
    juce::Graphics& g)
{
    g.fillAll(background);

    auto bounds = getLocalBounds().toFloat().reduced(12.0f);

    g.setColour(panel);
    g.fillRoundedRectangle(bounds, 14.0f);

    g.setColour(accent);
    g.setFont(
        juce::Font(juce::FontOptions(25.0f).withStyle("Bold")));

    g.drawText(
        "ant!human", 28, 22, 250, 34,
        juce::Justification::centredLeft);

    g.setColour(mutedText);
    g.setFont(
        juce::Font(juce::FontOptions(10.5f)));

    g.drawText(
        "BITCRUSH  /  DIGITAL DEGRADATION",
        30, 57, 330, 18,
        juce::Justification::centredLeft);

    g.setColour(juce::Colour(0xff302a38));

    g.drawHorizontalLine(
        88, 28.0f, static_cast<float>(getWidth() - 28));

    g.setColour(mutedText);
    g.setFont(
        juce::Font(juce::FontOptions(10.0f)));

    g.drawText(
        "LOWER VALUES = ROUGHER SOUND",
        0, getHeight() - 26,
        getWidth(), 14,
        juce::Justification::centred);
}

void AntihumanBitcrushAudioProcessorEditor::resized()
{
    const int columnWidth = getWidth() / 3;
    const int labelY = 103;
    const int sliderY = 119;
    const int sliderHeight = 165;

    bitsLabel.setBounds(0, labelY, columnWidth, 22);
    rateLabel.setBounds(columnWidth, labelY, columnWidth, 22);
    mixLabel.setBounds(columnWidth * 2, labelY, columnWidth, 22);

    bitsSlider.setBounds(
        12, sliderY, columnWidth - 24, sliderHeight);

    rateSlider.setBounds(
        columnWidth + 12, sliderY,
        columnWidth - 24, sliderHeight);

    mixSlider.setBounds(
        columnWidth * 2 + 12, sliderY,
        columnWidth - 24, sliderHeight);
}
