// Snap Websites Server -- snap watchdog daemon
// Copyright (c) 2011-2018  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#pragma once

// snapwebsites lib
//
#include <snapwebsites/snapwebsites.h>

// Qt lib
//
#include <QDomDocument>


namespace snap
{

namespace watchdog
{
enum class name_t
{
    SNAP_NAME_WATCHDOG_ADMINISTRATOR_EMAIL,
    SNAP_NAME_WATCHDOG_CACHE_PATH,
    SNAP_NAME_WATCHDOG_DATA_PATH,
    SNAP_NAME_WATCHDOG_DEFAULT_LOG_PATH,
    SNAP_NAME_WATCHDOG_FROM_EMAIL,
    SNAP_NAME_WATCHDOG_LOG_PATH,
    SNAP_NAME_WATCHDOG_SERVER_NAME,
    SNAP_NAME_WATCHDOG_SERVERSTATS,
    SNAP_NAME_WATCHDOG_STATISTICS_FREQUENCY,
    SNAP_NAME_WATCHDOG_STATISTICS_PERIOD,
    SNAP_NAME_WATCHDOG_STATISTICS_TTL
};
char const * get_name(name_t name) __attribute__ ((const));
} // watchdog namespace



class snapwatchdog_exception : public snap_exception
{
public:
    explicit snapwatchdog_exception(char const *        whatmsg) : snap_exception("snapwatchdog", whatmsg) {}
    explicit snapwatchdog_exception(std::string const & whatmsg) : snap_exception("snapwatchdog", whatmsg) {}
    explicit snapwatchdog_exception(QString const &     whatmsg) : snap_exception("snapwatchdog", whatmsg) {}
};

class snapwatchdog_exception_invalid_parameters : public snapwatchdog_exception
{
public:
    explicit snapwatchdog_exception_invalid_parameters(char const *        whatmsg) : snapwatchdog_exception(whatmsg) {}
    explicit snapwatchdog_exception_invalid_parameters(std::string const & whatmsg) : snapwatchdog_exception(whatmsg) {}
    explicit snapwatchdog_exception_invalid_parameters(QString const &     whatmsg) : snapwatchdog_exception(whatmsg) {}
};


class watchdog_child;



class watchdog_server
    : public server
{
public:
    typedef std::shared_ptr<watchdog_server>         pointer_t;

                        watchdog_server();

    static pointer_t    instance();
    void                watchdog();

    time_t              get_server_start_date() const;
    virtual void        show_version();
    int64_t             get_statistics_period() const { return f_statistics_period; }
    int64_t             get_statistics_ttl() const { return f_statistics_ttl; }
    void                stop(bool quitting);
    void                set_snapcommunicator_connected(bool status);
    void                set_snapcommunicator_disconnected(bool status);
    bool                get_snapcommunicator_is_connected() const;
    int64_t             get_snapcommunicator_connected_on() const;
    int64_t             get_snapcommunicator_disconnected_on() const;

    SNAP_SIGNAL_WITH_MODE(process_watch, (QDomDocument doc), (doc), NEITHER);

    // internal functions
    void                process_tick();
    void                process_sigchld();
    void                process_message(snap::snap_communicator_message const & message);

private:
    void                define_server_name();
    void                init_parameters();
    void                run_watchdog_process();

    time_t const                                    f_server_start_date;
    int64_t                                         f_statistics_frequency = 0;
    int64_t                                         f_statistics_period = 0;
    int64_t                                         f_statistics_ttl = 0;
    std::vector<std::shared_ptr<watchdog_child>>    f_processes = std::vector< std::shared_ptr<watchdog_child>>();
    bool                                            f_stopping = false;
    int64_t                                         f_snapcommunicator_connected = 0;
    int64_t                                         f_snapcommunicator_disconnected = 0;
};


class watchdog_child
    : public snap_child
{
public:
    typedef std::shared_ptr<watchdog_child>     pointer_t;

                        watchdog_child(server_pointer_t s, bool tick);
    virtual             ~watchdog_child() override;

    bool                is_tick() const;
    bool                run_watchdog_plugins();
    bool                record_usage(snap::snap_communicator_message const & message);
    virtual void        exit(int code) override;

    pid_t               get_child_pid() const;
    void                append_error(QDomDocument doc, QString const & plugin_name, QString const & message, int priority = 50);
    void                append_error(QDomDocument doc, QString const & plugin_name, std::string const & message, int priority = 50);
    void                append_error(QDomDocument doc, QString const & plugin_name, char const * message, int priority = 50);

    watchdog_server::pointer_t  get_server();

private:
    pid_t               f_child_pid = -1;
    bool const          f_tick = true;
    bool                f_has_cassandra = false;
};


} // namespace snap
// vim: ts=4 sw=4 et