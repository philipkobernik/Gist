#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using namespace std;

#include "AudioFile.h"
#include "Gist.h"

AudioFile<float> audioFile;

std::vector<std::string> get_filenames(std::filesystem::path path) {
	std::vector<std::string> filenames;

	// http://en.cppreference.com/w/cpp/experimental/fs/directory_iterator
	const std::filesystem::directory_iterator end{};

	for (std::filesystem::directory_iterator iter{path}; iter != end;
	     ++iter) {
		// http://en.cppreference.com/w/cpp/experimental/fs/is_regular_file
		if (std::filesystem::is_regular_file(*iter)) {
			// comment out if all names (names of
			// directories tc.) are required
			string filename = iter->path().string();
			if (filename.substr(filename.length() - 3) == "wav") {
				filenames.push_back(filename);
			}
		}
	}

	return filenames;
}

float mtof(float m) { return 8.175799f * powf(2.0f, m / 12.0f); }

const int sampleRate = 44100;
const int frameSize = 1024;
const int hopSize = frameSize / 4;

int main() {
	Gist<float> gist(frameSize, sampleRate);

	// audioFile.printSummary();

	for (const auto& name : get_filenames("/Users/ptk/src/Gist/audio")) {
		std::cout << "👾 " << name << '\n';
		// create meta file
		ofstream outFile;
		std::string metaName = name.substr(0);
		metaName.append(".csv");
		outFile.open(metaName);

		//
		// load audio file
		audioFile.load(name);
		int channel = 0;
		int numSamples = audioFile.getNumSamplesPerChannel();

		// process audio file
		// use Gist to analyze and print a bunch of frames
		//
		//cout << "📝 n, RMS, peak, ZCR, centroid" << endl;
		outFile << "sample, rms, s.centroid, pitch" << endl;
		for (int n = 0; n + frameSize < numSamples; n += hopSize) {
			gist.processAudioFrame(&audioFile.samples[channel][n],
					       frameSize);
			outFile << n << ','
				<< gist.rootMeanSquare() << ','
				//<< gist.peakEnergy() << ','
				//<< gist.zeroCrossingRate() << ','
				<< gist.spectralCentroid() << ','
				//<< gist.spectralCrest() << ','
				//<< gist.spectralFlatness() << ','
				//<< gist.spectralRolloff() << ','
				//<< gist.spectralKurtosis() << ','
				//<< gist.energyDifference() << ','
				//<< gist.spectralDifference() << ','
				//<< gist.highFrequencyContent() << ','
				<< gist.pitch() << endl;
		}

		outFile.close();
	}

	return 0;
}
