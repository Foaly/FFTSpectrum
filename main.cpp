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

#include "fftw.h"

#include <iostream>
#include <vector>

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


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "FFT Spectrogram");
    window.setFramerateLimit(60);

    // test rect
    sf::RectangleShape rect( {100, 100} );
    rect.setFillColor( sf::Color::Red );

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
    const unsigned int outputSize = FFTSize / 2 + 1; // FFTW returns N/2+1

    // get the samples as ints
    std::vector<sf::Int16> samples(soundBuffer.getSamples(), soundBuffer.getSamples() + soundBuffer.getSampleCount());
    // make sure it can always be devided through FFTSize without remainder, if necessary add 0's
    auto remainder = FFTSize - (samples.size() % FFTSize);
    samples.insert(samples.end(), remainder, 0);
    unsigned int numberOfRepeats = samples.size() / (FFTSize / 2);

    sf::Image image;
    image.create(numberOfRepeats, outputSize);

    int x = 0;

    auto iter = samples.cbegin();

    FFTW fft(FFTSize);

    sf::Texture texture;
    if (!texture.loadFromImage(image))
    {
        std::cout << "Could not create a texture!" << std::endl;
        return 1;
    }
    sf::Sprite sprite(texture);
    sprite.setPosition(100, 40);

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
                  sound.play();
            }
        }

        if (iter + FFTSize < samples.end()) // maybe this should be <=
        {
            int i = 0;
            std::vector<float> sampleChunck(FFTSize);
            std::transform(iter, iter + FFTSize, sampleChunck.begin(),
                           [&i, &FFTSize] (sf::Int16 sample)
                           {
                               float scaledFloat = static_cast<float>(sample) / 32767.f;
                               scaledFloat *= windowFunction(static_cast<float>(i) / FFTSize);
                               ++i;
                               return scaledFloat;
                           } );
            iter += FFTSize / 2; // sliding window

            std::vector<float> realPart (outputSize, 0.f);
            std::vector<float> imagPart (outputSize, 0.f);
            fft.process(&sampleChunck[0], realPart, imagPart);


            std::vector<float> magnitudeVector;
            for (int i = 0; i < outputSize; ++i)
            {
                magnitudeVector.push_back(std::sqrt(realPart[i] * realPart[i] + imagPart[i] * imagPart[i]));
            }

            for (unsigned int i = 0; i < magnitudeVector.size(); ++i)
            {
                //std::cout << magnitudeVector[i] << " ";
                sf::Uint8 intensity = static_cast<sf::Uint8>(magnitudeVector[i] * 255);
                image.setPixel(x, magnitudeVector.size() - 1 - i, sf::Color(intensity, intensity, intensity));
            }
            //std::cout << std::endl << std::endl;

            texture.update(image);

            x++;
        }


        // clear the window to black
        window.clear(sf::Color(50, 50, 50));

        window.draw(sprite);

        // draw text
        //window.draw(rect);

        // display the windows content
        window.display();
    }
    return 0;
}
