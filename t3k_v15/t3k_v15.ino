#include <Audio.h>
//#include <Wire.h>
//#include <SPI.h>
//#include <SD.h>
#include "wave_tables.h"
//#include <SerialFlash.h>
#include <EEPROM.h>
#include <MIDI.h>


#define ledacle_pin 5
#define adj_pin 2
#define bleep_pin 28
#define eye_down_pin 8
#define eye_up_pin 6
#define range_down_pin 29
#define range_up_pin 30

#define top_button_pin 7
#define midi_in_sw 10
#define cv_in_sw 12
#define audio_in_sw 27
#define audio_out_sw 25

#define vol_pot_pin A10
#define cv_in_pin A2
#define trig_in_pin 17 //A3
#define pcell_pin A1

#define left_pot_pin A6
#define middle_pot_pin A0
#define right_pot_pin A7

IntervalTimer timer1;


int16_t granular_bank_len = 3000;
int16_t granular_bank[3000] = {};

#define CHORUS_LEN (30*AUDIO_BLOCK_SAMPLES)
short chorus_bank[CHORUS_LEN] = {};

#define FLANGE_LEN (6*AUDIO_BLOCK_SAMPLES)
short flange_bank[FLANGE_LEN] = {};

#define DLY_MAX 10000
short dlyb[DLY_MAX] = {};

int16_t freqbank[16] = {};
int16_t freq_out[16] = {};
float amp_out[16] = {};
float ampbank[16] = {};
float statusbank[16] = {};
float fft_bank[100] = {};

const float chromatic[108] = {16.3516, 17.32391673, 18.35405043, 19.44543906, 20.60172504, 21.82676736, 23.12465449, 24.499718, 25.95654704, 27.50000365, 29.13523896, 30.86771042, 32.7032, 34.64783346, 36.70810085, 38.89087812, 41.20345007, 43.65353471, 46.24930897, 48.99943599, 51.91309407, 55.00000728, 58.27047791, 61.73542083, 65.40639999, 69.29566692, 73.4162017, 77.78175623, 82.40690014, 87.30706942, 92.49861792, 97.99887197, 103.8261881, 110.0000146, 116.5409558, 123.4708417, 130.8128, 138.5913338, 146.8324034, 155.5635124, 164.8138003, 174.6141388, 184.9972358, 195.9977439, 207.6523763, 220.0000291, 233.0819116, 246.9416833, 261.6255999, 277.1826676, 293.6648067, 311.1270248, 329.6276005, 349.2282776, 369.9944716, 391.9954878, 415.3047525, 440.0000581, 466.1638231, 493.8833665, 523.2511997, 554.3653352, 587.3296134, 622.2540496, 659.2552009, 698.4565551, 739.9889431, 783.9909755, 830.6095048, 880.0001162, 932.3276461, 987.7667329, 1046.502399, 1108.73067, 1174.659227, 1244.508099, 1318.510402, 1396.91311, 1479.977886, 1567.981951, 1661.219009, 1760.000232, 1864.655292, 1975.533466, 2093.004798, 2217.46134, 2349.318453, 2489.016198, 2637.020803, 2793.82622, 2959.955772, 3135.963901, 3322.438019, 3520.000464, 3729.310584, 3951.066931, 4186.009596, 4434.92268, 4698.636906, 4978.032395, 5274.041605, 5587.652439, 5919.911543, 6271.927802, 6644.876037, 7040.000927, 7458.621167, 7902.133861};
const int16_t eigth_octave[table_n] = {1, 100, 120, 130, 140, 150, 160, 170, 180, 190, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 220, 240, 260, 280, 300
                                       , 320, 340, 360, 380, 400, 440, 480, 520, 560, 600, 640, 680, 720, 760, 800, 880, 960, 1040, 1120, 1200, 1280, 1360, 1520, 1600, 1760, 1920,
                                       2240, 2400, 2560, 2720, 2880, 3040, 3200, 3840, 4800, 6400, 9600,
                                      }; // unessceary ones removed. leaving 63 = table_n

byte once[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
float amp5[16] = {0};
float freq[16] = {};
byte wave_switch[8][table_n] = {};
int16_t  out_wave[256] = {};
byte scan_bank[20] = {};
byte scan_vel_bank[20] = {};
float dbp1, amp_cutoff;
uint32_t loopd, loopt;
byte pot_tick, eyem;
int16_t pcell1, pot2, pot3, pot6, pot7, pot1;
byte pbleep_button, bleep_button, flatch;
int16_t ftc, fstep;
float lerpy;
uint32_t prev0, prev1, prev2, midiprev, prevmax;
uint16_t lerp;
int16_t j;
int16_t patch = 0;
float main_freq, granadj, granfreq;
int16_t patch_select, prev_patch, dly_len1;
int16_t bl_sel[16] = {};
float space, space2, gp, printer1, fm_env, fm_env_inv, fm_env2, fm_env_inv2;;
byte fm_env_latch1, fm_env_latch2, fx_mode;
byte padj_button, adj_button;
uint32_t accumulator;
byte sqindex, input_enable, input_mode, prev_input_mode;
byte ononce = 1;
byte offonce = 0;
byte fdone;
float fade_rate = .025;
byte voice_n = 8;
int16_t left_pot, middle_pot, right_pot, top_button, prev_top_button, tb_cnt, tb_start, shift_mode;
int16_t eys_switch, range_switch, eye_mode, master_freq, master_adj, master_fx_adjm, master_fx, patch_ctrl, led_ctrl, fx_ctrl;
int16_t patch_sel, led_sel, fx_sel, pcell_adj, fx_adj, adj_ctrl, master_freq_temp;
float dds_tune, dds_rate, fx_sel_adj, fx_sel_pot;
uint8_t ltick, bleep_latch, pbleep_latch, bbutton_mode;
int16_t left_lin, middle_lin, right_lin, pleft, pmiddle, pright, pleft2, pmiddle2, pright2, cv_in;
int16_t prev_patch_sel, prev_led_sel, prev_fx_sel = fx_sel, eet_patch, eet_fx, eet_led;
int type, note, velocity, channel, d1, d2, pnon, non, pnote, note_active, note_on, note_off;
int16_t first_time = 1;
int16_t scan_cnt = 1;
int16_t scan_cnt_max = 12;
int16_t mono_mode, prev_note, ms_note, prev_ms_note, master_fx_inv;
uint8_t fx_delay, fx_AM, fx_crush, fx_filter, fx_flange;
float freq11, sh_adjf, amp_pot_f, amp_pot_f_inv;
float sdt, amp_pot_f2, cut;
int16_t bb_act, midiing, cc_num, cc_amt, usbmidiing, dinmidiing, misw;
float range_mod, current_note;
float pitchbend = 1;
int16_t cc_modeadj, cc_modesel, cc_ledadj, cc_ledsel, cc_fxsel, cc_fxadj;
float cv_fm, cv_am, eyeam, vol_pot;
int16_t cv_mode, aosw, aisw, am_freq;
int16_t pbt, prev22, cur23, on23, cv_out, led_out, led_outf, cv_trig, pcv_trig, top_latch, cv_led, midi_led, midi_led_sw, cv_in_sel, pcv_in_sel;
uint8_t midi_read_ch , pmidi_read_ch, midi_send_ch, pmidi_send_ch;
int16_t cv_out_sel, pcv_out_sel;
uint32_t potmicros, dtp;
uint8_t cvisw;
int16_t p_msn, msn, f7ma, pcell_raw, on22, cur22, cv_in_raw, poscv, bpcv, mch_temp;
int16_t cv_mid = 2077;
uint8_t mscc[20] = {};
uint8_t pmscc[20] = {};
uint8_t oncein[16] = {};
int16_t ledr1, ledr2, pinput_enable;
int16_t ledacle_mode, random_cnt, random_ledacle1, rp_len;
uint32_t ledtd, ledtm;
float ledlerp, lerpdiv, ledacle_rate, mixl, mixli, res_atten;
uint8_t eet_in, eet_midisend, eet_midi, eet_cvi, eet_cvo, midi_adj_lock, midi_led_lock, midi_fx_lock;
uint8_t usb_midi_en;

float voc1[8][8] = {
  {40, 80,  160, 320, 640, 1280,  2560,  5120,},
  {40, 72, 129.6, 233.28, 419.904, 755.8272, 1360.48, 2448.88,},
  {40, 60, 90, 135, 202.5, 303.75, 455.625, 683.4375,},
  {40, 66.664, 111.10, 185.16, 308.59, 514.30, 857.13, 1428.49,},
  {68.25, 204.75, 341.25, 546, 682.5, 887.25, 1023.75, 1160.25,},
  {68.25, 341.25, 546, 819, 1023.75, 1228.5, 1501.5, 1706.25,},
  {68.25, 341.25, 546, 750.75, 1023.75, 1228.5, 1433.25, 1706.25,},
  {68.25, 409.5, 819, 1160.25, 1501.5, 1842.75, 2184, 2525.25,},
};


////////////////////////////////////////////////////////////////////////////////////////////////////////

AudioEffectvariableDelay delay1;
AudioEffectDigitalCombine     multiply1;
AudioEffectSH          sh1;
AudioEffectGranular       granular1;
AudioEffectDelay          delay2;
//AudioOutputAnalog dac1;
// GUItool: begin automatically generated code
AudioSynthWaveform       waveform6;      //xy=109.00002670288086,597.0475997924805
AudioSynthWaveform       waveform5;      //xy=119.00006103515625,555.3808908462524
AudioSynthWaveformSineModulated sine_fm1;       //xy=147.33336639404297,675.3809868395329
AudioSynthWaveformSineModulated sine_fm4;       //xy=154.00006866455078,725.3810288012028
AudioSynthNoiseWhite     noise1;         //xy=242.3333854675293,512.0475873947144
AudioSynthWaveform       waveform1;      //xy=244,295.38097763061523
AudioSynthNoisePink      pink1;          //xy=247.33337020874023,470.3809394836426
AudioSynthWaveform       waveform2;      //xy=250.66667556762695,335.3809700012207
AudioSynthWaveform       waveform3;      //xy=255.66672134399414,377.04766845703125
AudioSynthWaveform       waveform4;      //xy=262.3333282470703,417.04766845703125

AudioEffectEnvelope      envelope1;      //xy=290.6667251586914,1017.0476474761963
//AudioEffectMultiply      DigitalCombine1;      //xy=296.66672134399414,588.3332958221436
AudioSynthWaveformSineModulated sine_fm2;       //xy=299.0000762939453,682.0476337969303
AudioSynthWaveformSineModulated sine_fm3;       //xy=304.0000457763672,723.7143017351627
AudioMixer4              mixer5;         //xy=462.3333320617676,827.0476989746094
AudioEffectMultiply      multiply2;      //xy=489.00006103515625,702.0475918352604
AudioMixer4              mixer2;         //xy=490.6666831970215,523.7142963409424
AudioMixer4              mixer1;         //xy=502.3333511352539,405.38097953796387
AudioFilterStateVariable filter1;        //xy=512.3333435058594,1080.3810024261475
AudioFilterStateVariable filter2;        //xy=512.3333435058594,1080.3810024261475


AudioSynthWaveformDc     dc1;            //xy=534.0000038146973,1180.3808975219727
AudioSynthWaveformSine   sine10;         //xy=535.6666717529297,1222.0476474761963
AudioSynthWaveform       waveform11;     //xy=544.0000228881836,1137.0475425720215
AudioMixer4              mixer4;         //xy=690.2856941223145,716.3333435058594
AudioMixer4              mixer6;         //xy=712.5714340209961,1088.238037109375
AudioMixer4              mixer8;         //xy=713.761848449707,1170.3809986114502
AudioAnalyzeFFT256       fft256_1;       //xy=764.0000305175781,212.7143039703369
AudioEffectMultiply      multiply3;      //xy=900.0000114440918,1125.0475826263428
AudioSynthWaveformSine   sine5;          //xy=1049.0000495910645,590.3809413909912
AudioSynthWaveformSine   sine3;          //xy=1049.6667137145996,513.0476369857788
AudioSynthWaveformSine   sine2;          //xy=1050.6666717529297,475.0476360321045
AudioSynthWaveformSine   sine4;          //xy=1051.6667137145996,549.0476069450378
AudioSynthWaveformSine   sine1;          //xy=1057.3331985473633,380.14284896850586
AudioSynthWaveform       waveform13;     //xy=1060.4286499023438,337.0475597381592
AudioSynthWaveformSine   sine9;          //xy=1059.6666793823242,788.7142992019653
AudioSynthWaveformSine   sine6;          //xy=1063.3333015441895,667.7143161296844
AudioMixer4              mixer7;         //xy=1062.3332901000977,1110.0476245880127

AudioSynthWaveformSine   sine7;          //xy=1064.3333854675293,706.7143173217773

AudioSynthWaveformSine   sine8;          //xy=1064.9999694824219,746.3809661865234
AudioSynthWaveformDc     dc2;            //xy=1068.523769378662,424.19055938720703
//AudioEffectBitcrusher    sh1;    //xy=1118.3333473205566,1211.6666741371155

AudioMixer4              mixer14;        //xy=1224.6666717529297,1116.7142486572266
AudioMixer4              mixer11;        //xy=1282.6666831970215,561.7143135070801
AudioMixer4              mixer10;        //xy=1295.3333435058594,378.42856216430664
AudioMixer4              mixer12;        //xy=1313.000015258789,708.714298248291
AudioEffectChorus        chorus1;        //xy=1317.6667175292969,1226.0477781295776
//AudioEffectDelay         delay1;         //xy=1321.6668014526367,1383.3333930969238
AudioEffectFlange        flange1;        //xy=1326.6667175292969,1274.0477805137634
AudioEffectMultiply      multiply4;      //xy=1457.6666412353516,314.71428632736206
AudioMixer4              mixer3;         //xy=1459.666603088379,1131.3807411193848
AudioMixer4              mixer9;         //xy=1546.3334121704102,893.0475063323975
AudioMixer4              mixer13;        //xy=1563.333236694336,788.7142868041992
AudioMixer4              mixer16;        //xy=1563.333236694336,788.7142868041992
AudioMixer4              mixer17;        //xy=1563.333236694336,788.7142868041992

AudioMixer4              mixer5555;        //xy=1563.333236694336,788.7142868041992
AudioMixer4              mixerno;        //xy=1563.333236694336,788.7142868041992

AudioInputI2S           codecIn;           //xy=1634.9997806549072,1115.7143573760986

AudioOutputI2S           codecOut;           //xy=1634.9997806549072,1115.7143573760986

AudioAnalyzePeak         peak1;
AudioAnalyzePeak         peak2;

/*
  AudioSynthWaveformSine   sine11;
  AudioSynthWaveformSine   sine12;
  AudioSynthWaveformSine   sine13;
  AudioSynthWaveformSine   sine14;
  AudioMixer4              mixer20;

  AudioConnection          patchCord360(sine11, 0, mixer20, 1);
  AudioConnection          patchCord307(sine12, 0, mixer20, 0);
  AudioConnection          patchCord308(sine13, 0, mixer20, 2);
  AudioConnection          patchCord309(sine14, 0, mixer20, 3);
  AudioConnection          patchCord520(mixer20, 0, mixer13, 2);
*/

AudioConnection          patchCord1(waveform6, 0, multiply1, 1);
AudioConnection          patchCord2(waveform5, 0, multiply1, 0);
AudioConnection          patchCord3(sine_fm1, sine_fm2);
AudioConnection          patchCord4(sine_fm4, sine_fm3);
AudioConnection          patchCord5(noise1, 0, mixer2, 1);
AudioConnection          patchCord6(waveform1, 0, mixer1, 0);
AudioConnection          patchCord7(pink1, 0, mixer2, 0);
AudioConnection          patchCord8(waveform2, 0, mixer1, 1);
AudioConnection          patchCord9(waveform3, 0, mixer1, 2);
AudioConnection          patchCord10(waveform4, 0, mixer1, 3);
AudioConnection          patchCord13(envelope1, 0, filter1, 0);
AudioConnection          patchCord14(envelope1, 0, mixer6, 0);
AudioConnection          patchCord15(multiply1, 0, mixer2, 2);
AudioConnection          patchCord16(sine_fm2, 0, multiply2, 0);
AudioConnection          patchCord17(sine_fm3, 0, multiply2, 1);

//AudioConnection          patchCord193(codecIn, 0, mixerno, 0); //Erros with audio tool and maybe compiling if the left channel is used but the right isnt
AudioConnection          patchCord194(codecIn, 0, mixer16, 1);

AudioConnection          patchCord18(codecIn, 0, fft256_1, 0);

AudioConnection          patchCord19(codecIn, 0, multiply4, 0);
AudioConnection          patchCord191(codecIn, 0, granular1, 0);
AudioConnection          patchCord192(granular1, 0, mixer9, 1);

AudioConnection          patchCord20(mixer5, 0, mixer4, 3);

AudioConnection          patchCord21(multiply2, 0, mixer4, 2);
AudioConnection          patchCord22(mixer2, 0, mixer4, 0);
AudioConnection          patchCord23(mixer1, 0, mixer4, 1);
AudioConnection          patchCord24(filter1, 0, mixer6, 1);
AudioConnection          patchCord25(filter1, 1, mixer6, 2);
AudioConnection          patchCord26(filter1, 2, mixer6, 3);
AudioConnection          patchCord27(dc1, 0, mixer8, 1);
AudioConnection          patchCord28(sine10, 0, mixer8, 2);
AudioConnection          patchCord29(waveform11, 0, mixer8, 0);
//AudioConnection          patchCord30(mixer4, 0, mixer9, 0);
AudioConnection          patchCord30(mixer4, 0, mixer16, 2);
AudioConnection          patchCord301(mixer16, 0, mixer17, 0);

AudioConnection          patchCord31(mixer6, 0, mixer7, 0);

AudioConnection          patchCord32(mixer6, 0, multiply3, 0);
AudioConnection          patchCord33(mixer8, 0, multiply3, 1);
AudioConnection          patchCord34(multiply3, 0, mixer7, 1);
AudioConnection          patchCord35(sine5, 0, mixer11, 3);
AudioConnection          patchCord36(sine3, 0, mixer11, 1);
AudioConnection          patchCord37(sine2, 0, mixer11, 0);
AudioConnection          patchCord38(sine4, 0, mixer11, 2);
AudioConnection          patchCord39(sine1, 0, mixer10, 1);
AudioConnection          patchCord40(waveform13, 0, mixer10, 0);
AudioConnection          patchCord41(sine9, 0, mixer12, 3);
AudioConnection          patchCord42(sine6, 0, mixer12, 0);
AudioConnection          patchCord43(mixer7, 0, mixer14, 0);
AudioConnection          patchCord44(mixer7, sh1);
AudioConnection          patchCord45(sine7, 0, mixer12, 1);
AudioConnection          patchCord46(sine8, 0, mixer12, 2);
AudioConnection          patchCord47(dc2, 0, mixer10, 3);
AudioConnection          patchCord48(sh1, 0, mixer14, 1);
AudioConnection          patchCord49(mixer14, chorus1);
AudioConnection          patchCord55(chorus1, flange1);
AudioConnection          patchCord57(flange1, 0, mixer3, 1);
AudioConnection          patchCord50(mixer14, 0, mixer3, 0);

AudioConnection          patchCord562(delay1, 0, filter2, 0);
AudioConnection          patchCord56(filter2, 0, mixer3, 3);
AudioConnection          patchCord52(mixer11, 0, mixer13, 0);
AudioConnection          patchCord53(mixer10, 0, multiply4, 1);
AudioConnection          patchCord54(mixer12, 0, mixer13, 1);
AudioConnection          patchCord58(multiply4, 0, mixer9, 2);
AudioConnection          patchCord60(mixer9, 0, mixer16, 0);
AudioConnection          patchCord601(mixer17, 0, envelope1, 0);
AudioConnection          patchCord61(mixer13, 0, mixer9, 3);

AudioConnection          patchCord1111(mixer5555, 0, codecOut, 0);
AudioConnection          patchCord591(mixer5555, 0, peak1, 0);

AudioConnection          patchCord51(mixer3, delay1);
AudioConnection          patchCord5221(mixer3, 0, mixer5555, 0);

AudioConnection          patchCord512(mixer5555, delay2);
AudioConnection          patchCord5921(envelope1, 0, peak2, 0);
AudioConnection          patchCord592(delay2, 0, codecOut, 1);
//AudioConnection          patchCord592(multiply4, 0, codecOut, 1);


AudioControlSGTL5000     sgtl5000_1;     //xy=450.6666679382324,146.0476417541504
// GUItool: end automatically generated code


////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(void)
{
  AudioNoInterrupts();



  randomSeed(pcell_pin);

  analogReadResolution(12);
  analogReadAveraging(11);

  analogWriteResolution(12);
  analogWriteFrequency(ledacle_pin, 11718.75 );
  pinMode(ledacle_pin, OUTPUT);
  analogWrite(ledacle_pin, 2000);


  pinMode(trig_in_pin, INPUT_PULLUP);
  pinMode(top_button_pin, INPUT_PULLUP);
  pinMode(bleep_pin, INPUT_PULLUP);
  pinMode(adj_pin, INPUT_PULLUP);
  pinMode(eye_down_pin, INPUT_PULLUP);
  pinMode(eye_up_pin, INPUT_PULLUP);
  pinMode(range_up_pin, INPUT_PULLUP);
  pinMode(range_down_pin, INPUT_PULLUP);
  pinMode(audio_out_sw, INPUT_PULLUP);
  pinMode(audio_in_sw, INPUT_PULLUP);
  pinMode(midi_in_sw, INPUT_PULLUP);
  pinMode(cv_in_sw, INPUT_PULLUP);
  

  byte br1 = digitalRead(bleep_pin);
  byte br2 = digitalRead(top_button_pin);
  byte br3 = digitalRead(adj_pin);
  if (br1==0 && br2==0 && br3==0){
  for (int i;i<2047;i++){
  EEPROM.write(i, 0);
  }

  analogWrite(ledacle_pin,1000);
  delay(250);
  analogWrite(ledacle_pin,0);
  delay(250);
  analogWrite(ledacle_pin,1000);
  delay(250);
  analogWrite(ledacle_pin,0);
  delay(250);
  analogWrite(ledacle_pin,1000);
  delay(250);
  analogWrite(ledacle_pin,0);
  delay(250);  
  }



  sgtl5000_1.enable();

  sgtl5000_1.muteHeadphone();
  sgtl5000_1.muteLineout();

  sgtl5000_1.volume(.8);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(7);
  sgtl5000_1.lineOutLevel(16);

  AudioMemory(30);

  waveform1.begin(1, 0, WAVEFORM_ARBITRARY);
  waveform2.begin(1, 0, WAVEFORM_ARBITRARY);
  waveform3.begin(1, 0, WAVEFORM_ARBITRARY);
  waveform4.begin(1, 0, WAVEFORM_ARBITRARY);
  waveform5.begin(1, 0, WAVEFORM_SAWTOOTH);
  waveform6.begin(1, 0, WAVEFORM_PULSE);

  waveform11.begin(1, 0, WAVEFORM_SQUARE);

  waveform13.begin(1, 0, WAVEFORM_SQUARE);

  pink1.amplitude(1);
  noise1.amplitude(1);
  delay1.begin(dlyb, DLY_MAX, 500, 0, 10);

  delay2.delay(0, 4.25);  //setereo size

  fft_off();

  mixer1.gain(0, 0, 0, 0);
  mixer2.gain(0, 0, 0, 0);

  mixer4.gain(0, 0, 0, 0);
  mixer5.gain(0, 0, 0, 0);
  mixer9.gain(0, 0, 0, 0);
  mixer10.gain(0, 0, 0, 0);
  mixer11.gain(0, 0, 0, 0);
  mixer12.gain(0, 0, 0, 0);
  mixer13.gain(0, 0, 0, 0);
  mixer14.gain(1, 0, 0, 0);
  mixer16.gain(1, 0, 0, 0);

  mixer17.gain(1, 0, 0, 0);

  mixer5555.gain(0, 0, 0, 0);

  mixer13.gain(1, 1, 1, 0);

  mixer3.gain(1, 0, 0, 0);

  mixer7.gain(1, 0, 0, 0);
  mixer6.gain(1, 0, 0, 0);
  mixer8.gain(0, 0, 0, 0);

  filter1.resonance(1);
  filter1.frequency(20000);

  filter2.resonance(.7);   //varible delay feedback filter
  filter2.frequency(9000);

  envelope1.hold(0);
  envelope1.noteOff();
  envelope1.attack(5);
  envelope1.release(60);

  sh1.begin(1, 0); // (smooth enable 0 or 1 ,amount 0.00-1.00 )

  granular1.begin(granular_bank, granular_bank_len / 2);

  chorus1.begin(chorus_bank, CHORUS_LEN, 2);

  flange1.begin(flange_bank, FLANGE_LEN, FLANGE_LEN / 4, FLANGE_LEN / 4, .5);

  dds_rate = 3000;
  dds_tune = 1.00 / (dds_rate * .000001);




  sgtl5000_1.muteHeadphone();
  sgtl5000_1.muteLineout();


  for (int i = 0; i < 10; ++i)
  {
    once[i] = 1;
  }

  for (int i = 0; i < 10; ++i)
  {
    oncein[i] = 0;
  }
  /*

    for (int i = 0; i < 8; i++) {
      // input_enable = 0;
      patch = i;
      voice_modes();

    }


    for (int i = 0; i < 8; i++) {
      // input_enable = 0;
      patch = i;
      voice_modes();

    }
  */
  delay(500);
  byte ts = digitalRead(top_button_pin);
  //Serial.println(ts);


  if ( ts == 1) {

    AudioInterrupts();

    controls();
    midi_check();
    midi_send();
    mono_synth();
    effects();
    input_modes();
    voice_modes();

    patch = EEPROM.read(10);
    led_sel = EEPROM.read(11) << 8 | EEPROM.read(12);

    if (led_sel > 4094) {
      led_sel = 4094;
    }

    if (led_sel < 0) {
      led_sel = 0;
    }

    fx_sel = EEPROM.read(13);
    input_mode = EEPROM.read(14);
    prev_input_mode = input_mode;

    midi_read_ch = EEPROM.read(16);
    midi_send_ch  = EEPROM.read(18);

    if (midi_send_ch < 1)
    {
      midi_send_ch = 1;
    }

    if (midi_send_ch > 12)
    {
      midi_send_ch = 1;
    }

    if (midi_read_ch > 12)
    {
      midi_read_ch = 0;
    }

    EEPROM.write(18, midi_send_ch);
    EEPROM.write(16, midi_read_ch);

    pmidi_send_ch = midi_send_ch;
    pmidi_read_ch = midi_read_ch;

    MIDI.begin(midi_read_ch);

    cv_in_sel = EEPROM.read(20);
    cv_out_sel = EEPROM.read(22);

    if (cv_in_sel > 7)
    {
      cv_in_sel = 3;
    }

    if (cv_out_sel > 4)
    {
      cv_out_sel = 0;
    }
    pcv_out_sel = cv_out_sel;
    pcv_in_sel = cv_in_sel;

    EEPROM.write(20, cv_in_sel);
    EEPROM.write(22, cv_out_sel);

    if (patch < 1) {
      patch = 1;
    }
  }


  if (ts == 0) {

    for (int er; er < 2048; er++) {
      EEPROM.write(er, 0);
    }

    led_sel = 0;
    cv_out_sel = 0;
    cv_in_sel = 0;
    midi_read_ch = 0;
    midi_send_ch = 1;
    patch = 1;
    fx_sel = 0;
    input_mode = prev_input_mode = 0;

    delay(500);
    analogWrite(ledacle_pin, 4000);
    delay(500);
    analogWrite(ledacle_pin, 0);

    delay(500);
    analogWrite(ledacle_pin, 4000);
    delay(500);
    analogWrite(ledacle_pin, 0);

    Serial.println("clear");
    
    AudioInterrupts();

    controls();
    midi_check();
    midi_send();
    mono_synth();
    effects();
    input_modes();
    voice_modes();


  }


  timer1.begin(ledtimer, dds_rate);
  timer1.priority(255);



  bleep_latch = 1;
  pbleep_latch = 1;
  bbutton_mode = 1;


}


////////////////////////////////////////////////////////////////////////////////////////////////////////



void loop()
{

  controls();
  // printer();
  midi_check();
  midi_send();
  mono_synth();
  effects();
  input_modes();
  voice_modes();
  eeproming();


}



void midi_print() {


  for (int i = 0; i < scan_cnt_max; i++) {
    //  Serial.print(i);
    // Serial.print("-");
    Serial.print(scan_bank[i]);
    Serial.print(" ");
  }
  Serial.println();

  for (int i = 0; i < scan_cnt_max; i++) {
    //  Serial.print(i);
    // Serial.print("-");
    Serial.print(scan_vel_bank[i]);
    Serial.print(" ");
  }

  Serial.println();
  Serial.print(current_note);    Serial.print(" ");    Serial.println(ms_note);
  Serial.print(midiing);    Serial.print(" ");    Serial.println(misw);
  Serial.println();

}

float p2, p1, p2r;
int mtick;

void printer() {

  loopd = micros() - loopt;
  loopt = micros();


  usb_midi_en = 0; //can't do both quickly;
  /*
    float p1r = peak1.read();
    if (p1r > .95) {
      Serial.print("-----------1 CLIP");
      Serial.println(p1r);

    }
  */

  if ((millis() - prevmax) > 2000 ) {
    prevmax = millis();
    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();


  }

  // Serial.println(millis());

  if ((millis() - prev0) > 500) {
    prev0 = millis();
  Serial.println(bpcv);
  Serial.println(cv_in_sel);
  Serial.println(cv_fm);
    Serial.print(AudioProcessorUsageMax());    Serial.print(" ");    Serial.println(AudioMemoryUsageMax());
    
    // Serial.println(String() + ledacle_rate  + " "+ ledlerp  + " " + led_sel  + " " + led_ctrl);
    Serial.println(loopd);


    Serial.println();
  }

}


//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%
//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%
//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%//%%


void ledtimer() {

  ledtd = micros() - ledtm;
  ledtm = micros();

  ltick++;
  if (ltick > 2) {
    amp();  //fft amp
    ltick = 0;
  }


  if (shift_mode == 0)
  {

    if (led_sel < 1388) //12b /3
    {
      ledacle_rate = (led_ctrl * (30.00 / 4095.00)) + .5;
      int16_t tri_adj = (led_sel * (1000.00 / 1388.00)) + 10;
      int16_t led_vtri = vtri(ledacle_rate, tri_adj); //tri adj expeces 0-1023
      led_out = led_vtri;
    }

    if (led_sel > 1388 && led_sel < (3015) )
    {
      ledacle_rate = (led_ctrl * (30.00 / 4095.00)) + .5;
      int16_t tri_adj = map(led_sel, 3015, 1388, 970, 50);
      int16_t led_vsq = vsqu(ledacle_rate, tri_adj);
      led_out = led_vsq;
    }


    if (led_sel > 3015   )
    {
      ledacle_rate = (((led_ctrl - 4095) * -1) / 11.5) + 2.00; //give about 0-350 => 350*ddsrate micorscond max osc period

      random_cnt++;


      lerpdiv = (ledr2 - ledr1) / (ledacle_rate);

      ledlerp += lerpdiv;

      if (ledlerp < 0)
      {
        ledlerp = ledlerp * -1;
      }

      if (ledlerp > 4095)
      {
        ledlerp = ledlerp - ((ledlerp - 4095) * 2);

      }


      if (random_cnt > ledacle_rate)
      {
        ledr1 = ledr2; //current
        ledr2 = random(0, 32) << 7;  //next
        random_ledacle1 = ledr1;
        random_cnt = 0;
      }

      rp_len = (ledacle_rate / 5.00);
      int16_t rlpout;
      if (random_cnt < rp_len)
      {
        rlpout = ledr1;
      }

      if (random_cnt >= rp_len)
      {
        rlpout = 100;
      }


      if (led_sel < 3232)
      {
        led_out = (ledlerp);
      }

      if (led_sel > 3232 && led_sel < 3600)
      {
        // mixl = (3232.00 - led_sel) / (3600.00 - 3232.00);
        //   mixli = (mixl - 1) * -1;
        led_out = (ledlerp * .5) + (random_ledacle1 * .5);
      }
      if (led_sel > 3600 && led_sel < 3900)
      {
        led_out = random_ledacle1;
      }

      if (led_sel > 3930 && led_sel < 4070)
      {
        //mixl = (3930.00 - led_sel) / (4070.00 - 3930.00);
        // mixli = (mixl - 1) * -1;
        led_out = (random_ledacle1 * .5) + (rlpout * .5);
      }

      if (led_sel > 4070)
      {
        led_out = rlpout;
      }

      if (led_out > 4095) {
        // led_out = led_out - ((led_out - 4095) * 2);
        led_out = 4095;
      }

      if (led_out < 0) {
        // led_out = led_out * -1;
        led_out = 0;
      }
      // led_out=led_ctrl;

    }


    if (midi_led_sw == 1)
    {
      led_out = midi_led;
    }

    if ( cv_in_sel == 1)
    {
      led_out = cv_led;
    }

    if (led_ctrl > 4080)
    {
      led_out = 4095;
    }

  }

  if (shift_mode == 1)
  {
    int16_t twinkle1 = random(1700, 1900);
    led_out = twinkle1;
  }


  led_outf = map(led_out, 4095, 0, 4095, 550); //PWM frequency + .5W LED nneds a little bit of shifting up.
  analogWrite(ledacle_pin, led_outf);


  if (cv_out_sel == 0)
  {
    cv_out = pcell1 * range_mod;
  }

  if (cv_out_sel == 1)
  {
    cv_out = ((pcell1 * range_mod) - 4095) * -1;
  }

  if (cv_out_sel == 2)
  {
    cv_out = led_out;
  }

  if (cv_out_sel == 3)
  {
    cv_out = (led_out - 4095) * -1;
  }

  if (cv_out_sel == 4)
  {
    cv_out = (bb_act * 4095);
  }


  analogWrite(A14, cv_out);


}

const uint16_t waveamp = 4095;
const uint16_t wavelength = 1024;
uint32_t accumulator0, increment0, waveindex0;


int16_t vtri(float freq, int16_t knee) {
  int16_t tout;

  if (knee < 0)
  {
    knee = 0;
  }

  if (knee > wavelength - 1)
  {
    knee = wavelength - 1;
  }

  increment0 = (pow(2, 32) * (freq)) / (dds_tune);
  accumulator0 += increment0;
  waveindex0 = ((accumulator0) >> 22);
  //   out[vn]= pgm_read_word(&tritable[waveindex[vn]]);

  if (waveindex0 < knee) {
    tout = ((waveindex0) * waveamp) / knee;
  }

  if (waveindex0 >= knee) {
    tout = ((((waveindex0 - knee) * waveamp) / (wavelength - knee)) * -1) + waveamp;
  }
  return tout;

}


int16_t vsqu(float freq, int16_t knee) {
  int16_t tout;

  if (knee < 0)
  {
    knee = 0;
  }

  if (knee > wavelength - 1)
  {
    knee = wavelength - 1;
  }

  increment0 = (pow(2, 32) * (freq)) / (dds_tune);
  accumulator0 += increment0;
  waveindex0 = ((accumulator0) >> 22);
  //   out[vn]= pgm_read_word(&tritable[waveindex[vn]]);

  if (waveindex0 < knee) {
    tout = waveamp;
  }

  if (waveindex0 >= knee) {
    tout = 0;
  }
  return tout;

}

uint16_t previnput[8];

int read_chage(byte n, int input) {
  int diff = 4;
  //int input = analogRead(n);
  int output;
  if ((input > (previnput[n] + diff)) || (input < (previnput[n] - diff))) {
    output = input;
    previnput[n] = input;

  }


  else {
    output =  previnput[n];

  }
  return output;
}





void bandlimit (byte wave_n) {
  for (int h = 1; h < wave_n; h++) {
    if (freq[h] > 0) {
      for (int wj = 0; wj < table_n; wj++) {


        if (freq[h] > eigth_octave[wj]  && freq[h] <= eigth_octave[wj + 1] && wave_switch[h][wj] == 0) {

          bl_sel[h] = wj;
          //break;
          // flip_switches(h, wj); //???????
        }
      }
    }
  }
}

/*

  void flip_switches(byte voice_num, byte switch_n) {
  for (int i = 0; i < table_n; i++) {

    wave_switch[voice_num][i] = 0;
  }
  wave_switch[voice_num][switch_n] = 1;
  }
*/

void fft_off() {
  fft256_1.activate(0);
  if (patch != 5) {
    sine2.amplitude(0);
    sine3.amplitude(0);
    sine4.amplitude(0);
    sine5.amplitude(0);
    sine6.amplitude(0);
    sine7.amplitude(0);
    sine8.amplitude(0);
    sine9.amplitude(0);

  }
}


void fft_on() {
  fft256_1.activate(1);

  sine2.frequency(freq_out[1]);
  sine2.amplitude(amp_out[1]);

  sine3.frequency(freq_out[2]);
  sine3.amplitude(amp_out[2]);

  sine4.frequency(freq_out[3]);
  sine4.amplitude(amp_out[3]);

  sine5.frequency(freq_out[4]);
  sine5.amplitude(amp_out[4]);

  sine6.frequency(freq_out[5]);
  sine6.amplitude(amp_out[5]);

  sine7.frequency(freq_out[6]);
  sine7.amplitude(amp_out[6]);

  sine8.frequency(freq_out[8]);
  sine8.amplitude(amp_out[8]);

  sine9.frequency(freq_out[9]);
  sine9.amplitude(amp_out[9]);


}



void amp() {
  // ampt = micros();

  if (input_mode == 6) {
    for (int k = 0; k < voice_n; ++k) {
      //fade_rate = .005;
      statusbank[k] = fft_bank[ freqbank[k]];

      if (statusbank[k] < amp_cutoff / 2) {
        statusbank[k] = 0;
      }

      if (statusbank[k] > .9) {
        statusbank[k] = .9;
      }

      if (ampbank[k] < statusbank[k] ) {
        ampbank[k] += fade_rate * 20;
      }

      if (ampbank[k] > statusbank[k] ) {
        ampbank[k] -= fade_rate;
      }

      if (ampbank[k] <= fade_rate && statusbank[k] == 0 ) {
        ampbank[k] = 0;
        freqbank[k] = 0;
        //   phase_out(k);
        statusbank[k] = 0;
      }

    }
  }

  if (input_mode == 7) {

  }

  // ampd = micros() - ampt;

}



float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


#define filterSamples   9              // filterSamples should  be an odd number, no smaller than 3
int sensSmoothArray[12][filterSamples];


int digitalSmooth(int array_n, int rawIn) {   // "int *sensSmoothArray" passes an array to the function - the asterisk indicates the array name is a pointer

  int j, k, temp, top, bottom;
  long total;
  static int i;
  // static int raw[filterSamples];
  static int sorted[filterSamples];
  boolean done;

  i = (i + 1) % filterSamples;    // increment counter and roll over if necc. -  % (modulo operator) rolls over variable
  sensSmoothArray[array_n][i] = rawIn;                 // input new data into the oldest slot

  // Serial.print("raw = ");

  for (j = 0; j < filterSamples; j++) { // transfer data array into anther array for sorting and averaging
    sorted[j] = sensSmoothArray[array_n][j];
  }

  done = 0;                // flag to know when we're done sorting
  while (done != 1) {      // simple swap sort, sorts numbers from lowest to highest
    done = 1;
    for (j = 0; j < (filterSamples - 1); j++) {
      if (sorted[j] > sorted[j + 1]) {    // numbers are out of order - swap
        temp = sorted[j + 1];
        sorted [j + 1] =  sorted[j] ;
        sorted [j] = temp;
        done = 0;
      }
    }
  }

  /*
    for (j = 0; j < (filterSamples); j++){    // print the array to debug
      Serial.print(sorted[j]);
      Serial.print("   ");
    }
    Serial.println();
  */

  // throw out top and bottom 15% of samples - limit to throw out at least one from top and bottom
  bottom = max(((filterSamples * 15)  / 100), 1);
  top = min((((filterSamples * 85) / 100) + 1  ), (filterSamples - 1));   // the + 1 is to make up for asymmetry caused by integer rounding
  k = 0;
  total = 0;
  for ( j = bottom; j < top; j++) {
    total += sorted[j];  // total remaining indices
    k++;
    // Serial.print(sorted[j]);
    // Serial.print("   ");
  }

  //  Serial.println();
  //  Serial.print("average = ");
  //  Serial.println(total/k);
  return total / k;    // divide by number of samples
}

void bubsort(uint16_t a[], uint16_t b[], int size) {
  for (int i = 0; i < (size - 1); i++) {
    for (int o = 0; o < (size - (i + 1)); o++) {
      if (a[o] > a[o + 1]) {
        int t = a[o];
        a[o] = a[o + 1];
        a[o + 1] = t;
        int g = b[o];
        b[o] = b[o + 1];
        b[o + 1] = g;
      }
    }
  }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


