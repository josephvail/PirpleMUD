/*!
 * @file CCircleInput.cpp
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
#include <cstring>
#include "CCircleInput.h"

namespace Pirple {
    extern char *one_word(char *argument, char *first_arg);
    extern size_t strlcpy(char *dest, const char *source, size_t totalsize);
    extern int get_line(FILE *fl, char *buf);
    extern void skip_spaces(char **string);
    extern char *fread_string(FILE *fl, const char *error);
    extern unsigned long asciiflag_conv(char *flag);
    extern int str_cmp(const char *arg1, const char *arg2);
    extern void read_line(FILE *shop_f, const char *pString, void *data);
    extern int read_list(FILE *shop_f, struct shop_buy_data *list, int new_format,
                  int max, int type);
    extern char *read_shop_message(int mnum, int shr, FILE *shop_f, const char *why);
    extern int read_type_list(FILE *shop_f, struct shop_buy_data *list,
                       int new_format, int max);
    extern void get_one_line(FILE *fl, char *buf);

    bool CCircleInput::Read() {
        if (!ReadZones())
            return false;
        return ReadHelps();
    }

    bool CCircleInput::ReadHelps() {
        FILE *db_index, * db_file;
        char indexBuffer[9999], extraBuffer[9999];

        cout << "Reading CircleMUD help index file to write PirpleMUD help index file." << endl;
        snprintf(indexBuffer, sizeof(indexBuffer), "%stext/help/index", path.c_str());
        cout << "Reading from CircleMUD help index file " << indexBuffer << endl;
        if (!(db_index = fopen(indexBuffer, "r"))) {
            cerr << "SYSERR: opening index file '" << indexBuffer << "': " << strerror(errno) << endl;
            exit(8);
        }
        fscanf(db_index, "%s\n", extraBuffer);
        while (*extraBuffer != '$') {
            char fileBuffer[9999];
            snprintf(fileBuffer, sizeof(fileBuffer), "%stext/help/%s", path.c_str(), extraBuffer);
            if (!(db_file = fopen(fileBuffer, "r"))) {
                cerr << "SYSERR: " << fileBuffer << ": " << strerror(errno) << endl;
                exit(69);
            }
            if (!ReadOneHelpFile(db_file))
                cerr << "SYSERR: Error creating help from ReadHelp, nullptr '" << fileBuffer << "'" << endl;
            else
                cout << "Added CircleMUD helpfiles to table." << endl;

            fclose(db_file);
            fscanf(db_index, "%s\n", extraBuffer);
        }
        cout << "Finished reading CircleMUD help index and help files." << endl;
        // TODO maybe add index.mini but it isnt actually utilized in stock Circle
        fclose(db_index);
        return true;
    }

    bool CCircleInput::ReadOneHelpFile(FILE *fl) {
        char key[READ_SIZE + 1], next_key[READ_SIZE + 1], entry[32384];
        size_t entrylen;
        char line[READ_SIZE + 1], *scan;

        /* get the first keyword line */
        get_one_line(fl, key);
        while (*key != '$') {
            strcat(key, "\r\n");	/* strcat: OK (READ_SIZE - "\n" + "\r\n" == READ_SIZE + 1) */
            entrylen = strlcpy(entry, key, sizeof(entry));

            /* read in the corresponding help entry */
            get_one_line(fl, line);
            while (*line != '#' && entrylen < sizeof(entry) - 1) {
                entrylen += strlcpy(entry + entrylen, line, sizeof(entry) - entrylen);

                if (entrylen + 2 < sizeof(entry) - 1) {
                    strcpy(entry + entrylen, "\r\n");	/* strcpy: OK (size checked above) */
                    entrylen += 2;
                }
                get_one_line(fl, line);
            }

            if (entrylen >= sizeof(entry) - 1) {
                int keysize;
                const char *truncmsg = "\r\n*TRUNCATED*\r\n";

                strcpy(entry + sizeof(entry) - strlen(truncmsg) - 1, truncmsg);	/* strcpy: OK (assuming sane 'entry' size) */

                keysize = strlen(key) - 2;
                cerr << "SYSERR: Help entry exceeded buffer space: " << keysize << " " << key << endl;

                /* If we ran out of buffer space, eat the rest of the entry. */
                while (*line != '#')
                    get_one_line(fl, line);
            }

            auto *el = new help_index_element;
            /* now, add the entry to the index with each keyword on the keyword line */
            el->entry = strdup(entry);
            scan = one_word(key, next_key);
            while (*next_key) {
                el->keywords.push_back(strdup(next_key));
                scan = one_word(scan, next_key);
            }
            cout << "Added help entry: " << *el->keywords.begin() << endl;
            helpTable.push_back(el);
            /* get next keyword line (or $) */
            get_one_line(fl, key);
        }
        return true;
    }

    bool CCircleInput::ReadZones() {
        FILE *db_index;
        char indexBuffer[9999], extraBuffer[9999];

        cout << "Reading CircleMUD zone index file to write PirpleMUD zone index file." << endl;
        snprintf(indexBuffer, sizeof(indexBuffer), "%sworld/zon/index", path.c_str());
        cout << "Reading from CircleMUD zone index file " << indexBuffer << endl;
        if (!(db_index = fopen(indexBuffer, "r"))) {
            cerr << "SYSERR: opening zone index file '" << indexBuffer << "': " << strerror(errno) << endl;
            exit(8);
        }
        fscanf(db_index, "%s\n", extraBuffer);
        while (*extraBuffer != '$') {
            int znum = 0;
            sscanf(extraBuffer, "%d.zon", &znum);
            char fileBuffer[9999];
            snprintf(fileBuffer, sizeof(fileBuffer), "%sworld/zon/%s", path.c_str(), extraBuffer);
            cout << "Reading from CircleMUD zone file " << fileBuffer << endl;
            FILE *db_file = nullptr;
            if (!(db_file = fopen(fileBuffer, "r"))) {
                cerr << "SYSERR: opening zone file '" << fileBuffer << "': " << strerror(errno) << endl;
                exit(9);
            }
            struct zone_data *zone = ReadZone(db_file, extraBuffer);
            if (zone == nullptr) {
                cerr << "SYSERR: Error creating zone from ReadZone, nullptr '" << extraBuffer << "'" << endl;
            } else
                zoneTable.push_back(zone);
            cout << "Added CircleMUD zone to table: " << zone->number << endl;
            fclose(db_file);
            char roomBuffer[9999];
            sprintf(roomBuffer, "%sworld/wld/%ld.wld", path.c_str(), zone->number);
            cout << "Loading CircleMUD rooms for zone: " << zone->number << endl;
            if (!(db_file = fopen(roomBuffer, "r"))) {
                cerr << "SYSERR: opening room file '" << roomBuffer << "': " << strerror(errno) << endl;
            } else {
                ReadRooms(zone, db_file);
                fclose(db_file);
            }
            sprintf(roomBuffer, "%sworld/shp/%ld.shp", path.c_str(), zone->number);
            cout << "Loading CircleMUD shops for zone: " << zone->number << endl;
            if (!(db_file = fopen(roomBuffer,
                                  "r"))) { ;//            cerr << "SYSERR: opening shop file '" << roomBuffer << "': " << strerror(errno) << endl;
            } else {
                ReadShops(zone, db_file);
                fclose(db_file);
            }
            sprintf(roomBuffer, "%sworld/mob/%ld.mob", path.c_str(), zone->number);
            cout << "Loading CircleMUD mobs for zone: " << zone->number << endl;
            if (!(db_file = fopen(roomBuffer, "r"))) {
                cerr << "SYSERR: opening mob file '" << roomBuffer << "': " << strerror(errno) << endl;
            } else {
                ReadMobs(zone, db_file);
                fclose(db_file);
            }
            sprintf(roomBuffer, "%sworld/obj/%ld.obj", path.c_str(), zone->number);
            cout << "Loading CircleMUD objects for zone: " << zone->number << endl;
            if (!(db_file = fopen(roomBuffer, "r"))) {
                cerr << "SYSERR: opening object file '" << roomBuffer << "': " << strerror(errno) << endl;
            } else {
                ReadObjects(zone, db_file);
                fclose(db_file);
            }
            fscanf(db_index, "%s\n", extraBuffer);
        }
        fclose(db_index);
        cout << "Finished reading CircleMUD zone index and zone files." << endl;
        cout << "Reading CircleMUD zone index minimud file to write PirpleMUD index file." << endl;
        snprintf(indexBuffer, sizeof(indexBuffer), "%sworld/zon/index.mini", path.c_str());
        cout << "Reading from CircleMUD index minimud file " << indexBuffer << endl;
        if (!(db_index = fopen(indexBuffer, "r"))) {
            cerr << "SYSERR: opening index file '" << indexBuffer << "': " << strerror(errno) << endl;
            exit(10);
        }
        fscanf(db_index, "%s\n", extraBuffer);
        while (*extraBuffer != '$') {
            int znum = 0;
            sscanf(extraBuffer, "%d.zon", &znum);
            cout << "Creating minimud entry for zone: " << znum << endl;
            zoneMiniNumbers.push_back(znum);
            cout << "Parsing mini zone: " << znum << endl;
            fscanf(db_index, "%s\n", extraBuffer);
        }
        fclose(db_index);
        return true;
    }

    struct zone_data* CCircleInput::ReadZone(FILE *fl, const string &zfileName) {
        int cmd_no, num_of_cmds = 0, line_num = 0, tmp, error;
        char *ptr, buf[READ_SIZE], zname[READ_SIZE], buf2[20000];
        struct zone_data *zone = new zone_data;

        strlcpy(zname, zfileName.c_str(), sizeof(zname));

        /* Skip first 3 lines lest we mistake the zone name for a command. */
        for (tmp = 0; tmp < 3; tmp++)
            get_line(fl, buf);

        /*  More accurate count. Previous was always 4 or 5 too high. -gg 2001/1/17
         *  Note that if a new zone command is added to reset_zone(), this string
         *  will need to be updated to suit. - ae.
         */
        while (get_line(fl, buf))
            if ((strchr("MOPGERD", buf[0]) && buf[1] == ' ') || (buf[0] == 'S' && buf[1] == '\0'))
                num_of_cmds++;

        rewind(fl);

        if (num_of_cmds == 0) {
            cerr << "SYSERR: " << zname << " is empty!" << endl;
            exit(1);
        } else
            zone->cmd = new struct reset_com[num_of_cmds];

        line_num += get_line(fl, buf);

        if (sscanf(buf, "#%ld", &zone->number) != 1) {
            cerr << "SYSERR: Format error in " << zname << ", line " << line_num << endl;
            exit(2);
        }
        snprintf(buf2, sizeof(buf2), "beginning of zone #%ld", zone->number);

        line_num += get_line(fl, buf);
        if ((ptr = strchr(buf, '~')) != NULL)    /* take off the '~' if it's there */
            *ptr = '\0';
        zone->name = strdup(buf);

        line_num += get_line(fl, buf);
        if (sscanf(buf, " %ld %ld %d %d ", &zone->bot, &zone->top, &zone->lifespan, &zone->reset_mode) != 4) {
            cerr << "SYSERR: Format error in numeric constant line of " << zname << endl;
            exit(3);
        }
        if (zone->bot > zone->top) {
            cerr << "SYSERR: Zone " << zone->number << " bottom (" << zone->bot << ") > top (" << zone->top << ")."
                 << endl;
            exit(4);
        }

        if (num_of_cmds) {
            cmd_no = 0;

            for (;;) {
                if ((tmp = get_line(fl, buf)) == 0) {
                    cerr << "SYSERR: Format error in " << zname << " - premature end of file" << endl;
                    exit(5);
                }
                line_num += tmp;
                ptr = buf;
                skip_spaces(&ptr);

                if ((zone->cmd[cmd_no].command = *ptr) == '*')
                    continue;

                ptr++;

                if (zone->cmd[cmd_no].command == 'S' || zone->cmd[cmd_no].command == '$') {
                    zone->cmd[cmd_no].command = 'S';
                    break;
                }
                error = 0;
                if (strchr("MOEPD", zone->cmd[cmd_no].command) == NULL) {    /* a 3-arg command */
                    if (sscanf(ptr, " %d %d %d ", &tmp, &zone->cmd[cmd_no].arg1, &zone->cmd[cmd_no].arg2) != 3)
                        error = 1;
                } else {
                    if (sscanf(ptr, " %d %d %d %d ", &tmp, &zone->cmd[cmd_no].arg1, &zone->cmd[cmd_no].arg2,
                               &zone->cmd[cmd_no].arg3) != 4)
                        error = 1;
                }

                zone->cmd[cmd_no].if_flag = tmp == 0 ? false : true;

                if (error) {
                    cerr << "SYSERR: Format error in " << zname << ", line " << line_num << ": '" << buf << "'" << endl;
                    exit(6);
                }
                zone->cmd[cmd_no].line = line_num;
                cmd_no++;
            }

            if (num_of_cmds != cmd_no + 1) {
                cerr << "SYSERR: Zone command count mismatch for " << zname << ". Estimated: " << num_of_cmds
                     << ", Actual: " << cmd_no + 1 << endl;
                exit(7);
            }
        } else
            zone->cmd = nullptr;
        return zone;
    }

    bool ReadCircleRoomDirection(struct room_data* room, FILE* fl, int dir) {
        int t[5];
        char line[READ_SIZE], buf2[128];

        snprintf(buf2, sizeof(buf2), "room #%d, direction D%d", room->number, dir);

        room->dir_option[dir] = new struct room_direction_data;
        room->dir_option[dir]->general_description = fread_string(fl, buf2);
        room->dir_option[dir]->keyword = fread_string(fl, buf2);

        if (!get_line(fl, line)) {
            cerr << "SYSERR: Format error, " << buf2 << endl;
            exit(800);
        }
        if (sscanf(line, " %d %d %d ", t, t + 1, t + 2) != 3) {
            cerr << "SYSERR: Format error, " << buf2 << endl;
            exit(801);
        }
        if (t[0] == 1)
            room->dir_option[dir]->exit_info = EX_ISDOOR;
        else if (t[0] == 2)
            room->dir_option[dir]->exit_info = EX_ISDOOR | EX_PICKPROOF;
        else
            room->dir_option[dir]->exit_info = 0;

        room->dir_option[dir]->key = t[1];
        room->dir_option[dir]->to_room = t[2];
        return true;
    }

    struct room_data* CCircleInput::ReadRoom(struct zone_data* zone, FILE* fl, int virtual_nr) {
        int t[10];
        char line[READ_SIZE], flags[128], buf2[MAX_STRING_LENGTH], buf[128];
        struct extra_descr_data *new_descr;

        /* This really had better fit or there are other problems. */
        snprintf(buf2, sizeof(buf2), "room #%d", virtual_nr);

        if (virtual_nr < zone->bot) {
            cerr << "SYSERR: Room #" << virtual_nr << " is below zone " << zone->number << "." << endl;
            exit(300);
        }

        struct room_data *room = new struct room_data;
        room->zone = zone->number;
        room->number = virtual_nr;
        room->name = fread_string(fl, buf2);
        room->description = fread_string(fl, buf2);

        if (!get_line(fl, line)) {
            cerr << "SYSERR: Expecting roomflags/sector type of room #" << virtual_nr << " but file ended!" << endl;
            exit(301);
        }

        if (sscanf(line, " %d %s %d ", t, flags, t + 2) != 3) {
            cerr << "SYSERR: Format error in roomflags/sector type of room #" << virtual_nr << endl;
            exit(302);
        }
        /* t[0] is the zone number; ignored with the zone-file system */

        room->room_flags = asciiflag_conv(flags);
        room->sector_type = t[2];
        // Adjust up 1 for NONE to be 0
        room->sector_type++;

        snprintf(buf, sizeof(buf), "SYSERR: Format error in room #%d (expecting D/E/S)", virtual_nr);

        for (;;) {
            if (!get_line(fl, line)) {
                cerr << buf << endl;
                exit(303);
            }
            switch (*line) {
                case 'D':
                    ReadCircleRoomDirection(room, fl, atoi(line + 1));
                    break;
                case 'E':
                    new_descr = new struct extra_descr_data;
                    new_descr->keyword = fread_string(fl, buf2);
                    new_descr->description = fread_string(fl, buf2);
                    room->ex_description.push_back(new_descr);
                    break;
                case 'S':            /* end of room */
                    return room;
                default:
                    cerr << buf << endl;
                    exit(304);
            }
        }
        return room;
    }

    bool CCircleInput::ReadRooms(struct zone_data* zone, FILE* fl) {
        string rmNum = to_string(zone->number) + ".wld";
        const char *filename = rmNum.c_str();
        int nr = -1, last;
        char line[READ_SIZE];

        for (;;) {
            if (!get_line(fl, line)) {
                if (nr == -1) {
                    cerr << "SYSERR: room file " << filename << " is empty!" << endl;
                } else {
                    cerr << "SYSERR: Format error in " << filename << " after room #" << nr << endl;
                    cerr << "...expecting a new room, but file ended!" << endl;
                    cerr << "(maybe the file is not terminated with '$'?)" << endl;
                }
                exit(101);
            }
            if (*line == '$')
                return true;

            if (*line == '#') {
                last = nr;
                if (sscanf(line, "#%d", &nr) != 1) {
                    cerr << "SYSERR: Format error after room #" << last << endl;
                    exit(102);
                }
                if (nr >= 99999)
                    return false;
                else {
                    struct room_data *room = ReadRoom(zone, fl, nr);
                    if (room == nullptr) {
                        // TODO log error
                    }
                    zone->rooms.push_back(room);
                }
            } else {
                cerr << "SYSERR: Format error in room file " << filename << " near room #" << nr << endl;
                cerr << "SYSERR: ... offending line: '" << line << "'" << endl;
                exit(103);
            }
        }
        return true;
    }

    char* CCircleInput::ReadObject(struct zone_data* zone, FILE* obj_f, int nr, struct obj_data*& pObj) {
        struct obj_data *obj = new struct obj_data;
        static char line[READ_SIZE];
        int t[10], j, retval;
        char f1[READ_SIZE], f2[READ_SIZE], buf2[128];
        struct extra_descr_data *new_descr = nullptr;

        obj->item_number = nr;

        sprintf(buf2, "object #%d", nr);    /* sprintf: OK (for 'buf2 >= 19') */

        /* *** string data *** */
        if ((obj->name = fread_string(obj_f, buf2)) == NULL) {
            cerr << "SYSERR: Null obj name or format error at or near " << buf2 << endl;
            exit(833);
        }
        obj->short_description = fread_string(obj_f, buf2);
        obj->description = fread_string(obj_f, buf2);
        obj->action_description = fread_string(obj_f, buf2);

        /* *** numeric data *** */
        if (!get_line(obj_f, line)) {
            cerr << "SYSERR: Expecting first numeric line of " << buf2 << ", but file ended!" << endl;
            exit(834);
        }
        if ((retval = sscanf(line, " %d %s %s", t, f1, f2)) != 3) {
            cerr << "SYSERR: Format error in first numeric line (expecting 3 args, got " << retval << ", " << buf2
                 << endl;
            exit(835);
        }

        /* Object flags checked in check_object(). */
        obj->obj_flags.type_flag = t[0];
        obj->obj_flags.extra_flags = asciiflag_conv(f1);
        obj->obj_flags.wear_flags = asciiflag_conv(f2);

        if (!get_line(obj_f, line)) {
            cerr << "SYSERR: Expecting second numeric line of " << buf2 << ", but file ended!" << endl;
            exit(836);
        }
        if ((retval = sscanf(line, "%d %d %d %d", t, t + 1, t + 2, t + 3)) != 4) {
            cerr << "SYSERR: Format error in second numeric line (expecting 4 args, got " << retval << "), " << buf2
                 << endl;
            exit(837);
        }
        obj->obj_flags.value[0] = t[0];
        obj->obj_flags.value[1] = t[1];
        obj->obj_flags.value[2] = t[2];
        // Adjust drink containers so we do not need negative numbers. Move up 1 with 0 == NONE
        if (obj->obj_flags.type_flag == 17)
            obj->obj_flags.value[2]++;

        obj->obj_flags.value[3] = t[3];

        if (!get_line(obj_f, line)) {
            cerr << "SYSERR: Expecting third numeric line of " << buf2 << ", but file ended!" << endl;
            exit(838);
        }
        if ((retval = sscanf(line, "%d %d %d", t, t + 1, t + 2)) != 3) {
            cerr << "SYSERR: Format error in third numeric line (expecting 3 args, got " << retval << "), " << buf2
                 << endl;
            exit(839);
        }
        obj->obj_flags.weight = t[0];
        obj->obj_flags.cost = t[1];
        obj->obj_flags.cost_per_day = t[2];

        /* *** extra descriptions and affect fields *** */

        for (j = 0; j < MAX_OBJ_AFFECT; j++) {
            obj->affected[j].location = 0;
            obj->affected[j].modifier = 0;
        }

        strcat(buf2, ", after numeric constants\n"    /* strcat: OK (for 'buf2 >= 87') */
                "...expecting 'E', 'A', '$', or next object number");
        j = 0;

        for (;;) {
            if (!get_line(obj_f, line)) {
                cerr << "SYSERR: Format error in " << buf2 << endl;
                exit(840);
            }
            switch (*line) {
                case 'E':
                    new_descr = new struct extra_descr_data;
                    new_descr->keyword = fread_string(obj_f, buf2);
                    new_descr->description = fread_string(obj_f, buf2);
                    obj->ex_description.push_back(new_descr);
                    break;
                case 'A':
                    if (j >= MAX_OBJ_AFFECT) {
                        cerr << "SYSERR: Too many A fields (" << MAX_OBJ_AFFECT << " max), " << buf2 << endl;
                        exit(841);
                    }
                    if (!get_line(obj_f, line)) {
                        cerr << "SYSERR: Format error in 'A' field, " << buf2 << endl;
                        cerr << "...expecting 2 numeric constants but file ended!" << endl;
                        exit(842);
                    }

                    if ((retval = sscanf(line, " %d %d ", t, t + 1)) != 2) {
                        cerr << "SYSERR: Format error in 'A' field, " << buf2 << endl;
                        cerr << "...expecting 2 numeric arguments, got " << retval << endl;
                        cerr << "...offending line: '" << line << "'" << endl;
                        exit(843);
                    }
                    obj->affected[j].location = t[0];
                    obj->affected[j].modifier = t[1];
                    j++;
                    break;
                case '$':
                case '#':
                    pObj = obj;
                    return (line);
                default:
                    cerr << "SYSERR: Format error in (" << *line << "): " << buf2 << endl;
                    exit(844);
            }
        }

        pObj = obj;
        return nullptr;
    }

    bool CCircleInput::ReadObjects(struct zone_data* zone, FILE* fl) {
        string iNum = to_string(zone->number) + ".obj";
        const char *filename = iNum.c_str();
        int nr = -1, last;
        char line[READ_SIZE];

        for (;;) {
            if (nr < 0) {
                if (!get_line(fl, line)) {
                    if (nr == -1) {
                        cerr << "SYSERR: obj file " << filename << " is empty!" << endl;
                    } else {
                        cerr << "SYSERR: Format error in " << filename << " after obj #" << nr << endl;
                        cerr << "...expecting a new obj, but file ended!" << endl;
                        cerr << "(maybe the file is not terminated with '$'?)" << endl;
                    }
                    exit(501);
                }
            }
            if (*line == '$')
                return true;

            if (*line == '#') {
                last = nr;
                if (sscanf(line, "#%d", &nr) != 1) {
                    cerr << "SYSERR: Format error after obj #" << last << endl;
                    exit(502);
                }
                if (nr >= 99999)
                    return false;
                else {
                    struct obj_data *obj = nullptr;
                    strlcpy(line, ReadObject(zone, fl, nr, obj), sizeof(line));

                    if (obj == nullptr) {
                        // TODO log error
                    }
                    zone->objs.push_back(obj);
                }
            } else {
                cerr << "SYSERR: Format error in obj file " << filename << " near obj #" << nr << endl;
                cerr << "SYSERR: ... offending line: '" << line << "'" << endl;
                exit(503);
            }
        }
        return true;
    }

#define CASE(test)	\
	if (value && !matched && !str_cmp(keyword, test) && (matched = true))

#define BOOL_CASE(test)	\
	if (!value && !matched && !str_cmp(keyword, test) && (matched = true))

#define RANGE(low, high)	\
	(num_arg = MAX((low), MIN((high), (num_arg))))

/*
 * interpret_espec is the function that takes espec keywords and values
 * and assigns the correct value to the mob as appropriate.  Adding new
 * e-specs is absurdly easy -- just add a new CASE statement to this
 * function!  No other changes need to be made anywhere in the code.
 *
 * CASE		: Requires a parameter through 'value'.
 * BOOL_CASE	: Being specified at all is its value.
 */
    void CCircleInput::InterpretMobEspec(const char *keyword, const char *value, struct char_data* mob, int nr) {
        int num_arg = 0;
        bool matched = false;

        /*
         * If there isn't a colon, there is no value.  While Boolean options are
         * possible, we don't actually have any.  Feel free to make some.
        */
        if (value)
            num_arg = atoi(value);

        CASE("BareHandAttack") {
            RANGE(0, 99);
            mob->attack_type = num_arg;
        }

        CASE("Str") {
            RANGE(3, 25);
            mob->real_abils.str = num_arg;
        }

        CASE("StrAdd") {
            RANGE(0, 100);
            mob->real_abils.str_add = num_arg;
        }

        CASE("Int") {
            RANGE(3, 25);
            mob->real_abils.intel = num_arg;
        }

        CASE("Wis") {
            RANGE(3, 25);
            mob->real_abils.wis = num_arg;
        }

        CASE("Dex") {
            RANGE(3, 25);
            mob->real_abils.dex = num_arg;
        }

        CASE("Con") {
            RANGE(3, 25);
            mob->real_abils.con = num_arg;
        }

        CASE("Cha") {
            RANGE(3, 25);
            mob->real_abils.cha = num_arg;
        }

        if (!matched) {
            cerr << "SYSERR: Warning: unrecognized espec keyword " << keyword << " in mob #" << nr << endl;
        }
    }

#undef CASE
#undef BOOL_CASE
#undef RANGE

    bool CCircleInput::ReadMobEspec(struct char_data* mob, char* buf, FILE* mob_f, int nr) {
        char *ptr;

        if ((ptr = strchr(buf, ':')) != NULL) {
            *(ptr++) = '\0';
            while (isspace(*ptr))
                ptr++;
        }
        InterpretMobEspec(buf, ptr, mob, nr);
        return true;
    }

    bool CCircleInput::ReadSimpleMob(struct char_data* mob, FILE* mob_f, int nr) {
        int t[10];
        char line[READ_SIZE];

        mob->real_abils.str = 11;
        mob->real_abils.intel = 11;
        mob->real_abils.wis = 11;
        mob->real_abils.dex = 11;
        mob->real_abils.con = 11;
        mob->real_abils.cha = 11;

        if (!get_line(mob_f, line)) {
            cerr << "SYSERR: Format error in mob #" << nr << ", file ended after S flag!" << endl;
            exit(975);
        }

        if (sscanf(line, " %d %d %d %dd%d+%d %dd%d+%d ",
                   t, t + 1, t + 2, t + 3, t + 4, t + 5, t + 6, t + 7, t + 8) != 9) {
            cerr << "SYSERR: Format error in mob #" << nr << ", first line after S flag" << endl;
            cerr << "...expecting line of form '# # # #d#+# #d#+#'" << endl;
            exit(976);
        }

        mob->level = t[0];
        mob->points.hitroll = 20 - t[1];
        mob->points.armor = 10 * t[2];

        /* max hit = 0 is a flag that H, M, V is xdy+z */
        mob->points.max_hit = 0;
        mob->points.hit = t[3];
        mob->points.mana = t[4];
        mob->points.move = t[5];
        mob->points.max_mana = mob->points.mana;
        mob->points.max_move = mob->points.move;

        mob->damnodice = t[6];
        mob->damsizedice = t[7];
        mob->points.damroll = t[8];

        if (!get_line(mob_f, line)) {
            cerr << "SYSERR: Format error in mob #" << nr << ", second line after S flag" << endl;
            cerr << "...expecting line of form '# #', but file ended!" << endl;
            exit(977);
        }

        if (sscanf(line, " %d %d ", t, t + 1) != 2) {
            cerr << "SYSERR: Format error in mob #" << nr << ", second line after S flag" << endl;
            cerr << "...expecting line of form '# #'" << endl;
            exit(978);
        }

        mob->points.gold = t[0];
        mob->points.exp = t[1];

        if (!get_line(mob_f, line)) {
            cerr << "SYSERR: Format error in last line of mob #" << nr << endl;
            cerr << "...expecting line of form '# # #', but file ended!" << endl;
            exit(979);
        }

        if (sscanf(line, " %d %d %d ", t, t + 1, t + 2) != 3) {
            cerr << "SYSERR: Format error in last line of mob #" << nr << endl;
            cerr << "...expecting line of form '# # #'" << endl;
            exit(980);
        }

        mob->state = 6;
        mob->default_state = 6;
        mob->position = t[0];
        // Increment 1 forward from Circle positions for Pirple. 0 = NONE
        mob->position++;
        switch(mob->position)
        {
        // POS_DEAD = POSITION_LAYING and STATE_DEAD
        case 1:
        // POS_MORTALLYW = POSITION_LAYING and STATE_MORTALLY_WOUNDED
        case 2:
        // POS_INCAP = POSITION_LAYING and STATE_INCAPACITATED
        case 3:
        // POS_STUNNED = POSITION_LAYING and STATE_STUNNED
        case 4:
        // POS_SLEEPING = POSITION_LAYING and STATE_SLEEPING
        case 5:
            mob->state = mob->position; // 0-4 are converted to part of Pirple player states.
            mob->position = 1; // Laying
            break;
        // POS_RESTING = POSITION_RECLINED and STATE_AWAKE
        case 6:
            mob->state = 6; // Awake
            mob->position = 2; // Reclined
            break;
        // POS_SITTING = POSITION_SITTING and STATE_AWAKE
        case 7:
            mob->state = 6; // Awake
            mob->position = 3; // Sitting
            break;
        // POS_FIGHTING = POSITION_STANDING and STATE_AWAKE
        // Fighting is not a position or a state anymore.
        case 8:
        // POS_STANDING = POSITION_STANDING and STATE_AWAKE
        case 9:
        default:
            mob->state = 6; // Awake
            mob->position = 5; // Standing
            break;
        }

        mob->default_pos = t[1];
        // Increment 1 forward from Circle positions for Pirple. 0 = NONE
        mob->default_pos++;
        switch(mob->default_pos)
        {
        // POS_DEAD = POSITION_LAYING and STATE_DEAD
        case 1:
        // POS_MORTALLYW = POSITION_LAYING and STATE_MORTALLY_WOUNDED
        case 2:
        // POS_INCAP = POSITION_LAYING and STATE_INCAPACITATED
        case 3:
        // POS_STUNNED = POSITION_LAYING and STATE_STUNNED
        case 4:
        // POS_SLEEPING = POSITION_LAYING and STATE_SLEEPING
        case 5:
            mob->default_state = mob->default_pos; // 0-4 are converted to part of Pirple player states.
            mob->default_pos = 1; // Laying
            break;
        // POS_RESTING = POSITION_RECLINED and STATE_AWAKE
        case 6:
            mob->default_state = 6; // Awake
            mob->default_pos = 2; // Reclined
            break;
        // POS_SITTING = POSITION_SITTING and STATE_AWAKE
        case 7:
            mob->default_state = 6; // Awake
            mob->default_pos = 3; // Sitting
            break;
        // POS_FIGHTING = POSITION_STANDING and STATE_AWAKE
        // Fighting is not a position or a state anymore.
        case 8:
        // POS_STANDING = POSITION_STANDING and STATE_AWAKE
        case 9:
        default:
            mob->default_state = 6; // Awake
            mob->default_pos = 5; // Standing
            break;
        }
        mob->sex = t[2];

        return true;
    }

    bool CCircleInput::ReadEnhancedMob(struct char_data* mob, FILE* mob_f, int nr) {
        char line[READ_SIZE];

        if (ReadSimpleMob(mob, mob_f, nr) == false)
            return false;

        while (get_line(mob_f, line)) {
            if (!strcmp(line, "E"))    /* end of the enhanced section */
                return true;
            else if (*line == '#') {    /* we've hit the next mob, maybe? */
                cerr << "SYSERR: Unterminated E section in mob #" << nr << endl;
                exit(951);
            } else
                ReadMobEspec(mob, line, mob_f, nr);
        }

        cerr << "SYSERR: Unexpected end of file reached after mob #" << nr << endl;
        exit(950);
        return true;
    }

    struct char_data* CCircleInput::ReadMob(struct zone_data* zone, FILE* mob_f, int nr) {
        int t[10];
        char line[READ_SIZE], letter;
        char f1[128], f2[128], buf2[128];
        auto mob = new struct char_data;

        mob->nr = nr;
        sprintf(buf2, "mob vnum %d", nr);    /* sprintf: OK (for 'buf2 >= 19') */

        /***** String data *****/
        mob->name = fread_string(mob_f, buf2);
        mob->short_descr = fread_string(mob_f, buf2);
        mob->long_descr = fread_string(mob_f, buf2);
        mob->description = fread_string(mob_f, buf2);

        /* *** Numeric data *** */
        if (!get_line(mob_f, line)) {
            cerr << "SYSERR: Format error after string section of mob #" << nr << endl;
            cerr << "...expecting line of form '# # # {S | E}', but file ended!" << endl;
            exit(900);
        }

        if (sscanf(line, "%s %s %d %c", f1, f2, t + 2, &letter) != 4) {
            cerr << "SYSERR: Format error after string section of mob #" << nr << endl;
            cerr << "...expecting line of form '# # # {S | E}'" << endl;
            exit(901);
        }

        mob->act = asciiflag_conv(f1);
        mob->affected_by = asciiflag_conv(f2);
        mob->alignment = t[2];

        switch (letter) {
            case 'S':    /* Simple monsters */
            case 's':
                ReadSimpleMob(mob, mob_f, nr);
                break;
            case 'E':    /* Circle3 Enhanced monsters */
            case 'e':
                ReadEnhancedMob(mob, mob_f, nr);
                break;
                /* add new mob types here.. */
            default:
                cerr << "SYSERR: Unsupported mob type '" << letter << "' in mob #" << nr << endl;
                exit(902);
        }

        return mob;
    }

    bool CCircleInput::ReadMobs(struct zone_data* zone, FILE* fl) {
        string rmNum = to_string(zone->number) + ".mob";
        const char *filename = rmNum.c_str();
        int nr = -1, last;
        char line[READ_SIZE];

        for (;;) {
            if (!get_line(fl, line)) {
                if (nr == -1) {
                    cerr << "SYSERR: mob file " << filename << " is empty!" << endl;
                } else {
                    cerr << "SYSERR: Format error in " << filename << " after mob #" << nr << endl;
                    cerr << "...expecting a new mob, but file ended!" << endl;
                    cerr << "(maybe the file is not terminated with '$'?)" << endl;
                }
                exit(1000);
            }
            if (*line == '$')
                return true;

            if (*line == '#') {
                last = nr;
                if (sscanf(line, "#%d", &nr) != 1) {
                    cerr << "SYSERR: Format error after mob #" << last << endl;
                    exit(1001);
                }
                if (nr >= 99999)
                    return false;
                else {
                    struct char_data *mob = ReadMob(zone, fl, nr);
                    if (mob == nullptr) {
                        // TODO log error
                    }
                    zone->mobs.push_back(mob);
                }
            } else {
                cerr << "SYSERR: Format error in mob file " << filename << " near mob #" << nr << endl;
                cerr << "SYSERR: ... offending line: '" << line << "'" << endl;
                exit(1002);
            }
        }
        return true;
    }

    bool CCircleInput::ReadShops(struct zone_data* zone, FILE* shop_f) {
        string rmNum = to_string(zone->number) + ".shp";
        const char *filename = rmNum.c_str();
        char *buf, buf2[256];
        int temp, count;
        bool new_format = false, done = false;
        struct shop_buy_data list[MAX_SHOP_OBJ + 1];

        snprintf(buf2, sizeof(buf2), "beginning of shop file %s", filename);

        while (!done) {
            buf = fread_string(shop_f, buf2);
            if (*buf == '#') {        /* New shop */
                sscanf(buf, "#%d\n", &temp);
                snprintf(buf2, sizeof(buf2), "shop #%d in shop file %s", temp, filename);
                free(buf);        /* Plug memory leak! */
                auto shop = new struct shop_data;
                shop->vnum = temp;
                temp = read_list(shop_f, list, new_format, MAX_PROD, LIST_PRODUCE);
                shop->producing = new int[temp];
                shop->producing_count = temp;
                for (count = 0; count < temp; count++)
                    shop->producing[count] = list[count].type;

                read_line(shop_f, "%f", &shop->profit_buy);
                read_line(shop_f, "%f", &shop->profit_sell);

                temp = read_type_list(shop_f, list, new_format, MAX_TRADE);
                shop->type = new struct shop_buy_data[temp];
                shop->type_count = temp;
                for (count = 0; count < temp; count++) {
                    shop->type[count].type = list[count].type;
                    shop->type[count].keywords = list[count].keywords;
                }

                shop->no_such_item1 = read_shop_message(0, shop->vnum, shop_f, buf2);
                shop->no_such_item2 = read_shop_message(1, shop->vnum, shop_f, buf2);
                shop->do_not_buy = read_shop_message(2, shop->vnum, shop_f, buf2);
                shop->missing_cash1 = read_shop_message(3, shop->vnum, shop_f, buf2);
                shop->missing_cash2 = read_shop_message(4, shop->vnum, shop_f, buf2);
                shop->message_buy = read_shop_message(5, shop->vnum, shop_f, buf2);
                shop->message_sell = read_shop_message(6, shop->vnum, shop_f, buf2);
                read_line(shop_f, "%d", &shop->temper1);
                read_line(shop_f, "%d", &shop->bitvector);
                read_line(shop_f, "%d", &shop->keeper);
                read_line(shop_f, "%d", &shop->with_who);

                temp = read_list(shop_f, list, new_format, 1, LIST_ROOM);
                shop->in_room = new int[temp];
                shop->in_room_count = temp;
                for (count = 0; count < temp; count++)
                    shop->in_room[count] = list[count].type;

                read_line(shop_f, "%d", &shop->open1);
                read_line(shop_f, "%d", &shop->close1);
                read_line(shop_f, "%d", &shop->open2);
                read_line(shop_f, "%d", &shop->close2);
                zone->shops.push_back(shop);
            } else {
                if (*buf == '$')        /* EOF */
                    done = true;
                else if (strstr(buf, VERSION3_TAG))    /* New format marker */
                    new_format = true;
                free(buf);        /* Plug memory leak! */
            }
        }
        return true;
    }
}
