/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 DELL
 *
 * @author Tingyu Zeng 
 */

#include "ndndataservice.hpp"
#include "core/version.hpp"

namespace ndn {
namespace dell{
namespace dataservice {

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
  ServiceOptions options;  

  po::options_description genericOptDesc("Generic options");
  genericOptDesc.add_options()
    ("help,h",        "print help and exit")        
  ;  

  po::options_description visibleOptDesc;
  visibleOptDesc.add(genericOptDesc);

  po::options_description optDesc;
  optDesc.add(visibleOptDesc);

  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv).options(optDesc).run(), vm);
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

  try {
    Face face;
    KeyChain keyChain;
    NdnDataservice program(face, keyChain, options);

    program.start();
    face.processEvents();

    return static_cast<int>(program.getResult());
  }
  catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}

} // namespace dataservice
} // namespace dell
} // namespace ndn

int
main(int argc, char* argv[])
{
  return ndn::dell::dataservice::main(argc, argv);
}
