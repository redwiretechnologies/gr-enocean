#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2022 Toby Flynn.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#


import numpy as np

from gnuradio import gr
from enocean.protocol import crc8
from enocean.protocol.packet import Packet
from enocean.protocol.constants import PACKET, RORG
import pmt

class encoean_packat_parser(gr.basic_block):
    """
    Currently this only processes single telegram data types of the enocean data type 
    """
    def __init__(self):
        gr.basic_block.__init__(self,
            name="encoean_packat_parser",
            in_sig=None,
            out_sig=None)
        self.message_port_register_in(pmt.intern("radio_data_in"))
        self.set_msg_handler(pmt.intern("radio_data_in"), self.process_radio_data)
        self.telegram_types = [0xF6, 0xD5, 0xA5, 0xD0, 0xD2, 0xD4, 0xD1, 0x30, 0x31, 0x35, 0xB3, 0xA8, 0x00, 0x00, 0x00, 0x00]


    def process_radio_data(self,msg):
         data = pmt.to_python(pmt.cdr(msg))
         meta = pmt.car(msg)
         #based on enocean serial document and running the python enocean receiver
         
         #check that the crc is valid
         crc=crc8.calc(data[1:data.size-1])
         print(data)
         print(crc)
         print(data[data.size-1])
         if(crc != data[data.size-1]):
             #Bad CRC return
             print("BAD CRC on packet")
             return
         
         # Header
         data_list=data.tolist()
         serial_packet = [0x55]
         serial_packet.append(0x00)
         serial_packet.append(data_list[0])
         serial_packet.append(0x07) 
         serial_packet.append(0x01) 
         serial_packet.append(crc8.calc(serial_packet[1:5]))
         
         # Data
         telegram_type_id = (data[1] & 0x0f)
         serial_packet.append(self.telegram_types[telegram_type_id])
         serial_packet = serial_packet + data_list[6:len(data_list)-1]
         serial_packet = serial_packet + data_list[2:6]
         serial_packet.append(0x80)  # This is the 0x80 that was used for the status, can we assume this is always this value?
         serial_packet = serial_packet + [1, 255, 255, 255, 255, 41, 0]
         serial_packet.append(crc8.calc(serial_packet[6:data_list[0]+14]))

         status, temp, packet = Packet.parse_msg(serial_packet)
         print(packet.__str__())
         
         if packet.packet_type == PACKET.RADIO_ERP1 and packet.rorg == RORG.VLD:
             packet.select_eep(0x05, 0x00)
             packet.parse_eep()
             for k in packet.parsed:
                 print('%s: %s' % (k, packet.parsed[k]))
         if packet.packet_type == PACKET.RADIO_ERP1 and packet.rorg == RORG.BS4:
             # parse packet with given FUNC and TYPE
             for k in packet.parse_eep(0x02, 0x05):
                 print('%s: %s' % (k, packet.parsed[k]))
         if packet.packet_type == PACKET.RADIO_ERP1 and packet.rorg == RORG.BS1:
             # alternatively you can select FUNC and TYPE explicitely
             packet.select_eep(0x00, 0x01)
             # parse it
             packet.parse_eep()
             for k in packet.parsed:
                 print('%s: %s' % (k, packet.parsed[k]))
         if packet.packet_type == PACKET.RADIO_ERP1 and packet.rorg == RORG.RPS:
             for k in packet.parse_eep(0x02, 0x02):
                 print('%s: %s' % (k, packet.parsed[k]))

