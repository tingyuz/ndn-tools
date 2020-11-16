/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 
 * @author Tingyu Zeng
 */

#include "ndndataclient.hpp"
#include "served/served.hpp"

namespace ndn {
namespace dell{
namespace dataservice {

NdnDataclient::NdnDataclient(Face& face, const ClientOptions& options)
  : m_options(options)
  , m_face(face)  
{
}

void
NdnDataclient::start()
{
  m_pendingInterest = m_face.expressInterest(createInterest(),
                                             [this] (auto&&, const auto& data) { this->onData(data); },
                                             [this] (auto&&, const auto& nack) { this->onNack(nack); },
                                             [this] (auto&&) { this->onTimeout(); });
}

Interest
NdnDataclient::createInterest() const
{
  Interest interest(m_options.name);
  interest.setCanBePrefix(m_options.canBePrefix);
  interest.setMustBeFresh(m_options.mustBeFresh);

  return interest;
}

void
NdnDataclient::onData(const Data& data)
{
  m_result = Result::DATA;
  //m_timeoutEvent.cancel();
  const Block& block = data.getContent();
  m_location = reinterpret_cast<const char*>(block.value()), block.value_size();
  //std::cout.write(m_location);
}

void
NdnDataclient::onNack(const lp::Nack& nack)
{
  m_result = Result::NACK;
  //m_timeoutEvent.cancel();

  lp::NackHeader header = nack.getHeader();

  if (m_options.wantPayloadOnly) {
    std::cout << header.getReason() << std::endl;
  }
  else {
    const Block& block = header.wireEncode();
    std::cout.write(reinterpret_cast<const char*>(block.wire()), block.size());
  }
}

void
NdnDataclient::onTimeout()
{
  m_result = Result::TIMEOUT;
  //m_timeoutEvent.cancel();

  //if (m_options.isVerbose) {
    std::cerr << "TIMEOUT" << std::endl;
  //}
  served::multiplexer mux;
    mux.handle("/api/v1/greeting")
        .get([&](served::response &res, const served::request &req) {
            std::string name = req.query["name"];
            res.set_header("content-type", "application/json");
            res << "{ \"content\": \"Hello, " << ((name.length() > 0) ? name : "world") << "!\" }\n";
        });

}

} // namespace dataservice
} // namespace dell
} // namespace ndn
