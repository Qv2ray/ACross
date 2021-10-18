#ifndef SERIALIZETOOLS_H
#define SERIALIZETOOLS_H

#include "../models/dbtools.h"
#include "jsontools.h"

#include "magic_enum.hpp"
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QUrlQuery>
#include <optional>

#include "v2ray_config.grpc.pb.h"

namespace across {
namespace config {
class SerializeTools
{
public:
  SerializeTools();

  // Shadowsocks
  static std::optional<SIP008> sip008Parser(const std::string& data);

  static std::optional<SIP008::Server> sip002Decode(const QUrl& url);
  static std::optional<QUrl> sip002Encode(const SIP008::Server& sip008);

  // Trojan
  static std::optional<URLMetaObject> trojanDecode(const QUrl& url);
  static std::optional<QUrl> trojanEncode(const URLMetaObject& meta);

  // VMESS
  static std::optional<URLMetaObject> vmessBase64Decode(
    const std::string& data);
  static std::optional<QUrl> vmessBase64Encode(const URLMetaObject& meta);

  // Decode From URL
  static bool decodeOutboundFromURL(NodeInfo& node, const QString& raw_url);
  static bool setShadowsocksOutboundFromURL(NodeInfo& node, const QUrl& url);
  static bool setVMessOutboundFromBase64(NodeInfo& node,
                                         const std::string& data);
  static bool setTrojanOutboundFromURL(NodeInfo& node, const QUrl& url);
};
}
}

#endif // SERIALIZETOOLS_H
