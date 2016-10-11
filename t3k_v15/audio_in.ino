void input_modes() {


  /////////// INININININI/////////// INININININI/////////// INININININI/////////// INININININI/////////// INININININI

  // input_enable = 0;

  if (input_enable == 1) {
    //  mixer17.gain(1, 0, 0, 0);
    if (pinput_enable == 0) {
      for (int i = 0; i < 9; ++i)
      {
        once[i] = 0;
      }
    }

    if (input_mode == 0) { //through
      mixer16.gain(0, 1, 0, 0);
    }


    if (input_mode == 1) { //through w eye am

      eyeam = (master_freq / 4095.00) * ((master_adj / 1500.00) + 1);
      mixer16.gain(0, eyeam, 0, 0);

    }


    if (input_mode == 2) { //sine am

      mixer16.gain(1, 0, 0, 0);
      mixer9.gain(0, 0, 1, 0);
      mixer10.gain(0, 1, 0, 1);
      waveform13.amplitude(0);
      dc2.amplitude(.5);
      waveform13.frequency(0);

      float sa1 = master_adj / 4095.00;
      sine1.amplitude(sa1);
      am_freq = master_freq * ((2500) / 4095.00);
      sine1.frequency(am_freq);
    }

    if (input_mode != 2) {
      // Serial.println(" 2 0");
      oncein[2] = 0;
    }

    if (input_mode == 3) { //square am

      mixer16.gain(1, 0, 0, 0);
      mixer9.gain(0, 0, 1, 0);
      mixer10.gain(1, 0, 0, 1);
      sine1.amplitude(0);
      dc2.amplitude(.5);
      sine1.frequency(0);

      float sa1 = master_adj / 4095.00;
      waveform13.amplitude(sa1);

      am_freq = master_freq * ((2500) / 4095.00);
      waveform13.frequency(am_freq);

    }

    if (input_mode != 3) {
      oncein[3] = 0;
    }

    if (input_mode == 4) { //glitch shift

      mixer9.gain(0, 1, 0, 0);
      mixer16.gain(1, 0, 0, 0);

      int16_t granfreq = master_freq;
      //int16_t shift_adj=map(master_adj,0,4095,200,(granular_bank_len - 1));
      float shift_adj = master_adj * ((granular_bank_len - 1) / 4095.00);

      granular1.shift(1, master_freq, shift_adj);
    }

    if (input_mode == 5) { //freeze
      mixer9.gain(0, 1.5, 0, 0);
      mixer16.gain(1, 0, 0, 0);

      int16_t granfreq = pcell1;
      int16_t freeze_adj = map(master_adj, 0, 4095, 100, (granular_bank_len / 2  )); //only half of the bank is getting used??
      granular1.freeze(!bleep_button, master_freq, freeze_adj);

    }

    if (input_mode != 4 && input_mode != 5) {
      granular1.shift(0, 0, 0);
      // granular1.freeze(0, 0, 0);

    }


    if (input_mode < 6 ) {
      if (once[4] == 0)
      {
        fft_off();
        // Serial.println(" Foff");
        once[4] = 1;
      }
      // once[5] = 0;

    }

    if (input_mode >= 6 ) {

      fft_on();

      once[4] = 0;

    }




    if (input_mode == 6) {    //fft1
      amp_cutoff = .01;

      mixer9.gain(0, 0, 0, 1);
      //  mixer10.gain(0, 0, 1, 0);
      float g7t = .35;
      mixer11.gain(g7t, g7t, g7t, g7t);
      mixer12.gain(g7t, g7t, g7t, g7t);
      mixer13.gain(.5, .5, 0, 0);
      mixer16.gain(1, 0, 0, 0);

      granfreq = mapfloat(master_freq, 0, 8000.00, .2, 4.00);
      float  ma = (master_adj / 80000.00);
      //float log_ma = pow(ma, 2);
      granadj = ma;
      fade_rate = granadj;

      if (fft256_1.available()) {
        fdone = 0;
        //Serial.println(" a");
        for (int i = 99; i > 0; i -= 2)
        {
          fft_bank[i] = fft256_1.read(i) * 12;

          if (fft_bank[i] >= amp_cutoff) {
            //is it already there?
            byte added = 0;
            for (int j = 0; j < voice_n; ++j)
            {
              if (freqbank[j] == (i))
              {
                added = 1;
              }
            }

            //add
            if (added == 0) {
              for (int k = 0; k < voice_n; ++k)
              {
                if (ampbank[k] == 0) {
                  //  Serial.print(k);      Serial.print(" ");       Serial.println(fft_amp);
                  // phase_out(k);
                  freqbank[k] = (i);
                  statusbank[k] = fft_bank[i];

                  if (statusbank[k] > .9) {
                    statusbank[k] = .9;
                  }
                  added = 1;
                  break;
                }
              }
            }

          }


        }

      }
      for (int m = 0; m < voice_n; m++) {

        amp_out[m] = ampbank[m];

        if (amp_out[m] > 1) {
          amp_out[m] = 1;
        }

        freq_out[m] = (freqbank[m] * 172) * granfreq;
        //freq_out[m] = (freqbank[m] * 64);
        if (freq_out[m] < 1) {
          freq_out[m] = 0;

        }

      }

    }

    if (input_mode >= 7) {    //fft2

      mixer9.gain(0, 0, 0, 1);
      //  mixer10.gain(0, 0, 1, 0);
      float g7t = 1;
      mixer11.gain(g7t, g7t, g7t, g7t);
      mixer12.gain(g7t, g7t, g7t, g7t);
      mixer13.gain(1, 1, 0, 0);
      mixer16.gain(1, 0, 0, 0);

      granfreq = mapfloat(master_freq, 0, 4000.00, .2, 8.00);
      granadj = (master_adj >> 9);
      int16_t ma = granadj;

      if (fft256_1.available()) {


        for (uint8_t m = 0; m < 8; m++) {
          float at;
          if (m < 2)    {
            at = (fft256_1.read((m * 8) + 1, (m * 8))) * 18;
          }
          if (m >= 2) {
            at = (fft256_1.read((m * 8) + 1, (m * 8))) * 25;

          }
          if (at > .9)
          {
            at = .9;
          }


          amp_out[m + 1] = at;
          freq_out[m + 1] = voc1[ma][m] * granfreq ;


        }
      }
    }




  }
}

