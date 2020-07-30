/*!
 * @file CZone.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_CZONE_H
#define PIRPLEMUD_CZONE_H

#include "pirple_.h"

namespace Pirple
{
class CRoom;
class CItem;
class CMobile;

class CZone
{
private:
    vector<shared_ptr<CRoom>> rooms;
    list<shared_ptr<CItem>> items;
    list<shared_ptr<CMobile>> mobiles;
};

}

#endif //PIRPLEMUD_CZONE_H
