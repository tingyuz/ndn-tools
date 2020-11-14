/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020
 * @author Tingyu Zeng
 */

#include "ndndataclient.hpp"
#include "core/version.hpp"

#include <ndn-cxx/util/io.hpp>

#include <cerrno>
#include <cstring>
#include <fstream>

namespace ndn {
namespace dell{
namespace dataservice {

namespace po = boost::program_options;

static void
usage(std::ostream& os, const std::string& program, const po::options_description& options)
{
  os << "Usage: " << program << " /name\n"
     << "\n"
     << "Fetch one data item matching the specified name and write it to the standard output.\n"
     << options;
}

static int
main(int argc, char* argv[])
{
  std::string progName(argv[0]);
  ClientOptions options;

  po::options_description genericOptDesc("Generic options");
  genericOptDesc.add_options()
    ("help,h",    "print help and exit")    
  ;

  po::options_description visibleOptDesc;
  visibleOptDesc.add(genericOptDesc);
  
   po::options_description hiddenOptDesc;
  hiddenOptDesc.add_options()
    ("name", po::value<std::string>(), "Interest name");

  po::options_description optDesc;
  optDesc.add(visibleOptDesc).add(hiddenOptDesc);

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

  if (vm.count("name") == 0) {
    std::cerr << "ERROR: missing name\n\n";
    usage(std::cerr, progName, visibleOptDesc);
    return 2;
  }

  try {
    options.name = vm["name"].as<std::string>();
  }
  catch (const Name::Error& e) {
    std::cerr << "ERROR: invalid name: " << e.what() << std::endl;
    return 2;
  }

  try {
    Face face;
    NdnDataclient program(face, options);

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
