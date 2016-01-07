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

#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "fft.h"

#include <vector>

class Spectrogram : public sf::Drawable, public sf::Transformable
{
public:
    Spectrogram(const sf::SoundBuffer& soundbuffer, unsigned int FFTSize);

    void process();

    void updateBar(float relativePosition);

    sf::FloatRect        getLocalBounds() const;


private:

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    const unsigned int                      m_FFTSize;
    const unsigned int                      m_outputSize;
    FFT                                     m_fft;
    std::vector<sf::Int16>                  m_samples;
    std::vector<sf::Int16>::const_iterator  m_iter;
    sf::Image                               m_image;
    sf::Sprite                              m_sprite;
    sf::Texture                             m_texture;
    int                                     m_x;
    sf::RectangleShape                      m_playPositionBar;
};

#endif // SPECTROGRAM_H
