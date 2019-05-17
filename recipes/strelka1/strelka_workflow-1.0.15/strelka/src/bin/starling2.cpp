// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Copyright (c) 2009-2016 Illumina, Inc.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

/// \file
///
/// \author Chris Saunders
///
/// note coding convention for all ranges '_pos fields' is:
/// XXX_begin_pos is zero-indexed position at the begining of the range
/// XXX_end_pos is zero-index position 1 step after the end of the range
///

#include "blt_common/blt_arg_parse_util.hh"
#include "blt_util/blt_exception.hh"
#include "blt_util/log.hh"
#include "blt_util/sig_handler.hh"
#include "common/Exceptions.hh"
#include "starling_common/starling_arg_parse.hh"
#include "starling_common/starling_option_parser.hh"
#include "starling/starling_info.hh"
#include "starling/starling_run.hh"

#include <cassert>
#include <cstdlib>


namespace {
const prog_info& pinfo(starling_info::get());
}



static
void
try_main(int argc,char* argv[]) {

    starling_options opt;

    for (int i(0); i<argc; ++i) {
        if (i) opt.cmdline += ' ';
        opt.cmdline += argv[i];
    }

    initialize_blt_signals(pinfo.name(),opt.cmdline.c_str());

    std::vector<std::string> legacy_starling_args;
    po::variables_map vm;
    try {
        po::options_description visible(get_starling_option_parser(opt));
        po::parsed_options parsed(po::command_line_parser(argc,argv).options(visible).allow_unregistered().run());
        po::store(parsed,vm);
        po::notify(vm);

        // allow remaining options to be parsed using old starling command-line parser:
        legacy_starling_args = po::collect_unrecognized(parsed.options,po::include_positional);

    } catch (const boost::program_options::error& e) {
        pinfo.usage(e.what());
    }

    if ((argc==1) || vm.count("help")) {
        pinfo.usage();
    }

    // temp workaround for blt/starling options which are not (yet)
    // under program_options control:
    //
    arg_data ad(legacy_starling_args,pinfo,opt.cmdline);
    legacy_starling_arg_parse(ad,opt);

    finalize_starling_options(pinfo,vm,opt);

    starling_run(opt);
}



static
void
dump_cl(int argc,
        char* argv[],
        std::ostream& os) {

    os << "cmdline:";
    for (int i(0); i<argc; ++i) {
        os << ' ' << argv[i];
    }
    os << std::endl;
}



int
main(int argc,char* argv[]) {

    std::ios_base::sync_with_stdio(false);

    // last chance to catch exceptions...
    //
    try {
        try_main(argc,argv);

    } catch (const blt_exception& e) {
        log_os << "FATAL_ERROR: " << pinfo.name() << " EXCEPTION: " << e.what() << "\n"
               << "...caught in main()\n";
        dump_cl(argc,argv,log_os);
        exit(EXIT_FAILURE);
    } catch (const illumina::common::ExceptionData& e) {
        log_os << "FATAL_ERROR: " << pinfo.name() << " EXCEPTION: "
               << e.getContext() << ": " << e.getMessage() << "\n"
               << "...caught in main()\n";
        dump_cl(argc,argv,log_os);
        exit (EXIT_FAILURE);
    } catch (const std::exception& e) {
        log_os << "FATAL_ERROR: EXCEPTION: " << e.what() << "\n"
               << "...caught in main()\n";
        dump_cl(argc,argv,log_os);
        exit(EXIT_FAILURE);
    } catch (...) {
        log_os << "FATAL_ERROR: UNKNOWN EXCEPTION\n"
               << "...caught in main()\n";
        dump_cl(argc,argv,log_os);
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}