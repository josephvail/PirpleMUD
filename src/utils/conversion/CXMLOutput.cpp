/*!
 * @file CXMLOutput.cpp
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#include <iostream>
#include "CCircleInput.h"
#include "CXMLOutput.h"

namespace Pirple {

    bool CXMLOutput::Write() {
        if (!WriteZones())
            return false;
        return WriteHelps();
    }

    bool CXMLOutput::WriteHelps() {
        auto lInput = dynamic_cast<CCircleInput*>(input);
        list<struct help_index_element*>& helpTable = lInput->GetHelps();
        xml_document index;
        xml_node hindextop = index.append_child("helps");
        xml_attribute count = hindextop.append_attribute("count");
        count.set_value(helpTable.size());
        xml_attribute count_mini_mud = hindextop.append_attribute("count_mini_mud");
        count_mini_mud.set_value(helpTable.size());
        for (auto help: helpTable) {
            xml_node node = hindextop.append_child("help");
            xml_attribute fkw = node.append_attribute("first_keyword");
            fkw.set_value(*help->keywords.begin());
            xml_attribute nkw = node.append_attribute("keyword_count");
            nkw.set_value(help->keywords.size());
            xml_attribute attr2 = node.append_attribute("mini_mud");
            attr2.set_value("true");
            if (help->keywords.size() > 1) {
                xml_node addkws = node.append_child("additional_keywords");
                bool first = true;
                for(auto kw: help->keywords) {
                    if (first == true) {
                        first = false;
                        continue;
                    }
                    xml_node addkw = addkws.append_child("additional_keyword");
                    xml_attribute kwval = addkw.append_attribute("value");
                    kwval.set_value(kw);
                }
            }
            xml_document *hfile = ConstructHelpFile(help, attr2.as_bool());
            string dn;
            if (isalnum((*help->keywords.begin())[0]))
                dn += LOWER((*help->keywords.begin())[0]);
            else
                dn = '_';
            string fname = path + "help/" + dn + "/" + *help->keywords.begin() + ".help.xml";
            hfile->save_file(fname.c_str());
            delete hfile;
        }
        string file = path + "help/helps.xml";
        index.save_file(file.c_str());
        return true;
    }

    xml_document* CXMLOutput::ConstructHelpFile(struct help_index_element* help, bool miniMUD) {
        auto file = new xml_document();
        xml_document &hfile = *file;
        xml_node top = hfile.append_child("help");
        xml_attribute fkw = top.append_attribute("first_keyword");
        fkw.set_value(*help->keywords.begin());
        xml_attribute nkw = top.append_attribute("keyword_count");
        nkw.set_value(help->keywords.size());
        xml_attribute attr2 = top.append_attribute("mini_mud");
        attr2.set_value(miniMUD);
        if (help->keywords.size() > 1) {
            xml_node addkws = top.append_child("additional_keywords");
            bool first = true;
            for(auto kw: help->keywords) {
                if (first == true) {
                    first = false;
                    continue;
                }
                xml_node addkw = addkws.append_child("additional_keyword");
                xml_attribute kwval = addkw.append_attribute("value");
                kwval.set_value(kw);
            }
        }
        xml_node entry = top.append_child("entry");
        entry.text() = help->entry;

        return file;
    }

    bool CXMLOutput::WriteZones() {
        auto lInput = dynamic_cast<CCircleInput*>(input);
        list<struct zone_data*>& zoneTable = lInput->GetZones();
        list<int>& zoneMiniNumbers = lInput->GetMiniNumbers();
        xml_document index;
        xml_node zindextop = index.append_child("zones");
        xml_attribute count = zindextop.append_attribute("count");
        count.set_value(zoneTable.size());
        xml_attribute count_mini_mud = zindextop.append_attribute("count_mini_mud");
        count_mini_mud.set_value(zoneMiniNumbers.size());
        for (auto zone: zoneTable) {
            xml_node node = zindextop.append_child("zone");
            xml_attribute attr = node.append_attribute("number");
            xml_attribute attr2 = node.append_attribute("mini_mud");
            attr2.set_value("false");
            attr.set_value(zone->number);
            for (auto znum: zoneMiniNumbers) {
                if (znum == zone->number)
                    attr2.set_value("true");
            }
            xml_document *zfile = ConstructZoneFiles(zone, attr2.as_bool());
            string fname = path + "world/zones/" + to_string(zone->number) + ".zone.xml";
            zfile->save_file(fname.c_str());
            delete zfile;
        }
        string file = path + "world/zones.xml";
        index.save_file(file.c_str());
        return true;
    }

    xml_document* CXMLOutput::ConstructZoneFiles(struct zone_data* zone, bool miniMUD) {
        xml_document *file = new xml_document();
        xml_document &zfile = *file;
        xml_node top = zfile.append_child("zone");
        xml_attribute znumber = top.append_attribute("number");
        znumber.set_value(zone->number);
        xml_attribute name = top.append_attribute("name");
        name.set_value(zone->name);
        xml_attribute zbot = top.append_attribute("bottom");
        zbot.set_value(zone->bot);
        xml_attribute ztop = top.append_attribute("top");
        ztop.set_value(zone->top);
        xml_attribute lifespan = top.append_attribute("lifespan");
        lifespan.set_value(zone->lifespan);
        xml_attribute resetmode = top.append_attribute("reset_mode");
        resetmode.set_value(zone->reset_mode);
        xml_attribute mm2 = top.append_attribute("mini_mud");
        mm2.set_value(miniMUD);
        xml_attribute rooms = top.append_attribute("rooms");
        rooms.set_value(zone->rooms.size());
        if (!zone->rooms.empty()) {
            xml_document *roomdoc = ConstructRoomFile(zone);
            string fname = path + "world/zones/rooms/" + to_string(zone->number) + ".rooms.xml";
            roomdoc->save_file(fname.c_str());
            delete roomdoc;
        }
        xml_attribute mobiles = top.append_attribute("mobiles");
        mobiles.set_value(zone->mobs.size());
        if (!zone->mobs.empty()) {
            xml_document *mobdoc = ConstructMobileFile(zone);
            string fname = path + "world/zones/mobiles/" + to_string(zone->number) + ".mobiles.xml";
            mobdoc->save_file(fname.c_str());
            delete mobdoc;
        }
        xml_attribute items = top.append_attribute("items");
        items.set_value(zone->objs.size());
        if (!zone->objs.empty()) {
            xml_document *itemdoc = ConstructItemFile(zone);
            string fname = path + "world/zones/items/" + to_string(zone->number) + ".items.xml";
            itemdoc->save_file(fname.c_str());
            delete itemdoc;
        }
        xml_attribute shops = top.append_attribute("shops");
        shops.set_value(zone->shops.size());
        if (!zone->shops.empty()) {
            xml_document *shopdoc = ConstructShopFile(zone);
            string fname = path + "world/zones/shops/" + to_string(zone->number) + ".shops.xml";
            shopdoc->save_file(fname.c_str());
            delete shopdoc;
        }
        if (zone->cmd) {
            int numcmds = 0;
            xml_node resets = top.append_child("resets");
            int cmd_no, last_cmd = 0;
            for (cmd_no = 0; zone->cmd[cmd_no].command != 'S'; cmd_no++) {
                xml_node reset = resets.append_child("reset");
                xml_attribute num = reset.append_attribute("number");
                num.set_value(numcmds + 1);
                xml_attribute iffl = reset.append_attribute("if_flag");
                iffl.set_value(zone->cmd[cmd_no].if_flag);
                xml_attribute type = reset.append_attribute("type");
                xml_attribute arg1 = reset.append_attribute("arg1");
                xml_attribute arg2 = reset.append_attribute("arg2");
                xml_attribute arg3 = reset.append_attribute("arg3");
                arg1.set_value(zone->cmd[cmd_no].arg1);
                arg2.set_value(zone->cmd[cmd_no].arg2);
                arg3.set_value(zone->cmd[cmd_no].arg3);
                if (zone->cmd[cmd_no].if_flag && !last_cmd) {
                    cerr << "If flag shouldn't exist in zone " << zone->number << endl;
                }

                /*  This is the list of actual zone commands.  If any new
                 *  zone commands are added to the game, be certain to update
                 *  the list of commands in load_zone() so that the counting
                 *  will still be correct. - ae.
                 */
                switch (zone->cmd[cmd_no].command) {
                    case '*':           /* ignore command */
                        resets.remove_child(reset);
                        last_cmd = 0;
                        break;

                    case 'M':           /* read a mobile */
                        numcmds++;
                        last_cmd = 1;
                        type.set_value("M");
                        break;

                    case 'O':           /* read an object */
                        numcmds++;
                        last_cmd = 1;
                        type.set_value("O");
                        break;
                    case 'P':           /* object to object */
                        numcmds++;
                        last_cmd = 1;
                        type.set_value("P");
                        break;

                    case 'G':        /* obj_to_char */
                        numcmds++;
                        last_cmd = 1;
                        reset.remove_attribute("arg3");
                        type.set_value("G");
                        break;

                    case 'E':           /* object to equipment list */
                        numcmds++;
                        last_cmd = 1;
                        type.set_value("E");
                        break;

                    case 'R': /* rem obj from room */
                        numcmds++;
                        last_cmd = 1;
                        reset.remove_attribute("arg3");
                        type.set_value("R");
                        break;

                    case 'D':           /* set state of door */
                        numcmds++;
                        last_cmd = 1;
                        type.set_value("D");
                        break;

                    default:
                        cerr << "Unknown cmd in resets for zone " << zone->number << endl;
                        resets.remove_child(reset);
                        last_cmd = 0;
                        break;
                }
            }
            if (!numcmds)
                top.remove_child(resets);
        }

        return file;
    }

    const char* DirNames[NUM_OF_DIRS] = {
            "north",
            "east",
            "south",
            "west",
            "up",
            "down"
    };

    bool CXMLOutput::ConstructRoomNode(struct room_data* room, xml_node& parent) {
        xml_node roomn = parent.append_child("room");
        xml_attribute num = roomn.append_attribute("number");
        num.set_value(room->number);
        xml_attribute name = roomn.append_attribute("name");
        name.set_value(room->name);
        xml_attribute sector = roomn.append_attribute("sector");
        sector.set_value(room->sector_type);
        xml_attribute flags = roomn.append_attribute("flags");
        flags.set_value(room->room_flags);
        xml_node description = roomn.append_child("description");
        description.text() = room->description;
        xml_node directions = roomn.append_child("directions");
        bool found = false;
        for (int i = 0; i < NUM_OF_DIRS; i++) {
            if (room->dir_option[i]) {
                found = true;
                xml_node direction = directions.append_child("direction");
                xml_attribute dir = direction.append_attribute("dir");
                dir.set_value(DirNames[i]);
                if (room->dir_option[i]->to_room >= 0) {
                    xml_attribute toroom = direction.append_attribute("to_room");
                    toroom.set_value(room->dir_option[i]->to_room);
                }
                if (room->dir_option[i]->key >= 0) {
                    xml_attribute key = direction.append_attribute("key");
                    key.set_value(room->dir_option[i]->key);
                }
                if (room->dir_option[i]->keyword) {
                    xml_attribute keyword = direction.append_attribute("keyword");
                    keyword.set_value(room->dir_option[i]->keyword);
                }
                if (room->dir_option[i]->exit_info) {
                    xml_attribute doorflags = direction.append_attribute("flags");
                    doorflags.set_value(room->dir_option[i]->exit_info);
                }
                if (room->dir_option[i]->general_description) {
                    xml_node generaldesc = direction.append_child("general_description");
                    generaldesc.text() = room->dir_option[i]->general_description;
                }
            }
        }
        if (!found)
            roomn.remove_child(directions);
        found = false;
        if (!room->ex_description.empty()) {
            xml_node extradescriptions = roomn.append_child("extra_descriptions");
            for (auto exdesc: room->ex_description) {
                xml_node extradescription = extradescriptions.append_child("extra_description");
                if (exdesc->keyword) {
                    xml_attribute exkw = extradescription.append_attribute("keyword");
                    exkw.set_value(exdesc->keyword);
                    found = true;
                }
                if (exdesc->description) {
                    xml_node exdescript = extradescription.append_child("description");
                    exdescript.text() = exdesc->description;
                    found = true;
                }
            }
            if (!found)
                roomn.remove_child(extradescriptions);
        }
        return true;
    }

    xml_document* CXMLOutput::ConstructRoomFile(struct zone_data* zone) {
        auto rfile = new xml_document();
        xml_node top = rfile->append_child("rooms");
        xml_attribute count = rfile->append_attribute("count");
        count.set_value(zone->rooms.size());
        for (auto room: zone->rooms) {
            ConstructRoomNode(room, top);
        }
        return rfile;
    }

    bool CXMLOutput::ConstructMobileNode(struct char_data* mob, xml_node& parent) {
        xml_node mobn = parent.append_child("mobile");
        xml_attribute num = mobn.append_attribute("number");
        num.set_value(mob->nr);
        xml_attribute name = mobn.append_attribute("name");
        name.set_value(mob->name);
        xml_attribute level = mobn.append_attribute("level");
        level.set_value(mob->level);
        xml_attribute sex = mobn.append_attribute("sex");
        sex.set_value(mob->sex);
        xml_attribute alignment = mobn.append_attribute("alignment");
        alignment.set_value(mob->alignment);
        xml_attribute flags = mobn.append_attribute("flags");
        flags.set_value(mob->act);
        xml_attribute aff_flags = mobn.append_attribute("aff_flags");
        aff_flags.set_value(mob->affected_by);
        xml_attribute position = mobn.append_attribute("position");
        position.set_value(mob->position);
        xml_attribute default_position = mobn.append_attribute("default_position");
        default_position.set_value(mob->default_pos);
        xml_attribute state = mobn.append_attribute("state");
        state.set_value(mob->state);
        xml_attribute default_state = mobn.append_attribute("default_state");
        default_state.set_value(mob->default_state);
        if (mob->damnodice) {
            xml_attribute dam_nodice = mobn.append_attribute("dam_nodice");
            dam_nodice.set_value(mob->damnodice);
        }
        if (mob->damsizedice) {
            xml_attribute dam_sizedice = mobn.append_attribute("dam_sizedice");
            dam_sizedice.set_value(mob->damsizedice);
        }
        if (mob->attack_type) {
            xml_attribute attack_type = mobn.append_attribute("attack_type");
            attack_type.set_value(mob->attack_type);
        }
        if (mob->points.max_hit) {
            xml_attribute max_hitpoints = mobn.append_attribute("max_hitpoints");
            max_hitpoints.set_value(mob->points.max_hit);
        }
        if (mob->points.max_mana) {
            xml_attribute max_manapoints = mobn.append_attribute("max_manapoints");
            max_manapoints.set_value(mob->points.max_mana);
        }
        if (mob->points.max_move) {
            xml_attribute max_movepoints = mobn.append_attribute("max_movepoints");
            max_movepoints.set_value(mob->points.max_move);
        }
        xml_attribute armor = mobn.append_attribute("armor");
        armor.set_value(mob->points.armor);
        if (mob->points.gold) {
            xml_attribute gold = mobn.append_attribute("gold");
            gold.set_value(mob->points.gold);
        }
        xml_attribute experience = mobn.append_attribute("experience");
        experience.set_value(mob->points.exp);
        if (mob->points.hitroll) {
            xml_attribute hitroll = mobn.append_attribute("hitroll");
            hitroll.set_value(mob->points.hitroll);
        }
        if (mob->points.damroll) {
            xml_attribute damroll = mobn.append_attribute("damroll");
            damroll.set_value(mob->points.damroll);
        }
        if (mob->real_abils.str && mob->real_abils.str != 11) {
            xml_attribute strength = mobn.append_attribute("strength");
            strength.set_value(mob->real_abils.str);
        }
        if (mob->real_abils.str_add) {
            xml_attribute str_add = mobn.append_attribute("str_add");
            str_add.set_value(mob->real_abils.str_add);
        }
        if (mob->real_abils.intel && mob->real_abils.intel != 11) {
            xml_attribute intelligence = mobn.append_attribute("intelligence");
            intelligence.set_value(mob->real_abils.intel);
        }
        if (mob->real_abils.wis && mob->real_abils.wis != 11) {
            xml_attribute wisdom = mobn.append_attribute("wisdom");
            wisdom.set_value(mob->real_abils.wis);
        }
        if (mob->real_abils.dex && mob->real_abils.dex != 11) {
            xml_attribute dexterity = mobn.append_attribute("dexterity");
            dexterity.set_value(mob->real_abils.dex);
        }
        if (mob->real_abils.con && mob->real_abils.con != 11) {
            xml_attribute constitution = mobn.append_attribute("constitution");
            constitution.set_value(mob->real_abils.con);
        }
        if (mob->real_abils.cha && mob->real_abils.cha != 11) {
            xml_attribute charisma = mobn.append_attribute("charisma");
            charisma.set_value(mob->real_abils.cha);
        }
        xml_attribute short_description = mobn.append_attribute("short_description");
        short_description.set_value(mob->short_descr);
        if (mob->long_descr) {
            xml_node long_description = mobn.append_child("long_description");
            long_description.text() = mob->long_descr;
        }
        if (mob->description) {
            xml_node description = mobn.append_child("description");
            description.text() = mob->description;
        }
        return true;
    }

    xml_document* CXMLOutput::ConstructMobileFile(struct zone_data* zone) {
        xml_document *mfile = new xml_document();
        xml_node top = mfile->append_child("mobiles");
        xml_attribute count = mfile->append_attribute("count");
        count.set_value(zone->mobs.size());
        for (auto mob: zone->mobs) {
            ConstructMobileNode(mob, top);
        }

        return mfile;
    }

    bool CXMLOutput::ConstructItemNode(struct obj_data* obj, xml_node& parent) {
        xml_node itemn = parent.append_child("item");
        xml_attribute num = itemn.append_attribute("number");
        num.set_value(obj->item_number);
        xml_attribute name = itemn.append_attribute("name");
        name.set_value(obj->name);
        xml_attribute type = itemn.append_attribute("type");
        type.set_value(obj->obj_flags.type_flag);
        xml_attribute wear_flags = itemn.append_attribute("wear_flags");
        wear_flags.set_value(obj->obj_flags.wear_flags);
        xml_attribute extra_flags = itemn.append_attribute("extra_flags");
        extra_flags.set_value(obj->obj_flags.extra_flags);
        xml_attribute bitvector = itemn.append_attribute("bitvector");
        bitvector.set_value(obj->obj_flags.bitvector);
        xml_attribute weight = itemn.append_attribute("weight");
        weight.set_value(obj->obj_flags.weight);
        xml_attribute cost = itemn.append_attribute("cost");
        cost.set_value(obj->obj_flags.cost);
        xml_attribute cost_per_day = itemn.append_attribute("cost_per_day");
        cost_per_day.set_value(obj->obj_flags.cost_per_day);
        xml_attribute short_description = itemn.append_attribute("short_description");
        short_description.set_value(obj->short_description);
        xml_node values = itemn.append_child("values");
        for (int i = 0; i < 4; i++) {
            xml_node value = values.append_child("value");
            xml_attribute vnum = value.append_attribute("number");
            vnum.set_value(i);
            xml_attribute val = value.append_attribute("val");
            val.set_value(obj->obj_flags.value[i]);
        }
        bool found = false;
        xml_node affects = itemn.append_child("affects");
        for (int i = 0; i < MAX_OBJ_AFFECT; i++) {
            if (obj->affected[i].location != 0) {
                found = true;
                xml_node affect = affects.append_child("affect");
                xml_attribute alocation = affect.append_attribute("location");
                alocation.set_value(obj->affected[i].location);
                xml_attribute amodifier = affect.append_attribute("modifier");
                amodifier.set_value(obj->affected[i].modifier);
            }
        }
        if (found == false)
            itemn.remove_child(affects);
        if (obj->description) {
            xml_node description = itemn.append_child("description");
            description.text() = obj->description;
        }
        if (obj->action_description) {
            xml_node action_description = itemn.append_child("action_description");
            action_description.text() = obj->action_description;
        }
        found = false;
        if (!obj->ex_description.empty()) {
            xml_node extradescriptions = itemn.append_child("extra_descriptions");
            for (auto exdesc: obj->ex_description) {
                xml_node extradescription = extradescriptions.append_child("extra_description");
                if (exdesc->keyword) {
                    xml_attribute exkw = extradescription.append_attribute("keyword");
                    exkw.set_value(exdesc->keyword);
                    found = true;
                }
                if (exdesc->description) {
                    xml_node exdescript = extradescription.append_child("description");
                    exdescript.text() = exdesc->description;
                    found = true;
                }
            }
            if (found == false)
                itemn.remove_child(extradescriptions);
        }
        return true;
    }

    xml_document* CXMLOutput::ConstructItemFile(struct zone_data* zone) {
        xml_document *ifile = new xml_document();
        xml_node top = ifile->append_child("items");
        xml_attribute count = ifile->append_attribute("count");
        count.set_value(zone->objs.size());
        for (auto item: zone->objs) {
            ConstructItemNode(item, top);
        }

        return ifile;
    }

    bool CXMLOutput::ConstructShopNode(struct shop_data* shop, xml_node& parent) {
        extern const char* ItemTypes;
        xml_node shopn = parent.append_child("shop");
        xml_attribute num = shopn.append_attribute("number");
        num.set_value(shop->vnum);
        xml_attribute keeper = shopn.append_attribute("keeper");
        keeper.set_value(shop->keeper);
        xml_attribute trade_with_flags = shopn.append_attribute("trade_with_flags");
        trade_with_flags.set_value(shop->with_who);
        xml_attribute no_money_temper = shopn.append_attribute("no_money_temper");
        no_money_temper.set_value(shop->temper1);
        if (shop->profit_buy != 0.0f) {
            xml_attribute buy_profit = shopn.append_attribute("buy_profit");
            buy_profit.set_value((int) (shop->profit_buy * 100));
        }
        if (shop->profit_sell != 0.0f) {
            xml_attribute sell_profit = shopn.append_attribute("sell_profit");
            sell_profit.set_value((int) (shop->profit_sell * 100));
        }
        if (shop->bitvector) {
            xml_attribute bitvector = shopn.append_attribute("bitvector");
            bitvector.set_value(shop->bitvector);
        }
        xml_node shop_hours = shopn.append_child("shop_hours");
        xml_node hours1 = shop_hours.append_child("hours");
        xml_attribute hrsnum1 = hours1.append_attribute("number");
        hrsnum1.set_value("1");
        xml_attribute first_open = hours1.append_attribute("open");
        first_open.set_value(shop->open1);
        xml_attribute first_close = hours1.append_attribute("close");
        first_close.set_value(shop->close1);
        if (shop->open2 && shop->close2) {
            xml_node hours2 = shop_hours.append_child("hours");
            xml_attribute hrsnum2 = hours2.append_attribute("number");
            hrsnum2.set_value("2");
            xml_attribute second_open = hours2.append_attribute("open");
            second_open.set_value(shop->open2);
            xml_attribute second_close = hours2.append_attribute("close");
            second_close.set_value(shop->close2);
        }
        xml_node keeper_messages = shopn.append_child("keeper_messages");
        for (int i = 0; i < 7; i++) {
            xml_node keeper_message = keeper_messages.append_child("keeper_message");
            xml_attribute kmnum = keeper_message.append_attribute("number");
            kmnum.set_value(i + 1);
            xml_attribute skmessage = keeper_message.append_attribute("message");
            switch (i) {
                case 0:
                    skmessage.set_value(shop->no_such_item1);
                    break;
                case 1:
                    skmessage.set_value(shop->no_such_item2);
                    break;
                case 2:
                    skmessage.set_value(shop->do_not_buy);
                    break;
                case 3:
                    skmessage.set_value(shop->missing_cash1);
                    break;
                case 4:
                    skmessage.set_value(shop->missing_cash2);
                    break;
                case 5:
                    skmessage.set_value(shop->message_buy);
                    break;
                case 6:
                    skmessage.set_value(shop->message_sell);
                    break;
            }
        }

        bool found = false;
        if (shop->type_count) {
            xml_node will_buy = shopn.append_child("will_buy");
            shop->type_count--;
            for (int i = 0; i < shop->type_count; i++) {
                xml_node criteria = will_buy.append_child("criteria");
                if (shop->type[i].type > 0) {
                    xml_attribute wbtype = criteria.append_attribute("type");
                    wbtype.set_value(ItemTypes[shop->type[i].type]);
                    found = true;
                }
                if (shop->type[i].keywords) {
                    xml_attribute wbkw = criteria.append_attribute("keywords");
                    wbkw.set_value(shop->type[i].keywords);
                    found = true;
                }
                if (found == false)
                    will_buy.remove_child(criteria);
            }
            if (found == false)
                shopn.remove_child(will_buy);
        }

        if (shop->producing_count) {
            found = false;
            xml_node will_construct = shopn.append_child("will_construct");
            for (int i = 0; i < shop->producing_count; i++) {
                if (shop->producing[i] > 0) {
                    xml_node construct_item = will_construct.append_child("construct_item");
                    xml_attribute cs_number = construct_item.append_attribute("number");
                    cs_number.set_value(shop->producing[i]);
                    found = true;
                }
            }
            if (found == false)
                shopn.remove_child(will_construct);
        }
        if (shop->in_room_count) {
            found = false;
            xml_node locations = shopn.append_child("locations");
            for (int i = 0; i < shop->in_room_count; i++) {
                if (shop->in_room[i] > 0) {
                    xml_node location = locations.append_child("location");
                    xml_attribute loc_number = location.append_attribute("room_number");
                    loc_number.set_value(shop->in_room[i]);
                    found = true;
                }
            }
            if (found == false)
                shopn.remove_child(locations);
        }
        return true;
    }

    xml_document* CXMLOutput::ConstructShopFile(struct zone_data* zone) {
        xml_document *sfile = new xml_document();
        xml_node top = sfile->append_child("shops");
        xml_attribute count = sfile->append_attribute("count");
        count.set_value(zone->shops.size());
        for (auto shop: zone->shops) {
            ConstructShopNode(shop, top);
        }

        return sfile;
    }
}
