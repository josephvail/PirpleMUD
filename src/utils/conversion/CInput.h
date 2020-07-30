/*!
 * @file CInput.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_UTILS_CONVERSION_CINPUT_H
#define PIRPLEMUD_UTILS_CONVERSION_CINPUT_H

#include <string>

using namespace std;

namespace Pirple {

    class CInput {
    protected:
        string path = "";
    protected:
        CInput(): path("") {

        }
    public:
        explicit CInput(const string& pPath): path(pPath) {

        }

        virtual bool Read() = 0;
    };

}

#endif //PIRPLEMUD_UTILS_CONVERSION_CINPUT_H
