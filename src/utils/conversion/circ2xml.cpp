/*!
 * @file circ2xml.cpp
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#include "CPirpleConverter.h"
#include "CCircleInput.h"
#include "CXMLOutput.h"

using namespace Pirple;

// Expects an absolute path to the CircleMUD lib dir
string CONVERT_FROM_CIRCLE_PATH = "/home/jvail/Documents/code/CircleMUD/circleforpirple/lib/";
// Expects an absolute path to the PirpleMUD lib dir
string CONVERT_TO_PIRPLE_LIB = "/home/jvail/Documents/code/eclipsecpp-workspaces/PirpleMUD/lib/";

int main(int argc, char** argv) {
    auto input = new CCircleInput(CONVERT_FROM_CIRCLE_PATH);
    auto output = new CXMLOutput(CONVERT_TO_PIRPLE_LIB, input);
    CPirpleConverter converter(input, output);
    converter.Convert();
    delete input;
    delete output;
    return 0;
}
