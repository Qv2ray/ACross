#include "nodelist.h"

using namespace across;
using namespace across::core;
using namespace across::config;
using namespace across::setting;

NodeList::NodeList(QObject* parent)
  : QObject(parent)
{}

void
NodeList::init(std::shared_ptr<spdlog::details::thread_pool> thread_pool,
               DBTools& db,
               ConfigTools& config_tools,
               CoreTools& core_tools)
{
  p_logger = std::make_shared<LogTools>(thread_pool, "node_list");

  p_json = std::make_shared<JsonTools>();

  p_db = &db;

  p_config = &config_tools;

  p_core = &core_tools;

  reloadItems();
}

QVector<NodeInfo>
NodeList::items()
{
  return m_items;
}

void
NodeList::reloadItems()
{
  emit preItemsReset();

  m_items.clear();
  m_all_items.clear();

  for (auto& item : p_db->listAllNodes()) {
    m_all_items.insert(item.first, item.second);
  }

  for (auto& item : m_all_items.first().nodes) {
    m_items.append(item);
  }

  emit postItemsReset();
}

void
NodeList::appendNode(NodeInfo node)
{
  node.group_id = currentGroupID();

  auto groups = p_db->listAllGroupsInfo();

  for (auto& item : groups) {
    if (item.id == node.group_id) {
      node.group = item.name;
      break;
    }
  }

  p_db->insert(node);

  reloadItems();
}

void
NodeList::removeCurrentNode(int id)
{
  do {
    for (auto& item : m_items) {
      if (item.id == id) {
        auto result = p_db->removeItemFromID(item.group.toStdString(), item.id);
        if (result == SQLITE_OK) {
          reloadItems();
        }
        break;
      }
    }
  } while (false);
}

int
NodeList::currentNodeID()
{
  auto temp_items = m_all_items.find(m_group_id).value();

  return temp_items.current_id;
}

int
NodeList::currentNodeIndex()
{
  auto temp_items = m_all_items.find(m_group_id).value();

  return temp_items.current_index;
}

int
NodeList::currentGroupID()
{
  return m_group_id;
}

int
NodeList::displayGroupID()
{
  return m_display_group_id;
}

QString
NodeList::currentNodeName()
{
  return m_current_node.name;
}

QString
NodeList::currentNodeGroup()
{
  return m_current_node.group;
}

QString
NodeList::currentNodeProtocol()
{
  return magic_enum::enum_name(m_current_node.protocol).data();
}

QString
NodeList::currentNodeAddress()
{
  return m_current_node.address;
}

int
NodeList::currentNodePort()
{
  return m_current_node.port;
}

void
NodeList::setCurrentGroupID(int group_id)
{
  if (m_group_id == group_id) {
    return;
  }

  m_group_id = group_id;

  emit currentGroupIDChanged();
}

void
NodeList::setDisplayGroupID(int group_id)
{
  do {
    if (group_id <= 0 || group_id == m_display_group_id) {
      break;
    }

    m_display_group_id = group_id;

    emit preItemsReset();

    m_items.clear();
    for (auto& item : m_all_items.find(m_display_group_id).value().nodes) {
      m_items.append(item);
    }

    emit postItemsReset();

    emit displayGroupIDChanged();

  } while (false);
}

void
NodeList::setCurrentNode(int id, int index)
{
  do {
    auto items = m_all_items.find(m_display_group_id);
    if (items != m_all_items.end()) {
      items->current_id = id;
      items->current_index = index;

      emit currentNodeIDChanged();
      emit currentNodeIndexChanged();
    } else {
      p_logger->error("Failed to get group: {}", m_group_id);
      break;
    }

    auto iter = std::find_if(
      items->nodes.begin(), items->nodes.end(), [&](NodeInfo item) {
        if (item.id == id) {
          return true;
        } else {
          return false;
        }
      });

    if (iter != items->nodes.end()) {
      m_current_node = *iter;

      setCurrentGroupID(m_current_node.group_id);

      emit currentNodeNameChanged();
      emit currentNodeGroupChanged();
      emit currentNodeProtocolChanged();
      emit currentNodeAddressChanged();
      emit currentNodePortChanged();
    } else {
      break;
    }

    if (p_config == nullptr) {
      p_logger->error("Failed to load config");
      break;
    }

    if (p_json == nullptr) {
      p_logger->error("Failed to load json tools");
    };

    Json::Value root;

#ifdef QT_DEBUG
    {
      root["log"]["loglevel"] = "debug";
    }
#endif

    {
      InboundSettings inbound_settings = p_config->getInboundConfig();
      inbound_settings.setObject(root);
    }

    {
      p_json->setData(m_current_node.raw.toStdString());

      OutboundObjects outbound_objects;
      outbound_objects.appendOutboundObject(p_json->getRoot());
      outbound_objects.setObject(root);
    }

#ifdef QT_DEBUG
    std::ofstream file("generation_test.json");
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &file);
    file.close();
#endif

    p_core->setConfig(root.toStyledString());
    p_core->run();
  } while (false);
}
