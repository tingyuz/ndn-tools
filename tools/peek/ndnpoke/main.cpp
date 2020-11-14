/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 DELL
 * This file is part of ndn-tools (Named Data Networking Essential Tools).
 * See AUTHORS.md for complete list of ndn-tools authors and contributors.
 *
 * ndn-tools is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndn-tools is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndn-tools, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Tingyu Zeng 
 */

#include "ndnpoke.hpp"
#include "core/version.hpp"

namespace ndn {
namespace peek {

namespace po = boost::program_options;

static void
usage(std::ostream& os, const std::string& program, const po::options_description& options)
{
  os << "Usage: " << program << "\n"
     << "\n"
     << "Sends a single Data packet back to the client.\n"
     << options;
}

static int
main(int argc, char* argv[])
{
  std::string progName(argv[0]);
  PokeOptions options;
  std::string signingStr;  

  po::options_description genericOptDesc("Generic options");
  genericOptDesc.add_options()
    ("help,h",        "print help and exit")    
    ("timeout,w",     po::value<time::milliseconds::rep>(), "execution timeout, in milliseconds")
    ("verbose,v",     po::bool_switch(&options.isVerbose), "turn on verbose output")    
  ;

  po::options_description dataOptDesc("Data construction");
  dataOptDesc.add_options()
    ("final,F",     po::bool_switch(&options.wantFinalBlockId),
                    "set FinalBlockId to the last component of the Data name")
    ("freshness,x", po::value<time::milliseconds::rep>()->default_value(options.freshnessPeriod.count()),
                    "set FreshnessPeriod, in milliseconds")
    ("signing-info,S",  po::value<std::string>(&signingStr), "see 'man ndnpoke' for usage")
  ;

  po::options_description visibleOptDesc;
  visibleOptDesc.add(genericOptDesc).add(dataOptDesc);

  po::options_description optDesc;
  optDesc.add(visibleOptDesc);

  po::positional_options_description optPos;
  optPos.add("name", -1);

  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv).options(optDesc).positional(optPos).run(), vm);
    po::notify(vm);
  }
  catch (const po::error& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 2;
  }
 

  if (vm.count("help") > 0) {
    usage(std::cout, progName, visibleOptDesc);
    return 0;
  }

  options.freshnessPeriod = time::milliseconds(vm["freshness"].as<time::milliseconds::rep>());
  if (options.freshnessPeriod < 0_ms) {
    std::cerr << "ERROR: freshness cannot be negative" << std::endl;
    return 2;
  }
  
  try {
    options.signingInfo = security::SigningInfo(signingStr);
  }
  catch (const std::invalid_argument& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 2;
  }

  if (vm.count("timeout") > 0) {    
    options.timeout = time::milliseconds(vm["timeout"].as<time::milliseconds::rep>());
    if (*options.timeout < 0_ms) {
      std::cerr << "ERROR: timeout cannot be negative" << std::endl;
      return 2;
    }
  }

  try {
    Face face;
    KeyChain keyChain;
    NdnPoke program(face, keyChain, options);

    program.start();
    face.processEvents();

    return static_cast<int>(program.getResult());
  }
  catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}

} // namespace peek
} // namespace ndn

int
main(int argc, char* argv[])
{
  return ndn::peek::main(argc, argv);
}
