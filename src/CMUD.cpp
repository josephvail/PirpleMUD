/*!
 * @file CMUD.cpp
 * @brief See CMUD.h
 * @details See CMUD.h
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 */

#include <sys/time.h>
#include "CMUD.h"

namespace Pirple
{
unsigned int CMUD::TICS = 0;

// See CMUD.h for description.
bool CMUD::Initialize(pm_ushint pPort)
{
    if (IsInitialized())
    {
        Log("SYSERR: Trying to initialize AGAIN.");
        return false;
    }

    /* All arguments have been parsed, try to open log file. */
    if (!InitializeLog())
        return false;
    /*
     * Moved here to distinguish command line options and to show up
     * in the log if stderr is redirected to a file.
     */
    Log("%s", VERSION.c_str());
    /*Attempt to move to our lib directory*/
    if (chdir(path.c_str()) < 0)
    {
        perror("SYSERR: Fatal error changing to data directory");
        exit(1);
    }
    Log("Using %s as data directory.", path.c_str());
    if (IsRunFlagged(RF_SYNTAX_CHECK))
        ;// TODO boot_world();
    else
    {
        // TODO socket_t mother_desc;

        /* We don't want to restart if we crash before we get up. */
        // TODO touch(KILLSCRIPT_FILE);

        // TODO circle_srandom(time(0));

        Log("Finding player limit.");
        // TODO max_players = get_max_players();

        Log("Opening server connection.");
        server.Initialize(pPort);

        // TODO boot_db();

        Log("Signal trapping.");
//TODO            signal_setup();

        /* If we made it this far, we will be able to restart without problem. */
        //TODO remove(KILLSCRIPT_FILE);
        Log("Running game on port %d.", server.GetPort());
    }

    initialized = true;
    return true;
}

// See CMUD.h for description
bool CMUD::OpenLog(const string &filename, FILE *stderr_fp)
{
    if (stderr_fp)	/* freopen() the descriptor. */
        log = freopen(filename.c_str(), "w", stderr_fp);
    else
        log = fopen(filename.c_str(), "w");

    if (log)
    {
        cout << "Using log file '" << filename << "' " << (stderr_fp ? " with redirection" : "")
             << "." << endl;
        return true;
    }

    cout << "SYSERR: Error opening file '" << filename << "': " << strerror(errno) << endl;
    return false;
}

// See CMUD.h
bool CMUD::InitializeMaxClients()
{
    if (IsInitialized())
        return false;

    string method;
    memset((char *) &clientLimit, 0, sizeof(clientLimit));

    /* find the limit of file descs */
    method = "rlimit";
    if (getrlimit(RLIMIT_NOFILE, &clientLimit) < 0)
    {
        perror("SYSERR: calling getrlimit");
        exit(1); // TODO throw instead of exit?
    }

    /* set the current to the maximum */
    clientLimit.rlim_cur = clientLimit.rlim_max;
    if (setrlimit(RLIMIT_NOFILE, &clientLimit) < 0)
    {
        perror("SYSERR: calling setrlimit");
        exit(1);
    }
    maxClients = MIN(maxClients + NUM_RESERVED_DESCRIPTORS, clientLimit.rlim_max);
    if (maxClients == 0)
    {
        Log("SYSERR: Non-positive max user limit!  (Set at %d using %s).",
            maxClients, method.c_str());
        exit(1);
    }
    Log("   Setting player limit to %d using %s.", maxClients, method.c_str());
    return true;
}

// See CMUD.h for description
bool CMUD::InitializeLog()
{
    FILE* s_fp;

    if ((s_fp = fdopen(STDERR_FILENO, "w")) == nullptr)
    {
        cout << "SYSERR: Error opening stderr, trying stdout." << endl;

        if ((s_fp = fdopen(STDOUT_FILENO, "w")) == nullptr)
        {
            cout << "SYSERR: Error opening stdout, trying a file." << endl;

            // Already defaults to a logname in CMUD.h
        }
    }

    if (logName.empty())
    {
        /* No filename, set us up with the descriptor we just opened. */
        log = s_fp;
        cout << "Using file descriptor for logging." << endl;
        return true;
    }

    /* We honor the default filename first. */
    if (OpenLog(logName, s_fp))
        return true;

    /* Erp, that didn't work either, just die. */
    cout << "SYSERR: Couldn't open anything to log to, giving up." << endl;
    return false;
}

// See CMUD.h for description
bool CMUD::LogInternal(const string &format, va_list args)
{
    time_t ct = time(nullptr);
// TODO        time_t ct = time(0);
    char *time_s = asctime(localtime(&ct));

    if (log == nullptr)
    {
        cout << "SYSERR: Using log() before stream was initialized!" << endl;
        return false;
    }

    time_s[strlen(time_s) - 1] = '\0';

    fprintf(log, "%-15.15s :: ", time_s + 4);
    vfprintf(log, format.empty() ? "SYSERR: log() received an empty format." : format.c_str(), args);
    fputc('\n', log);
    fflush(log);
    return true;
}

// See CMUD.h for description
bool CMUD::Log(const string& format, ...)
{
    va_list args;

    va_start(args, format);
    LogInternal(format, args);
    va_end(args);
    return true;
}

/*
 *  new code to calculate time differences, which works on systems
 *  for which tv_usec is unsigned (and thus comparisons for something
 *  being < 0 fail).  Based on code submitted by ss@sirocco.cup.hp.com.
 */
/*
 * code to return the time difference between a and b (a-b).
 * always returns a nonnegative value (floors at 0).
 */
void TimeDiff(timeval& rslt, const timeval& a, const timeval& b)
{
    timeval nullTime;
    nullTime.tv_sec = 0;
    nullTime.tv_usec = 0;

    if (a.tv_sec < b.tv_sec)
        rslt = nullTime;
    else if (a.tv_sec == b.tv_sec)
    {
        if (a.tv_usec < b.tv_usec)
            rslt = nullTime;
        else
        {
            rslt.tv_sec = 0;
            rslt.tv_usec = a.tv_usec - b.tv_usec;
        }
    }
    else  			/* a->tv_sec > b->tv_sec */
    {
        rslt.tv_sec = a.tv_sec - b.tv_sec;
        if (a.tv_usec < b.tv_usec)
        {
            rslt.tv_usec = a.tv_usec + 1000000 - b.tv_usec;
            rslt.tv_sec--;
        }
        else
            rslt.tv_usec = a.tv_usec - b.tv_usec;
    }
}

/*
 * Add 2 time values.
 *
 * Patch sent by "d. hall" <dhall@OOI.NET> to fix 'static' usage.
 */
void TimeAdd(timeval& rslt, const timeval& a, const timeval& b)
{
    rslt.tv_sec = a.tv_sec + b.tv_sec;
    rslt.tv_usec = a.tv_usec + b.tv_usec;

    while (rslt.tv_usec >= 1000000)
    {
        rslt.tv_usec -= 1000000;
        rslt.tv_sec++;
    }
}

// See CMUD.h for description.
bool CMUD::Run()
{
    fd_set nullSet = {};
    timeval lastTime = {}, optTime = {}, processTime = {}, tempTime = {};
    timeval beforeSleep = {}, now = {}, timeout = {};
    pm_pulse pulse = 0, missedPulses;

    /* initialize various time values */
    optTime.tv_usec = OPT_USEC;
    optTime.tv_sec = 0;
    FD_ZERO(&nullSet);

    gettimeofday(&lastTime, nullptr);

    /* The Main Loop.  The Big Cheese.  The Top Dog.  The Head Honcho.  The.. */
    while (!shuttingDown)
    {
        /* Sleep if we don't have any connections */
        server.SleepUntil(lastTime);
        /* Set up the input, output, and exception sets for select(). */
        server.PrepareFDSets();
        /*
         * At this point, we have completed all input, output and heartbeat
         * activity from the previous iteration, so we have to put ourselves
         * to sleep until the next 0.1 second tick.  The first step is to
         * calculate how long we took processing the previous iteration.
         */

        gettimeofday(&beforeSleep, nullptr); /* current time */
        TimeDiff(processTime, beforeSleep, lastTime);

        /*
         * If we were asleep for more than one pass, count missed pulses and sleep
         * until we're resynchronized with the next upcoming pulse.
         */
        if (processTime.tv_sec == 0 && processTime.tv_usec < OPT_USEC)
        {
            missedPulses = 0;
        }
        else
        {
            missedPulses = processTime.tv_sec * PASSES_PER_SEC;
            missedPulses += processTime.tv_usec / OPT_USEC;
            processTime.tv_sec = 0;
            processTime.tv_usec = processTime.tv_usec % OPT_USEC;
        }

        /* Calculate the time we should wake up */
        TimeDiff(tempTime, optTime, processTime);
        TimeAdd(lastTime, beforeSleep, tempTime);

        /* Now keep sleeping until that time has come */
        gettimeofday(&now, nullptr);
        TimeDiff(timeout, lastTime, now);

        /* Go to sleep */
        do
        {
            Sleep(&timeout);
            gettimeofday(&now, nullptr);
            TimeDiff(timeout, lastTime, now);
        }
        while (timeout.tv_usec || timeout.tv_sec);

        /* Poll (without blocking) for new input, output, and exceptions */
        if (!server.Poll())
            return false; // TODO do something more with this situation possibly or leave for Stop() cleanup.
        /* If there are new connections waiting, accept them. */
        if (!server.AcceptNewClients())
            return false; // TODO see above
        /* Transfer the freaky folks in the exception set and/or marked for close */
        if (!server.TransferClosingClients())
            return false; // TODO see above
        /* Process descriptors with input pending and process commands for the input */
        server.ProcessInput();
        /* Send queued output out to the operating system (ultimately to user) and print prompts for
         * for those with no output.
         */
        server.ProcessOutput();
        /* Kick out folks in the CON_CLOSE or CON_DISCONNECT state or on the exception fd set */
        server.ClosePendingClients();
        /*
         * Now, we execute as many pulses as necessary--just one if we haven't
         * missed any pulses, or make up for lost time if we missed a few
         * pulses by sleeping for too long.
         */
        missedPulses++;

        if (missedPulses <= 0)
        {
            Log("SYSERR: **BAD** MISSED_PULSES NONPOSITIVE (%d), TIME GOING BACKWARDS!!", missedPulses);
            missedPulses = 1;
        }

        /* If we missed more than 30 seconds worth of pulses, just do 30 secs */
        if (missedPulses > 30 RL_SEC)
        {
            Log("SYSERR: Missed %d seconds worth of pulses.", missedPulses / PASSES_PER_SEC);
            missedPulses = 30 RL_SEC;
        }

        /* Now execute the heartbeat functions */
        while (missedPulses--)
            Heartbeat(++pulse);

        /* Check for any signals we may have received. */
        /*TODO            if (reread_wizlist) {
                        reread_wizlist = FALSE;
                        mudlog(CMP, LVL_IMMORT, TRUE, "Signal received - rereading wizlists.");
                        reboot_wizlists();
                    }
                    if (emergency_unban) {
                        emergency_unban = FALSE;
                        mudlog(BRF, LVL_IMMORT, TRUE, "Received SIGUSR2 - completely unrestricting game (emergent)");
                        ban_list = NULL;
                        circle_restrict = 0;
                        num_invalid = 0;
                    }
        */
        /* Roll pulse over after 10 hours */
        if (pulse >= (10 * 60 * 60 * PASSES_PER_SEC))
            pulse = 0;

        /* Update tics for deadlock protection (UNIX only) */
        TICS++;
    }
    return true;
}

// See CMUD.h for description.
bool CMUD::Stop()
{
    // TODO Crash_save_all();

    Log("Closing all sockets.");
    // TODO while (descriptor_list)
    // TODO     close_socket(descriptor_list);

    // TODO CLOSE_SOCKET(mother_desc);
    // TODO fclose(player_fl);

    Log("Saving current MUD time.");
    // TODO save_mud_time(&time_info);

    /* TODO
    if (circle_reboot) {
        Log("Rebooting.");
        // what's so great about HHGTTG, anyhow?
        exit(52);
    }*/
    Log("Normal termination of game.");
// TODO        Crash_save_all();

    Log("Closing all sockets.");
    /* TODO        while (descriptor_list)
                close_socket(descriptor_list);
    */
// TODO        server.Close();
// TODO        fclose(player_fl);

    Log("Saving current MUD time.");
// TODO        save_mud_time(&time_info);

    /* TODO
    if (circle_reboot) {
        Log("Rebooting.");
        exit(52);                   // what's so great about HHGTTG, anyhow?
    }*/
    Log("Normal termination of game.");
    Log("Clearing game world.");
    // TODO destroy_db();
    if (!IsRunFlagged(RF_SYNTAX_CHECK))
    {
        Log("Clearing other memory.");
        // TODO
        /*
                    free_player_index();        // db.c
                    free_messages();            // fight.c
                    clear_free_list();          // mail.c
                    free_text_files();    	// db.c
                    Board_clear_all();          // boards.c
                    free(cmd_sort_info);        // act.informative.c
                    free_social_messages();     // act.social.c
                    free_help();                // db.c
                    Free_Invalid_List();        // ban.c
                    */
    }

    Log("Done.");
    if (log != stderr)
        fclose(log);
    return true;
}

CMUD::SBan::TYPE CMUD::HostBanned(const string& pHost)
{
    return CMUD::SBan::NONE;
}

void CMUD::Sleep(timeval* pTimeOut)
{
    if (select(0, nullptr, nullptr, nullptr, pTimeOut) < 0)
    {
//TODO            if (select(0, (fd_set *) 0, (fd_set *) 0, (fd_set *) 0, pTimeOut) < 0) {
        if (errno != EINTR)
        {
            perror("SYSERR: Select sleep");
            exit(1);
        }
    }
}

void CMUD::Heartbeat(const pm_pulse& pPulse)
{
    if (!(pPulse % PULSE_ZONE))
        HeartbeatZones();

    if (!(pPulse % PULSE_IDLEPWD))		// 15 seconds
        HeartbetDCIdlePasswordPrompt();

    if (!(pPulse % PULSE_VIOLENCE))
        HeartbeatCombat();

    if (!(pPulse % (SECS_PER_MUD_HOUR * PASSES_PER_SEC)))
    {
        /*TODO
                    weather_and_time(1);
                    affect_update();
                    point_update();
                    fflush(player_fl);*/
    }

    /* TODO
    if (auto_save && !(pPulse % PULSE_AUTOSAVE)) {	// 1 minute
        if (++mins_since_crashsave >= autosave_time) {
            mins_since_crashsave = 0;
            Crash_save_all();
            House_save_all();
        }
    }*/

    if (!(pPulse % PULSE_TIMESAVE))
        HeartbeatSaveMUDTime();

    /* Every pulse! Don't want them to stink the place up... */
    ;//TODO        extract_pending_chars();

}

void CMUD::HeartbeatZones()
{
    for(auto zone: zones)
    {
        // TODO add zone_update code

    }
}

void CMUD::HeartbetDCIdlePasswordPrompt()
{
    //TODO            check_idle_passwords();

}

void CMUD::HeartbeatCombat()
{
    //TODO            perform_violence();

}

void CMUD::HeartbeatSaveMUDTime()
{
    //TODO            save_mud_time(&time_info);

}
}
