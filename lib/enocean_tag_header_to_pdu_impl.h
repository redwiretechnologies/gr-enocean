/* -*- c++ -*- */
/*
 * Copyright 2022 Toby Flynn.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_ENOCEAN_ENOCEAN_TAG_HEADER_TO_PDU_IMPL_H
#define INCLUDED_ENOCEAN_ENOCEAN_TAG_HEADER_TO_PDU_IMPL_H

#include <gnuradio/enocean/enocean_tag_header_to_pdu.h>
#include <stdbool.h> 
namespace gr {
  namespace enocean {

    class enocean_tag_header_to_pdu_impl : public enocean_tag_header_to_pdu
    {
     private:
         pmt::pmt_t d_sob_tag_key;
         std::vector<uint8_t> d_data;
         std::vector<uint8_t> d_packed_data;
         bool d_in_packet;
         uint8_t d_packet_length;
         uint32_t d_data_in_packet;
         std::vector<tag_t> d_tags;
         tag_t d_tag;
         uint32_t d_data_to_copy;
         void publish_data_packet(void);
         //The maximum amount of data is 255 bytes, plus 4 bytes for the preamble and sync word, and another byte of the the data length

     public:
         enocean_tag_header_to_pdu_impl(pmt::pmt_t tag_name);
      ~enocean_tag_header_to_pdu_impl() override;
       


      int work(int noutput_items,
               gr_vector_const_void_star& input_items,
               gr_vector_void_star& output_items) override;

    };

  } // namespace enocean
} // namespace gr

#endif /* INCLUDED_ENOCEAN_ENOCEAN_TAG_HEADER_TO_PDU_IMPL_H */
