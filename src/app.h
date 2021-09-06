#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QFont>
#include <QIcon>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStringLiteral>
#include <QTranslator>
#include <QUrl>

#include "models/dbtools.h"
#include "models/envtools.h"
#include "view_models/logview.h"
#include "view_models/configtools.h"
#include "view_models/grouplist.h"
#include "view_models/groupmodel.h"
#include "view_models/nodelist.h"
#include "view_models/nodemodel.h"
#include "view_models/nodeformmodel.h"
#include "view_models/vmessformmodel.h"
#include "view_models/trojanformmodel.h"
#include "view_models/shadowsocksformmodel.h"
#include "view_models/rawoutboundformmodel.h"
#include "view_models/urlschemeformmodel.h"

namespace across {
#define APP_NAME "ACross"

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);

    ~Application();

    int run();

    void setupTranslator(const QString& lang = "current");

    void setupQmlRegisterTypes();

    void setupQmlContextProperties();

  private:
    const QString m_application_name = APP_NAME;
    QMap<int, QString> m_args;
    int m_thread_nums = 1;
    int m_queue_size = 8192;

    across::EnvTools acrossEnvs;
    across::LogView acrossLog;
    across::setting::ConfigTools acrossConfig;
    across::DBTools acrossDB;
    across::core::CoreTools acrossCore;
    across::network::CURLTools acrossCurl;
    across::NodeList acrossNodes;
    across::GroupList acrossGroups;

    QApplication* p_app = nullptr;
    QTranslator m_translator;
    QQmlApplicationEngine m_engine;
    std::shared_ptr<LogTools> p_logger;
    std::shared_ptr<spdlog::details::thread_pool> m_thread_pool;
};
}

#endif // APP_H
