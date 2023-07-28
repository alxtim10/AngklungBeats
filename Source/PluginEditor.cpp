/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <windows.h>

//==============================================================================
PadaengAudioProcessorEditor::PadaengAudioProcessorEditor (PadaengAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
        setSize (900, 400);
        
        #pragma region Slider
        addAndMakeVisible(AttackBorder);
        AttackBorder.setText("Attack");
        AttackBorder.setTextLabelPosition(juce::Justification::centredTop);
        AttackBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::antiquewhite);
        AttackBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::sandybrown);

        addAndMakeVisible(DecayBorder);
        DecayBorder.setText("Decay");
        DecayBorder.setTextLabelPosition(juce::Justification::centredTop);
        DecayBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::antiquewhite);
        DecayBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::sandybrown);

        addAndMakeVisible(SustainBorder);
        SustainBorder.setText("Sustain");
        SustainBorder.setTextLabelPosition(juce::Justification::centredTop);
        SustainBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::antiquewhite);
        SustainBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::sandybrown);

        addAndMakeVisible(ReleaseBorder);
        ReleaseBorder.setText("Release");
        ReleaseBorder.setTextLabelPosition(juce::Justification::centredTop);
        ReleaseBorder.setColour(juce::GroupComponent::ColourIds::textColourId, juce::Colours::antiquewhite);
        ReleaseBorder.setColour(juce::GroupComponent::ColourIds::outlineColourId, juce::Colours::sandybrown);

        AttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        AttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
        AttackSlider.setTextValueSuffix("s");
        AttackSlider.setValue(0.1);
        AttackSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::burlywood);
        AttackSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::sandybrown);
        AttackSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::antiquewhite);
        AttackSlider.setRange(0.0f, 5.0f, 0.01f);
        AttackSlider.addListener(this);
        addAndMakeVisible(AttackSlider);

        DecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        DecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
        DecaySlider.setTextValueSuffix("s");
        DecaySlider.setValue(0.1);
        DecaySlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::burlywood);
        DecaySlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::sandybrown);
        DecaySlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::antiquewhite);
        DecaySlider.setRange(0.0f, 5.0f, 0.01f);
        DecaySlider.addListener(this);
        addAndMakeVisible(DecaySlider);


        SustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        SustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
        SustainSlider.setValue(1);
        SustainSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::burlywood);
        SustainSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::sandybrown);
        SustainSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::antiquewhite);
        SustainSlider.setRange(0.0f, 1.0f, 0.01f);
        SustainSlider.addListener(this);
        addAndMakeVisible(SustainSlider);


        ReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        ReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
        ReleaseSlider.setTextValueSuffix("s");
        ReleaseSlider.setValue(0.1);
        ReleaseSlider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::burlywood);
        ReleaseSlider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::sandybrown);
        ReleaseSlider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::antiquewhite);
        ReleaseSlider.setRange(0.0f, 5.0f, 0.01f);
        ReleaseSlider.addListener(this);
        addAndMakeVisible(ReleaseSlider);
        #pragma endregion

        #pragma region MenuDropdown
        addAndMakeVisible(intensityLabel);
        intensityLabel.setFont(17.0f);
        intensityLabel.setText("Shake Intensity", juce::dontSendNotification);
        intensityLabel.attachToComponent(&menu, false);
        intensityLabel.setJustificationType(juce::Justification::centredTop);

        addAndMakeVisible(menu);
        menu.addItem("Low", 1);
        menu.addItem("Medium", 2);
        menu.addItem("High", 3);
        menu.setSelectedId(2);
        menu.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        menu.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::burlywood);
        menu.setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colours::lightgoldenrodyellow);
        menu.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::beige);

        menu.onChange = [this]() {
            juce::String item = menu.getItemText(menu.getSelectedItemIndex());
            audioProcessor.updateSound(item);
            notificationLabel.setText(item + " Intensity " + " Selected", juce::dontSendNotification);
        };


        //addAndMakeVisible(presetLabel);
        //presetLabel.setFont(17.0f);
        //presetLabel.setText("Preset", juce::dontSendNotification);
        //presetLabel.attachToComponent(&menuPreset, true);
        //presetLabel.setJustificationType(juce::Justification::left);
        //presetLabel.setVisible(false);

        addAndMakeVisible(menuPreset);
        menuPreset.addItem("Presets", 1);
        menuPreset.addItem("Pads", 2);
        menuPreset.addItem("Plucks", 3);
        menuPreset.addItem("Percussion", 4);
        menuPreset.setSelectedId(1);
        menuPreset.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
        menuPreset.setColour(juce::ComboBox::ColourIds::outlineColourId, juce::Colours::burlywood);
        menuPreset.setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, juce::Colours::lightgoldenrodyellow);
        menuPreset.setColour(juce::ComboBox::ColourIds::textColourId, juce::Colours::beige);

        menuPreset.onChange = [this]() {
            juce::String item = menuPreset.getItemText(menuPreset.getSelectedItemIndex());
            presetChanged(item);
            menuPreset.giveAwayKeyboardFocus();
        };
        #pragma endregion


        #pragma region misc
        addAndMakeVisible(notificationLabel);
        notificationLabel.setFont(15.0f);
        notificationLabel.setText("", juce::dontSendNotification);
        notificationLabel.setJustificationType(juce::Justification::bottomLeft);

        addAndMakeVisible(helpButton);
        helpButton.setButtonText("Help");
        helpButton.onClick = [this]() {
            helpLabel1.setVisible(true);
            helpLabel2.setVisible(true);
            helpLabel3.setVisible(true);
            helpLabel4.setVisible(true);
            helpLabel5.setVisible(true);
            helpLabel6.setVisible(true);
            helpLabel7.setVisible(true);

            backButton.setVisible(true);
            helpButton.setVisible(false);
            AttackBorder.setVisible(false);
            DecayBorder.setVisible(false);
            SustainBorder.setVisible(false);
            ReleaseBorder.setVisible(false);
            AttackSlider.setVisible(false);
            DecaySlider.setVisible(false);
            SustainSlider.setVisible(false);
            ReleaseSlider.setVisible(false);
            menu.setVisible(false);
            menuPreset.setVisible(false);
            presetLabel.setVisible(false);
            intensityLabel.setVisible(false);
            resetButton.setVisible(false);
            saveButton.setVisible(false);
            aPresetButton.setVisible(false);
            bPresetButton.setVisible(false);

            notificationLabel.setText("", juce::dontSendNotification);

            helpButton.giveAwayKeyboardFocus();
        };

        addAndMakeVisible(backButton);
        backButton.setButtonText("Back");
        backButton.onClick = [this]() {
            helpLabel1.setVisible(false);
            helpLabel2.setVisible(false);
            helpLabel3.setVisible(false);
            helpLabel4.setVisible(false);
            helpLabel5.setVisible(false);
            helpLabel6.setVisible(false);
            helpLabel7.setVisible(false);
            backButton.setVisible(false);
            helpButton.setVisible(true);

            AttackBorder.setVisible(true);
            DecayBorder.setVisible(true);
            SustainBorder.setVisible(true);
            ReleaseBorder.setVisible(true);
            AttackSlider.setVisible(true);
            DecaySlider.setVisible(true);
            SustainSlider.setVisible(true);
            ReleaseSlider.setVisible(true);
            menu.setVisible(true);
            menuPreset.setVisible(true);
            presetLabel.setVisible(true);
            intensityLabel.setVisible(true);
            resetButton.setVisible(true);
            saveButton.setVisible(true);
            aPresetButton.setVisible(true);
            bPresetButton.setVisible(true);

            notificationLabel.setText("", juce::dontSendNotification);
            backButton.giveAwayKeyboardFocus();
        };
        backButton.setVisible(false);

        addAndMakeVisible(resetButton);
        resetButton.setButtonText("Reset");
        resetButton.onClick = [this]() {

            AttackSlider.setValue(0.1);
            DecaySlider.setValue(0.1);
            SustainSlider.setValue(1);
            ReleaseSlider.setValue(0.1);
            menu.setSelectedId(2);
            menuPreset.setSelectedId(1);
            aA = 0.0;
            dA = 0.0;
            sA = 0.0;
            rA = 0.0;           
            aB = 0.0;
            dB = 0.0;
            sB = 0.0;
            rB = 0.0;
            xA = 2;
            xB = 2;
            isA = false;
            isB = false;
            isASaved = false;
            isBSaved = false;
            aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
            bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
            notificationLabel.setText("", juce::dontSendNotification);
            resetButton.giveAwayKeyboardFocus();
        };

        addAndMakeVisible(aPresetButton);
        aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        aPresetButton.setButtonText("A");
        aPresetButton.onClick = [this]() {
            if (isASaved) {
                if (!isA && !isB) {
                    isA = true;
                    aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::sandybrown);
                }
                else if (!isA && isB) {
                    isA = true;
                    isB = false;
                    aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::sandybrown);
                    bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
                }
                else if (isA && !isB) {
                    isA = false;
                    aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
                }
                AttackSlider.setValue(aA);
                DecaySlider.setValue(dA);
                SustainSlider.setValue(sA);
                ReleaseSlider.setValue(rA);
                menuPreset.setSelectedId(1);
                menu.setSelectedId(xA);
            }
            else {
                if (!isA && !isB) {
                    isA = true;
                    aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::sandybrown);
                }
                else if (!isA && isB) {
                    isA = true;
                    isB = false;
                    aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::sandybrown);
                    bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
                }
                else if (isA && !isB) {
                    isA = false;
                    aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
                }
            }

            notificationLabel.setText("State A selected", juce::dontSendNotification);
        };

        addAndMakeVisible(bPresetButton);
        bPresetButton.setButtonText("B");
        bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
        bPresetButton.onClick = [this]() {
            if (isBSaved) {
                if (!isA && !isB) {
                    isB = true;
                    bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::sandybrown);
                }
                else if (!isB && isA) {
                    isB = true;
                    isA = false;
                    bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::sandybrown);
                    aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
                }
                else if (isB && !isA) {
                    isB = false;
                    bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
                }
                AttackSlider.setValue(aB);
                DecaySlider.setValue(dB);
                SustainSlider.setValue(sB);
                ReleaseSlider.setValue(rB);
                menuPreset.setSelectedId(1);
                menu.setSelectedId(xB);
            }
            else {
                if (!isA && !isB) {
                    isB = true;
                    bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::sandybrown);
                }
                else if (!isB && isA) {
                    isB = true;
                    isA = false;
                    bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::sandybrown);
                    aPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
                }
                else if (isB && !isA) {
                    isB = false;
                    bPresetButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgrey);
                }
            }
            notificationLabel.setText("State B selected", juce::dontSendNotification);
        };

        addAndMakeVisible(saveButton);
        saveButton.setButtonText("SAVE");
        saveButton.onClick = [this]() {
            if (isA) {
                aA = AttackSlider.getValue();
                dA = DecaySlider.getValue();
                sA = SustainSlider.getValue();
                rA = ReleaseSlider.getValue();
                xA = menu.getSelectedId();
                isASaved = true;
            }
            else if (isB) {
                aB = AttackSlider.getValue();
                dB = DecaySlider.getValue();
                sB = SustainSlider.getValue();
                rB = ReleaseSlider.getValue();
                xB = menu.getSelectedId();
                isBSaved = true;
            }
            else {
                notificationLabel.setText("Pilih salah satu state!", juce::dontSendNotification);
                return;
            }
            notificationLabel.setText("State sementara telah disimpan", juce::dontSendNotification);
        };
#pragma endregion


        #pragma region HelpLabel
        addAndMakeVisible(helpLabel1);
        helpLabel1.setFont(17.5f);
        helpLabel1.setText("Attack - Time for audio to reach maximum volume", juce::dontSendNotification);
        helpLabel1.setJustificationType(juce::Justification::centred);
        helpLabel1.setVisible(false);

        addAndMakeVisible(helpLabel2);
        helpLabel2.setFont(17.5f);
        helpLabel2.setText("Decay - Time for audio to reach sustain volume", juce::dontSendNotification);
        helpLabel2.setJustificationType(juce::Justification::centred);
        helpLabel2.setVisible(false);

        addAndMakeVisible(helpLabel3);
        helpLabel3.setFont(17.5f);
        helpLabel3.setText("Sustain - Volume level after Attack until the note is released", juce::dontSendNotification);
        helpLabel3.setJustificationType(juce::Justification::centred);
        helpLabel3.setVisible(false);

        addAndMakeVisible(helpLabel4);  
        helpLabel4.setFont(17.5f);
        helpLabel4.setText("Release - Time for audio to reach minimum volume", juce::dontSendNotification);
        helpLabel4.setJustificationType(juce::Justification::centred);
        helpLabel4.setVisible(false);

        addAndMakeVisible(helpLabel5);
        helpLabel5.setFont(17.5f);
        helpLabel5.setText("Intensity Level - Option for level of Angklung shake intensity", juce::dontSendNotification);
        helpLabel5.setJustificationType(juce::Justification::centred);
        helpLabel5.setVisible(false);

        addAndMakeVisible(helpLabel6);
        helpLabel6.setFont(17.5f);
        helpLabel6.setText("Preset - Set of configuration for ADSR", juce::dontSendNotification);
        helpLabel6.setJustificationType(juce::Justification::centred);
        helpLabel6.setVisible(false);

        addAndMakeVisible(helpLabel7);
        helpLabel7.setFont(17.5f);
        helpLabel7.setText("State A dan B - Temporary state that able to save plugin configuration", juce::dontSendNotification);
        helpLabel7.setJustificationType(juce::Justification::centred);
        helpLabel7.setVisible(false);
        #pragma endregion
}

PadaengAudioProcessorEditor::~PadaengAudioProcessorEditor()
{
}

//==============================================================================
void PadaengAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.fillAll(juce::Colours::transparentBlack);
    auto myImage = juce::ImageCache::getFromMemory(BinaryData::wallpaperBambu_png, BinaryData::wallpaperBambu_pngSize);
    g.drawImage(myImage, getLocalBounds().toFloat());

    g.setColour(juce::Colours::blanchedalmond);
    g.setFont(20.0f);
    g.drawFittedText("AngklungBeats", getLocalBounds(), juce::Justification::centredTop, 1);
}

void PadaengAudioProcessorEditor::resized()
{
    auto leftMargin = getWidth() * 0.05;
    auto topMargin = getHeight() * 0.25;
    auto dialSize = getWidth() * 0.2;
    auto xSize = getWidth() * 0.25;

    AttackSlider.setBounds(leftMargin, topMargin, dialSize, dialSize);
    DecaySlider.setBounds(leftMargin + (dialSize * 1.18), topMargin, dialSize, dialSize);
    SustainSlider.setBounds(leftMargin + (dialSize * 2.35), topMargin, dialSize, dialSize);
    ReleaseSlider.setBounds(leftMargin + (dialSize * 3.5), topMargin, dialSize, dialSize);

    menu.setBounds(getWidth() * 0.415, getHeight() * 0.86, xSize * 0.33 * 2, xSize * 0.33 * 0.5);
    menuPreset.setBounds(getWidth() * 0.055, getHeight() * 0.07, xSize * 0.33 * 2, xSize * 0.33 * 0.5);
    resetButton.setBounds(getWidth() * 0.88, getHeight() * 0.07, xSize * 0.25, xSize * 0.33 * 0.5);

    AttackBorder.setBounds(leftMargin, topMargin - (dialSize * 0.15), dialSize, dialSize * 1.27);
    DecayBorder.setBounds(leftMargin + (dialSize * 1.18), topMargin - (dialSize * 0.15), dialSize, dialSize * 1.27);
    SustainBorder.setBounds(leftMargin + (dialSize * 2.35), topMargin - (dialSize * 0.15), dialSize, dialSize * 1.27);
    ReleaseBorder.setBounds(leftMargin + (dialSize * 3.5), topMargin - (dialSize * 0.15), dialSize, dialSize * 1.27);

    notificationLabel.setBounds(getWidth() * 0.005, getHeight() * 0.88, xSize * 0.5 * 2, xSize * 0.33 * 0.5);

    backButton.setBounds(getWidth() * 0.068, getHeight() * 0.07, xSize * 0.4, xSize * 0.33 * 0.5);
    helpButton.setBounds(getWidth() * 0.85, getHeight() * 0.88, xSize * 0.4, xSize * 0.33 * 0.5);
    helpLabel1.setBounds(getWidth() * 0.25, getHeight() * 0.3, xSize * 2, xSize * 0.33 * 0.5);
    helpLabel2.setBounds(getWidth() * 0.25, getHeight() * 0.35, xSize * 2, xSize * 0.33 * 0.5);
    helpLabel3.setBounds(getWidth() * 0.25, getHeight() * 0.4, xSize * 2, xSize * 0.33 * 0.5);
    helpLabel4.setBounds(getWidth() * 0.25, getHeight() * 0.45, xSize * 2, xSize * 0.33 * 0.5);
    helpLabel5.setBounds(getWidth() * 0.25, getHeight() * 0.5, xSize * 2, xSize * 0.33 * 0.5);
    helpLabel6.setBounds(getWidth() * 0.25, getHeight() * 0.6, xSize * 2, xSize * 0.33 * 0.5);
    helpLabel7.setBounds(getWidth() * 0.25, getHeight() * 0.675, xSize * 2, xSize * 0.33 * 0.5);

    aPresetButton.setBounds(getWidth() * 0.055, getHeight() * 0.8, xSize * 0.13, xSize * 0.33 * 0.5);
    bPresetButton.setBounds(getWidth() * 0.095, getHeight() * 0.8, xSize * 0.135, xSize * 0.33 * 0.5);
    saveButton.setBounds(getWidth() * 0.14, getHeight() * 0.8, xSize * 0.4, xSize * 0.33 * 0.5);
}

void PadaengAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &AttackSlider) {
        audioProcessor.getADSRParams().attack = AttackSlider.getValue();
    }
    else if (slider == &DecaySlider) {
        audioProcessor.getADSRParams().decay = DecaySlider.getValue();
    }
    else if (slider == &SustainSlider) {
        audioProcessor.getADSRParams().sustain = SustainSlider.getValue();
    }
    else if (slider == &ReleaseSlider) {
        audioProcessor.getADSRParams().release = ReleaseSlider.getValue();
    }

    audioProcessor.updateADSR();
}

void PadaengAudioProcessorEditor::presetChanged(juce::String preset) 
{
    notificationLabel.setText(preset + " preset selected", juce::dontSendNotification);
    if (preset == "Pads") {
        AttackSlider.setValue(0.3);
        DecaySlider.setValue(0.7);
        SustainSlider.setValue(0.7);
        ReleaseSlider.setValue(0.8);
    }
    else if (preset == "Plucks") {
        AttackSlider.setValue(0.02);
        DecaySlider.setValue(0.2);
        SustainSlider.setValue(0.5);
        ReleaseSlider.setValue(0.4);
    }
    else if (preset == "Percussion") {
        AttackSlider.setValue(0.05);
        DecaySlider.setValue(0.01);
        SustainSlider.setValue(0.1);
        ReleaseSlider.setValue(0.1);
    }
    else {
        notificationLabel.setText("", juce::dontSendNotification);
        return;
    }

}

//void comboBoxClicked(juce::String item) {
//
//    DBG(item);
//}
