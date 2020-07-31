/*!
 * @file CMUD.h
 * @brief Main game object contains most everything in the game.
 * @details The main game object holds all other objects in the game
 * and is the basis for running the game in main.cpp
 *
 * The CMUD object contains many of the comm.c init, game loop,
 * and cleanup game functions.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.@n
 * @sa CMUD.cpp, PirpleMUD.h, CTCPServer.h, CTCPClient.h, CSocket.h
 *
 * @sa CircleMUD comm.c, CircleMUD db.c
 */
#ifndef PIRPLEMUD_CMUD_H
#define PIRPLEMUD_CMUD_H

#include <sys/resource.h>
#include "CTCPServer.h"

namespace Pirple
{
class CZone;
class CCharacter;

/*!
 * @class CMUD
 * @brief Our main game object.
 * @details The main game object initialized in main.cpp to run the MUD.
 *
 * CMUD includes internals and functions that were
 * part of comm.c and other utility files in CircleMUD.
 * @sa CircleMUD comm.c, CircleMUD db.c
 */
class CMUD
{
public:
    /*!
     * @enum RUNFLAG
     * @brief Enumeration of our flags for run options with the MUD.
     * @details Mostly changed in command line options in main().
     *
     * Replaces similar mud run options from CircleMUD::mini_mud,
     * CircleMUD::no_rent_check, CircleMUD::scheck, CircleMUD::circle_restrict, CircleMUD::no_specials,
     * CircleMUD::nameserver_is_slow
     * @sa CMUD::runFlags, main()
     *
     * @sa CircleMUD::mini_mud, CircleMUD::no_rent_check, CircleMUD::scheck,
     * CircleMUD::circle_restrict, CircleMUD::no_specials, CircleMUD::nameserver_is_slow,
     * CircleMUD comm.c
     */
    enum RUNFLAG
    {
        RF_MINI_MUD          = (1L << 0L), /*!< Boot the mud with a small world and no houses loaded. Will
             * also be set in conjunction with CMUD::RFNORENTCHECK. */
        RF_NO_RENT_CHECK     = (1L << 1L), /*!< Boot the mud in quick mode, with checking of rent files disabled. */
        RF_SYNTAX_CHECK      = (1L << 2L), /*!< Boot the mud only to check syntax in world files and then close. */
        RF_NO_NEW_PLAYERS    = (1L << 3L), /*!< If toggled on, no new player are allowed to connect, temporarily. */
        RF_NO_SPECIALS       = (1L << 4L), /*!< If enabled, no special procedures will be loaded. */
        RF_SLOW_NAMESERVER   = (1L << 5L)  /*!< If enabled, we do not do DNS lookup. */
    };
    enum { NUM_RUNFLAGS = 6 };

    enum { NUM_RESERVED_DESCRIPTORS = 8 };
    static pm_pulse TICS;
public:
    struct SBan
    {
    public:
        enum TYPE
        {
            NONE = 0,
            NEWUSERS = 1,
            BYNAME = 2,
            ALL = 3
        };
        enum { NUM_TYPES = ALL + 1 };
    public:
        string Site = "";
        TYPE Type = NONE;
        time_t Date = 0;
        list<string> Names;
    };
private:
    CTCPServer server;
    /*!
     * @var port
     * @sa CMUD::GetPort(), CMUD::SetPort()
     *
     * @sa CircleMUD comm.c
     */
    pm_ushint port = 4444;
    /*!
     * @var name
     * @sa CMUD::GetName()
     */
    string name = "PirpleMUD";
    /*!
     * @var path
     * @sa CMUD::GetPath(), CMUD::SetPath(), CMUD::Initialize()
     *
     * @sa CircleMUD comm.c
     */
    string path = "lib";
    /*!
     * @var log
     * @sa CMUD::GetLog(), CMUD::GetLogName(), CMUD::SetLogName(), CMUD::InitializeLog(),
     * CMUD::Log(), CMUD::LogInternal()
     *
     * @sa CircleMUD comm.c, CircleMUD utils.c
     */
    FILE* log = nullptr;
    /*!
     * @var logName
     * @sa CMUD::GetLog(), CMUD::GetLogName(), CMUD::SetLogName(), CMUD::InitializeLog(),
     * CMUD::Log(), CMUD::LogInternal()
     *
     * @sa CircleMUD comm.c CircleMUD utils.c
     */
    string logName = "log/syslog";
    /*!
     * @var initialized
     * @sa CMUD::Initialize(), CMUD::IsInitialized()
     */
    bool initialized = false;
    /*!
     * @var runFlags
     * @sa CMUD::RUNFLAG, CMUD::IsRunFlagged(), CMUD::ToggleRunFlag(), CMUD::AddRunFlag(),
     * CMUD::RemoveRunFlag(), main(), UtilSetBit(), UtilRemoveBit(), UtilToggleBit(), UtilBitIsSet()
     */
    pm_flags runFlags = 0;
    rlimit clientLimit;
    unsigned int maxClients = 999;
    bool shuttingDown = false;
    bool rebooting = false;
    list<SBan> bans;
    list<shared_ptr<CZone>> zones;
    list<weak_ptr<CCharacter>> combatList;
public:
    const CTCPServer& GetServer() const noexcept
    {
        return server;
    }

    /*!
     * @fn GetName()
     * @brief The MUD's name
     * @return const std::string& CMUD::name
     * @sa CMUD::name
     */
    inline const string& GetName() const noexcept
    {
        return name;
    }

    /*!
     * @fn GetPath()
     * @brief The game's working directory.
     * @details Set in CMUD initialization, normally in main().
     *
     * From CircleMUD comm.c. chdir called in CMUD::Initialize().
     * @return const std::string& CMUD::path
     * @sa CMUD::path, CMUD::SetPath(), CMUD::Initialize()
     *
     * @sa CircleMUD comm.c
     */
    inline const string& GetPath() const noexcept
    {
        return path;
    }
    /*!
     * @fn SetPath(const string& pPath)
     * @brief Change the CMUD::path string if CMUD::IsInitialized() is false,
     * @param pPath const std::string&
     * @return bool true if CMUD::path name set, or false if failed due to CMUD::IsInitialized().
     * @sa CMUD::path, CMUD::GetPath(), CMUD::Initialize(), CMUD::IsInitialized()
     *
     * @sa CircleMUD comm.c
     */
    inline bool SetPath(const string& pPath) noexcept
    {
        if (IsInitialized() || pPath.empty())
            return false;
        path = pPath;
        return true;
    }

    /*!
     * @fn GetLog()
     * @brief The CMUD::log file pointer for in game logging.
     * @details The log file pointer initialized in CMUD::InitializeLog().
     *
     * Replaces some utility log functions in CircleMUD comm.c.
     * @return const std::FILE* CMUD::log or nullptr if not initialized.
     * @sa CMUD::log, CMUD::GetLogName(), CMUD::SetLogName(), CMUD::InitializeLog(),
     * CMUD::Log(), CMUD::LogInternal()
     *
     * @sa CircleMUD comm.c, CircleMUD utils.c
     */
    inline const FILE* GetLog() const noexcept
    {
        return log;
    }
    /*!
     * @fn GetLogName()
     * @brief CMUD::logName string containing name and path of our logfile.
     * @details Similar to LOGNAME from CircleMUD in comm.c
     * @return const std::string& CMUD::logName
     * @sa CMUD::logName, CMUD::GetLog(), CMUD::SetLogName(), CMUD::InitializeLog(),
     * CMUD::Log(), CMUD::LogInternal()
     *
     * @sa CircleMUD comm.c and utils.c log functions and variables
     */
    inline const string& GetLogName() const noexcept
    {
        return logName;
    }

    /*!
     * @fn SetLogName(const string& pName)
     * @brief Set CMUD::logName if CMUD::initialized is false.
     * @details Replaces CircleMUD::LOGNAME from comm.c.
     * @param pName const std::string&
     * @return bool true if the name was set, or false if not.
     * @sa CMUD::logName, CMUD::GetLogName(), CMUD::GetLog(), CMUD::InitializeLog(),
     * CMUD::Log(), CMUD::LogInternal()
     *
     * @sa CircleMUD comm.c and utils.c log functions and variables
     */
    inline bool SetLogName(const string& pName) noexcept
    {
        if (IsInitialized() || pName.empty())
            return false;
        logName = pName;
        return true;
    }

    inline const unsigned int& GetMaxClients() const noexcept
    {
        return maxClients;
    }
    /*!
     * @fn IsInitialized()
     * @brief Returns CMUD::initialized
     * @return const bool& CMUD::initialized
     * @sa CMUD::initialized, CMUD::Initialize()
     */
    inline const bool& IsInitialized() const noexcept
    {
        return initialized;
    }

    /*!
     * @fn IsRunFlagged(RUNFLAG pFlag)
     * @brief See if our CMUD::runFlags has the bit pFlag set.
     * @param pFlag CMUD::RUNFLAG bit we want to check in our bitset.
     * @return bool true if the flag exists, false if not.
     * @sa CMUD::runFlags, CMUD::RUNFLAG, UtilBitIsSet()
     */
    inline bool IsRunFlagged(RUNFLAG pFlag) const noexcept
    {
        return UtilBitIsSet(runFlags, pFlag);
    }

    /*!
     * @fn ToggleRunFlag(RUNFLAG pFlag)
     * @brief Toggle pFlag in CMUD::runFlags.
     * @param pFlag CMUD::RUNFLAG bit we want to toggle in our bitset.
     * @sa CMUD::runFlags, CMUD::RUNFLAG, UtilToggleBit()
     */
    inline void ToggleRunFlag(RUNFLAG pFlag) noexcept
    {
        runFlags = UtilToggleBit(runFlags, pFlag);
    }

    /*!
     * @fn AddRunFlag(RUNFLAG pFlag)
     * @brief Add pFlag in CMUD::runFlags.
     * @param pFlag CMUD::RUNFLAG bit we want to add in our bitset.
     * @return bool true if successful, false if not.
     * @sa CMUD::runFlags, CMUD::RUNFLAG, UtilSetBit()
     */
    inline bool AddRunFlag(RUNFLAG pFlag) noexcept
    {
        runFlags = UtilSetBit(runFlags, pFlag);
        return true;
    }

    /*!
     * @fn RemoveRunFlag(RUNFLAG pFlag)
     * @brief Remove pFlag in CMUD::runFlags.
     * @param pFlag CMUD::RUNFLAG bit we want to remove from our bitset.
     * @return bool true if successful, false if not.
     * @sa CMUD::runFlags, CMUD::RUNFLAG, UtilRemoveBit()
     */
    inline bool RemoveRunFlag(RUNFLAG pFlag) noexcept
    {
        runFlags = UtilRemoveBit(runFlags, pFlag);
        return true;
    }
    /*!
     * @fn Initialize()
     * @brief Initialize everything needed before the game runs.
     * @details Initialize all variables, build databases and everything
     * necessary to start the game. Set CMUD::initialized to true if successful.
     *
     * Equivalent to CircleMUD::init_game().
     * @return bool true if successful, false if there was a problem.
     * @sa CMUD::IsInitialized(), CMUD::GetPath(), CMUD::SetPath(),
     * CMUD::GetLog(), CMUD::GetLogName(), CMUD::SetLogName(),
     * CMUD::InitializeLog()
     *
     * @sa CircleMUD comm.c utils.c and db.c game initialization
     */
    bool Initialize(pm_ushint pPort = 0);

    /*!
     * @fn Run()
     * @brief Runs our main game loop.
     * @details Runs our game and loops to process at an interval.
     *
     * Equivalent to CircleMUD::game_loop().
     * @return bool true if successful, false if there was a problem
     * @sa CircleMUD comm.c
     */
    bool Run();

    /*!
     * @fn Stop()
     * @brief Stop the game. Close files, clean memory. Try to shutdown smoothly.
     * @details Close out the game.
     *
     * Equivalent to CircleMUD::destroy_db() as well as
     * the mass free functions to clear out player and world databases in CircleMUD.
     * @return bool true if successful, false if there was a problem
     * @sa CircleMUD comm.c
     */
    bool Stop();

    /*!
     * @fn Log(const string& format, ...)
     * @brief Log mud status and error messages.
     * @details Variadic log function.
     *
     * Replaces CircleMUD::basic_mud_log().
     * @param format const std::string& format string to be utlized with arguments
     * @param ... ellipsis for variadic argument list.
     * @return bool returns true if successful, false if there was a problem
     * @sa CMUD::LogInternal(), CMUD::InitializeLog(), CMUD::GetLog(),
     * CMUD::GetLogName(), CMUD::SetLogName(), CMUD::Log(), CMUD::LogInternal()
     *
     * @sa CircleMUD utils.c and comm.c log functions and variables
     */
    bool Log(const string& format, ...);

    SBan::TYPE HostBanned(const string& pHost);
private:
    bool InitializeMaxClients();
    /*!
     * @fn InitializeLog()
     * @brief Initialize our CMUD::log for system logging when the game runs.
     * @details Initialize our CMUD::log for status and error messages while
     * the game runs. Should normally only be called within CMUD::Initialize().
     * @return bool true if successful, false if there was a problem
     * @sa CMUD::Initialize(), CMUD::GetLog(), CMUD::GetLogName(),
     * CMUD::SetLogName(), CMUD::Log(), CMUD::LogInternal()
     *
     * @sa CircleMUD comm.c and utils.c log functions and variables
     */
    bool InitializeLog();

    /*!
     * @fn OpenLog(const string& filename, FILE* stderr_fp)
     * @brief Internal function to open log on initialization.
     * @details Internal function to open log on initialization.
     *
     * Replaces CircleMUD::open_logfile().
     * @param filename const std::string& points to filename including path to be used for CMUD::log.
     * @param stderr_fp FILE* file handle to the stderr file to utilize.
     * @return bool returns true if successful, false if there was a problem
     * @sa CMUD::InitializeLog(), CMUD::GetLog(), CMUD::GetLogName(),
     * CMUD::SetLogName(), CMUD::Log(), CMUD::LogInternal()
     *
     * @sa CircleMUD comm.c and utils.c log functions and variables
     */
    bool OpenLog(const string& filename, FILE* stderr_fp);

    /*!
     * @fn LogInternal(const std::string& format, va_list args)
     * @brief Internal log function utilizing va_list.
     * @details Internal log function utilizing va_list. Used only in CMUD::Log() generally.
     *
     * Takes the place of CircleMUD::basic_mud_vlog().
     * @return bool true if successful, false if there was a problem
     * @sa CMUD::Log(), CMUD::GetLog(), CMUD::GetLogName(),
     * CMUD::SetLogName(), CMUD::InitializeLog(), CMUD::OpenLog()
     *
     * @sa CircleMUD comm.c and utils.c log functions and variables
     */
    bool LogInternal(const string& format, va_list args);

    void Sleep(struct timeval* pTimeOut);
    void Heartbeat(const pm_pulse& pPulse);
    void HeartbeatZones();
    void HeartbetDCIdlePasswordPrompt();
    void HeartbeatCombat();
    void HeartbeatSaveMUDTime();
};
}

extern Pirple::CMUD MUD;

#endif //PIRPLEMUD_CMUD_H
