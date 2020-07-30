/*!
 * @file CItem.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_CITEM_H
#define PIRPLEMUD_CITEM_H

#include "pirple_.h"
#include "CExtraDescription.h"
#include "CCharacter.h"

namespace Pirple
{
class CZone;
class CRoom;

class CItem
{
public:
    enum
    {
        DEFAULT_STAFF_LEVEL = 12,
        DEFAULT_WAND_LEVEL = 12
    };

    enum TYPE
    {
        TYPE_NONE = 0,
        TYPE_LIGHT,
        TYPE_SCROLL,
        TYPE_WAND,
        TYPE_STAFF,
        TYPE_WEAPON,
        TYPE_RANGED_WEAPON,
        TYPE_MISSILE,
        TYPE_TREASURE,
        TYPE_ARMOR,
        TYPE_POTION,
        TYPE_WORN,
        TYPE_OTHER,
        TYPE_TRASH,
        TYPE_TRAP,
        TYPE_CONTAINER,
        TYPE_NOTE,
        TYPE_DRINK_CONTAINER,
        TYPE_KEY,
        TYPE_FOOD,
        TYPE_MONEY,
        TYPE_PEN,
        TYPE_BOAT,
        TYPE_FOUNTAIN
    };
    enum { NUM_TYPES = TYPE_FOUNTAIN + 1 };

    enum WEARFLAG
    {
        WF_TAKE         = (1 << 0L),
        WF_FINGER       = (1 << 1L),
        WF_NECK         = (1 << 2L),
        WF_BODY         = (1 << 3L),
        WF_HEAD         = (1 << 4L),
        WF_LEGS         = (1 << 5L),
        WF_FEET         = (1 << 6L),
        WF_HANDS        = (1 << 7L),
        WF_ARMS         = (1 << 8L),
        WF_SHIELD       = (1 << 9L),
        WF_ABOUT        = (1 << 10L),
        WF_WAIST        = (1 << 11L),
        WF_WRIST        = (1 << 12L),
        WF_WIELD        = (1 << 13L),
        WF_HOLD         = (1 << 14L)
    };
    enum { NUM_WEARFLAGS = 15 };

    enum SYSTEMFLAG
    {
        SF_GLOW         = (1 << 0L),
        SF_HUM          = (1 << 1L),
        SF_NO_RENT      = (1 << 2L),
        SF_NO_DONATE    = (1 << 3L),
        SF_NO_INVISIBLE = (1 << 4L),
        SF_INVISIBLE    = (1 << 5L),
        SF_MAGIC        = (1 << 6L),
        SF_NO_DROP      = (1 << 7L),
        SF_BLESS        = (1 << 8L),
        SF_ANTI_GOOD    = (1 << 9L),
        SF_ANTI_EVIL    = (1 << 10L),
        SF_ANTI_NEUTRAL = (1 << 11L),
        SF_ANTI_MAGE    = (1 << 12L),
        SF_ANTI_CLERIC  = (1 << 13L),
        SF_ANTI_THIEF   = (1 << 14L),
        SF_ANTI_WARRIOR = (1 << 15L),
        SF_NO_SELL      = (1 << 16L)
    };
    enum { NUM_SYSTEMFLAGS = 17 };

    enum APPLY
    {
        APPLY_NONE = 0,
        APPLY_STRENGTH,
        APPLY_DEXTERITY,
        APPLY_INTELLIGENCE,
        APPLY_WISDOM,
        APPLY_CONSTITUTION,
        APPLY_CHARISMA,
        APPLY_CLASS,
        APPLY_LEVEL,
        APPLY_AGE,
        APPLY_HEIGHT,
        APPLY_WEIGHT,
        APPLY_MANA_POINTS,
        APPLY_HIT_POINTS,
        APPLY_MOVE_POINTS,
        APPLY_GOLD,
        APPLY_EXPERIENCE,
        APPLY_ARMOR_CLASS,
        APPLY_HITROLL,
        APPLY_DAMROLL,
        APPLY_SAVING_PARA,
        APPLY_SAVING_ROD,
        APPLY_SAVING_PETRI,
        APPLY_SAVING_BREATH,
        APPLY_SAVING_SPELL
    };
    enum { NUM_APPLIES = APPLY_SAVING_SPELL + 1 };

    enum CONTAINERFLAG
    {
        CF_CLOSEABLE        = (1 << 0),
        CF_NO_PICK          = (1 << 1),
        CF_CLOSED           = (1 << 2),
        CF_LOCKED           = (1 << 3)
    };
    enum { NUM_CONTAINERFLAGS = 4 };

    enum LIQUID
    {
        LIQUID_NONE = 0,
        LIQUID_WATER,
        LIQUID_BEER,
        LIQUID_WINE,
        LIQUID_ALE,
        LIQUID_DARK_ALE,
        LIQUID_WHISKY,
        LIQUID_LEMONADE,
        LIQUID_FIRE_BREATH,
        LIQUID_LOCAL_SPECIALTY,
        LIQUID_SLIME,
        LIQUID_MILK,
        LIQUID_TEA,
        LIQUID_COFFEE,
        LIQUID_BLOOD,
        LIQUID_SALT_WATER,
        LIQUID_CLEAR_WATER
    };
    enum { NUM_LIQUIDS = LIQUID_CLEAR_WATER + 1 };

    struct SApply
    {
        APPLY Apply = APPLY_NONE;
        signed long Modifier = 0;
    };

    enum { MAX_VALUES = 4 };
    enum { MAX_APPLIES = 6 };
private:
    pm_index number = NOTHING;
    weak_ptr<CZone> zone;
    string name = "";
    string description = "";
    string shortDescription = "";
    string actionDescription = "";
    list<CExtraDescription> extraDescriptions;
    signed long values[MAX_VALUES];
    vector<SApply> applies;
    TYPE type = TYPE_NONE;
    pm_flags wearFlags = 0;
    pm_flags systemFlags = 0;
    pm_flags affectFlags = 0;
    pm_weight weight = 0;
    pm_currency cost = 0;
    pm_currency costPerDay = 0;
    pm_pulse timer = 0;
    list<weak_ptr<CItem>> items;
    weak_ptr<CCharacter> wornBy;
    CCharacter::WEARPOSITION wornOn = CCharacter::WEAR_NONE;
    weak_ptr<CCharacter> carriedBy;
    weak_ptr<CItem> inItem;
    weak_ptr<CRoom> inRoom;
public:
    virtual ~CItem()
    {

    }

    const pm_index& GetNumber() const noexcept
    {
        return number;
    }

    const list<CExtraDescription>& GetExtraDescriptions() const noexcept
    {
        return extraDescriptions;
    }
};

}

#endif //PIRPLEMUD_CITEM_H
