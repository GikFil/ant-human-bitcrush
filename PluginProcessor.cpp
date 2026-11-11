#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>

AntihumanBitcrushAudioProcessor::AntihumanBitcrushAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
AntihumanBitcrushAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "bits", 1 }, "Bit depth", 2, 16, 8));

    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID { "rate", 1 }, "Rate reduction", 1, 64, 1));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "mix", 1 }, "Mix", 0.0f, 100.0f, 100.0f));

    return { params.begin(), params.end() };
}

const juce::String AntihumanBitcrushAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

void AntihumanBitcrushAudioProcessor::prepareToPlay(double sampleRate, int)
{
    heldSamples.assign(static_cast<size_t>(getTotalNumOutputChannels()), 0.0f);
    holdCounter = 0;

    smoothedMix.reset(sampleRate, 0.02);
    const auto* mixParameter = parameters.getRawParameterValue("mix");
    const float initialMix = mixParameter != nullptr ? mixParameter->load() / 100.0f : 1.0f;
    smoothedMix.setCurrentAndTargetValue(initialMix);
}

void AntihumanBitcrushAudioProcessor::releaseResources()
{
}

bool AntihumanBitcrushAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    const auto input = layouts.getMainInputChannelSet();
    const auto output = layouts.getMainOutputChannelSet();

    if (input != output)
        return false;

    return input == juce::AudioChannelSet::mono()
        || input == juce::AudioChannelSet::stereo();
}

void AntihumanBitcrushAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                                    juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const int inputChannels = getTotalNumInputChannels();
    const int outputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();

    for (int channel = inputChannels; channel < outputChannels; ++channel)
        buffer.clear(channel, 0, numSamples);

    const auto* bitsParameter = parameters.getRawParameterValue("bits");
    const auto* rateParameter = parameters.getRawParameterValue("rate");
    const auto* mixParameter = parameters.getRawParameterValue("mix");

    const int bitDepth = bitsParameter != nullptr
        ? juce::jlimit(2, 16, static_cast<int>(std::round(bitsParameter->load())))
        : 8;
    const int rateReduction = rateParameter != nullptr
        ? juce::jlimit(1, 64, static_cast<int>(std::round(rateParameter->load())))
        : 1;
    const float targetMix = mixParameter != nullptr
        ? juce::jlimit(0.0f, 1.0f, mixParameter->load() / 100.0f)
        : 1.0f;

    smoothedMix.setTargetValue(targetMix);

    // Signed PCM-style quantisation. Lower bit depth means fewer available levels.
    const float quantisationScale = std::ldexp(1.0f, bitDepth - 1);
    const float maxPositive = 1.0f - (1.0f / quantisationScale);

    if (static_cast<int>(heldSamples.size()) < inputChannels)
        heldSamples.resize(static_cast<size_t>(inputChannels), 0.0f);

    // Sample-and-hold: refresh the quantised sample once every N input samples.
    for (int sample = 0; sample < numSamples; ++sample)
    {
        if (holdCounter == 0)
        {
            for (int channel = 0; channel < inputChannels; ++channel)
            {
                const float input = juce::jlimit(-1.0f, maxPositive,
                                                 buffer.getSample(channel, sample));
                heldSamples[static_cast<size_t>(channel)] =
                    std::round(input * quantisationScale) / quantisationScale;
            }
        }

        const float mix = smoothedMix.getNextValue();

        for (int channel = 0; channel < inputChannels; ++channel)
        {
            const float dry = buffer.getSample(channel, sample);
            const float wet = heldSamples[static_cast<size_t>(channel)];
            buffer.setSample(channel, sample, dry + (wet - dry) * mix);
        }

        ++holdCounter;
        if (holdCounter >= rateReduction)
            holdCounter = 0;
    }
}

juce::AudioProcessorEditor* AntihumanBitcrushAudioProcessor::createEditor()
{
    return new AntihumanBitcrushAudioProcessorEditor(*this);
}

void AntihumanBitcrushAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    if (auto state = parameters.copyState().createXml())
        copyXmlToBinary(*state, destData);
}

void AntihumanBitcrushAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary(data, sizeInBytes))
    {
        if (xml->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AntihumanBitcrushAudioProcessor();
}
