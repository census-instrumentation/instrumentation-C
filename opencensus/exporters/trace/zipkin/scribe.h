/**
 * Autogenerated by Thrift Compiler (1.0.0-dev)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef Scribe_H
#define Scribe_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "scribe_types.h"

namespace opencensus {
namespace scribe {

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4250)  // inheriting methods via dominance
#endif

class ScribeIf {
 public:
  virtual ~ScribeIf() {}
  virtual ResultCode::type Log(const std::vector<LogEntry>& messages) = 0;
};

class ScribeIfFactory {
 public:
  typedef ScribeIf Handler;

  virtual ~ScribeIfFactory() {}

  virtual ScribeIf* getHandler(
      const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(ScribeIf* /* handler */) = 0;
};

class ScribeIfSingletonFactory : virtual public ScribeIfFactory {
 public:
  ScribeIfSingletonFactory(
      const ::apache::thrift::stdcxx::shared_ptr<ScribeIf>& iface)
      : iface_(iface) {}
  virtual ~ScribeIfSingletonFactory() {}

  virtual ScribeIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(ScribeIf* /* handler */) {}

 protected:
  ::apache::thrift::stdcxx::shared_ptr<ScribeIf> iface_;
};

class ScribeNull : virtual public ScribeIf {
 public:
  virtual ~ScribeNull() {}
  ResultCode::type Log(const std::vector<LogEntry>& /* messages */) {
    ResultCode::type _return = (ResultCode::type)0;
    return _return;
  }
};

typedef struct _Scribe_Log_args__isset {
  _Scribe_Log_args__isset() : messages(false) {}
  bool messages : 1;
} _Scribe_Log_args__isset;

class Scribe_Log_args {
 public:
  Scribe_Log_args(const Scribe_Log_args&);
  Scribe_Log_args& operator=(const Scribe_Log_args&);
  Scribe_Log_args() {}

  virtual ~Scribe_Log_args() throw();
  std::vector<LogEntry> messages;

  _Scribe_Log_args__isset __isset;

  void __set_messages(const std::vector<LogEntry>& val);

  bool operator==(const Scribe_Log_args& rhs) const {
    if (!(messages == rhs.messages)) return false;
    return true;
  }
  bool operator!=(const Scribe_Log_args& rhs) const { return !(*this == rhs); }

  bool operator<(const Scribe_Log_args&) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;
};

class Scribe_Log_pargs {
 public:
  virtual ~Scribe_Log_pargs() throw();
  const std::vector<LogEntry>* messages;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;
};

typedef struct _Scribe_Log_result__isset {
  _Scribe_Log_result__isset() : success(false) {}
  bool success : 1;
} _Scribe_Log_result__isset;

class Scribe_Log_result {
 public:
  Scribe_Log_result(const Scribe_Log_result&);
  Scribe_Log_result& operator=(const Scribe_Log_result&);
  Scribe_Log_result() : success((ResultCode::type)0) {}

  virtual ~Scribe_Log_result() throw();
  ResultCode::type success;

  _Scribe_Log_result__isset __isset;

  void __set_success(const ResultCode::type val);

  bool operator==(const Scribe_Log_result& rhs) const {
    if (!(success == rhs.success)) return false;
    return true;
  }
  bool operator!=(const Scribe_Log_result& rhs) const {
    return !(*this == rhs);
  }

  bool operator<(const Scribe_Log_result&) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;
};

typedef struct _Scribe_Log_presult__isset {
  _Scribe_Log_presult__isset() : success(false) {}
  bool success : 1;
} _Scribe_Log_presult__isset;

class Scribe_Log_presult {
 public:
  virtual ~Scribe_Log_presult() throw();
  ResultCode::type* success;

  _Scribe_Log_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
};

class ScribeClient : virtual public ScribeIf {
 public:
  ScribeClient(
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          prot) {
    setProtocol(prot);
  }
  ScribeClient(
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          iprot,
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          oprot) {
    setProtocol(iprot, oprot);
  }

 private:
  void setProtocol(
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          prot) {
    setProtocol(prot, prot);
  }
  void setProtocol(
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          iprot,
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          oprot) {
    piprot_ = iprot;
    poprot_ = oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }

 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
  getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
  getOutputProtocol() {
    return poprot_;
  }
  ResultCode::type Log(const std::vector<LogEntry>& messages);
  void send_Log(const std::vector<LogEntry>& messages);
  ResultCode::type recv_Log();

 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
      piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
      poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class ScribeProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::apache::thrift::stdcxx::shared_ptr<ScribeIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot,
                            ::apache::thrift::protocol::TProtocol* oprot,
                            const std::string& fname, int32_t seqid,
                            void* callContext);

 private:
  typedef void (ScribeProcessor::*ProcessFunction)(
      int32_t, ::apache::thrift::protocol::TProtocol*,
      ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_Log(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot,
                   ::apache::thrift::protocol::TProtocol* oprot,
                   void* callContext);

 public:
  ScribeProcessor(::apache::thrift::stdcxx::shared_ptr<ScribeIf> iface)
      : iface_(iface) {
    processMap_["Log"] = &ScribeProcessor::process_Log;
  }

  virtual ~ScribeProcessor() {}
};

class ScribeProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  ScribeProcessorFactory(
      const ::apache::thrift::stdcxx::shared_ptr<ScribeIfFactory>&
          handlerFactory)
      : handlerFactory_(handlerFactory) {}

  ::apache::thrift::stdcxx::shared_ptr< ::apache::thrift::TProcessor>
  getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::apache::thrift::stdcxx::shared_ptr<ScribeIfFactory> handlerFactory_;
};

class ScribeMultiface : virtual public ScribeIf {
 public:
  ScribeMultiface(
      std::vector<apache::thrift::stdcxx::shared_ptr<ScribeIf> >& ifaces)
      : ifaces_(ifaces) {}
  virtual ~ScribeMultiface() {}

 protected:
  std::vector<apache::thrift::stdcxx::shared_ptr<ScribeIf> > ifaces_;
  ScribeMultiface() {}
  void add(::apache::thrift::stdcxx::shared_ptr<ScribeIf> iface) {
    ifaces_.push_back(iface);
  }

 public:
  ResultCode::type Log(const std::vector<LogEntry>& messages) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->Log(messages);
    }
    return ifaces_[i]->Log(messages);
  }
};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class ScribeConcurrentClient : virtual public ScribeIf {
 public:
  ScribeConcurrentClient(
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          prot) {
    setProtocol(prot);
  }
  ScribeConcurrentClient(
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          iprot,
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          oprot) {
    setProtocol(iprot, oprot);
  }

 private:
  void setProtocol(
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          prot) {
    setProtocol(prot, prot);
  }
  void setProtocol(
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          iprot,
      apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
          oprot) {
    piprot_ = iprot;
    poprot_ = oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }

 public:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
  getInputProtocol() {
    return piprot_;
  }
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
  getOutputProtocol() {
    return poprot_;
  }
  ResultCode::type Log(const std::vector<LogEntry>& messages);
  int32_t send_Log(const std::vector<LogEntry>& messages);
  ResultCode::type recv_Log(const int32_t seqid);

 protected:
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
      piprot_;
  apache::thrift::stdcxx::shared_ptr< ::apache::thrift::protocol::TProtocol>
      poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

}  // namespace scribe
}  // namespace opencensus

#endif
