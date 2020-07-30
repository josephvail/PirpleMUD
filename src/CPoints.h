/*!
 * @file CPoints.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_CPOINTS_H
#define PIRPLEMUD_CPOINTS_H

#include "pirple_.h"

// TODO make this much better using operator overloads
template<typename T>
class CPoints
{
private:
    T current = 0;
    T minimum = 0;
    T maximum = 0;
public:
    CPoints(const T& pMin, const T& pMax, T pCurrent = 0): current(pCurrent), minimum(pMin), maximum(pMax)
    {
    }

    const T& GetCurrent() const
    {
        return current;
    }

    const T& GetMinimum() const
    {
        return minimum;
    }

    const T& GetMaximum() const
    {
        return maximum;
    }

    T Add(const T& pPoints)
    {
        if (pPoints < 0)
        {
            return Subtract(pPoints);
        }
        else
        {
            if (current + pPoints > maximum)
                current = maximum;
            else if (current + pPoints < minimum)
                current = minimum;
            else
                current += pPoints;
            return pPoints;
        }
    }

    T Subtract(const T& pPoints)
    {
        if (pPoints > 0)
        {
            return Add(pPoints);
        }
        else
        {
            if (current - pPoints < minimum)
                current = minimum;
            else if (current - pPoints > maximum)
                current = maximum;
            else
                current -= pPoints;
            return current;
        }
    }
};

#endif // PIRPLEMUD_CPOINTS_H
