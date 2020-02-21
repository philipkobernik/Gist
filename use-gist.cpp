#include <cmath>
#include <iostream>

using std::cout;
using std::endl;

#include "Gist.h"

float mtof(float m) { return 8.175799f * powf(2.0f, m / 12.0f); }

const int sampleRate = 44100;
const int frameSize = 1024;
const int hopSize = frameSize / 4;

int main() {
  // build a sine sweep in memory
  //
  std::vector<float> audio;
  double phase = 0;
  for (float note = 127; note > 0; note -= 0.001) {
    float frequency = mtof(note);
    audio.push_back(tanh(sin(phase)) * 0.707f);
    phase += 2 * M_PI * frequency / sampleRate;
    if (phase > 2 * M_PI)  //
      phase -= 2 * M_PI;
  }

  // use Gist to analyze and print a bunch of frames
  //
  Gist<float> gist(frameSize, sampleRate);
  cout << "n,RMS,peak,ZCR,centroid" << endl;
  for (int n = 0; n + frameSize < audio.size(); n += hopSize) {
    gist.processAudioFrame(&audio[n], frameSize);
    cout << n                               //
         << ',' << gist.rootMeanSquare()    //
         << ',' << gist.peakEnergy()        //
         << ',' << gist.zeroCrossingRate()  //
         << ',' << gist.spectralCentroid()  //
         << endl;                           //
  }
}
