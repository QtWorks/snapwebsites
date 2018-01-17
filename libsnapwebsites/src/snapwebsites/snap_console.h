// Snap Console -- two panel console with ncurses
// Copyright (C) 2018  Made to Order Software Corp.
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

// our lib
//
#include "snapwebsites/snap_communicator.h"



namespace snap
{


// see .cpp for implementation
class ncurses_impl;


class snap_console
    : public snap_communicator::snap_fd_connection
{
public:
    typedef std::shared_ptr<snap_console>    pointer_t;

                    snap_console(std::string const & history_filename = std::string());

    virtual         ~snap_console();

    void            output(std::string const & line);
    void            clear_output();
    void            set_prompt(std::string const & prompt);

    // implementation of snap_communicator::snap_fd_connection
    virtual void    process_read() override;

    // new callbacks
    //
    // the "command" (whatever text was typed in the command
    // area gets sent to you through this function)
    //
    // the "quit" is called whenever Ctrl-D was clicked on an
    // empty line; you must get the console closed or it will
    // be blocked; further typing will go to the normal console
    // instead of the "command", so it is important to take
    // that callback in account
    //
    virtual void    process_command(std::string const & command) = 0;
    virtual void    process_quit() = 0;

private:
    friend ncurses_impl;

    std::shared_ptr<ncurses_impl>     f_impl;
};


}
// snap namespace

// vim: ts=4 sw=4 et
