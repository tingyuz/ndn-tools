/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 DELL
 *
 * @author Tingyu Zeng 
 */

#ifndef NDN_DATASERVICE_HPP
#define NDN_DATASERVICE_HPP

#include "core/common.hpp"

#include <ndn-cxx/util/scheduler.hpp>

namespace ndn {
namespace dell{
namespace dataservice {

/**
 * \brief options for NdnDataservice
 */
struct ServiceOptions
{
  // Data construction options
  Name name;
  time::milliseconds freshnessPeriod = DEFAULT_FRESHNESS_PERIOD;    

  // program behavior options  
  optional<time::milliseconds> timeout;
};

class NdnDataservice : boost::noncopyable
{
public:
  NdnDataservice(Face& face, KeyChain& keyChain, const ServiceOptions& options);

  enum class Result {
    DATA_SENT = 0,
    UNKNOWN = 1,
    // 2 is reserved for "malformed command line"
    TIMEOUT = 3,
    // 4 is reserved for "nack"
    PREFIX_REG_FAIL = 5,
  };

  /**
   * @return the result of execution
   */
  Result
  getResult() const
  {
    return m_result;
  }

  void
  start();

private:
  shared_ptr<Data>
  createData(std::string s) const;  

  void
  sendData(const Data& data);

  void
  onInterest(const Interest& interest, const Data& data);

  void
  onRegSuccess();

  void
  onRegFailure(const std::string& reason);

private:
  const ServiceOptions m_options;
  Face& m_face;
  KeyChain& m_keyChain;    
  Result m_result = Result::UNKNOWN;
};

} // namespace dataservice
} // namespace dell
} // namespace ndn

#endif // NDN_DATASERVICE_HPP
