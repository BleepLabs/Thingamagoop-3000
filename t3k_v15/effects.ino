void effects() {
  /*
    if (fx_sel == 0 ) // this won't happen
    {
      //  mixer3.gain(1, 0, 0, 0);
      mixer14.gain(1, 0, 0, 0);
      mixer6.gain(1, 0, 0, 0);
      mixer7.gain(1, 0, 0, 0);
      //1 off
    }
  */

  if (fx_sel > 2)
  {
    sine10.frequency(0);
    sine10.amplitude(0);
    waveform11.frequency(0);
    waveform11.amplitude(0);
  }


  if (fx_sel == 1)  {

    int16_t am_adj = master_fx;

    mixer6.gain(1, 0, 0, 0);
    mixer14.gain(1, 0, 0, 0);
    mixer3.gain(1, 0, 0, 0);

    if (master_fx < 20) {
      mixer8.gain(0, 1, 0, 0);
      mixer7.gain(1, 0, 0, 0);
    }

    if (master_fx >= 20 ) {
      mixer8.gain(0, 1, 1, 0);
      mixer7.gain(0, 1, 0, 0);
    }

    if (master_fx >= 20 && master_fx < 2048) {
      freq11 = mapfloat(master_fx, 0.00, 2048.00, 0.00, 10.00);
      freq11 = pow(freq11, 2) / 10.00;
    }

    if (master_fx >= 2048 ) {
      freq11 = mapfloat(master_fx, 2048.00, 4095.00, 10.00, 100.00);
      //freq11=(pow(freq11,2)/100.00)+9;
    }

    dc1.amplitude(.5);
    sine10.frequency(freq11);
    sine10.amplitude(.5);

  }


  if (fx_sel == 2)  {

    int16_t am_adj = master_fx;
    mixer6.gain(1, 0, 0, 0);
    mixer14.gain(1, 0, 0, 0);
    mixer3.gain(1, 0, 0, 0);


    if (master_fx < 20) {
      mixer8.gain(0, 1, 0, 0);
      mixer7.gain(1, 0, 0, 0);
    }

    if (master_fx >= 20 ) {
      mixer8.gain(1, 1, 0, 0);
      mixer7.gain(0, 1, 0, 0);
    }

    if (master_fx >= 20 && master_fx < 2048) {
      freq11 = mapfloat(master_fx, 0.00, 2048.00, 0.00, 10.00);
      freq11 = pow(freq11, 2) / 10.00;
    }

    if (master_fx >= 2048 ) {
      freq11 = mapfloat(master_fx, 2048.00, 4095.00, 10.00, 100.00);
    }

    dc1.amplitude(.5);
    waveform11.frequency(freq11);
    waveform11.amplitude(.5);

  }




  if (fx_sel != 3 && fx_sel != 4)
  {
    mixer6.gain(1, 0, 0, 0);
    mixer17.gain(1, 0, 0, 0);

    filter1.frequency(20000);
    filter1.resonance(.8);
  }

  if (fx_sel == 3)
  {
    // mixer6.gain(0, 1, 0, 0);                            // ADD MIDI AMOUNT AND FILTER TYPE
    master_fx_inv = (master_fx - 4095) * -1;
    res_atten = .75;
    float res = 1.2;

    if (master_fx < 10 ) {

      mixer6.gain(1, 0, 0, 0);
      mixer17.gain(res_atten, 0, 0, 0);

      filter1.frequency(18000);
      filter1.resonance(.71);
    }

    if (master_fx >= 10 && master_fx < 2048) {
      mixer6.gain(0, 0, 1, 0);
      mixer17.gain(res_atten, 0, 0, 0);

      //  float cut = filter_adj * (20000.00 / 4085.00);
      // cut=pow(cut,2);
      cut = mapfloat(master_fx_inv, 4085.00, 2048.00, 20000.00, 6000.00);
      filter1.frequency(cut);
      filter1.resonance(res);
    }

    if (master_fx >= 2048) {
      mixer6.gain(0, 0, 1, 0);
      mixer17.gain(res_atten, 0, 0, 0);

      cut = mapfloat(master_fx_inv, 2048.00, 0.00, 6000.00, 0.00);

      filter1.frequency(cut);
      filter1.resonance(res);
    }

  }

  if (fx_sel == 4)
  {
    // mixer6.gain(0, 1, 0, 0);                            // ADD MIDI AMOUNT AND FILTER TYPE
    master_fx_inv = (master_fx - 4095) * -1;
    res_atten = .57;
    float res = 2.2;

    if (master_fx < 10 ) {

      mixer6.gain(1, 0, 0, 0);
      mixer17.gain(res_atten, 0, 0, 0);

      filter1.frequency(20000);
      filter1.resonance(.9);
    }

    if (master_fx >= 10 && master_fx < 2048) {
      mixer6.gain(0, 1, 0, 0);
      mixer17.gain(res_atten, 0, 0, 0);

      //  float cut = filter_adj * (20000.00 / 4085.00);
      // cut=pow(cut,2);
      cut = mapfloat(master_fx_inv, 4085.00, 2048.00, 18000.00, 6000.00);
      filter1.frequency(cut);
      filter1.resonance(res);
    }

    if (master_fx >= 2048) {
      mixer6.gain(0, 1, 0, 0);
      mixer17.gain(res_atten, 0, 0, 0);

      cut = mapfloat(master_fx_inv, 2048.00, 0.00, 6000.00, 0.00);

      filter1.frequency(cut);
      filter1.resonance(res);
    }

  }


  if (fx_sel != 5) {
    sh1.amount(1);
  }


  if (fx_sel == 5) {
    mixer7.gain(1, 0, 0, 0);

    mixer14.gain(0, 1, 0, 0);
    master_fx_inv = (master_fx - 4095) * -1;
    //  int16_t sh_adj = (master_fx-4080)*-1;

    if (master_fx < 10)
    {
      sh1.amount(1);
    }

    if (master_fx >= 10 && master_fx < 2048)
    {
      //  float sh_adj0=(pow(master_fx,2))/2048.00;
      // sh_adjf = (master_fx_inv-2048) * (.7500 / 2048.00);
      sh_adjf = mapfloat(master_fx_inv, 4085.00, 2048.00, 1.00, .15);
      sh1.amount(sh_adjf);
    }
    if (master_fx >= 2048 )
    {
      //  float sh_adj0=(pow(master_fx,2))/2048.00;
      //  sh_adjf = (master_fx_inv) * (.2500 / 2048.00);
      sh_adjf = mapfloat(master_fx_inv, 2048.00, 0.00, .15, .01);
      sh1.amount(sh_adjf);
    }

  }

  if (fx_sel < 6 )
  {
    mixer3.gain(1, 0, 0, 0);
  }


  if (fx_sel == 6)
  {
    if (master_fx <= 20) {
      mixer3.gain(1, 0, 0, 0);

    }

    master_fx_inv = (master_fx - 4095) * -1;

    if (master_fx >= 10 && master_fx < 2048) {
      mixer3.gain(0, 1, 0, 0);                            // ADD MIDI AMOUNT

      float flage_adj = mapfloat(master_fx_inv, 4085.00, 2048.00, 0.0, 4.00);
      flange1.rate(40, flage_adj);

    }

    if (master_fx >= 2048) {
      mixer3.gain(0, 1, 0, 0);                            // ADD MIDI AMOUNT

      float flage_adj = mapfloat(master_fx_inv, 2048.00, 0.00, 4.00, 60.00);
      flange1.rate(40, flage_adj);                            // ADD MIDI AMOUNT for depth



    }


  }

  if (fx_sel == 7)
  {
    mixer3.gain(1, 0);


    master_fx_inv = (master_fx - 4095) * -1;

    if (master_fx_inv <=  4085) {
      mixer3.gain(0, .7);                          // ADD MIDI AMOUNT for FEEDBACK
      mixer3.gain(3, .8);
    }
    if (master_fx_inv > 4085 ) {
      mixer3.gain(0, 1);
      mixer3.gain(3, 0);
    }


    if (master_fx_inv < 2048) {

      int16_t dly_len2 = map(master_fx_inv, 2048, 0, DLY_MAX * .80, 1);
      if (dly_len2<1){
        dly_len2=1;
      }
      if (dly_len2>DLY_MAX-2){
        dly_len2=DLY_MAX-2;
      }      
      delay1.length(dly_len2);

    }

    if (master_fx_inv >= 2048) {

      int16_t dly_len2 = map(master_fx_inv, 4095, 2048, DLY_MAX - 2, DLY_MAX * .80);
      if (dly_len2<1){
        dly_len2=1;
      }
      if (dly_len2>DLY_MAX-2){
        dly_len2=DLY_MAX-2;
      }    
      delay1.length(dly_len2);

    }



  }

}
