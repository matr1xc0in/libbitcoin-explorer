/**
 * Copyright (c) 2011-2014 sx developers (see AUTHORS)
 *
 * This file is part of sx.
 *
 * sx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
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
#ifndef SX_COIN_HPP
#define SX_COIN_HPP

#include <stdint.h>
#include <bitcoin/bitcoin.hpp>
#include <sx/utility/console.hpp>

/* NOTE: don't declare 'using namespace foo' in heders. */

namespace sx {

/**
 * Type for definiing lists of payment addresses. 
 */
typedef std::vector<bc::payment_address> payaddr_list;

/**
 * Key compression states (3-state boolean). 
 */
enum class key_compression { UNSPECIFIED, ON, OFF };

/**
 * Load a satoshi item from the specified file.
 *
 * @param      <TItem>   The type of the item to load.
 * @param[in]  item      The loaded item.
 * @param[in]  filename  The path and file name for the transaction file.
 * @return               True if a transaction was loaded.
 */
template <typename TItem>
bool load_satoshi_item(TItem& item, const std::string& filename)
{
    // TODO: extract localized text.

    std::ostringstream contents;
    if (filename == STDIN_PATH_SENTINEL)
        contents << sx::read_stream(std::cin);

    else
    {
        std::ifstream infile(filename, std::ifstream::binary);
        if (!infile)
        {
            std::cerr << "showtx: Bad file." << std::endl;
            return false;
        }
        contents << infile.rdbuf();
    }
    auto raw_tx = decode_hex(contents.str());
    try
    {
        satoshi_load(raw_tx.begin(), raw_tx.end(), item);
    }
    catch (end_of_stream)
    {
        std::cerr << "sx: Deserializing item failed." << std::endl;
        return false;
    }
    return true;
}

/**
 * Allocate a byte buffer and fill it with a random bit distribution.
 *
 * @param[in]  size  The byte size of the return buffer.
 * @return           The sized buffer with random bit distribution.
 */
bc::data_chunk random_fill(size_t size);

/**
 * Read two address arguments from the specified args and/or input stream.
 *
 * @param[in]  argc          The number of args.
 * @param[in]  argv          The arguments to read.
 * @param[in]  cin           The input stream.
 * @param[out] hex_str       The return string value.
 * @param[out] version_byte  The return byte value.
 * return                    True if there was no parse error.
 */
bool read_address_tuple(const int argc, const char* argv[], std::istream& cin,
    std::string& hex_str, uint8_t& version_byte);

/**
 * Read a set of payment addresses from the specified argv. Each argument
 * that is not an option is assumed to be a payment address.
 *
 * @param[in]  argc      The number of args.
 * @param[in]  argv      The arguments to read.
 * @param[out] payaddrs  The payment addresses read.
 * return                True if there was no payment address parse error.
 */
bool read_addresses(const int argc, const char* argv[], 
    sx::payaddr_list& payaddrs);

/**
 * Read optional hard/index argument pair from args, with defaults.
 * Side effects: writes to STDERR in case of parsing error.
 *
 * @param[in]  argc     The number of args.
 * @param[in]  argv     The arguments to read.
 * @param[out] is_hard  The parsed is hard value or false if not present.
 * @param[out] index    The parsed index value or zero if not present.
 * @return              True if there was no error parsing an argument.
 */
bool read_hard_index_args(const int argc, const char* argv[], bool& is_hard,
    uint32_t& index);

/**
 * Read a private key from the specified input stream.
 *
 * @param[out] key            The read key.
 * @param[in]  cin  The input stream.
 * @param[in]  is_compressed  Flag indicating whether compression is expected.
 * @return                    True if a key was read.
 */
bool read_private_key(bc::elliptic_curve_key& key, std::istream& cin,
    key_compression is_compressed = key_compression::UNSPECIFIED);

/**
 * Read a private key from STDIN.
 *
 * @param[out] key            The read key.
 * @param[in]  arg            The argument from which to read the key.
 * @param[in]  is_compressed  Flag indicating whether compression is expected.
 * @return                    True if a key was read.
 */
bool read_private_key(bc::elliptic_curve_key& key, const std::string& arg,
    key_compression is_compressed = key_compression::UNSPECIFIED);

/**
 * Read a public or private key from the specified input stream.
 *
 * @param[out] key  The read key.
 * @param[in]  cin  The input stream.
 * @return          True if a key was read.
 */
bool read_public_or_private_key(bc::elliptic_curve_key& key, std::istream& cin);

/**
 * Validate the bitcoin checksum of a chunk of binary data.
 *
 * @param[int] data  The data to validate.
 * @return           True if the checksum is valid.
 */
bool validate_checksum(bc::data_chunk& data);

} // sx

#endif