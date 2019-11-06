// Copyright (c) 2019  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/snapdatabase
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
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#pragma once


/** \file
 * \brief Block representing free space that can be allocated.
 *
 * This block is a _free_ blok meaning that it is not current used for
 * anything. It is part of the list of free blocks (linked list).
 */

// self
//
#include    "snapdatabase/block.h"



namespace snapdatabase
{



extern struct_description_t * g_free_block_description;


class block_free_block
    : public block
{
public:
    typedef std::shared_ptr<block_free_block>       pointer_t;

                                block_free_block(dbfile::pointer_t f, file_addr_t offset);

    file_addr_t                 get_next_free_block();
    void                        set_next_free_block(file_addr_t offset);

    static pointer_t            allocate_new_block(dbfile::pointer_t f, dbtype_t type);

private:
    structure                   f_structure = structure();
};



} // namespace snapdatabase
// vim: ts=4 sw=4 et
