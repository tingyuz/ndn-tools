/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 DELL
 *
 * @author Tingyu Zeng
 */

#include "ndndataservice.hpp"

#include <ndn-cxx/encoding/buffer-stream.hpp>

namespace ndn {
namespace dell{
namespace dataservice {

NdnDataservice::NdnDataservice(Face& face, KeyChain& keyChain, const ServiceOptions& options)
  : m_options(options)
  , m_face(face)
  , m_keyChain(keyChain)  
{
}

void
NdnDataservice::start()
{
  auto data1 = createData("/ndn/dell/ep1");
  auto data2 = createData("/ndn/dell/ep2");
     
  m_face.setInterestFilter(Name("/ndn/dell/ep1"),
    [this, data1] (auto&&, const auto& interest) { this->onInterest(interest, *data1); },    
    [this] (auto&&) { this->onRegSuccess(); },
    [this] (auto&&, const auto& reason) { this->onRegFailure(reason); });

  m_face.setInterestFilter(Name("/ndn/dell/ep2"),    
    [this, data2] (auto&&, const auto& interest) { this->onInterest(interest, *data2); },
    [this] (auto&&) { this->onRegSuccess(); }, 
    [this] (auto&&, const auto& reason) { this->onRegFailure(reason); });
  
}

shared_ptr<Data>
NdnDataservice::createData(std::string s) const
{
  auto data = make_shared<Data>(s);  
  data->setFreshnessPeriod(m_options.freshnessPeriod);

  std::stringstream ss;
  ss.str("nfs://mnt/data/" + s);
  OBufferStream obuf;
  obuf << ss.rdbuf();
  data->setContent(obuf.buf());

  m_keyChain.sign(*data);

  return data;
}

void
NdnDataservice::sendData(const Data& data)
{
  m_face.put(data);
  m_result = Result::DATA_SENT;  
}

void
NdnDataservice::onInterest(const Interest& interest, const Data& data)
{
  if (interest.matchesData(data)) {    
    sendData(data);
  }
  else {
    std::cerr << "Interest cannot be satisfied" << std::endl;
  }
}

void
NdnDataservice::onRegSuccess()
{}  

void
NdnDataservice::onRegFailure(const std::string& reason)
{}

} // namespace dataservice
} // namespace dell
} // namespace ndn
