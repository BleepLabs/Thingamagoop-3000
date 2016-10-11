
void midi_check() {

  if (usbMIDI.read()) {                      // Is there a usbMIDI message incoming ?
    channel = usbMIDI.getChannel();

    if (channel == midi_read_ch || midi_read_ch == 0)
    {


      byte type = usbMIDI.getType();
      //Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2);


      switch (type) {

        case 1: //def of note on and off is messed up somewhere?
          usbmidiing = 1;

          note = usbMIDI.getData1();
          velocity = usbMIDI.getData2();
          // channel = usbMIDI.getChannel();
          if (velocity > 0) {
            scan_note_on();
            // Serial.print("N ON "); Serial.println(note);

          }
          else {
            scan_note_off();
          }
          break;

        case 0:

          note = usbMIDI.getData1();
          velocity = usbMIDI.getData2();
          // Serial.print(" off "); Serial.println(note);

          scan_note_off();

          break;


        case 3: ///cccontrols

          cc_num = usbMIDI.getData1();
          cc_amt = usbMIDI.getData2();
          //   Serial.println(String("type ") + type + ",  " + cc_num + " " + cc_amt);
          ccup(cc_num, cc_amt);


          break;


        case 6: ///pitchbend
          d1 = usbMIDI.getData1();
          d2 = usbMIDI.getData2();
          pbt = ((d2 << 7) + d1);

          if (pbt >= 8192)
          {
            pitchbend = mapfloat(pbt, 8192.00, 16384.00, 1.00, 2.00);
          }

          if (pbt < 8192)
          {
            pitchbend = mapfloat(pbt, 0.00, 8192.00, .5, 1.00);
          }


        //    Serial.println(String() + d1 + " " + d2 + "   " + pbt + " " + pitchbend);

        default:
          d1 = usbMIDI.getData1();
          d2 = usbMIDI.getData2();
          //       Serial.println(String("Message, type=") + type + ", data = " + d1 + " " + d2);
      }
      //t = millis();

    }
  }


  if (misw == 1 && usbmidiing == 0)
  {
    dinmidiing = 0;
    midiing = 0;

    for (int i = 0; i < scan_cnt_max; i++) {
      scan_vel_bank[i] = 0;
      scan_bank[i] = 0;
    }
  }

  if (dinmidiing == 1)
  {
    midiing = 1;
  }

  if (usbmidiing == 1)
  {
    midiing = 1;
  }

  if (MIDI.read() && misw == 0) {                    // Is there a DIN MIDI message incoming ?

    int16_t type = MIDI.getType();
    channel = MIDI.getChannel();
    d1 = MIDI.getData1();
    d2 = MIDI.getData2();


    if (channel == midi_read_ch || midi_read_ch == 0)
    {


      switch (type) {

        case 144:  //definition of DIN note on and off is messed up somewhere?
          dinmidiing = 1;

          note = d1;
          velocity = d2;
          // channel = usbMIDI.getChannel();
          if (velocity > 0) {
            scan_note_on();
            // Serial.print("N ON "); Serial.println(note);
          }
          else {
            scan_note_off();
          }
          break;

        case 128:

          note = d1;
          velocity = d2;
          // Serial.print(" off "); Serial.println(note);

          scan_note_off();

          break;


        case 176: ///cc

          cc_num = d1;
          cc_amt = d2;
          ccup(d1, d2);

          break;


        case 224: ///pitchbend
          pbt = ((d2 << 7) + d1);

          if (pbt >= 8192)
          {
            pitchbend = mapfloat(pbt, 8192.00, 16384.00, 1.00, 2.00);
          }

          if (pbt < 8192)
          {
            pitchbend = mapfloat(pbt, 0.00, 8192.00, .5, 1.00);
          }
          break;

        //    Serial.println(String() + d1 + " " + d2 + "   " + pbt + " " + pitchbend);

        default:
          // Serial.println(String("DIN ") + type + " ch " + channel + "  " + d1 + " " + d2);
          ;

      }
      //t = millis();

    }

  }

}


void scan_note_on() {

  if (scan_cnt < scan_cnt_max - 1) {

    scan_bank[scan_cnt] = note;
    scan_vel_bank[scan_cnt] = velocity;
    scan_cnt++;

  }

}

void scan_note_off() {
  for (int i = 0; i < scan_cnt_max; ++i)
  {
    if (scan_bank[i] == note)
    {
      //scan_bank[i] = 0;
      //scan_cnt--;

      for (int h = i; h < scan_cnt_max; ++h) {
        if (h < scan_cnt_max) {
          scan_bank[h] = scan_bank[h + 1];
          scan_vel_bank[h] = scan_vel_bank[h + 1];
          //   Serial.print("h "); Serial.print(h); Serial.print(" "); Serial.println(scan_bank[h]);

        }
        if (h >= scan_cnt_max - 1) {
          scan_bank[scan_cnt_max - 1] = scan_bank[scan_cnt_max];
          scan_bank[scan_cnt_max - 1] = 0;

          scan_vel_bank[scan_cnt_max - 1] = scan_vel_bank[scan_cnt_max];
          scan_vel_bank[scan_cnt_max] = 0;
        }
      }
      scan_cnt--;
      if (scan_cnt < 0 || scan_cnt > scan_cnt_max) {
        scan_cnt = 0;
      }

    }
  }



}




int16_t last_note;

void mono_synth() {
  mono_mode = 0;
  prev_ms_note = ms_note;

  if (midiing == 1) {
    if (mono_mode == 0) { //simple mono synth no vel
      ms_note = scan_bank[scan_cnt - 1];

      if (ms_note > 0 && ms_note != prev_ms_note) {
        envelope1.noteOn();
        current_note = chromatic[ms_note];
        last_note = current_note;
      }

      if (ms_note == 0 && prev_ms_note > 0) {

        envelope1.noteOff();
        current_note = last_note;

      }

    }
  }

}

uint32_t prevm1;

void midi_send() {


  if ((millis() - midiprev) > 5) {
    midiprev = millis();


    p_msn = msn;
    msn = (master_freq_temp / 32);

    if (p_msn != msn) {
      usbMIDI.sendNoteOff(p_msn, 127, midi_send_ch);
      usbMIDI.sendNoteOn(msn, 127, midi_send_ch);

      MIDI.sendNoteOff(p_msn, 127, midi_send_ch);
      MIDI.sendNoteOn(msn, 127, midi_send_ch);

    }


    pmscc[0] = mscc[0];
    mscc[0] = master_freq_temp / 32;
    if (pmscc[0] != mscc[0])
    {
      usbMIDI.sendControlChange(14, mscc[0], midi_send_ch);
      MIDI.sendControlChange(14, mscc[0], midi_send_ch);


    }

    pmscc[1] = mscc[1];
    mscc[1] = led_out / 32;
    if (pmscc[1] != mscc[1])
    {
      usbMIDI.sendControlChange(15, mscc[1], midi_send_ch);
      MIDI.sendControlChange(15, mscc[1], midi_send_ch);

    }

  }



}


void panic() {
  for (int i = 0; i < 20; ++i)
  {
    scan_bank[i] = 0;
    scan_vel_bank[i] = 0;

  }
}


void ccup(int16_t cc_num, int16_t cc_amt) {

  if (cc_num == 14)
  {
    //float cct=cc_amt*20.00;
    envelope1.attack(cc_amt * 7); //max is 200ms
  }

  if (cc_num == 15)
  {
    // float cct=cc_amt*20.00;
    envelope1.release(cc_amt * 7);
  }

  if (cc_num == 16)
  {
    midi_adj_lock = 1;
    //     cc_modeadj = cc_amt;
    adj_ctrl = cc_amt << 5;
  }

  if (cc_num == 17)
  {

    patch = (cc_amt >> 4) + 1;

  }
  if (cc_num == 18)
  {
    midi_led_lock = 1;
    led_ctrl = cc_amt;

  }

  if (cc_num == 19)
  {
    led_sel = cc_amt;

  }

  if (cc_num == 20)
  {
    midi_led_sw = 1;
    midi_led = cc_amt << 5;
  }

  if (cc_num == 21)
  {
    fx_sel = cc_amt / 21;
  }
  if (cc_num == 22)
  {
    midi_fx_lock = 1;
    fx_ctrl = cc_amt << 5;
  }

  if (cc_num == 23)
  {
    cur23 = cc_amt;

    if (cur23 > 0 && on23 == 0) {
      envelope1.noteOn();
      Serial.println(String() + " on " + cur22 );
      on23 = 1;
    }

    if (cur23 == 0 && on23 == 1) {
      envelope1.noteOff();
      Serial.println(String() + "   off " + cur22 );
      on23 = 0;
    }




  }


}
