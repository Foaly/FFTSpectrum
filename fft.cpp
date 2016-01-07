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

#include "fft.h"

#include <mutex>
#include <vector>


namespace
{
    std::mutex s_fftwMutex;
}


FFT::FFT(unsigned int FFTLength) :
    m_outputSize(FFTLength / 2 + 1) // FFTW returns N/2+1
{
    m_realPart.resize(m_outputSize);
    m_imagPart.resize(m_outputSize);

    // make the initial state meaningful
    std::fill(m_realPart.begin(), m_realPart.end(), 0.f );
    std::fill(m_imagPart.begin(), m_imagPart.end(), 0.f );

    std::vector<float> tempInput(FFTLength);
    std::vector<float> tempReal(FFTLength);
    std::vector<float> tempImag(FFTLength);

    fftwf_iodim dim;
    dim.n  = FFTLength;
    dim.is = 1;
    dim.os = 1;

    std::lock_guard<std::mutex> lock(s_fftwMutex);
    m_plan = fftwf_plan_guru_split_dft_r2c(1, &dim, 0, NULL, &tempInput[0], &tempReal[0], &tempImag[0], FFTW_ESTIMATE);
}


FFT::~FFT()
{
    std::lock_guard<std::mutex> lock(s_fftwMutex);
    fftwf_destroy_plan(m_plan);
}


void FFT::process(const float *input)
{
    float* nonConstInput = const_cast<float*>(input);   // fftw does not take const input even though the data not be manipulated!
    fftwf_execute_split_dft_r2c(m_plan, nonConstInput, &m_realPart[0], &m_imagPart[0]);
}


const std::vector<float>& FFT::realPart()
{
    return m_realPart;
}


const std::vector<float>& FFT::imagPart()
{
  return m_imagPart;
}


const std::vector<float> &FFT::magnitudeVector()
{
    m_magnitudeVector.clear();

    for (std::size_t i = 0; i < m_outputSize; ++i)
    {
        m_magnitudeVector.push_back(std::sqrt(m_realPart[i] * m_realPart[i] + m_imagPart[i] * m_imagPart[i]));
    }

    return m_magnitudeVector;
}
