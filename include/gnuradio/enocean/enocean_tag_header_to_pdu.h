/* -*- c++ -*- */
/*
 * Copyright 2022 Toby Flynn.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_ENOCEAN_ENOCEAN_TAG_HEADER_TO_PDU_H
#define INCLUDED_ENOCEAN_ENOCEAN_TAG_HEADER_TO_PDU_H

#include <gnuradio/enocean/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
namespace enocean {

/*!
 * \brief This block tages a tagged stream from a correlate access block, reads the packet
 * length and then builds a PDU packet of the reminaing number data. \ingroup enocean
 *
 */
class ENOCEAN_API enocean_tag_header_to_pdu : virtual public gr::sync_block
{
public:
    typedef std::shared_ptr<enocean_tag_header_to_pdu> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of enocean::enocean_tag_header_to_pdu.
     *
     * To avoid accidental use of raw pointers, enocean::enocean_tag_header_to_pdu's
     * constructor is in a private implementation
     * class. enocean::enocean_tag_header_to_pdu::make is the public interface for
     * creating new instances.
     */
    static sptr make(pmt::pmt_t tag_name);
};

} // namespace enocean
} // namespace gr

#endif /* INCLUDED_ENOCEAN_ENOCEAN_TAG_HEADER_TO_PDU_H */
