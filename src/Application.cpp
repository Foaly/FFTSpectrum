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

#include "Application.hpp"
#include "SettingsParser.hpp"

#include <SFML/Window/Event.hpp>

#include <iostream>


namespace
{
    bool isPowerOf2(int x)
    {
        // check if number is bigger than 1 and exactly on bit is set
        return (x > 1) && !(x & (x - 1));
    }
}


Application::Application() :
    m_window(sf::VideoMode(1280, 720), "FFT Spectrogram"),
    m_FFTSize(1024)
{
    m_window.setFramerateLimit(60);

    // load a sound
    if (!m_soundBuffer.loadFromFile("1000Hz.wav"))
    {
        std::cout << "Could not load Soundfile!" << std::endl;
        // maybe throw exeption
    }

    std::cout << "Sound information:" << std::endl;
    std::cout << " " << m_soundBuffer.getDuration().asSeconds() << " seconds"           << std::endl;
    std::cout << " " << m_soundBuffer.getSampleRate()           << " samples / seconds" << std::endl;
    std::cout << " " << m_soundBuffer.getChannelCount()         << " channels"          << std::endl;
    std::cout << " " << m_soundBuffer.getSampleCount()          << " samples"           << std::endl;

    m_sound.setBuffer(m_soundBuffer);

    m_spectrogram = std::unique_ptr<Spectrogram>(new Spectrogram(m_soundBuffer, m_FFTSize));
    m_spectrogram->setPosition(100, 100);

    m_spectrogram->generate();

    m_playProgressBar.setPosition(m_spectrogram->getPosition());
    m_playProgressBar.setSize(sf::Vector2f(2.f, m_spectrogram->getLocalBounds().height));
    m_playProgressBar.setFillColor(sf::Color(133, 15, 15)); // dark red

    // save the initial mouse position
    m_previousMousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));

    // is the window focused
    m_hasFocus = m_window.hasFocus();

    /*sf::Font font;
    if (!font.loadFromFile("DejaVuSansMono.ttf"))
    {
        std::cout << "Could not load font!" << std::endl;
    }
    sf::Text topFreq("22050", font);
    topFreq.setCharacterSize(10);
    topFreq.setOrigin(topFreq.getLocalBounds().width, topFreq.getLocalBounds().height / 2);
    topFreq.setPosition(100, 100);
    */
}


int Application::run()
{
    // run the program as long as the m_window is open
    while (m_window.isOpen())
    {
        // handle events
        handleEvents();

        // update everything
        update();

        // draw everything
        draw();
    }

    return 0;
}


void Application::handleEvents()
{
    // check all the windows events
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            m_window.close();

        else if (event.type == sf::Event::GainedFocus)
            m_hasFocus = true;

        else if (event.type == sf::Event::LostFocus)
            m_hasFocus = false;

        else if (event.type == sf::Event::Resized)
        {
            // update the view to the new size of the window
            sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
            m_window.setView(sf::View(visibleArea));
        }

        else if (event.type == sf::Event::KeyReleased) {
            // close if escape key was pressed
            if (event.key.code == sf::Keyboard::Escape) {
                m_window.close();
            }

            // play the sound if space was released
            else if (event.key.code == sf::Keyboard::Space)
            {
                if (m_sound.getStatus() == sf::Sound::Playing)
                    m_sound.pause();
                else
                    m_sound.play();

                updatePlayProgressBar();
            }

            else if (event.key.code == sf::Keyboard::L)
            {
                SettingsParser settings;
                if (settings.loadFromFile("settings.txt"))
                {
                    std::string filename;
                    settings.get("filename", filename);

                    int newFFTSize = -1;
                    settings.get("FFTSize", newFFTSize);

                    if (!filename.empty())
                    {
                        if (isPowerOf2(newFFTSize))
                        {
                            m_FFTSize = newFFTSize;
                        }
                        else
                        {
                            std::cout << "The FFTSize has to be a power of 2." << std::endl;
                        }

                        // try to load the new sound
                        if (m_soundBuffer.loadFromFile(filename))
                        {
                            m_spectrogram = std::unique_ptr<Spectrogram>(new Spectrogram(m_soundBuffer, m_FFTSize));
                            m_spectrogram->setPosition(100.f, 100.f);
                            m_spectrogram->generate();
                        }
                        else
                        {
                            std::cout << "Could not load soundfile with name: " << filename << std::endl;
                            // maybe throw exeption
                        }
                    }
                    else
                    {
                        std::cout << "There was no filename specified in the settings file!" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Could not load settings file!" << std::endl;
                }
            }
        }

        else if (event.type == sf::Event::MouseWheelScrolled)
        {
            // "centered" zooming
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                // calculate the distance between the mouse and the left border of the spectrogram
                const sf::Vector2f mousePosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
                sf::FloatRect spectrogramRect = m_spectrogram->getLocalBounds();
                float distance = mousePosition.x - m_spectrogram->getPosition().x;
                // calculate the ration between the mouse and the width of the spectrogram
                float ratio = distance / spectrogramRect.width;

                // scale the spectrogram
                m_spectrogram->scale(1.f + 0.5f * event.mouseWheelScroll.delta, 1.f);

                // position spectrogram so that the ratio between the mouse and the width
                // of the spectrogram is the same as it was before scrolling
                float newDistance = m_spectrogram->getLocalBounds().width * ratio;
                m_spectrogram->setPosition(mousePosition.x - newDistance, m_spectrogram->getPosition().y);

                updatePlayProgressBar();
            }
        }
    }
}


void Application::update()
{
    if (m_hasFocus)
    {
        // scrolling
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2f mousePosition(m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)));
            sf::Vector2f difference = mousePosition - m_previousMousePos;

            m_spectrogram->move(difference.x, 0.f);

            updatePlayProgressBar();
        }
    }

    // save the mouse coordinates
    m_previousMousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));

    // do the FFT
    m_spectrogram->updateImage();
    m_spectrogram->updateImage();

    if (m_sound.getStatus() == sf::Sound::Playing)
    {
        updatePlayProgressBar();
    }
}


void Application::draw()
{
    // clear the window to dark grey
    m_window.clear(sf::Color(50, 50, 50));

    // draw the spectrogram
    m_window.draw(*m_spectrogram);

    // draw the play progress bar
    m_window.draw(m_playProgressBar);

    // display the windows content
    m_window.display();
}


void Application::updatePlayProgressBar()
{
    auto position = m_spectrogram->getPosition();
    position.x += m_spectrogram->getLocalBounds().width * m_sound.getPlayingOffset().asSeconds() / m_sound.getBuffer()->getDuration().asSeconds();

    m_playProgressBar.setPosition(position);
}
