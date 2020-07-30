/*!
 * @file CPirpleConverter.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_UTILS_CONVERSION_CPIRPLECONVERTER_H
#define PIRPLEMUD_UTILS_CONVERSION_CPIRPLECONVERTER_H

#include "CInput.h"
#include "COutput.h"

namespace Pirple {
    class CPirpleConverter {
    private:
        CInput* input = nullptr;
        COutput* output = nullptr;
    private:
        CPirpleConverter() = default;
    public:
        CPirpleConverter(CInput* pInput, COutput* pOutput): input(pInput), output(pOutput) {
        }

        bool Convert() {
            if (input) {
                if (input->Read()) {
                    if (output) {
                        return output->Write();
                    }
                }
            }
            return false;
        }

        ~CPirpleConverter() = default;
    };
}

#endif //PIRPLEMUD_UTILS_CONVERSION_CPIRPLECONVERTER_H
