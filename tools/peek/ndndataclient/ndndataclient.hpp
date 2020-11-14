/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2020 DELL
 * @author Tingyu Zeng
 */

#ifndef NDN_DATACLIENT_HPP
#define NDN_DATACLIENT_HPP

#include "core/common.hpp"

#include <ndn-cxx/link.hpp>
#include <ndn-cxx/util/scheduler.hpp>

namespace ndn {
namespace dell{
namespace dataservice {

/**
 * @brief options for NdnDataclient
 */
struct ClientOptions
{
  // Interest construction options
  Name name;
  bool canBePrefix = false;
  bool mustBeFresh = false;  

  // program behavior options
  //bool isVerbose = false;
  bool wantPayloadOnly = false;
  //optional<time::milliseconds> timeout;
};

class NdnDataclient : boost::noncopyable
{
public:
  NdnDataclient(Face& face, const ClientOptions& options);

  enum class Result {
    DATA = 0,
    UNKNOWN = 1,
    // 2 is reserved for "malformed command line"
    TIMEOUT = 3,
    NACK = 4,
  };

  /**
   * @return the result of NdnPeek execution
   */
  Result
  getResult() const
  {
    return m_result;
  }

  /**
   * @brief express the Interest
   * @note The caller must invoke face.processEvents() afterwards
   */
  void
  start();

private:
  Interest
  createInterest() const;

  /**
   * @brief called when a Data packet is received
   */
  void
  onData(const Data& data);

  /**
   * @brief called when a Nack packet is received
   */
  void
  onNack(const lp::Nack& nack);

  /**
   * @brief called when the Interest times out
   */
  void
  onTimeout();

private:
  const ClientOptions m_options;
  Face& m_face;
  //Scheduler m_scheduler;
  //time::steady_clock::TimePoint m_sendTime;
  ScopedPendingInterestHandle m_pendingInterest;
  //scheduler::ScopedEventId m_timeoutEvent;
  Result m_result = Result::UNKNOWN;
};

} // namespace dataservice
} // namespace dell
} // namespace ndn

#endif // NDN_DATACLIENT_HPP
