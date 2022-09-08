/* -*- c++ -*- */
/*
 * Copyright 2022 Toby Flynn.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include <gnuradio/pdu.h>
#include "enocean_tag_header_to_pdu_impl.h"

namespace gr {
  namespace enocean {

   
    using input_type = char;
   
   
    enocean_tag_header_to_pdu::sptr
    enocean_tag_header_to_pdu::make(pmt::pmt_t tag_name)
    {
      return gnuradio::make_block_sptr<enocean_tag_header_to_pdu_impl>(
        tag_name);
    }


    /*
     * The private constructor
     */
    enocean_tag_header_to_pdu_impl::enocean_tag_header_to_pdu_impl(pmt::pmt_t tag_name)
    : gr::sync_block("enocean_tag_header_to_pdu",
              gr::io_signature::make(1 , 1 , sizeof(input_type)),
              gr::io_signature::make(0,0,0)),
              d_sob_tag_key(tag_name)
    {
        
        this->message_port_register_out(pmt::mp("packet_out"));
        d_in_packet=false;
        d_packet_length=0;
        d_data_in_packet=0;
        d_data_to_copy=40;
    }

    /*
     * Our virtual destructor.
     */
    enocean_tag_header_to_pdu_impl::~enocean_tag_header_to_pdu_impl()
    {
    }

    
    
    void enocean_tag_header_to_pdu_impl::publish_data_packet()
    {
        //pack all the bits to bytes, MSB and send packet data as complete bytes
        pmt::pmt_t pdu_meta;
        pmt::pmt_t pdu_vector;
        
        pdu_meta =  pdu_meta = pmt::make_dict();
        pdu_meta = pmt::dict_add(pdu_meta, pmt::intern( "packet_length"), pmt::from_long(d_packet_length));
        
        uint8_t bytes;
        for (int jj=0; jj<d_packet_length+1;jj++)
        {
            bytes=0x00;
            for (unsigned int ii=0; ii<8;ii++)
            {
                bytes |= (0x01 & d_data[ii+(jj*8)]) << (7-ii);
                
            }
            d_packed_data.insert(d_packed_data.end(), bytes);
        }
          
       
        
        pdu_vector = pmt::init_u8vector(d_packed_data.size(), d_packed_data);
        
        // Send msg
        pmt::pmt_t msg = pmt::cons(pdu_meta, pdu_vector);
        message_port_pub(pmt::mp("packet_out"), msg);
        //Send the pdu of vector of data
        d_in_packet=false;
        d_packet_length=0;
        d_data_in_packet=0;
        d_data_to_copy=0;
        d_data.clear();
        d_packed_data.clear();
    }

    int
    enocean_tag_header_to_pdu_impl::work (int noutput_items,
                                          gr_vector_const_void_star& input_items,
                                          gr_vector_void_star& output_items)
    {
      auto in = static_cast<const input_type*>(input_items[0]);
      uint32_t consumed = noutput_items;
      
      uint64_t abs_start = this->nitems_read(0);
      uint64_t abs_end = abs_start + noutput_items;
      
      
      
      // get all tags:
      this->get_tags_in_range(d_tags, 0, abs_start, abs_end, d_sob_tag_key);
      //this->get_tags_in_window(tags, 0, 0, noutput_items);
      
      //Look for d_sob_tag_key
      //Make sure we have at least 5 bytes, 40 input items includig the sob, unless we are already processing a file.
      
    
     
      int tag_found;
      //do we_have a packet
      tag_found=d_tags.size();
          
         
      if ( !d_in_packet && !tag_found) //we are not in a packet, no packet  this is first because it is the most common state
      {
          
         consume_each (consumed);
         return 0;//just consume everything
      }
      else if(d_in_packet && !tag_found)
      {
          //copy enough data based on packet length, upto the amount of data we have
          if(noutput_items > d_data_to_copy)
          {
              d_data.insert(d_data.end(), &in[0], &in[d_data_to_copy]);
              consumed=d_data_to_copy;
              publish_data_packet();
              
                  
          }
          else
          {
              d_data.insert(d_data.end(), &in[0], &in[noutput_items]);
              d_data_to_copy=d_data_to_copy-noutput_items;
              
          }
      }
      else if (d_in_packet)
      {
          //in the packet and we have a tag found
          //just add data to the limit and send the message
          if(d_data_to_copy < (abs_end-d_tags[0].offset))
          {
              //We have enough data to finsh the packet
              d_data.insert(d_data.end(), &in[0], &in[d_data_to_copy]);
              consumed=d_data_to_copy;
              publish_data_packet();
          }
          else
          {//Not enough data to close the previous packet, delete it and the data up to the tag
              consumed=d_tags[0].offset - abs_start -1; //consume up to the the tag
              d_in_packet=false;
              d_packet_length=0;
              d_data_in_packet=0;
              d_data_to_copy=40;
              d_data.clear();
              
          }
      }
      else if (!d_in_packet)
      {
          //We are not in a packet but have a tag found
          if(abs_end-d_tags[0].offset >8)
          {
              //we have enough data to caluate the packet_length and start processing
              d_data.insert(d_data.end(), &in[d_tags[0].offset-abs_start], &in[d_tags[0].offset-abs_start+8]);
              d_packet_length <<=1;
              uint8_t bytes = 0x00;
              //for (unsigned int j = 0; j < d_k; j++) {
              //    bytes |= (0x01 & bits[i * d_k + j]) << j;
              //}
              for (unsigned int ii=0; ii<8;ii++)
              {
                 bytes |= (0x01 & d_data[ii]) << (7-ii);
              
              }
              d_packet_length = uint8_t(bytes);
              consumed=d_tags[0].offset-abs_start+8;
              d_data_to_copy=d_packet_length*8;  //the data should be in bytes but we have bits
          
              d_in_packet = true;
        }
          else{
              consumed=d_tags[0].offset - abs_start-1; //cosume up to the tag
             
          }
      }
     
          
      
      
      consume_each (consumed);
      

      // Tell runtime system how many output items we produced.
      return 0;
    }

  } /* namespace enocean */
} /* namespace gr */
