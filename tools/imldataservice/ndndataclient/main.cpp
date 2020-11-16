/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 DELL
 * @author Tingyu Zeng
 */

#include "ndndataclient.hpp"
#include "core/version.hpp"

#include <ndn-cxx/util/io.hpp>

#include <served/served.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>
#include <future>


namespace ndn {
namespace dell{
namespace dataservice {

static int 
main(int argc, char const* argv[]) {
	// Create a multiplexer for handling requests
	served::multiplexer mux;

	// simple http hander
	mux.handle("/interest")
		.post([](served::response & res, const served::request & req) {
      
      std::stringstream ss;
      //ss << "{ \"hash\": \"4445SDFRE4HDH\", \"mode\": \"sync\", \"name\": \"/ndn/dell/ep1\", \"callback\": \"http://localhost:8008/data/4445SDFRE4HDH\"}"; 
      ss << req.body();
      boost::property_tree::ptree pt;
      boost::property_tree::read_json(ss, pt);
      std::string name = pt.get<std::string>("name");
      
			Face face;
			ClientOptions options={
			  std::string("/ndn/dell/" + name),
        false,
			  false,
			  false,
			};

			NdnDataclient program(face, options);

			program.start();
		  face.processEvents();

      if (program.getLocation().empty() || 
	  		program.getLocation().find('/') == std::string::npos)
		{
        	res.set_status(404);
			res << "{ \"error\": \"failed to locate requested resource.\"}";
		}
      else 
        {
          res.set_status(200);
          res << program.getLocation();
        }			  
	});

  mux.handle("/locality")
		.post([](served::response & res, const served::request & req) {

      std::stringstream ss;
      ss << req.body();
      boost::property_tree::ptree pt;
      boost::property_tree::read_json(ss, pt);
      std::string name = pt.get<std::string>("name");

			Face face;
			ClientOptions options={
			  std::string("/ndn/dell/" + name),
        		false,
			 	false,
			  	false,
			};      

			NdnDataclient program(face, options);

			program.start();
		  	face.processEvents();

			res << program.getLocation();
	});  

	// Create the server and run with 10 handler threads.
	served::net::server server("127.0.0.1", "8080", mux);
	server.run(10);

	return (EXIT_SUCCESS);
}
} // namespace dataservice
} // namespace dell
} // namespace ndn

int
main(int argc, char const* argv[])
{
  return ndn::dell::dataservice::main(argc, argv);
}

