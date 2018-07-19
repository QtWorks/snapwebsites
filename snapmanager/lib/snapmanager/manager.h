// Snap Websites Server -- snap manager CGI, daemon, library, plugins
// Copyright (c) 2016-2018  Made to Order Software Corp.  All Rights Reserved
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

// ourselves
//
#include "server_status.h"
#include "snapmanager/version.h"        // Auto-generated by cmake

// snapwebsites lib
//
#include <snapwebsites/snapwebsites.h>

// C lib
//
#include <sys/stat.h>

// C++ lib
//
#include <set>


namespace snap_manager
{


enum class name_t
{
    SNAP_NAME_MANAGER_STATUS_FILE_HEADER
};
char const * get_name(name_t name) __attribute__ ((const));




class snapmanager_exception : public snap::snap_exception
{
public:
    explicit snapmanager_exception(char const *        whatmsg) : snap_exception("snapwatchdog", whatmsg) {}
    explicit snapmanager_exception(std::string const & whatmsg) : snap_exception("snapwatchdog", whatmsg) {}
    explicit snapmanager_exception(QString const &     whatmsg) : snap_exception("snapwatchdog", whatmsg) {}
};

class snapmanager_exception_cannot_load_plugins : public snapmanager_exception
{
public:
    explicit snapmanager_exception_cannot_load_plugins(char const *        whatmsg) : snapmanager_exception(whatmsg) {}
    explicit snapmanager_exception_cannot_load_plugins(std::string const & whatmsg) : snapmanager_exception(whatmsg) {}
    explicit snapmanager_exception_cannot_load_plugins(QString const &     whatmsg) : snapmanager_exception(whatmsg) {}
};

class snapmanager_exception_invalid_parameters : public snapmanager_exception
{
public:
    explicit snapmanager_exception_invalid_parameters(char const *        whatmsg) : snapmanager_exception(whatmsg) {}
    explicit snapmanager_exception_invalid_parameters(std::string const & whatmsg) : snapmanager_exception(whatmsg) {}
    explicit snapmanager_exception_invalid_parameters(QString const &     whatmsg) : snapmanager_exception(whatmsg) {}
};

class snapmanager_exception_undefined : public snapmanager_exception
{
public:
    explicit snapmanager_exception_undefined(char const *        whatmsg) : snapmanager_exception(whatmsg) {}
    explicit snapmanager_exception_undefined(std::string const & whatmsg) : snapmanager_exception(whatmsg) {}
    explicit snapmanager_exception_undefined(QString const &     whatmsg) : snapmanager_exception(whatmsg) {}
};



enum class service_status_t
{
    SERVICE_STATUS_UNKNOWN = -2,
    SERVICE_STATUS_NOT_INSTALLED = -1,
    SERVICE_STATUS_DISABLED,
    SERVICE_STATUS_ENABLED,
    SERVICE_STATUS_ACTIVE,
    SERVICE_STATUS_FAILED
};


typedef uint32_t   replace_configuration_value_t;

replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_NO_FLAGS        =  0x0000;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_CREATE_BACKUP   =  0x0001;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_DOUBLE_QUOTE    =  0x0002;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_SINGLE_QUOTE    =  0x0004;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_MUST_EXIST      =  0x0008;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_COLON           =  0x0010;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_SPACE_AFTER     =  0x0020;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_HASH_COMMENT    =  0x0040;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_SECTION         =  0x0080;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_FILE_MUST_EXIST =  0x0100;
replace_configuration_value_t const    REPLACE_CONFIGURATION_VALUE_TRIM_RESULT     =  0x0200;


class manager
    : public snap::plugins::plugin
    , public snap::snap_child
    , public std::enable_shared_from_this<manager>
{
public:
    typedef std::shared_ptr<manager>         pointer_t;

                                    manager(bool daemon);
    virtual                         ~manager() override;

    void                            init(int argc, char * argv[]);

    static pointer_t                instance();
    virtual QString                 description() const override;
    virtual QString                 dependencies() const override;
    virtual void                    bootstrap(snap::snap_child * snap);

    void                            load_plugins();
    std::vector<std::string>        get_list_of_servers();
    QString const &                 get_server_name() const;
    QString const &                 get_public_ip() const;
    std::string const &             get_signal_address() const;
    int                             get_signal_port() const;
    virtual snap::snap_string_list const & get_snapmanager_frontend() const;
    std::vector<std::string> const & get_bundle_uri() const;
    std::vector<std::string>        get_list_of_bundles() const;
    QString const &                 get_bundles_path() const;
    QString const &                 get_data_path() const;
    QString const &                 get_cache_path() const;
    QString const &                 get_reboot_required_path() const;
    virtual bool                    stop_now_prima() const;
    virtual void                    forward_message(snap::snap_communicator_message const & message);
    bool                            upgrader();
    std::string                     lock_filename() const;
    int                             update_packages(std::string const & command);
    void                            reset_aptcheck();
    bool                            installer(QString const & bundle_name, std::string const & command, std::string const & install_values, std::set<QString> & affected_services);
    service_status_t                service_status(std::string const & service_filename, std::string const & service_name);
    void                            service_apply_status(std::string const & service_name, service_status_t const status);
    void                            reboot(bool reboot = true);
    int                             package_status(std::string const & package_name, std::string & output);
    QString                         count_packages_that_can_be_updated(bool check_cache);
    std::vector<std::string>        read_filenames(std::string const & pattern) const;
    bool                            replace_configuration_value(QString const & filename, QString const & field_name, QString const & new_value, replace_configuration_value_t const flags = REPLACE_CONFIGURATION_VALUE_CREATE_BACKUP, QString const & trim_left = {});
    std::string                     get_parameter(std::string const & name) const;

    SNAP_SIGNAL_WITH_MODE(retrieve_status, (server_status & status), (status), NEITHER);
    SNAP_SIGNAL_WITH_MODE(handle_affected_services, (std::set<QString> & affected_services), (affected_services), NEITHER);
    SNAP_SIGNAL_WITH_MODE(communication_ready, (), (), NEITHER);
    SNAP_SIGNAL_WITH_MODE(process_plugin_message, (snap::snap_communicator_message const & message, bool & processed), (message, processed), NEITHER);
    SNAP_SIGNAL_WITH_MODE(add_plugin_commands, (snap::snap_string_list & understood_commands), (understood_commands), NEITHER);

    static int                      get_version_major() __attribute__ ((const));
    static int                      get_version_minor() __attribute__ ((const));
    static int                      get_version_patch() __attribute__ ((const));
    static char const *             get_version_string() __attribute__ ((const));
    static std::string::size_type   search_parameter(std::string const & configuration, std::string const & parameter_name, std::string::size_type const start_pos, bool const ignore_case);
    static char const *             service_status_to_string(service_status_t const status);
    static service_status_t         string_to_service_status(std::string const & status);

protected:
    static void                     sighandler( int sig );
    int                             install_package(std::string const & package_name, std::string const & command);

    bool const                      f_daemon = false;
    bool                            f_debug = false;
    advgetopt::getopt::pointer_t    f_opt;
    snap::snap_config               f_config;
    QString                         f_server_name;
    QString                         f_log_conf = "/etc/snapwebsites/logger/snapmanager.properties";
    QString                         f_data_path = "/var/lib/snapwebsites";
    QString                         f_cluster_status_path = f_data_path + "/cluster-status";
    QString                         f_bundles_path = f_data_path + "/bundles";
    QString                         f_public_ip;
    QString                         f_plugins_path = "/usr/lib/snapwebsites/manager_plugins";
    QString                         f_cache_path = "/var/cache/snapwebsites";
    QString                         f_apt_check = "/usr/lib/update-notifier/apt-check";
    QString                         f_reboot_required = "/run/reboot-required";
    QString                         f_lock_path = "/run/lock/snapwebsites";
    std::string                     f_signal_address = "127.0.0.1";
    int                             f_signal_port = 4041;
    std::vector<std::string>        f_bundle_uri;

private:
    bool                            f_plugins_loaded = false;
};


} // snap_manager namespace
// vim: ts=4 sw=4 et
