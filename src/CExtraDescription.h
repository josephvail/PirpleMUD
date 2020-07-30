/*!
 * @file CExtraDescription.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */
#ifndef PIRPLEMUD_CEXTRADESCRIPTION_H
#define PIRPLEMUD_CEXTRADESCRIPTION_H

#include "pirple_.h"

namespace Pirple
{
class CExtraDescription
{
private:
    string keyword;
    string description;
public:
    CExtraDescription(const string& pKeyword, const string& pDescription): keyword(pKeyword), description(pDescription)
    {
    }

    inline const string& GetKeyword() const noexcept
    {
        return keyword;
    }

    inline const string& GetDescription() const noexcept
    {
        return description;
    }
};
}

#endif // PIRPLEMUD_CEXTRADESCRIPTION_H
