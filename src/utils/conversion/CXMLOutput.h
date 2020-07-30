/*!
 * @file CXMLOutput.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_UTILS_CONVERSION_CXMLOUTPUT_H
#define PIRPLEMUD_UTILS_CONVERSION_CXMLOUTPUT_H

#include "../../pugixml.hpp"
#include "COutput.h"

using namespace pugi;

namespace Pirple {

    class CXMLOutput: public COutput {
    private:
        CXMLOutput() {

        }
    public:
        CXMLOutput(const string& pPath, CInput* pInput): COutput(pPath, pInput) {

        }

        virtual ~CXMLOutput() {
        }

        bool Write();
        bool WriteHelps();
        xml_document* ConstructHelpFile(struct help_index_element* help, bool miniMUD);
        bool WriteZones();
        xml_document* ConstructZoneFiles(struct zone_data* zone, bool miniMUD);
        bool ConstructRoomNode(struct room_data* room, xml_node& parent);
        xml_document* ConstructRoomFile(struct zone_data* zone);
        bool ConstructMobileNode(struct char_data* mob, xml_node& parent);
        xml_document* ConstructMobileFile(struct zone_data* zone);
        bool ConstructItemNode(struct obj_data* obj, xml_node& parent);
        xml_document* ConstructItemFile(struct zone_data* zone);
        bool ConstructShopNode(struct shop_data* shop, xml_node& parent);
        xml_document* ConstructShopFile(struct zone_data* zone);
    };

}

#endif //PIRPLEMUD_UTILS_CONVERSION_CXMLOUTPUT_H
