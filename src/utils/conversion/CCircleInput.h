/*!
 * @file CCircleInput.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_UTILS_CONVERSION_CCIRCLEINPUT_H
#define PIRPLEMUD_UTILS_CONVERSION_CCIRCLEINPUT_H

#include "../CircleMUD.h"
#include "CInput.h"

namespace Pirple {

    class CCircleInput: public CInput {
    private:
        list<struct zone_data*> zoneTable;
        list<int> zoneMiniNumbers;
        list<struct help_index_element*> helpTable;
    private:
        CCircleInput() = default;
    public:
        explicit CCircleInput(const string& pPath): CInput(pPath) {

        }

        virtual ~CCircleInput() {
            while(!zoneTable.empty()) {
                auto zone = *zoneTable.begin();
                zoneTable.erase(zoneTable.begin());
                delete zone;
            }
            while(!helpTable.empty()) {
                auto help = *helpTable.begin();
                helpTable.erase(helpTable.begin());
                delete help;
            }
        }

        bool Read();

        list<struct zone_data*>& GetZones() {
            return zoneTable;
        }

        list<int>& GetMiniNumbers() {
            return zoneMiniNumbers;
        }

        list<struct help_index_element*>& GetHelps() {
            return helpTable;
        }
    private:
        bool ReadZones();
        struct zone_data *ReadZone(FILE *fl, const string &zfileName);

        struct room_data *ReadRoom(struct zone_data *zone, FILE *fl, int virtual_nr);

        bool ReadRooms(struct zone_data *zone, FILE *fl);

        char *ReadObject(struct zone_data *zone, FILE *obj_f, int nr, struct obj_data *&pObj);

        bool ReadObjects(struct zone_data *zone, FILE *fl);

        void InterpretMobEspec(const char *keyword, const char *value, struct char_data *mob, int nr);

        bool ReadMobEspec(struct char_data *mob, char *buf, FILE *mob_f, int nr);

        bool ReadSimpleMob(struct char_data *mob, FILE *mob_f, int nr);

        bool ReadEnhancedMob(struct char_data *mob, FILE *mob_f, int nr);

        struct char_data *ReadMob(struct zone_data *zone, FILE *mob_f, int nr);

        bool ReadMobs(struct zone_data *zone, FILE *fl);

        bool ReadShops(struct zone_data *zone, FILE *shop_f);

        bool ReadHelps();

        bool ReadOneHelpFile(FILE *fl);
    };
}

#endif //PIRPLEMUD_UTILS_CONVERSION_CCIRCLEINPUT_H
