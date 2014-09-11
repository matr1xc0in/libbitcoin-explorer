/**
 * Copyright (c) 2011-2014 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin_explorer.
 *
 * libbitcoin_explorer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "precompile.hpp"
#include <bitcoin/explorer/primitives/base16.hpp>

#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdint>
#include <boost/program_options.hpp>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/explorer/define.hpp>

using namespace po;

namespace libbitcoin {
namespace explorer {
namespace primitives {

    base16::base16()
        : value_()
    {
    }

    base16::base16(const std::string& hexcode)
    {
        std::stringstream(hexcode) >> *this;
    }

    base16::base16(const data_chunk& value)
        : value_(value)
    {
    }

    base16::base16(const base16& other)
        : base16(other.value_)
    {
    }

    data_chunk& base16::data()
    {
        return value_;
    }

    base16::operator const data_chunk&() const
    {
        return value_; 
    }

    std::istream& operator>>(std::istream& input, base16& argument)
    {
        std::string hexcode;
        input >> hexcode;

        data_chunk chunk = decode_hex(hexcode);
        if (chunk.empty())
            throw invalid_option_value(hexcode);

        argument.value_.assign(chunk.begin(), chunk.end());
        return input;
    }

    std::ostream& operator<<(std::ostream& output, const base16& argument)
    {
        output << encode_hex(argument.value_);
        return output;
    }

} // namespace explorer
} // namespace primitives
} // namespace libbitcoin