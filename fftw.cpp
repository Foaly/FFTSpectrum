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

#include "fftw.h"

#include <mutex>
#include <vector>


namespace
{
    std::mutex s_fftwMutex;
}


FFTW::FFTW(unsigned int FFTLength)
{
    std::vector<float> tempInput(FFTLength);
    std::vector<float> tempReal(FFTLength);
    std::vector<float> tempImag(FFTLength);

    fftwf_iodim dim;
    dim.n  = FFTLength;
    dim.is = 1;
    dim.os = 1;

    std::lock_guard<std::mutex> lock(s_fftwMutex);
    plan = fftwf_plan_guru_split_dft_r2c(1, &dim, 0, NULL, &tempInput[0], &tempReal[0], &tempImag[0], FFTW_ESTIMATE);
}


FFTW::~FFTW()
{
    std::lock_guard<std::mutex> lock(s_fftwMutex);
    fftwf_destroy_plan(plan);
}


void FFTW::process(const float *input, std::vector<float> &realPart, std::vector<float> &imagPart)
{
    float* nonConstInput = const_cast<float*>(input);   // fftw does not take const input even though the data not be manipulated!
    fftwf_execute_split_dft_r2c(plan, nonConstInput, &realPart[0], &imagPart[0]);
}
