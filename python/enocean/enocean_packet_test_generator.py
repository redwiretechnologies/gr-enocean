#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2022 Toby Flynn.
#

#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from enocean.protocol.packet import Packet, RadioPacket
from enocean.protocol.constants import PACKET, RORG
from enocean.decorators import timing

import numpy as np
from gnuradio import gr
import pmt

class enocean_packet_test_generator(gr.sync_block):
    """
    docstring for block enocean_packet_test_generator
    """
    def __init__(self):
        gr.sync_block.__init__(self,
            name="enocean_packet_test_generator",
            in_sig=None,
            out_sig=None)
        self.message_port_register_in(pmt.intern("create_packet"))
        self.set_msg_handler(pmt.intern("create_packet"), self.create_packet_process)
        self.message_port_register_out(pmt.intern("radio_data_out"))
        self.packet_value=1;
        
       
        self.packet_1 = np.array([0xaa, 0xaa, 0xa9, 0x3c, 0x0a, 0x22, 0x04, 0x13, 0xd8, 0x4f, 0x00, 0x8f, 0x8f, 0x0a, 0xba, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],dtype=np.uint8)
        self.packet_2 = np.array([0xaa, 0xaa, 0xa9, 0x3c, 0x07, 0x21, 0x05, 0x91, 0xee, 0xa6, 0x09, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],dtype=np.uint8)
        self.packet_3 = np.array([0xaa, 0xaa, 0xa9, 0x3c, 0x07, 0x21, 0x05, 0x91, 0xee, 0xa6, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],dtype=np.uint8)
        self.packet_4 = np.array([0xaa, 0xaa, 0xa9, 0x3c, 0x0a, 0x22, 0x05, 0x8a, 0xd3, 0xd4, 0x65, 0xff, 0x6d, 0x0e, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00],dtype=np.uint8)
        self.packet_content=self.packet_1

    
         
         
         
    def create_packet_process(self,msg):
         if(pmt.is_pair(msg)):
              message=pmt.car(msg)
              value=pmt.cdr(msg)
              print(message)
            
              if(message == pmt.intern("sendpacket")):
                  self.msgName="radiopacket"
                  #data =  numpy.array(self.packet_content)
                  self.message_port_pub(pmt.intern("radio_data_out"),
                                  pmt.cons(pmt.PMT_NIL, pmt.to_pmt(self.packet_content)))
              
              
              elif(message == pmt.intern("Packet_Select")):
                  packetnumber=pmt.to_python(value)
                  if(packetnumber==1):
                      self.packet_content=self.packet_1
                  elif(packetnumber==2):
                      self.packet_content=self.packet_2
                  elif(packetnumber==3):
                      self.packet_content=self.packet_3
                  elif(packetnumber==4):
                      self.packet_content=self.packet_4
                  
                  
                


    def work(self, input_items, output_items):
        in0 = input_items[0]
        out = output_items[0]
        # <+signal processing here+>
        out[:] = in0
        return len(output_items[0])
