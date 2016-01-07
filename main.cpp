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

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "spectrogram.h"

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "FFT Spectrogram");
    window.setFramerateLimit(60);

    // load a sound
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("440Hz.wav"))
    {
        std::cout << "Could not load Soundfile!" << std::endl;
        return 1;
    }

    std::cout << "Sound information:" << std::endl;
    std::cout << " " << soundBuffer.getDuration().asSeconds() << " seconds"           << std::endl;
    std::cout << " " << soundBuffer.getSampleRate()           << " samples / seconds" << std::endl;
    std::cout << " " << soundBuffer.getChannelCount()         << " channels"          << std::endl;
    std::cout << " " << soundBuffer.getSampleCount()          << " samples"           << std::endl;

    sf::Sound sound(soundBuffer);

    const unsigned int FFTSize = 1024;
    Spectrogram spectrogram(soundBuffer, FFTSize);
    spectrogram.setPosition(100, 100);

    sf::Vector2f previousMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the windows events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            else if (event.type == sf::Event::KeyReleased) {
                // close if escape key was pressed
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }

                // play the sound if space was released
                if (event.key.code == sf::Keyboard::Space)
                {
                    if (sound.getStatus() == sf::Sound::Playing)
                        sound.pause();
                    else
                        sound.play();
                }
            }

            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                // "centered" zooming
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                    // calculate the distance between the mouse and the left border of the spectrogram
                    const sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    sf::FloatRect spectrogramRect = spectrogram.getLocalBounds();
                    float distance = mousePosition.x - spectrogram.getPosition().x;
                    // calculate the ration between the mouse and the width of the spectrogram
                    float ratio = distance / spectrogramRect.width;

                    // scale the spectrogram
                    spectrogram.scale(1.f + 0.5f * event.mouseWheelScroll.delta, 1.f);

                    // position spectrogram so that the ratio between the mouse and the width
                    // of the spectrogram is the same as it was before scrolling
                    float newDistance = spectrogram.getLocalBounds().width * ratio;
                    spectrogram.setPosition(mousePosition.x - newDistance, spectrogram.getPosition().y);
                }

            }
        }

        // scrolling
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f mousePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
            sf::Vector2f difference = mousePosition - previousMousePos;

            spectrogram.move(difference.x, 0.f);
        }
        // save the mouse coordinates
        previousMousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // do the FFT
        spectrogram.process();

        if (sound.getStatus() == sf::Sound::Playing)
        {
            spectrogram.updateBar(sound.getPlayingOffset().asSeconds() / sound.getBuffer()->getDuration().asSeconds());
        }

        // clear the window to dark grey
        window.clear(sf::Color(50, 50, 50));

        // draw the spectrogram
        window.draw(spectrogram);

        // display the windows content
        window.display();
    }
    return 0;
}
