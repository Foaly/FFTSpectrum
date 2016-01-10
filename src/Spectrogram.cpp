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

#include "Spectrogram.hpp"

#include "Interpolation.hpp"

#include <iostream>
#include <algorithm>

Spectrogram::Spectrogram(const sf::SoundBuffer &soundBuffer, unsigned int FFTSize) :
    m_FFTSize(FFTSize),
    m_outputSize(m_FFTSize / 2 + 1), // FFTW returns N/2+1
    m_fft(m_FFTSize),
    m_maxMagnitude(0.f)
{
    // get the samples as ints
    m_samples = std::vector<sf::Int16>(soundBuffer.getSamples(), soundBuffer.getSamples() + soundBuffer.getSampleCount());
    // make sure it can always be devided through FFTSize without remainder, if necessary add 0's
    auto remainder = FFTSize - (m_samples.size() % FFTSize);
    m_samples.insert(m_samples.end(), remainder, 0);

    // calculate how many times the FFT will be called
    m_numberOfRepeats = m_samples.size() / (FFTSize / 2);

    m_image.create(m_numberOfRepeats, m_outputSize);

    if (!m_texture.loadFromImage(m_image))
    {
        std::cout << "Could not create a texture!" << std::endl;
        // possibly throw exception
    }
    m_sprite.setTexture(m_texture);

    m_playPositionBar.setSize(sf::Vector2f(1.f, m_outputSize));
    m_playPositionBar.setFillColor(sf::Color(133, 15, 15));

    m_magnitudes.reserve(m_numberOfRepeats);

    m_currentX = 0;
}


void Spectrogram::generate()
{
    auto sampleIterator = m_samples.cbegin();
    for (unsigned int i = 0; i < m_numberOfRepeats; ++i)
    {
        int currentSampleIndex = 0;
        std::vector<float> sampleChunck(m_FFTSize);
        std::transform(sampleIterator, sampleIterator + m_FFTSize, sampleChunck.begin(),
                       [&currentSampleIndex, this] (sf::Int16 sample)
                       {
                           float scaledFloat = static_cast<float>(sample) / 32767.f;
                           scaledFloat *= windowFunction(static_cast<float>(currentSampleIndex) / m_FFTSize);
                           ++currentSampleIndex;
                           return scaledFloat;
                       } );
        sampleIterator += m_FFTSize / 2; // 50% sliding window

        m_fft.process(&sampleChunck[0]);


        m_magnitudes.push_back(m_fft.magnitudeVector());

        // find the max element
        auto minmax = std::minmax_element(m_magnitudes.back().begin(), m_magnitudes.back().end());
        // check if it's bigger than any previous one
        if (*minmax.second > m_maxMagnitude)
            m_maxMagnitude = *minmax.second;
    }
}


void Spectrogram::updateImage()
{
    if (m_currentX < m_magnitudes.size())
    {
        std::vector<float>& magnitudeVector = m_magnitudes[m_currentX];

        for (unsigned int i = 0; i < magnitudeVector.size(); ++i)
        {
            //std::cout << magnitudeVector[i] << " ";

            // normalized magnitude in range [0, 1]
            float amount = magnitudeVector[i] / m_maxMagnitude;

            // black and white
            //sf::Uint8 intensity = static_cast<sf::Uint8>(amount * 255);
            //sf::Color color = sf::Color(intensity, intensity, intensity);

            // sunset (white-yellow-red-pink-blue-black)
            // interpolates the hue in range 210 (blue) to 100 (yellow/greenish) with a wrap around
            float hue = std::fmod(linearInterpolation(210.f, 460.f, amount), 360.f);
            sf::Color color = HSLtoRGB(hue, 1.f, amount);

            m_image.setPixel(m_currentX, magnitudeVector.size() - 1 - i, color);
        }
        //std::cout << std::endl << std::endl;

        m_texture.update(m_image);

        ++m_currentX;
    }
}


void Spectrogram::updateBar(float relativePosition)
{
    m_playPositionBar.setPosition(m_sprite.getLocalBounds().width * relativePosition, 0.f);
}


sf::FloatRect Spectrogram::getLocalBounds() const
{
  return getTransform().transformRect(m_sprite.getLocalBounds());
}


void Spectrogram::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= getTransform();

    // draw the sprite
    target.draw(m_sprite, states);

    // draw the play position bar ontop
    target.draw(m_playPositionBar, states);
}
