/*!
 * @file COutput.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_UTILS_CONVERSION_COUTPUT_H
#define PIRPLEMUD_UTILS_CONVERSION_COUTPUT_H

#include <string>
#include "CInput.h"

using namespace std;

namespace Pirple {

    class COutput {
    protected:
        string path = "";
        CInput* input = nullptr;
    protected:
        COutput(): path(""), input(nullptr) {

        }
    public:
        COutput(const string& pPath, CInput* pInput): path(pPath), input(pInput) {

        }

        virtual bool Write() = 0;
    };

}

#endif //PIRPLEMUD_UTILS_CONVERSION_COUTPUT_H
