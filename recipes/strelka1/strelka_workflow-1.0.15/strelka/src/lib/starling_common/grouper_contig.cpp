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

#include "starling_common/grouper_contig.hh"

#include <iostream>



std::ostream&
operator<<(std::ostream& os,
           const grouper_contig& ctg) {

    os << "GROUPER_CONTIG id: " << ctg.id
       << " chrom: " << ctg.chrom
       << " usable: " << ctg.is_usable << "\n"
       << static_cast<const alignment&>(ctg)
       << "seq: " << ctg.seq << "\n";

    return os;
}