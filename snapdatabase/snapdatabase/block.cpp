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


/** \file
 * \brief Base block implementation.
 *
 * The block base class handles the loading of the block in memory using
 * mmap() and gives information such as its type and location.
 */

// self
//
#include    "snapdatabase/block.h"


// last include
//
#include    <snapdev/poison.h>



namespace snapdatabase
{



block::pointer_t block::create_block(dbfile::pointer_t f, file_addr_t offset)
{
    return block::pointer_t(new block(f, offset));
}


block::pointer_t block::create_block(dbfile::pointer_t f, dbtype_t type)
{
    pointer_t header(create_block(f, 0));

    file_addr_t const offset(f->get_new_block(type));
    return pointer_t(new block(f, offset));
}


block::block(dbfile::pointer_t f, file_addr_t offset)
    : f_file(f)
    , f_offset(offset)
{
}


block::dbtype_t block::get_dbtype() const
{
    return f_type;
}


size_t block::size()
{
    return f_size;
}


virtual_buffer::pointer_t block::data()
{
    return f_data;
}



} // namespace snapdatabase
// vim: ts=4 sw=4 et
