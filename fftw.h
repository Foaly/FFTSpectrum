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

#ifndef FFTW_H
#define FFTW_H

#include <fftw3.h>

#include <vector> // replace with boost aligned vector for SIMD

class FFTW
{
public:
    FFTW(unsigned int FFTLength);

    ~FFTW();

    void process(const float* input, std::vector<float>& realPart, std::vector<float>& imagPart);

private:
    fftwf_plan plan;
};

#endif // FFTW_H
