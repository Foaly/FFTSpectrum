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

#ifndef FFTSPECTRUM_APPLICATION_HPP
#define FFTSPECTRUM_APPLICATION_HPP

#include "Spectrogram.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include <memory>

class Application
{
public:
    Application();

    int run();

private:

    void handleEvents();

    void update();

    void draw();

    void updatePlayProgressBar();


    sf::RenderWindow                m_window;
    sf::SoundBuffer                 m_soundBuffer;
    sf::Sound                       m_sound;
    unsigned int                    m_FFTSize;
    std::unique_ptr<Spectrogram>    m_spectrogram;
    sf::RectangleShape              m_playProgressBar;
    sf::Vector2f                    m_previousMousePos;
    bool                            m_hasFocus;
};


#endif //FFTSPECTRUM_APPLICATION_HPP
