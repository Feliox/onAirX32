#include <MIDI.h> 

#define onair_main 2    
#define onair_mic1 3
#define onair_mic2 4
#define onair_mic3 5
#define onair_mic4 6

MIDI_CREATE_DEFAULT_INSTANCE();

struct Fader {
  char level = 127;
  bool unmute = false;
  bool onAir = false;
};

struct Fader faders[4];

#define MIN_LEVEL 10

void setup() {
  pinMode (onair_main, OUTPUT);
  pinMode (onair_mic1, OUTPUT);
  pinMode (onair_mic2, OUTPUT);
  pinMode (onair_mic3, OUTPUT);
  pinMode (onair_mic4, OUTPUT);
  
  MIDI.begin(MIDI_CHANNEL_OMNI);

  MIDI.setHandleControlChange(ccHandle);
}

void loop() {
  MIDI.read();
}

void ccHandle(byte _channel, byte _number, byte _value) {
  if (_channel == 1){
    if(_number < 4){
      faders[_number].level = _value;
    }
  }
  if (_channel == 2){
    if(_number < 4){
      if(_value == 127){
        faders[_number].unmute = false;
        
      }
      else if(_value == 0){
        faders[_number].unmute = true;
      }
    }
  }
  if((faders[_number].level > 10)&&(faders[_number].unmute)){
    faders[_number].onAir = true;
  }
  else faders[_number].onAir = false;
  if(faders[0].onAir||faders[1].onAir||faders[2].onAir||faders[3].onAir){
    digitalWrite(onair_main,HIGH);
  }
  else {
    digitalWrite(onair_main,LOW);
  }
  digitalWrite(onair_mic1, faders[0].onAir);
  digitalWrite(onair_mic2, faders[1].onAir);
  digitalWrite(onair_mic3, faders[2].onAir);
  digitalWrite(onair_mic4, faders[3].onAir);
}
