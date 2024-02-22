#include <Arduino.h>
// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
//
// Part 2-8: Oscillators

#include <Bounce.h>

int button0Pin = 41;
int button1Pin = 40;
int button2Pin = 39;
int button3Pin = 38;

Bounce button0 = Bounce(button0Pin, 15);
Bounce button1 = Bounce(button1Pin, 15); // 15 = 15 ms debounce time
Bounce button2 = Bounce(button2Pin, 15);
Bounce button3 = Bounce(button3Pin, 15);

int motorPin = 33;

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>


// GUItool: begin automatically generated code
AudioSynthNoisePink      pink1;          //xy=61,598
AudioSynthWaveform       waveform1;      //xy=65,368.0000057220459
AudioSynthWaveformSine   sine1;          //xy=71,544
AudioSynthWaveformSineModulated sine_fm1;       //xy=146,467
AudioMixer4              mixer1;         //xy=326.99999237060547,507.0000071525574
AudioEffectEnvelope      envelope1;      //xy=476.99999237060547,578.0000247955322
AudioMixer4              mixer2;         //xy=630.0000228881836,510.0000228881836
AudioFilterStateVariable filter1;        //xy=793.2000389099121,524.0000076293945
AudioMixer4              mixer3; //xy=938.2000427246094,517.0000076293945
AudioAnalyzePeak         peak1;          //xy=1084.9999685287476,551.0000095367432
AudioOutputI2S           i2s1;           //xy=1089.0001525878906,504.0000686645508
AudioConnection          patchCord1(pink1, 0, mixer1, 3);
AudioConnection          patchCord2(waveform1, sine_fm1);
AudioConnection          patchCord3(waveform1, 0, mixer1, 0);
AudioConnection          patchCord4(sine1, 0, mixer1, 2);
AudioConnection          patchCord5(sine_fm1, 0, mixer1, 1);
AudioConnection          patchCord6(mixer1, 0, mixer2, 0);
AudioConnection          patchCord7(mixer1, envelope1);
AudioConnection          patchCord8(envelope1, 0, mixer2, 1);
AudioConnection          patchCord9(mixer2, 0, filter1, 0);
AudioConnection          patchCord10(mixer2, 0, mixer3, 0);
AudioConnection          patchCord11(filter1, 0, mixer3, 1);
AudioConnection          patchCord12(filter1, 1, mixer3, 2);
AudioConnection          patchCord13(filter1, 2, mixer3, 3);
AudioConnection          patchCord14(mixer3, 0, i2s1, 0);
AudioConnection          patchCord15(mixer3, 0, i2s1, 1);
AudioConnection          patchCord16(mixer3, peak1);
AudioControlSGTL5000     sgtl5000_1;     //xy=255,650
// GUItool: end automatically generated code



void setup()
{
  Serial.begin(9600);
  AudioMemory(20);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.52);

  pinMode(button0Pin, INPUT_PULLUP);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(motorPin, OUTPUT);

  mixer1.gain(0, 0.75);
  mixer1.gain(1, 0.0);
  mixer1.gain(2, 0.0);
  mixer1.gain(3, 0.0);
  mixer2.gain(0, 0.75);
  mixer2.gain(1, 0.0);
  mixer2.gain(2, 0.0);
  mixer2.gain(3, 0.0);
  mixer3.gain(0, 0.75);
  mixer3.gain(1, 0.0);
  mixer3.gain(2, 0.0);
  mixer3.gain(3, 0.0);
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
}

int waveform_type = WAVEFORM_SAWTOOTH;
int mixer1_setting = 0;
int mixer2_setting = 0;
int mixer3_setting = 0;
elapsedMillis timeout = 0;
bool mixer2_envelope = false;
elapsedMillis msecs;

void loop()
{
  button0.update();
  button1.update();
  button2.update();
  button3.update();

  // Left changes the type of control waveform
  if (button0.fallingEdge())
  {
    Serial.print("Control waveform: ");
    if (waveform_type == WAVEFORM_SAWTOOTH)
    {
      waveform_type = WAVEFORM_SINE;
      Serial.println("Sine");
    }
    else if (waveform_type == WAVEFORM_SINE)
    {
      waveform_type = WAVEFORM_SQUARE;
      Serial.println("Square");
    }
    else if (waveform_type == WAVEFORM_SQUARE)
    {
      waveform_type = WAVEFORM_TRIANGLE;
      Serial.println("Triangle");
    }
    else if (waveform_type == WAVEFORM_TRIANGLE)
    {
      waveform_type = WAVEFORM_PULSE;
      Serial.println("Pulse");
    }
    else if (waveform_type == WAVEFORM_PULSE)
    {
      waveform_type = WAVEFORM_SAWTOOTH;
      Serial.println("Sawtooth");
    }
    waveform1.begin(waveform_type);
  }

  // middle button switch which source we hear from mixer1
  if (button1.fallingEdge())
  {
    Serial.println("Mixer1 button pressed");
    if (mixer1_setting == 0)
    {
      mixer1.gain(0, 0.75);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.0);
      Serial.println("Mixer1: Control oscillator");
      mixer1_setting = 1;
    }
    else if (mixer1_setting == 1)
    {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.75);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.0);
      Serial.println("Mixer1: Frequency Modulated Oscillator");
      mixer1_setting = 2;
    }
    else if (mixer1_setting == 2)
    {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.75);
      mixer1.gain(3, 0.0);
      Serial.println("Mixer1: Regular Sine Wave Oscillator");
      mixer1_setting = 3;
    }
    else if (mixer1_setting == 3)
    {
      mixer1.gain(0, 0.0);
      mixer1.gain(1, 0.0);
      mixer1.gain(2, 0.0);
      mixer1.gain(3, 0.75);
      Serial.println("Mixer1: Pink Noise");
      mixer1_setting = 0;
    }
  }

  // Right button activates the envelope
  if (button2.fallingEdge())
  {
    Serial.println("Envelope note pressed");
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

  // middle button switch which source we hear from mixer1
  if (button3.fallingEdge())
  {
    Serial.println("filter button pressed");
    if (mixer3_setting == 0)
    {
      mixer3.gain(0, 0.75);
      mixer3.gain(1, 0.0);
      mixer3.gain(2, 0.0);
      mixer3.gain(3, 0.0);
      Serial.println("Mixer3: No Filter");
      mixer3_setting = 1;
    }
    else if (mixer3_setting == 1)
    {
      mixer3.gain(0, 0.0);
      mixer3.gain(1, 0.75);
      mixer3.gain(2, 0.0);
      mixer3.gain(3, 0.0);
      Serial.println("Mixer3: Low Pass Filter");
      mixer3_setting = 2;
    }
    else if (mixer3_setting == 2)
    {
      mixer3.gain(0, 0.0);
      mixer3.gain(1, 0.0);
      mixer3.gain(2, 0.75);
      mixer3.gain(3, 0.0);
      Serial.println("Mixer3: Band Pass Filter");
      mixer3_setting = 3;
    }
    else if (mixer3_setting == 3)
    {
      mixer3.gain(0, 0.0);
      mixer3.gain(1, 0.0);
      mixer3.gain(2, 0.0);
      mixer3.gain(3, 0.75);
      Serial.println("Mixer3: High Pass Filter");
      mixer3_setting = 0;
    }
  }

  // after 4 seconds of inactivity, go back to
  // steady listening intead of the envelope
  if (mixer2_envelope == true && timeout > 500)
  {
    mixer2.gain(0, 0.75);
    mixer2.gain(1, 0.0);
    mixer2_envelope = false;
  }

  // use the knobs to adjust parameters
  // float knob1 = (float)analogRead(A1) / 1023.0;
  float knob2 = (float)analogRead(A10) / 1023.0;
  float knob3 = (float)analogRead(A11) / 1023.0;
  float knob4 = (float)analogRead(A12);
  float knob5 = (float)analogRead(A13) / 1023.0;

  float freq =  expf((float)knob4 / 150.0) * 10.0 + 80.0;
  filter1.frequency(freq);
  

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
