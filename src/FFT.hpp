////////////////////////////////////////////////////////////
//
// FFTSpectrum - draw a FFT spectrogram of a sound
// Copyright (C) 2016  Maximilian Wagenbach
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////

#ifndef FFT_H
#define FFT_H

#include <fftw3.h>

#include <vector> // replace with boost aligned vector for SIMD

class FFT
{
public:
    FFT(unsigned int FFTLength);

    ~FFT();

    void                        process(const float* input);
    const std::vector<float>&   realPart();
    const std::vector<float>&   imagPart();
    const std::vector<float>&   magnitudeVector();
    const std::vector<float>&   logarithmicMagnitudeVector();

private:
    fftwf_plan         m_plan;
    std::vector<float> m_realPart;
    std::vector<float> m_imagPart;
    std::vector<float> m_magnitudeVector;
    std::vector<float> m_logarithmicMagnitudeVector;
    const unsigned int m_outputSize;
};

#endif // FFT_H
