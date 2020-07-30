/*!
 * @file CRoom.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_CROOM_H
#define PIRPLEMUD_CROOM_H

#include "pirple_.h"
#include "CExtraDescription.h"

namespace Pirple
{
class CZone;
class CItem;
class CCharacter;

class CRoom
{
public:
    struct SDirectionData
    {
        string GeneralDescription = "";
        string Keyword = "";
        pm_flags DoorFlags = 0;
        weak_ptr<CItem> Key;
        weak_ptr<CRoom> ToRoom;
    };

    enum SYSTEMFLAG
    {
        SF_DARK         = (1 << 0L),
        SF_DEATH        = (1 << 1L),
        SF_NO_MOB       = (1 << 2L),
        SF_INDOORS      = (1 << 3L),
        SF_PEACEFUL     = (1 << 4L),
        SF_SOUNDPROOF   = (1 << 5L),
        SF_NO_TRACK     = (1 << 6L),
        SF_NO_MAGIC     = (1 << 7L),
        SF_TUNNEL       = (1 << 8L),
        SF_PRIVATE      = (1 << 9L),
        SF_GOD_ROOM     = (1 << 10L),
        SF_HOUSE        = (1 << 11L),
        SF_ATRIUM       = (1 << 12L)
    };
    enum { NUM_SYSTEMFLAGS = 13 };
    enum TEMPFLAG
    {
        TF_HOUSE_CRASH  = (1 << 0L),
        TF_OLC          = (1 << 1L),
        TF_BFS_MARK     = (1 << 2L)
    };
    enum { NUM_TEMPFLAGS = 3 };

    enum DOORFLAG
    {
        DF_IS_DOOR      = (1 << 0),
        DF_CLOSED       = (1 << 1),
        DF_LOCKED       = (1 << 2),
        DF_NO_PICK      = (1 << 3)
    };
    enum { NUM_DOORFLAGS = 4 };

    enum SECTOR
    {
        SECTOR_NONE = 0,
        SECTOR_INSIDE,
        SECTOR_CITY,
        SECTOR_FIELD,
        SECTOR_FOREST,
        SECTOR_HILLS,
        SECTOR_MOUNTAIN,
        SECTOR_WATER_SWIM,
        SECTOR_WATER_NOSWIM,
        SECTOR_FLYING,
        SECTOR_UNDERWATER
    };
    enum { NUM_SECTORS = SECTOR_UNDERWATER + 1 };

private:
    pm_index number = NOWHERE;
    weak_ptr<CZone> zone;
    SECTOR sector = SECTOR_NONE;
    string name = "";
    string description = "";
    list<CExtraDescription> extraDescriptions;
    unique_ptr<SDirectionData> directions[NUM_DIRECTIONS];
    pm_flags systemFlags = 0;
    pm_flags tempFlags = 0;
    unsigned short lights = DARKNESS;
    list<weak_ptr<CItem>> items;
    list<weak_ptr<CCharacter>> characters;
    // TODO SPECIAL(*func);
public:
    virtual ~CRoom()
    {

    }

    inline const pm_index& GetNumber() const noexcept
    {
        return number;
    }

    inline const SECTOR& GetSector() const noexcept
    {
        return sector;
    }

    inline const string& GetName() const noexcept
    {
        return name;
    }

    inline const string& GetDescription() const noexcept
    {
        return description;
    }

    const list<CExtraDescription>& GetExtraDescriptions() const noexcept
    {
        return extraDescriptions;
    }
};

}

#endif //PIRPLEMUD_CROOM_H
