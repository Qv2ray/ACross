#include "app.h"

using namespace across;
using namespace across::core;
using namespace across::utils;

Application::Application(int& argc, char** argv)
  : QApplication(argc, argv)
{

  spdlog::init_thread_pool(m_queue_size, m_thread_nums);

  registerModels();
  setRootContext();
  setTranslator(acrossConfig.currentLanguage());

  // dynamic change display language
  connect(&acrossConfig,
          &across::setting::ConfigTools::currentLanguageChanged,
          this,
          [&](QString lang) { setTranslator(lang); });
}

Application::~Application() {}

int
Application::run()
{
  return this->exec();
}

void
Application::setRootContext()
{
  const QUrl url(QStringLiteral("qrc:/src/views/main.qml"));
  QObject::connect(
    &m_engine,
    &QQmlApplicationEngine::objectCreated,
    this,
    [url](QObject* obj, const QUrl& objUrl) {
      if (!obj && url == objUrl)
        QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossAppLog"),
                                             &acrossAppLog);
  //  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossCoreLog"),
  //                                             &acrossCoreLog);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossConfig"),
                                             &acrossConfig);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossCore"),
                                             &acrossCore);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossNodes"),
                                             &acrossNodes);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossGroups"),
                                             &acrossGroups);
  m_engine.rootContext()->setContextProperty(QStringLiteral("acrossTray"),
                                             &acrossTray);
  m_engine.load(url);

  acrossConfig.init(acrossAppLog);
  acrossDB.init(acrossAppLog, acrossConfig);
  acrossCore.init(acrossAppLog, acrossConfig);
  acrossNodes.init(acrossAppLog, acrossConfig, acrossCore, acrossDB);
  acrossGroups.init(
    acrossAppLog, acrossConfig, acrossDB, acrossNodes, acrossCurl);
  acrossTray.init(acrossConfig, acrossCore);
}

void
Application::setTranslator(const QString& lang)
{
  if (!m_translator.isEmpty()) {
    this->removeTranslator(&m_translator);
  }

  const QString baseName = m_app_name.toLower() + "_" + QLocale(lang).name();
  auto file_path = ":/" + baseName + ".qm";

  if (QFile(file_path).exists()) {
    if (m_translator.load(file_path)) {
      this->installTranslator(&m_translator);
    }
  }

  m_engine.retranslate();
  acrossTray.retranslate();
}

void
Application::registerModels()
{
  qmlRegisterType<across::GroupModel>(APP_NAME, 1, 0, "GroupModel");
  qmlRegisterType<across::NodeModel>(APP_NAME, 1, 0, "NodeModel");
  qmlRegisterType<across::NodeFormModel>(APP_NAME, 1, 0, "NodeFormModel");
  qmlRegisterType<across::VMessFormModel>(APP_NAME, 1, 0, "VMessFormModel");
  qmlRegisterType<across::TrojanFormModel>(APP_NAME, 1, 0, "TrojanFormModel");
  qmlRegisterType<across::ShadowsocksFormModel>(
    APP_NAME, 1, 0, "ShadowsocksFormModel");
  qmlRegisterType<across::RawOutboundFormModel>(
    APP_NAME, 1, 0, "RawOutboundFormModel");
  qmlRegisterType<across::URLSchemeFormModel>(
    APP_NAME, 1, 0, "URLSchemeFormModel");
}
