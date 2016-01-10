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

#ifndef FFTSPECTRUM_INTERPOLATION_HPP
#define FFTSPECTRUM_INTERPOLATION_HPP

#include <SFML/Graphics/Color.hpp>

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
float windowFunction (float amount);


/**
 * @brief HSLtoRGB This function converts a color in the HSL color space to
 *                 the RGB color space. The conversion formula is taken from here:
 *                 https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSL
 *
 * @param hue         The hue in range [0, 360)
 * @param saturation  The saturation in range[0, 1]
 * @param lightness   The lightness in range [0, 1]
 *
 * @return The color in RGB color space.
 */
sf::Color HSLtoRGB (float hue, float saturation, float lightness);

/**
 * @brief Linear interpolation from start to end according to the interpolation weight amount.
 *
 * @param start   The minimum value
 * @param end     The maximum value
 * @param amount  The interpolation weight
 *
 * @return The interpolated value
 */
float linearInterpolation(float start, float end, float amount);

#endif //FFTSPECTRUM_INTERPOLATION_HPP
