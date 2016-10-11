void controls() {

  pcell_raw = analogRead(pcell_pin);


  uint32_t log_pcell = pow(pcell_raw, 2);
  pcell1 = log_pcell >> 12;


  pot_tick++;



  if (pot_tick == 1)
  {

    pleft = left_lin;
    // left_lin = (analogRead(left_pot_pin)-4095)*-1;
    int lt = (analogRead(left_pot_pin));
    left_lin = digitalSmooth(2, lt);

    pmiddle = middle_lin;
    //middle_lin = (analogRead(middle_pot_pin)-4095)*-1;
    int mt = (analogRead(middle_pot_pin));
    middle_lin = digitalSmooth(3, mt);
  }

  if (pot_tick == 2)
  {
    pright = right_lin;
    //right_lin = (analogRead(right_pot_pin)-4095)*-1;
    int rt = (analogRead(right_pot_pin));
    right_lin = digitalSmooth(4, rt);

    int vt = analogRead(vol_pot_pin);
    vt = digitalSmooth(1, vt);
    vol_pot = read_chage(0, vt) / 4095.00;
    mixer5555.gain(0, vol_pot);
    pot_tick = 0;
  }


  if (shift_mode == 0)
  {

    padj_button = adj_button;
    adj_button = digitalRead(adj_pin);

    pbleep_button = bleep_button;
    bleep_button = digitalRead(bleep_pin);
    pbleep_latch = bleep_latch;

    if (bbutton_mode == 0)
    {
      if (pbleep_button == 1 && bleep_button == 0) {
        if (midiing == 0)
        {
          envelope1.noteOn();
        }        bb_act = 1;
      }

      if (pbleep_button == 0 && bleep_button == 1) {
        if (midiing == 0)
        {
          envelope1.noteOff();
        }
        bb_act = 0;

      }
    }

    if (bbutton_mode == 1)
    {
      if (pbleep_button == 1 && bleep_button == 0) {
        bleep_latch = !bleep_latch;

      }


      if (pbleep_latch == 0 && bleep_latch == 1) {
        if (midiing == 0)
        {
          envelope1.noteOff();
        }
        bb_act = 0;

      }
      if (pbleep_latch == 1 && bleep_latch == 0) {
        if (midiing == 0)
        {
          envelope1.noteOn();
        }      bb_act = 1;

      }
    }



    if (adj_button == 0 && bleep_button == 0 && pbleep_button == 1) {
      bbutton_mode = !bbutton_mode;
    }


    pcv_trig = cv_trig;
    cv_trig = digitalRead(trig_in_pin);

    if (cv_trig == 0 && pcv_trig == 1) {
      envelope1.noteOn();
    }

    if (cv_trig == 1 && pcv_trig == 0) {
      envelope1.noteOff();
    }



    byte eye_up = digitalRead(eye_up_pin);
    byte eye_down = digitalRead(eye_down_pin);

    if (eye_up == 0) //eye=freq
    {
      eyem = 0;
      master_freq_temp = (pcell1) * range_mod;
      //master_freq_temp = chromatic[ms_note];
      master_adj = adj_ctrl;
      master_fx = fx_ctrl;
    }

    if (eye_up == 1 && eye_down == 1) // eye adj
    {
      eyem = 1;
      int32_t freq_log = pow(adj_ctrl * .7, 2);
      master_freq_temp = (freq_log >> 11) + 30;

      master_adj = pcell1 * range_mod;
      master_fx = fx_ctrl;
    }

    if (eye_down == 0)
    {
      eyem = 2;
      int32_t freq_log = pow(fx_ctrl * .7, 2);
      master_freq_temp = (freq_log >> 11) + 30;

      master_adj = adj_ctrl;
      master_fx = ((pcell1 - 4095.00) * -1.00 ) * range_mod;
    }



    byte range_up = digitalRead(range_up_pin);
    byte range_down = digitalRead(range_down_pin);



    if (range_down == 0)
    {
      range_mod = 2.50;
    }

    if (range_up == 1 && range_down == 1)
    {
      range_mod = 1;
    }

    if (range_up == 0)
    {
      range_mod = .40;

    }


    if (bb_act == 1 && midiing == 1)
    {
      float ft1 = ((master_freq_temp) / 2) - 500;
      master_freq = ((ft1 + current_note) * pitchbend) + cv_fm;

      //  master_freq = current_note+(master_freq_temp*range_mod);
    }

    if (bb_act == 0 && midiing == 1)
    {
      master_freq = (current_note * pitchbend) + cv_fm;
    }


    if (midiing == 0)
    {
      master_freq = (master_freq_temp) + cv_fm;
    }

    if (adj_button != padj_button) {
      pleft2 = left_lin;
      pmiddle2 = middle_lin;
      pright2 = right_lin;
    }
    //prev_patch_sel = patch_sel;


    if (adj_button == 0) {
      int16_t chg_amt1 = 8;

      if (pleft2 > left_lin + chg_amt1 || pleft2 < left_lin - chg_amt1)
      {
        int16_t pspace = 680;

        patch_sel = left_lin;
        uint8_t ptemp;

        if (patch_sel < 20)
        {
          ptemp = 1;
        }

        if (patch_sel >= 20 && patch_sel < 500)
        {
          ptemp = 2;
        }

        if (patch_sel >= 500 && patch_sel < 1250)
        {
          ptemp = 3;
        }

        if (patch_sel >= 1250 && patch_sel < 2028)
        {
          ptemp = 4;
        }

        if (patch_sel >= 2028 && patch_sel < 2830)
        {
          ptemp = 5;
        }
        if (patch_sel >= 2830 && patch_sel < 3550)
        {
          ptemp = 6;
        }
        if (patch_sel >= 3550 && patch_sel < 4060)
        {
          ptemp = 7;
        }
        if (patch_sel >= 4060)
        {
          ptemp = 8;
        }

        if (input_enable == 1) {
          prev_input_mode = input_mode;
          eet_in = 1;
          input_mode = ptemp - 1;
        }

        if (input_enable == 0) {
          prev_patch = patch;
          eet_patch = 1;
          patch = ptemp ;
        }

        //patch = (patch_sel >> 9) + 1;
      }

      if (pmiddle2 > middle_lin + chg_amt1 || pmiddle2 < middle_lin - chg_amt1)
      { prev_led_sel = led_sel;

        led_sel = middle_lin;
        if (led_sel > 4094) {
          led_sel = 4094;
        }

        if (led_sel < 0) {
          led_sel = 0;
        }
        eet_led = 1;
      }

      if (pright2 > right_lin + chg_amt1 || pright2 < right_lin - chg_amt1)
      {
        // int16_t fx_t = right_lin;
        //fx_sel = fx_t / 818; // 4090/818 =5 not 4095 jsut in case;
        prev_fx_sel = fx_sel;
        eet_fx = 1;
        fx_sel_pot = right_lin;


        //static int16_t fp = 4095.00 / 5.00;

        if (fx_sel_pot < 50)
        {
          fx_sel = 1;
        }


        if (fx_sel_pot >= 50 && fx_sel_pot < (465))
        {
          fx_sel = 2;
        }

        if (fx_sel_pot >= 465 && fx_sel_pot < (925))
        {
          fx_sel = 3;
        }

        if (fx_sel_pot >= 925 && fx_sel_pot < (1475))
        {
          fx_sel = 4;
        }


        if (fx_sel_pot >= 1475 && fx_sel_pot < (2500))
        {
          fx_sel = 5;
        }

        if (fx_sel_pot >= 2500 && fx_sel_pot < (3500))
        {
          fx_sel = 6;
        }


        if (fx_sel_pot >= (3500) )
        {
          fx_sel = 7;
        }

      }



    }


    if (adj_button == 1) {

      if (midi_adj_lock == 0) {
        adj_ctrl = left_lin;
      }
      if (midi_led_lock == 0) {
        led_ctrl = middle_lin;
      }
      if (midi_fx_lock == 0)
      {
        fx_ctrl = right_lin;
      }

      int16_t chg_amt1 = 8;


      if (pleft > left_lin + chg_amt1 || pleft < left_lin - chg_amt1)
      {
        midi_adj_lock = 0;
        //  adj_ctrl = (read_chage(4, left_pot));
      }
      if (pmiddle > middle_lin + chg_amt1 || pmiddle < middle_lin - chg_amt1)
      {
        midi_led_lock = 0;
      }
      if (pright > right_lin + chg_amt1 || pright < right_lin - chg_amt1)
      {
        midi_fx_lock = 0;
      }

    }


  }

  if (first_time == 1) {

    adj_ctrl = left_lin;
    led_ctrl = middle_lin;
    fx_ctrl = right_lin;
    first_time = 0;

  }

  cvisw = digitalRead(cv_in_sw);

  if (cvisw == 1)
  {
    cv_fm = 0;
    cv_in = 0;
  }

  if (cvisw == 0)
  {
    // cv_in = map(analogRead(cv_in_pin), 3800, 100, 4095, 0);
    cv_in_raw = (analogRead(cv_in_pin) - 4095) * -1;
  //  bpcv = cv_in_raw - cv_mid;
  
    bpcv = cv_in_raw;
    
    poscv = map(cv_in_raw, cv_mid, 3770, 0, 4095);
    poscv = constrain(poscv, 0, 4095);

    if (poscv < 0) {
      poscv = 0;
    }

    if (cv_in_sel <= 0)
    {
      cv_fm = (cv_in_raw-2048) / 3.00;
    }

    if (cv_in_sel != 0)
    {
      cv_fm = 0;
    }

    if (cv_in_sel == 1)
    {
      cv_led = (cv_in_raw);
    }

    if (cv_in_sel == 2)
    {
      patch = (poscv >> 9) + 1;
      if (patch<1){
        patch=1;
      }
      if (patch>8){
        patch=8;
      }
    }
    if (cv_in_sel == 3)
    {
      adj_ctrl = bpcv;
    }

    if (cv_in_sel == 4)
    {
      led_sel = poscv;
      if (led_sel > 4094) {
        led_sel = 4094;
      }

      if (led_sel < 0) {
        led_sel = 0;
      }
    }

    if (cv_in_sel == 5)
    {
      led_ctrl = bpcv;
    }

    if (cv_in_sel == 6)
    {
      fx_sel_pot = poscv;
    }

    if (cv_in_sel >= 7)
    {
      fx_ctrl = bpcv;
    }

  }
  prev_top_button = top_button;
  top_button = digitalRead(top_button_pin);

  if (top_button == 0 && prev_top_button == 1 && shift_mode == 0) {
    top_latch = 1;
    tb_start = 0;
    usbmidiing = 0;
    midiing = 0;
    midi_adj_lock = midi_led_lock = midi_fx_lock = 0;
    panic();

  }

  if (top_button == 0 && top_latch == 1) {
    tb_start++;
    if (tb_start > 350)
    {
      shift_mode = 1;
      //envelope1.noteOff();
    }

  }

  if (top_button == 1 && prev_top_button == 0 && shift_mode == 1) {
    top_latch = 0;


  }

  if (top_button == 0 && shift_mode == 1 && top_latch == 0) {
    tb_start = 0;
    shift_mode = 0;
    top_latch = 0;
  }

  if (shift_mode == 1)
  {
    static int chg_amt1 = 12;

    if (pleft2 > left_lin + chg_amt1 || pleft2 < left_lin - chg_amt1)
    {

      pcv_in_sel = cv_in_sel;
      eet_cvi = 1;
      if (left_lin < 30)
      {
        cv_in_sel = 0;
      }

      if (left_lin >= 30 && left_lin < 500)
      {
        cv_in_sel = 1;
      }

      if (left_lin >= 500 && left_lin < 1250)
      {
        cv_in_sel = 2;
      }

      if (left_lin >= 1250 && left_lin < 2028)
      {
        cv_in_sel = 3;
      }

      if (left_lin >= 2028 && left_lin < 2830)
      {
        cv_in_sel = 4;
      }
      if (left_lin >= 2830 && left_lin < 3550)
      {
        cv_in_sel = 5;
      }
      if (left_lin >= 3550 && left_lin < 4060)
      {
        cv_in_sel = 6;
      }
      if (left_lin >= 4060)
      {
        cv_in_sel = 7;
      }



    }

    if (pmiddle2 > middle_lin + chg_amt1 || pmiddle2 < middle_lin - chg_amt1)
    {

      //   midi_read_ch = (middle_lin >> 9);


      if (middle_lin < 20)
      {
        mch_temp = 0;
      }

      if (middle_lin >= 20 && middle_lin < 500)
      {
        mch_temp = 1;
      }

      if (middle_lin >= 500 && middle_lin < 1250)
      {
        mch_temp = 2;
      }

      if (middle_lin >= 1250 && middle_lin < 2028)
      {
        mch_temp = 3;
      }

      if (middle_lin >= 2028 && middle_lin < 2830)
      {
        mch_temp = 4;
      }
      if (middle_lin >= 2830 && middle_lin < 3550)
      {
        mch_temp = 5;
      }
      if (middle_lin >= 3550 && middle_lin < 4060)
      {
        mch_temp = 6;
      }
      if (middle_lin >= 4060)
      {
        mch_temp = 7;
      }

      if (misw == 1) {
        pmidi_read_ch = midi_read_ch;
        midi_read_ch = mch_temp;
        eet_midi = 1;
        if (midi_read_ch > 0)
        {
          MIDI.begin(midi_read_ch);
        }
        if (midi_read_ch == 0)
        {
          MIDI.begin(MIDI_CHANNEL_OMNI);
        }
      }

      if (misw == 0) {
        pmidi_send_ch = midi_send_ch;
        midi_send_ch = mch_temp;
        eet_midisend = 1;

        if (midi_send_ch == 0)
        {
          midi_send_ch = 1;
        }

      }

    }

    if (pright2 > right_lin + chg_amt1 || pright2 < right_lin - chg_amt1)
    {
      pcv_out_sel = cv_out_sel;
      eet_cvo = 1;
      if (right_lin < 550)
      {
        cv_out_sel = 0;
      }

      if (right_lin >= 550 && right_lin < 1540)
      {
        cv_out_sel = 1;
      }

      if (right_lin >= 1540 && right_lin < 2580)
      {
        cv_out_sel = 2;
      }

      if (right_lin >= 2580 && right_lin < 3600)
      {
        cv_out_sel = 3;
      }

      if (right_lin >= 3600 )
      {
        cv_out_sel = 4;
      }

    }

  }

  misw = digitalRead(midi_in_sw);


  aosw = digitalRead(audio_out_sw);

  if (aosw == 0)
  {
    sgtl5000_1.unmuteHeadphone();
    sgtl5000_1.muteLineout();
  }

  if (aosw == 1)
  {
    sgtl5000_1.muteHeadphone();
    sgtl5000_1.unmuteLineout();
  }

  aisw = digitalRead(audio_in_sw);
  pinput_enable = input_enable;
  if (aisw == 0)
  {
    input_enable = 1;
  }

  if (aisw == 1)
  {
    input_enable = 0;
  }

}





void eeproming() {

  if (shift_mode == 0) {
    /*
      if (prev_patch != patch) {
      eet_patch = 1;
      }

      if (prev_led_sel != led_sel) {
      eet_led = 1;
      }

      if (prev_fx_sel != fx_sel) {
      eet_fx = 1;
      }

      if (prev_input_mode != input_mode) {
      eet_in = 1;
      }

      if (pmidi_send_ch!=midi_send_ch)
      {
      //  eet_midisend=1;
      //  Serial.println("     ST ");
      }

      if (pmidi_read_ch!=midi_read_ch)
      {
      //    eet_midi=1;
      //         Serial.println("     RT ");

      }

      if (pcv_in_sel!=cv_in_sel)
      {
      // eet_cvi=1;
      }

      if (pcv_out_sel!=cv_out_sel)
      {
      //eet_cvo=1;
      }
    */

    if ((millis() - prev1) > 1000) {
      prev1 = millis();
      if (eet_patch == 1) {
        EEPROM.write(10, patch);
        eet_patch = 0;
        //  Serial.print("     patch "); Serial.println(patch);

      }
      if (eet_led == 1) {
        if (led_sel > 4094) {
          led_sel = 4094;
        }

        if (led_sel < 0) {
          led_sel = 0;
        }
        EEPROM.write(11, led_sel >> 8);
        EEPROM.write(12, led_sel & 0xFF);
        eet_led = 0;
        //        Serial.print("     led "); Serial.println(led_sel);

      }

      if (eet_fx == 1) {
        EEPROM.write(13, fx_sel);
        //              Serial.print("     fx_sel "); Serial.println(fx_sel);

        eet_fx = 0;
      }

      if (eet_in == 1) {
        EEPROM.write(14, input_mode);
        eet_in = 0;
        //                    Serial.print("     input_mode "); Serial.println(input_mode);
      }

      if (eet_midi == 1) {
        EEPROM.write(16, midi_read_ch);
        eet_midi = 0;
        // pmidi_read_ch=midi_read_ch;
        //  Serial.print("     mR "); Serial.println(midi_read_ch);

      }
      if (eet_midisend == 1) {
        EEPROM.write(18, midi_send_ch);
        eet_midisend = 0;
        //  pmidi_send_ch=midi_send_ch;
        //  Serial.print("     mS ");Serial.print(pmidi_send_ch); Serial.print(" " );  Serial.println(midi_send_ch);

      }


      if (eet_cvi == 1) {
        EEPROM.write(20, cv_in_sel);
        eet_cvi = 0;
        // Serial.print("     cvi "); Serial.println(cv_in_sel);
        // pcv_in_sel=cv_in_sel;
      }

      if (eet_cvo == 1) {
        EEPROM.write(22, cv_out_sel);
        eet_cvo = 0;
        //       Serial.print("     cvo "); Serial.println(cv_out_sel);

        //  pcv_out_sel=cv_out_sel;
      }

    }
  }
}







