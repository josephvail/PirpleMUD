/*!
 * @file CircleMUD.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_UTILS_CIRCLEMUD_H
#define PIRPLEMUD_UTILS_CIRCLEMUD_H

#include <cstdlib>
#include <list>

using namespace std;

namespace Pirple {
#define MAX_OBJ_AFFECT 6
#define NUM_OF_DIRS	6	/* number of directions in a room (nsewud) */
#define READ_SIZE 256
/* Exit info: used in room_data.dir_option.exit_info */
#define EX_ISDOOR (1 << 0)   /* Exit is a door        */
#define EX_CLOSED (1 << 1)   /* The door is closed    */
#define EX_LOCKED (1 << 2)   /* The door is locked    */
#define EX_PICKPROOF (1 << 3)   /* Lock can't be picked  */
#define MAX_STRING_LENGTH 8192
#define MAX_TRADE 5   /* List maximums for compatibility  */
#define MAX_PROD 5   /*  with shops before v3.0      */
#define VERSION3_TAG "v3.0"  /* The file has v3.0 shops in it!   */
#define MAX_SHOP_OBJ 100 /* "Soft" maximum for list maximums */
/* Types of lists to read */
#define LIST_PRODUCE 0
#define LIST_TRADE 1
#define LIST_ROOM 2
#define END_OF(buffer) ((buffer) + strlen((buffer)))
#define LOWER(c)   (((c)>='A'  && (c) <= 'Z') ? ((c)+('a'-'A')) : (c))
#define UPPER(c)   (((c)>='a'  && (c) <= 'z') ? ((c)+('A'-'a')) : (c) )
/* undefine MAX and MIN so that our functions are used instead */
#ifdef MAX
#undef MAX
#endif

#ifdef MIN
#undef MIN
#endif

    inline int MAX(int a, int b) {
        return (a < b ? a : b);
    }

    inline int MIN(int a, int b) {
        return (a > b ? a : b);
    }

    /* Extra description: used in objects, mobiles, and rooms */
    struct extra_descr_data {
        char *keyword;                 /* Keyword in look/examine          */
        char *description;             /* What to see                      */
        extra_descr_data() : keyword(nullptr), description(nullptr) {

        }

        ~extra_descr_data() {
            if (keyword)
                free(keyword);
            keyword = nullptr;
            if (description)
                free(description);
            description = nullptr;
        }
    };


    struct shop_buy_data {
        int type;
        char *keywords;

        shop_buy_data() : type(-1), keywords(nullptr) {

        }

        ~shop_buy_data() {
            if (keywords)
                free(keywords);
            keywords = nullptr;
        }
    };

    struct shop_data {
        int vnum;        /* Virtual number of this shop		*/
        int *producing;        /* Which item to produce (virtual)	*/
        int producing_count;
        float profit_buy;        /* Factor to multiply cost with		*/
        float profit_sell;        /* Factor to multiply cost with		*/
        struct shop_buy_data *type;    /* Which items to trade			*/
        int type_count;
        char *no_such_item1;        /* Message if keeper hasn't got an item	*/
        char *no_such_item2;        /* Message if player hasn't got an item	*/
        char *missing_cash1;        /* Message if keeper hasn't got cash	*/
        char *missing_cash2;        /* Message if player hasn't got cash	*/
        char *do_not_buy;        /* If keeper dosn't buy such things	*/
        char *message_buy;        /* Message when player buys item	*/
        char *message_sell;        /* Message when player sells item	*/
        int temper1;        /* How does keeper react if no money	*/
        unsigned long bitvector;    /* Can attack? Use bank? Cast here?	*/
        int keeper;    /* The mobile who owns the shop (rnum)	*/
        int with_who;        /* Who does the shop trade with?	*/
        int *in_room;        /* Where is the shop?			*/
        int in_room_count;
        int open1, open2;        /* When does the shop open?		*/
        int close1, close2;    /* When does the shop close?		*/
        shop_data() : vnum(-1), producing(nullptr), producing_count(0), profit_buy(0.0f), profit_sell(0.0f),
                      type(nullptr),
                      type_count(0), no_such_item1(nullptr), no_such_item2(nullptr), missing_cash1(nullptr),
                      missing_cash2(nullptr), do_not_buy(nullptr), message_buy(nullptr),
                      message_sell(nullptr), temper1(0), bitvector(0), keeper(-1), with_who(-1),
                      in_room(nullptr), in_room_count(0), open1(0), open2(0), close1(0), close2(0) {

        }

        ~shop_data() {
            delete[] producing;
            delete[] type;
            if (no_such_item1)
                free(no_such_item1);
            no_such_item1 = nullptr;
            if (no_such_item2)
                free(no_such_item2);
            no_such_item2 = nullptr;
            if (missing_cash1)
                free(missing_cash1);
            missing_cash1 = nullptr;
            if (missing_cash2)
                free(missing_cash2);
            missing_cash2 = nullptr;
            if (do_not_buy)
                free(do_not_buy);
            do_not_buy = nullptr;
            if (message_buy)
                free(message_buy);
            message_buy = nullptr;
            if (message_sell)
                free(message_sell);
            message_sell = nullptr;
            delete[] in_room;
        }
    };

/* Char's abilities.  Used in char_file_u *DO*NOT*CHANGE* */
    struct char_ability_data {
        short str;
        short str_add;      /* 000 - 100 if strength 18             */
        short intel;
        short wis;
        short dex;
        short con;
        short cha;

        char_ability_data() : str(11), str_add(0), intel(11), wis(11), dex(11), con(11), cha(11) {

        }
    };


/* Char's points.  Used in char_file_u *DO*NOT*CHANGE* */
    struct char_point_data {
        int mana;
        int max_mana;     /* Max mana for PC/NPC			   */
        int hit;
        int max_hit;      /* Max hit for PC/NPC                      */
        int move;
        int max_move;     /* Max move for PC/NPC                     */

        int armor;        /* Internal -100..100, external -10..10 AC */
        int gold;           /* Money carried                           */
        int exp;            /* The experience of the player            */

        short hitroll;       /* Any bonus or penalty to the hit roll    */
        short damroll;       /* Any bonus or penalty to the damage roll */
        char_point_data() : mana(0), max_mana(0), hit(0), max_hit(0), move(0),
                            max_move(0), armor(0), gold(0), exp(0), hitroll(0), damroll(0) {

        }
    };


/* ================== Structure for player/non-player ===================== */
    struct char_data {
        int nr;                          /* Mob's rnum			  */
        struct char_ability_data real_abils;     /* Abilities without modifiers   */
        struct char_point_data points;        /* Points                        */
        short attack_type;        /* The Attack Type Bitvector for NPC's     */
        short default_pos;        /* Default position for NPC                */
        short position;
        short default_state;
        short state;
        short damnodice;          /* The number of damage dice's	       */
        short damsizedice;        /* The size of the damage dice's           */
        int alignment;        /* +-1000 for alignments                */
        long /*bitvector_t*/ act;    /* act flag for NPC's; player flag for PC's */

        long /*bitvector_t*/    affected_by;
        /* Bitvector for spells/skills affected by */
        char *name;           /* PC / NPC s name (kill ...  )         */
        char *short_descr;  /* for NPC 'actions'                    */
        char *long_descr;   /* for 'look'			       */
        char *description;  /* Extra descriptions                   */
        short sex;           /* PC / NPC's sex                       */
        short level;         /* PC / NPC's level                     */
        char_data() : nr(-1), real_abils(), points(), attack_type(0), default_pos(0), position(0),
                      damnodice(0), damsizedice(0), alignment(0), act(0), affected_by(0), name(nullptr),
                      short_descr(nullptr), long_descr(nullptr), description(nullptr), sex(0), level(0) {

        }

        ~char_data() {
            if (name)
                free(name);
            name = nullptr;
            if (short_descr)
                free(short_descr);
            short_descr = nullptr;
            if (long_descr)
                free(long_descr);
            long_descr = nullptr;
            if (description)
                free(description);
            description = nullptr;
        }
    };

/* object-related structures ******************************************/


/* object flags; used in obj_data */
    struct obj_flag_data {
        int value[4];    /* Values of the item (see list)    */
        short type_flag;    /* Type of item			    */
        int /*bitvector_t*/    wear_flags;    /* Where you can wear it	    */
        int /*bitvector_t*/    extra_flags;    /* If it hums, glows, etc.	    */
        int weight;        /* Weigt what else                  */
        int cost;        /* Value when sold (gp.)            */
        int cost_per_day;    /* Cost to keep pr. real day        */
        long /*bitvector_t*/    bitvector;    /* To set chars bits                */
        obj_flag_data() : value{0, 0, 0, 0}, type_flag(0), wear_flags(0), extra_flags(0),
                          weight(0), cost(0), cost_per_day(0), bitvector(0) {

        }
    };


/* Used in obj_file_elem *DO*NOT*CHANGE* */
    struct obj_affected_type {
        short location;      /* Which ability to change (APPLY_XXX) */
        short modifier;     /* How much it changes by              */
        obj_affected_type() : location(0), modifier(0) {

        }
    };

/* ================== Memory Structure for Objects ================== */
    struct obj_data {
        int item_number;    /* Where in data-base			*/

        struct obj_flag_data obj_flags;/* Object information               */
        struct obj_affected_type affected[MAX_OBJ_AFFECT];  /* affects */

        char *name;                    /* Title of object :get etc.        */
        char *description;          /* When in room                     */
        char *short_description;       /* when worn/carry/in cont.         */
        char *action_description;      /* What to write when used          */
        list<struct extra_descr_data *> ex_description;

        obj_data() : item_number(-1), obj_flags(), name(nullptr), description(nullptr),
                     short_description(nullptr), action_description(nullptr) {

        }

        ~obj_data() {
            if (name)
                free(name);
            name = nullptr;
            if (description)
                free(description);
            description = nullptr;
            if (short_description)
                free(short_description);
            short_description = nullptr;
            if (action_description)
                free(action_description);
            action_description = nullptr;
            while (!ex_description.empty()) {
                struct extra_descr_data *ex = *ex_description.begin();
                ex_description.erase(ex_description.begin());
                delete ex;
            }
        }
    };

/* room-related structures ************************************************/

    struct room_direction_data {
        char *general_description;       /* When look DIR.			*/

        char *keyword;        /* for open/close			*/

        int /*bitvector_t*/ exit_info;    /* Exit info			*/
        int key;        /* Key's number (-1 for no key)		*/
        int to_room;        /* Where direction leads (NOWHERE)	*/
        room_direction_data() : general_description(nullptr), keyword(nullptr),
                                exit_info(0), key(-1), to_room(-1) {

        }

        ~room_direction_data() {
            if (general_description)
                free(general_description);
            general_description = nullptr;
            if (keyword)
                free(keyword);
            keyword = nullptr;
        }
    };

    struct room_data {
        int number;        /* Rooms number	(vnum)		      */
        int zone;              /* Room zone (for resetting)          */
        int sector_type;            /* sector type (move/hide)            */
        char *name;                  /* Rooms name 'You are ...'           */
        char *description;           /* Shown when entered                 */
        list<struct extra_descr_data *> ex_description;
        struct room_direction_data *dir_option[NUM_OF_DIRS]; /* Directions */
        int /*bitvector_t*/ room_flags;        /* DEATH,DARK ... etc */
        room_data() : number(-1), zone(-1), sector_type(0), name(nullptr), description(nullptr),
                      room_flags(0) {
            for (int i = 0; i < NUM_OF_DIRS; i++)
                dir_option[i] = nullptr;
        }

        ~room_data() {
            if (name)
                free(name);
            name = nullptr;
            if (description)
                free(description);
            description = nullptr;
            while (!ex_description.empty()) {
                struct extra_descr_data *ex = *ex_description.begin();
                ex_description.erase(ex_description.begin());
                delete ex;
            }
            for (int i = 0; i < NUM_OF_DIRS; i++) {
                if (dir_option[i])
                    delete dir_option[i];
                dir_option[i] = nullptr;
            }
        }
    };

    struct reset_com {
        char command;   /* current command                      */

        bool if_flag;    /* if TRUE: exe only if preceding exe'd */
        int arg1;       /*                                      */
        int arg2;       /* Arguments to the command             */
        int arg3;       /*                                      */
        int line;        /* line number this command appears on  */
    };

    struct zone_data {
        char *name;          /* name of this zone                  */
        int lifespan;           /* how long between resets (minutes)  */
        int age;                /* current age of this zone (minutes) */
        long bot;           /* starting room number for this zone */
        long top;           /* upper limit for rooms in this zone */

        int reset_mode;         /* conditions for reset (see below)   */
        long number;        /* virtual number of this zone    */
        struct reset_com *cmd;   /* command table for reset            */
        list<struct room_data *> rooms;
        list<struct obj_data *> objs;
        list<struct char_data *> mobs;
        list<struct shop_data *> shops;

        zone_data() : name(nullptr), lifespan(0), age(0), bot(0), top(0), reset_mode(0), number(0), cmd(nullptr) {

        }

        ~zone_data() {
            if (name)
                free(name);
            name = nullptr;
            if (cmd)
                delete[] cmd;
            cmd = nullptr;
            while (!rooms.empty()) {
                struct room_data *room = *rooms.begin();
                rooms.erase(rooms.begin());
                delete room;
            }
            while (!objs.empty()) {
                struct obj_data *obj = *objs.begin();
                objs.erase(objs.begin());
                delete obj;
            }
            while (!mobs.empty()) {
                struct char_data *ch = *mobs.begin();
                mobs.erase(mobs.begin());
                delete ch;
            }
            while (!shops.empty()) {
                struct shop_data *shop = *shops.begin();
                shops.erase(shops.begin());
                delete shop;
            }
        }
    };

    struct help_index_element {
        list< char* > keywords;
        char *entry;

        help_index_element() : keywords(), entry(nullptr) {

        }

        ~help_index_element() {
            while(!keywords.empty()) {
                char* keyword = *keywords.begin();
                keywords.erase(keywords.begin());
                delete keyword;
            }
            if (entry)
                free(entry);
            entry = nullptr;
        }
    };
} // Pirple
#endif //PIRPLEMUD_UTILS_CIRCLEMUD_H
