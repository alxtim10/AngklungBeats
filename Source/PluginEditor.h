/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PadaengAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    PadaengAudioProcessorEditor (PadaengAudioProcessor&);
    ~PadaengAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
    void presetChanged(juce::String item);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    double aA = 0.1, dA = 0.1, sA = 1.0, rA = 0.1;
    double aB = 0.1, dB = 0.1, sB = 1.0, rB = 0.1;
    bool isA = false;
    bool isASaved = false;
    bool isB = false;
    bool isBSaved = false;
    int xA = 2;
    int xB = 2;

    juce::Slider AttackSlider;

    juce::Slider DecaySlider;

    juce::Slider SustainSlider;

    juce::Slider ReleaseSlider;

    juce::Label intensityLabel;
    juce::ComboBox menu;

    juce::Label presetLabel;
    juce::ComboBox menuPreset;


    juce::GroupComponent AttackBorder;
    juce::GroupComponent DecayBorder;
    juce::GroupComponent SustainBorder;
    juce::GroupComponent ReleaseBorder;

    juce::TextButton helpButton;
    juce::TextButton resetButton;
    juce::TextButton backButton;

    juce::TextButton aPresetButton;
    juce::TextButton bPresetButton;
    juce::TextButton saveButton;

    juce::Label helpLabel1;
    juce::Label helpLabel2;
    juce::Label helpLabel3;
    juce::Label helpLabel4;
    juce::Label helpLabel5;
    juce::Label helpLabel6;
    juce::Label helpLabel7;

    juce::Label notificationLabel;

    PadaengAudioProcessor& audioProcessor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PadaengAudioProcessorEditor)
};
