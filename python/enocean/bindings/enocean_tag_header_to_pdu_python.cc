/*
 * Copyright 2022 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(enocean_tag_header_to_pdu.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(69e44a8f3862e553eaca8dbed51adc19)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/enocean/enocean_tag_header_to_pdu.h>
// pydoc.h is automatically generated in the build directory
#include <enocean_tag_header_to_pdu_pydoc.h>

void bind_enocean_tag_header_to_pdu(py::module& m)
{

    using enocean_tag_header_to_pdu    = gr::enocean::enocean_tag_header_to_pdu;


    py::class_<enocean_tag_header_to_pdu, 
               gr::sync_block,
               gr::block,
               gr::basic_block,
        std::shared_ptr<enocean_tag_header_to_pdu>>(m, "enocean_tag_header_to_pdu", D(enocean_tag_header_to_pdu))

        .def(py::init(&enocean_tag_header_to_pdu::make),
             py::arg("tag_name"),
           D(enocean_tag_header_to_pdu,make)
        )
        



        ;




}








