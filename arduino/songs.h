enum {
  A = 0xDC,
  As = 0xDD,
  B = 0xDE,
  C = 0xDF,
  Cs = 0xE0,
  D = 0xE1,
  Ds = 0xE2,
  E = 0xE3,
  F = 0xE4,
  Fs = 0xE5,
  G = 0xE6,
  Gs = 0xE7,
  P = 0xE8
};

int imperial_march[14] = {
  A,A,A,F,C,A,F,C,A,P,E,E,E,F
};


void PlaySong(int *song, int len)
{
  for(int i=0;i<len;i++){
    LCD.write(song[i]);
  }
}
