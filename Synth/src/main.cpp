#include <Arduino.h>
// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
//
// Part 2-8: Oscillators

#include <Bounce.h>
#include <iostream>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "ScreenLCD.h"

Bounce button0 = Bounce(41, 15);
Bounce button1 = Bounce(40, 15); // 15 = 15 ms debounce time
Bounce button2 = Bounce(39, 15);

int motorPin = 33;


ScreenLCD lcd1(SCL,SDA,0x27);
ScreenLCD lcd2(SCL,SDA,0x26);



// GUItool: begin automatically generated code
AudioSynthNoisePink pink1;                // xy=61,598
AudioSynthWaveform waveform1;             // xy=64,369
AudioSynthWaveformSine sine1;             // xy=71,544
AudioSynthWaveformSineModulated sine_fm1; // xy=146,467
AudioMixer4 mixer1;                       // xy=327,507
AudioEffectEnvelope envelope1;            // xy=426,658
AudioMixer4 mixer2;                       // xy=503,512
AudioAnalyzePeak peak1;                   // xy=607,611
AudioOutputI2S i2s1;                      // xy=711,507
AudioConnection patchCord1(pink1, 0, mixer1, 3);
AudioConnection patchCord2(waveform1, sine_fm1);
AudioConnection patchCord3(waveform1, 0, mixer1, 0);
AudioConnection patchCord4(sine1, 0, mixer1, 2);
AudioConnection patchCord5(sine_fm1, 0, mixer1, 1);
AudioConnection patchCord6(mixer1, 0, mixer2, 0);
AudioConnection patchCord7(mixer1, envelope1);
AudioConnection patchCord8(envelope1, 0, mixer2, 1);
AudioConnection patchCord9(mixer2, 0, i2s1, 0);
AudioConnection patchCord10(mixer2, 0, i2s1, 1);
AudioConnection patchCord11(mixer2, peak1);
AudioControlSGTL5000 sgtl5000_1; // xy=255,650
// GUItool: end automatically generated code
String ctrlWaveFormMsg;
String mixer1Msg;
String filterMsg;
String EnvMsg;



void setup()
{

  Serial.begin(9600);
  lcd1.begin();
  lcd2.begin();
  Wire2.begin();
  Wire1.begin();
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.32);

  pinMode(41, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP);
  pinMode(39, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);
  mixer1.gain(0, 0.75);
  mixer1.gain(1, 0.0);
  mixer1.gain(2, 0.0);
  mixer1.gain(3, 0.0);
  mixer2.gain(0, 0.75);
  mixer2.gain(1, 0.0);
  mixer2.gain(2, 0.0);
  mixer2.gain(3, 0.0);
  waveform1.begin(WAVEFORM_SAWTOOTH);
  waveform1.amplitude(0.75);
  waveform1.frequency(50);
  waveform1.pulseWidth(0.15);
  sine_fm1.frequency(440);
  sine_fm1.amplitude(0.75);
  sine1.frequency(200);
  sine1.amplitude(0.75);
  pink1.amplitude(0.75);
  envelope1.attack(10);
  envelope1.hold(10);
  envelope1.decay(25);
  envelope1.sustain(0.4);
  envelope1.release(70);

  // LCD

}

int waveform_type = WAVEFORM_SAWTOOTH;
int mixer1_setting = 0;
int mixer2_setting = 0;
elapsedMillis timeout = 0;
bool mixer2_envelope = false;
elapsedMillis msecs;

void loop()
{
  
  button0.update();
  button1.update();
  button2.update();

  // Left changes the type of control waveform
  if (button0.fallingEdge())
  {
    if (waveform_type == WAVEFORM_SAWTOOTH)
    {
      ctrlWaveFormMsg ="CTRLWF: Sine";
      waveform_type = WAVEFORM_SINE;
    
    }
    else if (waveform_type == WAVEFORM_SINE)
    {
      ctrlWaveFormMsg ="CTRLWF: Square";
      waveform_type = WAVEFORM_SQUARE;
    
    }
    else if (waveform_type == WAVEFORM_SQUARE)
    {
      ctrlWaveFormMsg ="CTRLWF: Triangle";
      waveform_type = WAVEFORM_TRIANGLE;
    
    }
    else if (waveform_type == WAVEFORM_TRIANGLE)
    {
      ctrlWaveFormMsg ="CTRLWF: Pulse";
      waveform_type = WAVEFORM_PULSE;
    
    }
    else if (waveform_type == WAVEFORM_PULSE)
    {
      ctrlWaveFormMsg ="CTRLWF: Sawtooth";
      waveform_type = WAVEFORM_SAWTOOTH;
      
    }
    waveform1.begin(waveform_type);
    lcd1.display(ctrlWaveFormMsg, mixer1Msg);
  }
  // middle button switch which source we hear from mixer1
  if (button1.fallingEdge())
  {
    Serial.println("button pressed");
    if (mixer1_setting == 0)
    {
      mixer1.gain(0, 0.75);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.0);
     // Serial.println("IN: Control oscillator");
      mixer1Msg = "IN: CTRL Osci";
      mixer1_setting = 1;
    }
    else if (mixer1_setting == 1)
    {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.75);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.0);
      mixer1Msg = "IN: Freq Mod Osci";
      mixer1_setting = 2;
    }
    else if (mixer1_setting == 2)
    {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.75);
      mixer1.gain(3, 0.0);
      mixer1Msg = "IN: Reg Sine Wave Osci";
      mixer1_setting = 3;
    }
    else if (mixer1_setting == 3)
    {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.75);
      mixer1Msg = "IN: Pink Noise";
      mixer1_setting = 0;
    }
    Serial.println(mixer1Msg);
    lcd1.display(ctrlWaveFormMsg, mixer1Msg);

  }

  // Right button activates the envelope
  if (button2.fallingEdge())
  {
    lcd2.display("Test", "twoo");
    Serial.println("button 2 pressed");
    mixer2.gain(0, 0.0);
    mixer2.gain(1, 0.75);
    mixer2_envelope = true;
    timeout = 0;
    envelope1.noteOn();
  }
  if (button2.risingEdge())
  {
    envelope1.noteOff();
    timeout = 0;
  }

  // after 4 seconds of inactivity, go back to
  // steady listening intead of the envelope
  if (mixer2_envelope == true && timeout > 250)
  {
    mixer2.gain(0, 0.75);
    mixer2.gain(1, 0.0);
    mixer2_envelope = false;
  }

  // use the knobs to adjust parameters
  // float knob1 = (float)analogRead(A1) / 1023.0;
  float knob2 = (float)analogRead(A12) / 1023.0;
  float knob3 = (float)analogRead(A13) / 1023.0;
  if (msecs > 1)
  {
    if (peak1.available())
    {
      msecs = 0;
      float amplitude = peak1.read();
      int peak = map(amplitude, 0, 1.0, 100, 200);
      analogWrite(motorPin, peak);
    }
  }

  waveform1.frequency(360 * knob2 + 0.25);
  sine_fm1.frequency(knob3 * 1500 + 50);
  sine1.frequency(knob3 * 1500 + 50);


}
