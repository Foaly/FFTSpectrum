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

#include "spectrogram.h"

#include <iostream>

namespace
{
    /**
     * @brief windowFunction This is the window function for the FFT. It interpolates
     *                       in a triangle that looks like this:
     *                       1_|
     *                         |   /\
     *                         |  /  \
     *                         | /    \
     *                       0_|/______\__
     *                         0   0.5  1.0
     *
     * @param amount A value between [0, 1] (corresponds to the x-axis)
     * @return The interpolated value (corresponds to the y-axis)
     */
    float windowFunction (float amount)
    {
        return -(std::abs(amount - 0.5f) * 2.f) + 1;
    }
}


Spectrogram::Spectrogram(const sf::SoundBuffer &soundBuffer, unsigned int FFTSize) :
    m_FFTSize(FFTSize),
    m_outputSize(m_FFTSize / 2 + 1), // FFTW returns N/2+1
    m_fft(m_FFTSize)
{
    // get the samples as ints
    m_samples = std::vector<sf::Int16>(soundBuffer.getSamples(), soundBuffer.getSamples() + soundBuffer.getSampleCount());
    // make sure it can always be devided through FFTSize without remainder, if necessary add 0's
    auto remainder = FFTSize - (m_samples.size() % FFTSize);
    m_samples.insert(m_samples.end(), remainder, 0);

    // calculate how many times the FFT will be called
    unsigned int numberOfRepeats = m_samples.size() / (FFTSize / 2);

    m_image.create(numberOfRepeats, m_outputSize);

    if (!m_texture.loadFromImage(m_image))
    {
        std::cout << "Could not create a texture!" << std::endl;
        // possibly throw exception
    }
    m_sprite.setTexture(m_texture);

    m_x = 0;

    m_iter = m_samples.cbegin();
}


void Spectrogram::process()
{
    if (m_iter + m_FFTSize < m_samples.end()) // maybe this should be <=
    {
        int i = 0;
        std::vector<float> sampleChunck(m_FFTSize);
        std::transform(m_iter, m_iter + m_FFTSize, sampleChunck.begin(),
                       [&i, this] (sf::Int16 sample)
                       {
                           float scaledFloat = static_cast<float>(sample) / 32767.f;
                           scaledFloat *= windowFunction(static_cast<float>(i) / m_FFTSize);
                           ++i;
                           return scaledFloat;
                       } );
        m_iter += m_FFTSize / 2; // 50% sliding window

        std::vector<float> realPart (m_outputSize, 0.f);
        std::vector<float> imagPart (m_outputSize, 0.f);
        m_fft.process(&sampleChunck[0], realPart, imagPart);


        std::vector<float> magnitudeVector;
        for (std::size_t i = 0; i < m_outputSize; ++i)
        {
            magnitudeVector.push_back(std::sqrt(realPart[i] * realPart[i] + imagPart[i] * imagPart[i]));
        }

        for (unsigned int i = 0; i < magnitudeVector.size(); ++i)
        {
            //std::cout << magnitudeVector[i] << " ";
            sf::Uint8 intensity = static_cast<sf::Uint8>(magnitudeVector[i] * 255);
            m_image.setPixel(m_x, magnitudeVector.size() - 1 - i, sf::Color(intensity, intensity, intensity));
        }
        //std::cout << std::endl << std::endl;

        m_texture.update(m_image);

        m_x++;
    }
}


void Spectrogram::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= getTransform();

    // draw the sprite
    target.draw(m_sprite, states);
}
