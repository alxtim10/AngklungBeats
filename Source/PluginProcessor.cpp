/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PadaengAudioProcessor::PadaengAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    FormatManager.registerBasicFormats();
    for (int i = 0; i < NumVoices; i++) {
        mSampler.addVoice(new juce::SamplerVoice());
    }

    inputSample = std::make_unique<juce::MemoryInputStream>(BinaryData::AngklungMed3_wav, BinaryData::AngklungMed3_wavSize, false);
    FormatReader = FormatManager.createReaderFor(std::move(inputSample));

    juce::BigInteger range;
    range.setRange(0, 128, true);

    mSampler.addSound(new juce::SamplerSound("Sample", *FormatReader, range, 60,
        0.1, 0.1, 15));
}

PadaengAudioProcessor::~PadaengAudioProcessor()
{
}

//==============================================================================
const juce::String PadaengAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PadaengAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PadaengAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PadaengAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PadaengAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PadaengAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PadaengAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PadaengAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PadaengAudioProcessor::getProgramName (int index)
{
    return {};
}

void PadaengAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PadaengAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
    updateADSR();
}

void PadaengAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PadaengAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PadaengAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool PadaengAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PadaengAudioProcessor::createEditor()
{
    return new PadaengAudioProcessorEditor (*this);
}

//==============================================================================
void PadaengAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PadaengAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void PadaengAudioProcessor::updateADSR()
{
    for (int i = 0; i < mSampler.getNumSounds(); i++) {

        if (auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get())) {
            sound->setEnvelopeParameters(ADSRParams);
        }
    }
}

void PadaengAudioProcessor::updateSound(juce::String item)
{

    auto attack = getADSRParams().attack;
    auto decay = getADSRParams().decay;
    auto sustain = getADSRParams().sustain;
    auto release = getADSRParams().release;

    mSampler.clearSounds();
    if (item == "Low") {
        inputSample = std::make_unique<juce::MemoryInputStream>(BinaryData::AngklungLow3_wav, BinaryData::AngklungLow3_wavSize , false);
    }
    else if (item == "Medium") {
        inputSample = std::make_unique<juce::MemoryInputStream>(BinaryData::AngklungMed3_wav, BinaryData::AngklungMed3_wavSize, false);
    }
    else if (item == "High") {
        inputSample = std::make_unique<juce::MemoryInputStream>(BinaryData::AngklungHigh3_wav, BinaryData::AngklungHigh3_wavSize, false);
    }

    FormatReader = FormatManager.createReaderFor(std::move(inputSample));

    juce::BigInteger range;
    range.setRange(0, 128, true);

    mSampler.addSound(new juce::SamplerSound("Sample", *FormatReader, range, 60,
        0.1, 0.1, 15));


    getADSRParams().attack = attack;
    getADSRParams().decay = decay;
    getADSRParams().sustain = sustain;
    getADSRParams().release = release;

    updateADSR();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PadaengAudioProcessor();
}
