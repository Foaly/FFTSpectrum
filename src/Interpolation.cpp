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

#include "Interpolation.hpp"

#include <cmath>


float windowFunction (float amount)
{
    return -(std::abs(amount - 0.5f) * 2.f) + 1;
}


sf::Color HSLtoRGB (float hue, float saturation, float lightness)
{
    const float chroma = (1.f - std::abs(2.f * lightness - 1.f)) * saturation;
    const float H1     = hue / 60.f;
    const float X      = chroma * (1.f - std::abs(std::fmod(H1, 2.f) - 1.f));

    float r, g, b;

    if (0.f <= H1 && H1 < 1.f)
    {
        r = chroma;
        g = X;
        b = 0.f;
    }
    else if (1.f <= H1 && H1 < 2.f)
    {
        r = X;
        g = chroma;
        b = 0.f;
    }
    else if (2.f <= H1 && H1 < 3.f)
    {
        r = 0.f;
        g = chroma;
        b = X;
    }
    else if (3.f <= H1 && H1 < 4.f)
    {
        r = 0.f;
        g = X;
        b = chroma;
    }
    else if (4.f <= H1 && H1 < 5.f)
    {
        r = X;
        g = 0.f;
        b = chroma;
    }
    else if (5.f <= H1 && H1 < 6.f)
    {
        r = chroma;
        g = 0.f;
        b = X;
    }
    else
    {
        r = 0.f;
        g = 0.f;
        b = 0.f;
    }

    const float m = lightness - chroma / 2.f;
    return sf::Color(static_cast<sf::Uint8>((r + m) * 255.f), static_cast<sf::Uint8>((g + m) * 255.f), static_cast<sf::Uint8>((b + m) * 255.f));
}


float linearInterpolation(float start, float end, float amount)
{
    return (end * amount) + (start * (1.f - amount));
}


