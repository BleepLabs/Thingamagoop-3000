

void voice_modes() {


  if (input_enable == 0) {

    if (pinput_enable == 0) {
      for (int i = 0; i < 9; ++i)
      {
        oncein[i] = 1;
      }
    }

    if (patch == 1) {

      mixer4.gain(0, .95, 0, 0);
      mixer16.gain(0, 0, 1, 0);

      freq[1] = master_freq / 2.00;
      freq[2] = master_freq / 4.00;
      freq[3] = master_freq / 8.00;
      freq[4] = master_freq / 16.00;

      waveform1.frequency(freq[1]);
      waveform2.frequency(freq[2]);
      waveform3.frequency(freq[3]);
      waveform4.frequency(freq[4]);

      bandlimit(4);

      waveform1.arbitraryWaveform(square[bl_sel[1]], 20000);
      waveform2.arbitraryWaveform(square[bl_sel[2]], 20000);
      waveform3.arbitraryWaveform(square[bl_sel[3]], 20000);
      waveform4.arbitraryWaveform(square[bl_sel[4]], 20000);

      int16_t amp_pot = master_adj;
      float adj_size = 4095.00 / 3.00;

      if (amp_pot < adj_size) {
        amp_pot_f = amp_pot * (.5 / adj_size);

        mixer1.gain(0, 1.2 - amp_pot_f);
        mixer1.gain(1, amp_pot_f);
        mixer1.gain(2, 0);
        mixer1.gain(3, 0);
      }

      if (amp_pot >= adj_size && amp_pot < (adj_size * 2)) {
        amp_pot_f2 = (amp_pot - adj_size) * (.22 / adj_size);
        amp_pot_f = (amp_pot - adj_size) * (.33 / adj_size);


        mixer1.gain(0, .7 - amp_pot_f2);
        mixer1.gain(1, .5 - amp_pot_f2);
        mixer1.gain(2, amp_pot_f);

        mixer1.gain(3, 0);
      }

      if (amp_pot >= adj_size * 2 && amp_pot < (adj_size * 3)) {
        amp_pot_f2 = (amp_pot - (adj_size * 2)) * (.08 / adj_size);
        amp_pot_f = (amp_pot - (adj_size * 2)) * (.33 / adj_size);
        mixer1.gain(0, .53 - amp_pot_f2);
        mixer1.gain(1, .33 - amp_pot_f2);
        mixer1.gain(2, .33 - amp_pot_f2);
        mixer1.gain(3, amp_pot_f);
      }


    }


    if (patch == 2) {

      bandlimit(4);

      mixer4.gain(0, 1, 0, 0);
      mixer16.gain(0, 0, 1, 0);

      waveform1.arbitraryWaveform(tri[bl_sel[1]], 20000);
      waveform2.arbitraryWaveform(tri[bl_sel[2]], 20000);
      waveform3.arbitraryWaveform(tri[bl_sel[3]], 20000);
      waveform4.arbitraryWaveform(square[bl_sel[4]], 20000);


      freq[1] = master_freq ;
      freq[2] = master_freq / 2 ;
      freq[3] = freq[1] * 2;
      freq[4] = freq[1] / 4;

      waveform1.frequency(freq[1]);
      waveform2.frequency(freq[2]);
      waveform3.frequency(freq[3]);
      waveform4.frequency(freq[4]);

      int16_t amp_pot = master_adj;
      float adj_size = 4095.00 / 3.00;

      if (amp_pot < adj_size) {
        amp_pot_f = amp_pot * (.5 / adj_size);

        mixer1.gain(0, 1 - amp_pot_f);
        mixer1.gain(1, amp_pot_f);
        mixer1.gain(2, 0);
        mixer1.gain(3, 0);
      }

      if (amp_pot >= adj_size && amp_pot < (adj_size * 2)) {
        amp_pot_f2 = (amp_pot - adj_size) * (.22 / adj_size);
        // amp_pot_f = (amp_pot - adj_size) * (1.33 / adj_size);


        mixer1.gain(0, .5 - amp_pot_f2);
        mixer1.gain(1, .5 - amp_pot_f2);
        mixer1.gain(2, amp_pot_f2);

        mixer1.gain(3, 0);
      }

      if (amp_pot >= adj_size * 2 && amp_pot < (adj_size * 3)) {
        amp_pot_f2 = (amp_pot - (adj_size * 2)) * (.08 / adj_size);
        amp_pot_f = (amp_pot - (adj_size * 2)) * (.3 / adj_size);
        mixer1.gain(0, .33 - amp_pot_f2);
        mixer1.gain(1, .33 - amp_pot_f2);
        mixer1.gain(2, .33 - amp_pot_f2);
        mixer1.gain(3, amp_pot_f);
      }

    }

    if (patch == 3) {


      bandlimit(4);

      mixer4.gain(0, 1, 0, 0);
      mixer16.gain(0, 0, 1, 0);

      waveform1.arbitraryWaveform(ramp[bl_sel[1]], 20000);
      waveform2.arbitraryWaveform(ramp[bl_sel[2]], 20000);
      waveform3.arbitraryWaveform(ramp[bl_sel[3]], 20000);
      waveform4.arbitraryWaveform(square[bl_sel[4]], 20000);

      freq[1] = master_freq;
      freq[2] = master_freq * 2.005;
      freq[3] = master_freq / 2.01;
      freq[4] = master_freq / 4.015;
      waveform1.frequency(freq[1]);
      waveform2.frequency(freq[2]);
      waveform3.frequency(freq[3]);
      waveform4.frequency(freq[4]);

      int16_t amp_pot = master_adj;
      float adj_size = 4095.00 / 3.00;

      if (amp_pot < adj_size) {
        amp_pot_f = amp_pot * (.5 / adj_size);

        mixer1.gain(0, 1.2 - amp_pot_f);
        mixer1.gain(1, amp_pot_f);
        mixer1.gain(2, 0);
        mixer1.gain(3, 0);
      }

      if (amp_pot >= adj_size && amp_pot < (adj_size * 2)) {
        amp_pot_f2 = (amp_pot - adj_size) * (.22 / adj_size);
        amp_pot_f = (amp_pot - adj_size) * (.33 / adj_size);


        mixer1.gain(0, .7 - amp_pot_f2);
        mixer1.gain(1, .5 - amp_pot_f2);
        mixer1.gain(2, amp_pot_f);

        mixer1.gain(3, 0);
      }

      if (amp_pot >= adj_size * 2 && amp_pot < (adj_size * 3)) {
        amp_pot_f2 = (amp_pot - (adj_size * 2)) * (.08 / adj_size);
        amp_pot_f = (amp_pot - (adj_size * 2)) * (.3 / adj_size);
        mixer1.gain(0, .53 - amp_pot_f2);
        mixer1.gain(1, .33 - amp_pot_f2);
        mixer1.gain(2, .33 - amp_pot_f2);
        mixer1.gain(3, amp_pot_f);
      }


    }
    if (patch == 4) {

      mixer1.gain(2, 0);
      mixer1.gain(3, 0);
      bandlimit(2);
      mixer4.gain(0, 1, 0, 0);
      mixer16.gain(0, 0, 1, 0);

      waveform1.arbitraryWaveform(ramp[bl_sel[1]], 20000);
      waveform2.arbitraryWaveform(ramp[bl_sel[2]], 20000);
      //  waveform3.arbitraryWaveform(ramp[bl_sel[3]], 20000);
      //  waveform4.arbitraryWaveform(ramp[bl_sel[4]], 20000);


      freq[1] = master_freq / 2.00;
      freq[3] = master_freq / 4.00;
      freq[4] = master_freq / 8.01;
      waveform1.frequency(freq[1]);
      //waveform3.frequency(freq[3]);
      //waveform4.frequency(freq[4]);

      int16_t adj_pot = master_adj;

      mixer1.gain(.5, .5, 0, 0);
      float adj_pot_1 = (adj_pot * (.35 / 4095)) - .01;
      freq[2] = (freq[1] * adj_pot_1) + freq[1];
      waveform2.frequency(freq[2]);

    }

    if (patch > 4 && patch != 7)
    {
      waveform1.frequency(0);
      waveform2.frequency(0);
      waveform3.frequency(0);
      waveform4.frequency(0);
    }

    if (patch != 5) {  //P5

      sine2.amplitude(0);
      sine3.amplitude(0);
      sine4.amplitude(0);
      sine5.amplitude(0 );

      sine6.amplitude(0);
      sine7.amplitude(0);
      sine8.amplitude(0 );
      sine9.amplitude(0);

      sine2.frequency(0);
      sine3.frequency(0);
      sine4.frequency(0);
      sine5.frequency(0 );

      sine6.frequency(0);
      sine7.frequency(0);
      sine8.frequency(0 );
      sine9.frequency(0);


    }

    if (patch == 5) {

      static float mmt = .2;
      mixer11.gain(mmt, mmt, mmt, mmt);
      mixer12.gain(mmt, mmt, mmt, mmt);
      // mixer20.gain(.15, .15, .15, .15);

      mixer9.gain(0, 0, 0, 1);
      mixer13.gain(1, 1, 0, 0);
      mixer16.gain(1, 0, 0, 0);



      uint32_t malog = pow(master_adj, 2);
      malog = malog >> 12;

      float adj_pot = (malog * (3.00 / 4095.00)) + 1;
      float mmmfmfm = master_freq * 1.00; // I thing this is better than casting but im a dummo
      amp5[2] = 1;

      freq[2] = mmmfmfm;
      freq[3] = mmmfmfm * adj_pot  ;
      freq[4] = mmmfmfm / adj_pot  ;
      freq[5] = mmmfmfm * adj_pot * adj_pot ;

      freq[6] = mmmfmfm / adj_pot / adj_pot ;
      freq[7] = mmmfmfm * adj_pot * adj_pot * adj_pot ;
      freq[8] = mmmfmfm / adj_pot / adj_pot / adj_pot ;
      freq[9] = mmmfmfm * adj_pot * adj_pot * adj_pot * adj_pot ;
      /*
            //big slow down when trying to run 12 here and in the fft modes
            freq[10] = mmmfmfm / adj_pot / adj_pot / adj_pot / adj_pot ;
            freq[11] = mmmfmfm * adj_pot * adj_pot * adj_pot * adj_pot * adj_pot;
            freq[12] = mmmfmfm / adj_pot / adj_pot / adj_pot / adj_pot / adj_pot ;
            freq[13] = mmmfmfm * adj_pot * adj_pot * adj_pot * adj_pot * adj_pot * adj_pot;
      */
      amp5[2] = 1;

      for (byte i = 2; i < 10; i++) {
        if (i > 2) {
          amp5[i] = amp5[i - 1] - (amp5[i - 1] * .15);
        }

        if (freq[i] < 40) {
          freq[i] = 0;
          amp5[i] = 0;
        }

      }



      sine2.amplitude(amp5[2]);
      sine3.amplitude(amp5[3]);
      sine4.amplitude(amp5[4]);
      sine5.amplitude(amp5[5]);

      sine6.amplitude(amp5[6]);
      sine7.amplitude(amp5[7]);
      sine8.amplitude(amp5[8]);
      sine9.amplitude(amp5[9]);
      /*
            sine11.amplitude(amp5[10]);
            sine12.amplitude(amp5[11]);
            sine13.amplitude(amp5[12]);
            sine14.amplitude(amp5[13]);
      */
      sine2.frequency(freq[2]);
      sine3.frequency(freq[3] );
      sine4.frequency(freq[4] );
      sine5.frequency(freq[5] );

      sine6.frequency(freq[6] );
      sine7.frequency(freq[7] );
      sine8.frequency(freq[8] );
      sine9.frequency(freq[9] );
      /*
            sine11.frequency(freq[10]);
            sine12.frequency(freq[11] );
            sine13.frequency(freq[12] );
            sine14.frequency(freq[13] );
      */
    }



    if (patch != 6) {  //P6

      sine_fm1.frequency(0);
      sine_fm2.frequency(0);
      sine_fm3.frequency(0);
      sine_fm4.frequency(0);
      sine_fm1.amplitude(0);
      sine_fm2.amplitude(0);
      sine_fm3.amplitude(0);
      sine_fm4.amplitude(0);
      sine_fm4.phase(45);
      mixer4.gain(2, 0);

    }

    if (patch == 6) {

      mixer1.gain(0, 0, 0, 0);
      mixer2.gain(0, 0, 0, 0);
      mixer4.gain(0, 0, 1, 0);
      mixer5.gain(0, 0, 0, 0);
      mixer16.gain(0, 0, 1, 0);

      mixer9.gain(1, 0, 0, 0);
      sine_fm1.amplitude(1);
      sine_fm2.amplitude(1);
      sine_fm3.amplitude(1);
      sine_fm4.amplitude(1);


      float freqt = master_freq / 2;
      freq[1] = freqt;

      sine_fm1.frequency(freq[1]);
      sine_fm2.frequency(freq[2]);
      sine_fm3.frequency(freq[3]);
      sine_fm4.frequency(freq[4]);


      int16_t amp_pot = master_adj;
      float adj_size = 4095.00 / 3.00;

      if (amp_pot < adj_size) {

        float amp_pot_f = amp_pot * (2.00 / adj_size);
        amp_pot_f = (amp_pot_f - 2.00) * -1;
        freq[2] = freq[1] / amp_pot_f;
        freq[3] = freq[1] * .02;
        freq[4] = 0;

      }

      if (amp_pot >= adj_size && amp_pot < (adj_size * 2)) {

        float amp_pot_f = (amp_pot - adj_size) * (30.00 / adj_size);
        amp_pot_f = (amp_pot_f - 30) * -1;
        freq[2] = freq[1] * 2 ;
        freq[3] = freq[1] * amp_pot_f;
        freq[4] = freq[3] * 4;

      }

      if (amp_pot >= adj_size * 2) {

        float amp_pot_f = (amp_pot - (adj_size * 2)) * (50.00 / (adj_size));
        freq[1] = freqt;
        freq[2] = freqt * amp_pot_f;
        freq[3] = freqt * amp_pot_f * .01;
        freq[4] = 0;

      }
    }


    if (patch != 7) {

    }

    if (patch == 7) {

      //mixer1.gain(1, 1, 1, 0);
      mixer2.gain(0, 0, 0, 0);
      mixer4.gain(0, 1.2, 0, 0);
      mixer5.gain(0, 0, 0, 0);
      mixer16.gain(0, 0, 1, 0);

      freq[1] = master_freq / 64.00;
      freq[2] = freq[1];
      freq[3] = freq[1];
      // freq[4] = freq[1];

      waveform1.frequency(freq[1]);
      waveform2.frequency(freq[2]);
      waveform3.frequency(freq[3]);
      //    waveform4.frequency(freq[4]);

      int16_t amp_pot = master_adj;
      float adj_size = 4095.00 / 2.00;

      if (amp_pot < adj_size) {
        waveform1.arbitraryWaveform(sns_e, 20000, sns_e_len);
        waveform2.arbitraryWaveform(sns_oh, 20000, sns_oh_len);
        //  waveform4.arbitraryWaveform(sns_oh, 20000, sns_oh_len);

        float amp_pot_f = amp_pot * (1.00 / adj_size);
        float amp_pot_f_inv = ((amp_pot * (1.00 / adj_size)) - 1) * -1;
        mixer1.gain(0, amp_pot_f_inv);
        mixer1.gain(1, amp_pot_f);
        mixer1.gain(2, 0);
        mixer1.gain(3, 0);
      }

      if (amp_pot >= adj_size && amp_pot < (adj_size * 2)) {

        waveform2.arbitraryWaveform(sns_oh, 20000, sns_oh_len);
        //  waveform4.arbitraryWaveform(sns_oh, 20000, sns_oh_len);
        waveform3.arbitraryWaveform(sns_ah, 20000, sns_ah_len);

        float amp_pot_f = (amp_pot - adj_size) * (1.00 / adj_size);
        float amp_pot_f_inv = (((amp_pot - adj_size) * (1.00 / adj_size)) - 1) * -1;
        mixer1.gain(0, 0);
        mixer1.gain(1, amp_pot_f_inv);
        mixer1.gain(2, amp_pot_f);
        mixer1.gain(3, 0);
      }
    }


    if (patch != 8) {

      //   mixer16.gain(0, 0, 1, 0);

      waveform5.frequency(0);
      waveform5.amplitude(0);
      waveform6.frequency(0);
      waveform6.amplitude(0);
      pink1.amplitude(0);
      noise1.amplitude(0);

    }

    if (patch >= 8) {
      pink1.amplitude(1);
      noise1.amplitude(1);
      mixer16.gain(0, 0, 1, 0);

      float freq_temp = master_freq * (3.00 / 4095);
      mixer4.gain(freq_temp, 0, 0, 0);
      mixer9.gain(1, 0, 0, 0);



      int16_t random_temp1 = random(1, 10);
      float random_amp1 = random_temp1 * .1;
      int16_t random_freq1 = random(1, 1111);

      float random_amp2 = random_temp1 * .1;
      int16_t random_freq2 = random(1, 500);


      int16_t amp_pot = master_adj;


      if (amp_pot < 2048) {
        amp_pot_f = amp_pot * (1.00 / 2048);
        amp_pot_f_inv = (amp_pot_f - 1) * -1;

        mixer2.gain(0, amp_pot_f_inv);
        mixer2.gain(1, amp_pot_f);
        mixer2.gain(2, 0);

      }

      if (amp_pot >= 2048) {

        waveform5.frequency(random_freq1);
        waveform5.amplitude(random_amp1);
        waveform6.frequency(random_freq2);
        waveform6.amplitude(random_amp2);
        waveform6.pulseWidth(random_amp2);

        amp_pot_f = (amp_pot - 2048) * (1.00 / 2048);
        amp_pot_f_inv = (amp_pot_f - 1) * -1;

        mixer2.gain(0, 0);
        mixer2.gain(1, amp_pot_f_inv);
        mixer2.gain(2, amp_pot_f * 6.00);

      }
    }

  }


}


