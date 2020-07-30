/*!
 * @file pirple_utils_.h
 * @brief Some internal utilities that will be included in all headers.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa pirple_.h, PirpleMUD.h
 */

#ifndef PIRPLEMUD_PIRPLE_UTILS_H
#define PIRPLEMUD_PIRPLE_UTILS_H

namespace Pirple
{

/*!
 * @typedef pm_flags
 * @return unsigned long long
 */
typedef unsigned long long pm_flags;

/*!
 * @fn UtilBitIsSet(pm_flags pList, pm_flags pBit)
 * @param pList pm_flags bitset to check if bit exists in.
 * @param pBit pm_flags bit to check in our bitset.
 * @return bool true if the bit is in our set, or false.
 * @sa UtilSetBit(), UtilRemoveBit(), UtilToggleBit()
 */
inline bool UtilBitIsSet(pm_flags pList, pm_flags pBit) noexcept
{
    return (pList & pBit) ? true : false;
}

/*!
 * @fn UtilSetBit(pm_flags pList, pm_flags pBit)
 * @param pList pm_flags bitset to set bit on.
 * @param pBit pm_flags bit to set in our bitset.
 * @return pm_flags the new bitset with the bit added.
 * @sa UtilBitIsSet(), UtilRemoveBit(), UtilToggleBit()
 */
inline pm_flags UtilSetBit(pm_flags pList, pm_flags pBit) noexcept
{
    pList |= pBit;
    return pList;
}

/*!
 * @fn UtilRemoveBit(pm_flags pList, pm_flags pBit)
 * @param pList pm_flags bitset to remove bit on.
 * @param pBit pm_flags bit to remove in our bitset.
 * @return pm_flags the new bitset with the bit removed.
 * @sa UtilBitIsSet(), UtilSetBit(), UtilToggleBit()
 */
inline pm_flags UtilRemoveBit(pm_flags pList, pm_flags pBit) noexcept
{
    pList &= ~pBit;
    return pList;
}

/*!
 * @fn UtilToggleBit(pm_flags pList, pm_flags pBit)
 * @param pList pm_flags bitset to toggle bit on.
 * @param pBit pm_flags bit to toggle in our bitset.
 * @return pm_flags the new bitset with the bit toggled.
 * @sa UtilBitIsSet(), UtilSetBit(), UtilRemoveBit()
 */
inline pm_flags UtilToggleBit(pm_flags pList, pm_flags pBit) noexcept
{
    pList ^= pBit;
    return pList;
}
}

#endif //PIRPLEMUD_PIRPLE_UTILS_H
