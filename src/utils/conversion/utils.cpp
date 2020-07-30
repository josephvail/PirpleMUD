/*!
 * @file utils.cpp
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#include <cstdio>
#include <cctype>
#include <cstring>
#include <iostream>
#include "../CircleMUD.h"

namespace Pirple {
    /* ITEM_x (ordinal object types) */
    const char *ItemTypes[] = {
            "UNDEFINED",
            "LIGHT",
            "SCROLL",
            "WAND",
            "STAFF",
            "WEAPON",
            "FIRE WEAPON",
            "MISSILE",
            "TREASURE",
            "ARMOR",
            "POTION",
            "WORN",
            "OTHER",
            "TRASH",
            "TRAP",
            "CONTAINER",
            "NOTE",
            "LIQ CONTAINER",
            "KEY",
            "FOOD",
            "MONEY",
            "PEN",
            "BOAT",
            "FOUNTAIN",
            "\n"
    };

    int fill_word(char *argument);
    int search_block(char *arg, const char **list, bool exact);
    void skip_spaces(char **string);
    char *read_shop_message(int mnum, int shr, FILE *shop_f, const char *why);
    void read_line(FILE *shop_f, const char *pString, void *data);
    int read_type_list(FILE *shop_f, struct shop_buy_data *list,
                       int new_format, int max);
    int read_list(FILE *shop_f, struct shop_buy_data *list, int new_format,
                  int max, int type);

    /*
    * one_word is like one_argument, except that words in quotes ("") are
    * considered one word.
    */
    char *one_word(char *argument, char *first_arg)
    {
        char *begin = first_arg;

        do {
            skip_spaces(&argument);

            first_arg = begin;

            if (*argument == '\"') {
                argument++;
                while (*argument && *argument != '\"') {
                    *(first_arg++) = LOWER(*argument);
                    argument++;
                }
                argument++;
            } else {
                while (*argument && !isspace(*argument)) {
                    *(first_arg++) = LOWER(*argument);
                    argument++;
                }
            }

            *first_arg = '\0';
        } while (fill_word(begin));

        return (argument);
    }

    const char *fill[] = {
            "in",
            "from",
            "with",
            "the",
            "on",
            "at",
            "to",
            "\n"
    };

    int fill_word(char *argument)
    {
        return (search_block(argument, fill, true) >= 0);
    }

    int search_block(char *arg, const char **list, bool exact)
    {
        int i, l;

        /*  We used to have \r as the first character on certain array items to
         *  prevent the explicit choice of that point.  It seems a bit silly to
         *  dump control characters into arrays to prevent that, so we'll just
         *  check in here to see if the first character of the argument is '!',
         *  and if so, just blindly return a '-1' for not found. - ae.
         */
        if (*arg == '!')
            return (-1);

        /* Make into lower case, and get length of string */
        for (l = 0; *(arg + l); l++)
            *(arg + l) = LOWER(*(arg + l));

        if (exact) {
            for (i = 0; **(list + i) != '\n'; i++)
                if (!strcmp(arg, *(list + i)))
                    return (i);
        } else {
            if (!l)
                l = 1;            /* Avoid "" to match the first available
                 * string */
            for (i = 0; **(list + i) != '\n'; i++)
                if (!strncmp(arg, *(list + i), l))
                    return (i);
        }

        return (-1);
    }

    void get_one_line(FILE *fl, char *buf)
    {
        if (fgets(buf, READ_SIZE, fl) == NULL) {
            cerr << "SYSERR: error reading help file: not terminated with $?" << endl;
            exit(68);
        }

        buf[strlen(buf) - 1] = '\0'; /* take off the trailing \n */
    }

    /*
    * Function to skip over the leading spaces of a string.
    */
    void skip_spaces(char **string)
    {
        for (; **string && isspace(**string); (*string)++);
    }

    size_t strlcpy(char *dest, const char *source, size_t totalsize)
    {
        strncpy(dest, source, totalsize - 1);	/* strncpy: OK (we must assume 'totalsize' is correct) */
        dest[totalsize - 1] = '\0';
        return strlen(source);
    }

    int get_line(FILE *fl, char *buf)
    {
        char temp[READ_SIZE];
        int lines = 0;
        int sl;

        do {
            if (!fgets(temp, READ_SIZE, fl))
                return (0);
            lines++;
        } while (*temp == '*' || *temp == '\n' || *temp == '\r');

        /* Last line of file doesn't always have a \n, but it should. */
        sl = strlen(temp);
        while (sl > 0 && (temp[sl - 1] == '\n' || temp[sl - 1] == '\r'))
            temp[--sl] = '\0';

        strcpy(buf, temp); /* strcpy: OK, if buf >= READ_SIZE (256) */
        return (lines);
    }

/*
* str_cmp: a case-insensitive version of strcmp().
* Returns: 0 if equal, > 0 if arg1 > arg2, or < 0 if arg1 < arg2.
*
* Scan until strings are found different or we reach the end of both.
*/
    int str_cmp(const char *arg1, const char *arg2)
    {
        int chk, i;

        if (arg1 == nullptr || arg2 == nullptr) {
            cerr << "SYSERR: str_cmp() passed a NULL pointer, " << arg1 << " or " << arg2 << "." << endl;
            return (0);
        }

        for (i = 0; arg1[i] || arg2[i]; i++)
            if ((chk = LOWER(arg1[i]) - LOWER(arg2[i])) != 0)
                return (chk); /* not equal */

        return (0);
    }

/* read and allocate space for a '~'-terminated string from a given file */
    char *fread_string(FILE *fl, const char *error)
    {
        char buf[MAX_STRING_LENGTH], tmp[513];
        char *point;
        int done = 0, length = 0, templength;

        *buf = '\0';

        do {
            if (!fgets(tmp, 512, fl)) {
                cerr << "SYSERR: fread_string: format error at or near " << error << endl;
                exit(200);
            }
            /* If there is a '~', end the string; else put an "\r\n" over the '\n'. */
            if ((point = strchr(tmp, '~')) != NULL) {
                *point = '\0';
                done = 1;
            } else {
                point = tmp + strlen(tmp) - 1;
                *(point++) = '\r';
                *(point++) = '\n';
                *point = '\0';
            }

            templength = strlen(tmp);

            if (length + templength >= MAX_STRING_LENGTH) {
                cerr << "SYSERR: fread_string: string too large (PirpleConvertWorld.cpp)" << endl;
                cerr << error << endl;
                exit(201);
            } else {
                strcat(buf + length, tmp);	/* strcat: OK (size checked above) */
                length += templength;
            }
        } while (!done);

        /* allocate space for the new string and copy it */
        return (strlen(buf) ? strdup(buf) : nullptr);
    }

/*
* strn_cmp: a case-insensitive version of strncmp().
* Returns: 0 if equal, > 0 if arg1 > arg2, or < 0 if arg1 < arg2.
*
* Scan until strings are found different, the end of both, or n is reached.
*/
    int strn_cmp(const char *arg1, const char *arg2, int n)
    {
        int chk, i;

        if (arg1 == nullptr || arg2 == nullptr) {
            cerr << "SYSERR: strn_cmp() passed a NULL pointer, " << arg1 << " or " << arg2 << "." << endl;
            return (0);
        }

        for (i = 0; (arg1[i] || arg2[i]) && (n > 0); i++, n--)
            if ((chk = LOWER(arg1[i]) - LOWER(arg2[i])) != 0)
                return (chk); /* not equal */

        return (0);
    }

    unsigned long asciiflag_conv(char *flag)
    {
        unsigned long flags = 0;
        bool is_num = true;
        char *p;

        for (p = flag; *p; p++) {
            if (islower(*p))
                flags |= 1 << (*p - 'a');
            else if (isupper(*p))
                flags |= 1 << (26 + (*p - 'A'));

            if (!isdigit(*p))
                is_num = false;
        }

        if (is_num)
            flags = atol(flag);

        return (flags);
    }

/* val == obj_vnum and obj_rnum (?) */
    int add_to_list(struct shop_buy_data *list, int type, int *len, int *val)
    {
        if (*val != -1) {
            if (*len < MAX_SHOP_OBJ) {
                if (*val != -1) {
                    list[*len].type = *val;
                    list[(*len)++].keywords = nullptr;
                } else
                    *val = -1;
                return (0);
            } else
                return (1);
        }
        return (0);
    }

    int end_read_list(struct shop_buy_data *list, int len, int error)
    {
        if (error)
            cerr << "SYSERR: Raise MAX_SHOP_OBJ constant in shop.h to %d" << len + error << endl;
        list[len].keywords = nullptr;
        list[len++].type = -1;
        return (len);
    }

    char *read_shop_message(int mnum, int shr, FILE *shop_f, const char *why)
    {
        int cht, ss = 0, ds = 0, err = 0;
        char *tbuf;

        if (!(tbuf = fread_string(shop_f, why)))
            return (nullptr);

        for (cht = 0; tbuf[cht]; cht++) {
            if (tbuf[cht] != '%')
                continue;

            if (tbuf[cht + 1] == 's')
                ss++;
            else if (tbuf[cht + 1] == 'd' && (mnum == 5 || mnum == 6)) {
                if (ss == 0) {
                    cerr << "SYSERR: Shop #" << shr << " has %%d before %%s, message #" << mnum << endl;
                    err++;
                }
                ds++;
            } else if (tbuf[cht + 1] != '%') {
                cerr << "SYSERR: Shop #" << shr << " has invalid format '" << tbuf[cht + 1] << "' in message #" << mnum << endl;
                err++;
            }
        }

        if (ss > 1 || ds > 1) {
            cerr << "SYSERR: Shop #" << shr << " has too many specifiers for message #" << mnum << ". %%s=" << ss << " %%d=" << ds << endl;
            err++;
        }

        if (err) {
            free(tbuf);
            return (nullptr);
        }
        return (tbuf);
    }

    int read_type_list(FILE *shop_f, struct shop_buy_data *list,
                       int new_format, int max)
    {
        int tindex, num, len = 0, error = 0;
        char *ptr;
        char buf[MAX_STRING_LENGTH];

        if (!new_format)
            return (read_list(shop_f, list, 0, max, LIST_TRADE));

        do {
            fgets(buf, sizeof(buf), shop_f);
            if ((ptr = strchr(buf, ';')) != nullptr)
                *ptr = '\0';
            else
                *(END_OF(buf) - 1) = '\0';

            num = -1;

            if (strncmp(buf, "-1", 4) != 0)
                for (tindex = 0; *ItemTypes[tindex] != '\n'; tindex++)
                    if (!strn_cmp(ItemTypes[tindex], buf, strlen(ItemTypes[tindex]))) {
                        num = tindex;
                        strcpy(buf, buf + strlen(ItemTypes[tindex]));	/* strcpy: OK (always smaller) */
                        break;
                    }

            ptr = buf;
            if (num == -1) {
                sscanf(buf, "%d", &num);
                while (!isdigit(*ptr))
                    ptr++;
                while (isdigit(*ptr))
                    ptr++;
            }
            while (isspace(*ptr))
                ptr++;
            while (isspace(*(END_OF(ptr) - 1)))
                *(END_OF(ptr) - 1) = '\0';
            error += add_to_list(list, LIST_TRADE, &len, &num);
            if (*ptr)
                list[len - 1].keywords = strdup(ptr);
        } while (num >= 0);
        return (end_read_list(list, len, error));
    }

    void read_line(FILE *shop_f, const char *pString, void *data)
    {
        char buf[READ_SIZE];

        if (!get_line(shop_f, buf) || !sscanf(buf, pString, data)) {
            cerr << "SYSERR: Error in shop, near '" << buf << "' with '" << pString << "%s'" << endl;
            exit(1);
        }
    }

    int read_list(FILE *shop_f, struct shop_buy_data *list, int new_format,
                  int max, int type)
    {
        int count, temp, len = 0, error = 0;

        if (new_format) {
            for (;;) {
                read_line(shop_f, "%d", &temp);
                if (temp < 0)	/* Always "-1" the string. */
                    break;
                error += add_to_list(list, type, &len, &temp);
            }
        } else
            for (count = 0; count < max; count++) {
                read_line(shop_f, "%d", &temp);
                error += add_to_list(list, type, &len, &temp);
            }
        return (end_read_list(list, len, error));
    }
}
