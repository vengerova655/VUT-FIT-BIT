#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(MqttController *controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_controller(controller)
{
    Q_ASSERT(controller!=nullptr);
    ui->setupUi(this);

    setupConnections();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createEntry()
{
     const int QOS = 1;
     const bool NO_LOCAL = true;


//    auto tree = ui->treeWidget;
//    ui->stackedWidget->setCurrentWidget(ui->menutopic);
    QString topicinput = ui->m_topic->text();
    QString server = ui->m_server->text();
    QString clientid = ui->m_client->text();
    int num = (ui->m_num->text()).toInt();

    /*QStringList pieces = topicinput.split( "/" );*///topic,/home,/jozo


    const std::string TOPIC = topicinput.toStdString();
    const std::string SERVER_ADDRESS = server.toStdString();
    const std::string CLIENT_ID = clientid.toStdString();


    // LWT message is broadcast to other users if out connection is lost
    auto lwt = mqtt::message(TOPIC, "<<< was disconnected>>>", QOS, false);

    mqtt::properties connectProperties{
               {mqtt::property::SESSION_EXPIRY_INTERVAL, 604800}
       };

       auto connOpts = mqtt::connect_options_builder()
               .mqtt_version(MQTTVERSION_5)
               .properties(connectProperties)
               .clean_start(true)
               .will(std::move(lwt))
               .keep_alive_interval(std::chrono::seconds(20))
               .finalize();

       mqtt::create_options createOpts;
       createOpts.set_mqtt_verison(5);

       mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID,
                                  createOpts);

       // Set a callback for connection lost.
           // This just exits the app.

       cli.set_connection_lost_handler([](const std::string&) {
               std::cout << "*** Connection Lost  ***" << std::endl;
               exit(2);
           });

       // Set the callback for incoming messages

          cli.set_message_callback([this](mqtt::const_message_ptr msg) {

              std::cout <<"CALLBACK: " <<msg->get_payload_str() << std::endl;
          });

          // We publish and subscribe to one topic,
         // so a 'topic' object is helpful.

         mqtt::topic topic { cli, TOPIC, QOS };


         try {
                cli.start_consuming();
                 std::cout << "Connecting to the chat server at '" << SERVER_ADDRESS
                           << "'..." << std::flush;
                 auto tok = cli.connect(connOpts);
                 tok->wait();



                 // Subscribe to the topic using "no local" so that
                 // we don't get own messages sent back to us

                 std::cout << "Ok\nSubscribing..." << std::flush;
                 auto subOpts = mqtt::subscribe_options(NO_LOCAL);
         //        topic.subscribe(subOpts)->wait();
                 cli.subscribe(TOPIC,QOS,subOpts);

                 while (true) {
                     auto msg = cli.consume_message();
                     if (!msg) break;
                     auto msgtopic = msg->get_topic();
                     QString qmsgtopic = QString::fromStdString(msgtopic);

                     QStringList pieces = qmsgtopic.split( "/" );
                     QTreeWidgetItem*root;
                     //topic/
                     for(int i=0;i<pieces.length();i++){
                         if(QString::compare(pieces.at(i),"")!=0){
                          if(i==0){
                              QList<QTreeWidgetItem*> items = ui->treeWidget->findItems(pieces.at(i),Qt::MatchExactly, 0);
                              if (items.count() == 0) {   // not found, create new entry
                                  root = new QTreeWidgetItem(ui->treeWidget);
                                  root->setText(0,pieces.at(0));
                                  ui->treeWidget->addTopLevelItem(root);
                              }
                              else {
                                  root = items.at(0);
                              }
                          }
                          else{
                             int j=0;
                             while(j < root->childCount() && (QString::compare((root->child(j))->text(0),pieces.at(i))!=0)){
                                 j++;
                             }
                             if(j<root->childCount()){
                                 root= root->child(j);
                             }
                             else{
                                 QTreeWidgetItem *child = new QTreeWidgetItem();
                                 child->setText(0,pieces.at(i));
                                 root->addChild(child);
                                 root=child;
                             }
                          }
                        }
                     }
                     std::cout << msg->get_payload_str() << std::endl;
                     std::cout << msg->get_topic() << ": " << msg->to_string() << std::endl;
                 }

                 std::cout << "Ok" << std::endl;
             }
         catch (const mqtt::exception& exc) {
                std::cerr << "\nERROR: Unable to connect. "
                          << exc.what() << std::endl;
                exit(1);
            }


         try {
                std::cout << "Disconnecting from the chat server..." << std::flush;
                cli.disconnect()->wait();
                std::cout << "OK" << std::endl;
            }
            catch (const mqtt::exception& exc) {
                std::cerr << exc.what() << std::endl;
                exit(1);
            }



//    connectSubscribe();

//    auto entry = m_controller->createEntry(&topic);
//    if(entry){

//        ui->listWidget->addItem(entry->Msq_name());
//       auto listItem = ui->listWidget->item(ui->listWidget->count()-1);
//        m_entryMap.insert(listItem, entry);
//    }

}

void MainWindow::deleteEntry()
{
    auto listItem = ui->listWidget->currentItem();
    if (listItem){
        auto entry = m_entryMap.value(listItem);
        if( entry ){
            if(m_controller->deleteEnntry(entry)){
               m_entryMap .remove(listItem);
                delete listItem;
            }
        }
    }
}

void MainWindow::editEntry()
{
//    auto widgetItem = ui->treeWidget->currentItem();
//    if(widgetItem){

//    }
    auto listItem = ui->listWidget->currentItem();
    if (listItem){
        auto entry = m_entryMap.value(listItem);
        if( entry ){
            ui->stackedWidget->setCurrentWidget(ui->menutopic);
            ui->menuEntries->setEnabled(false);
            resetEntry();
        }
    }
}

void MainWindow::saveEntry()
{
    auto listItem = ui->listWidget->currentItem();
    if (listItem){
        auto entry = m_entryMap.value(listItem);
        if( entry ){
            entry->setMsq_name(ui->m_topic->text());
            listItem->setText(entry->Msq_name());
            discardEntry();
        }
    }
}

void MainWindow::discardEntry()
{
    ui->stackedWidget->setCurrentWidget(ui->topicsubsribe);

}

void MainWindow::resetEntry()
{
    auto listItem = ui->listWidget->currentItem();
    if (listItem){
        auto entry = m_entryMap.value(listItem);
        if( entry ){
            ui->serveradress->setText(entry->Msq_name());
            ui->topic->setText(entry->Msq_topic());
        }
    }
}

void MainWindow::showTree()
{
    ui->stackedWidget->setCurrentWidget(ui->topicsubsribe);
}


void MainWindow::showList()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::subscribe()
{
    ui->stackedWidget->setCurrentWidget(ui->topicsubsribe);
}

void MainWindow::addsubscribe()
{
    ui->stackedWidget->setCurrentWidget(ui->menutopic);
}

void MainWindow::setupConnections()
{
//    connect(ui->actionAdd,SIGNAL(triggered(bool)),this, SLOT(subcribe()));
    connect(ui->actionDashboard,&QAction::triggered,this,&MainWindow::showTree);
    connect(ui->actionshowList,&QAction::triggered,this,&MainWindow::showList);
     connect(ui->actionaddSubscription,&QAction::triggered,this,&MainWindow::addsubscribe);
    connect(ui->actionAdd,&QAction::triggered,this,&MainWindow::subscribe);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok),&QPushButton::clicked, this, &MainWindow::createEntry);
}

//QTreeWidgetItem MainWindow::childroot(QTreeWidgetItem *root, QTreeWidgetItem *child)
//{
//    if(root->parent()==nullptr){
//        QList<QTreeWidgetItem*> items = ui->treeWidget->findItems(pieces.at(0),Qt::MatchExactly, 0);
//        if (items.count() == 0) {   // not found, create new entry
//            root = new QTreeWidgetItem(ui->treeWidget);
//            root->setText(0,pieces.at(0));
//            ui->treeWidget->addTopLevelItem(root);
//        }
//    }
//    else{
//         QList<QTreeWidgetItem*> items = ui->treeWidget->findItems(pieces.at(i),Qt::MatchExactly|Qt::MatchRecursive, 0);
//         if(items.count() > 0 ){
////                QList<QTreeWidgetItem*> items2 = ui->treeWidget->findItems(pieces.at(i-1),Qt::MatchExactly|Qt::MatchRecursive, 0);
//           root = items.at(0);
//         }
//         if (items.count() == 0) { // not found, create new entry
//             auto child  = new QTreeWidgetItem();
//             child->setText(0,pieces.at(i));
//             root->addChild(child);
//         }
//    }
///////////////////////////////////////////////////////
//for(int i=0;i<pieces.length();i++){
//    if(i==0){
//        QList<QTreeWidgetItem*> items = ui->treeWidget->findItems(pieces.at(i),Qt::MatchExactly, 0);
//        if (items.count() == 0) {   // not found, create new entry
//            root = new QTreeWidgetItem(ui->treeWidget);
//            root->setText(0,pieces.at(0));
//            ui->treeWidget->addTopLevelItem(root);
//        }
//        else {
//            root = items.at(0);
//        }
//    }
//    else{
//         QList<QTreeWidgetItem*> items = ui->treeWidget->findItems(pieces.at(i),Qt::MatchExactly|Qt::MatchRecursive, 0);

//         if(items.count() > 0){
//             for(int j=0;j<items.count();j++ ){
//                 QTreeWidgetItem* prevtreeItem = (ui->treeWidget->itemAbove(items.at(0)));
//                 if(QString::compare(prevtreeItem->text(0),pieces.at(i-1))==0){
//                     root = items.at(0);
//                 }
//                 else{
//                     QTreeWidgetItem * child  = new QTreeWidgetItem();
//                     child->setText(0,pieces.at(i));
//                     root->addChild(child);
//                       root=child;
//                 }
//             }
////                QList<QTreeWidgetItem*> items2 = ui->treeWidget->findItems(pieces.at(i-1),Qt::MatchExactly|Qt::MatchRecursive, 0);

//         }
//         if (items.count() == 0) { // not found, create new entry
//             QTreeWidgetItem * child  = new QTreeWidgetItem();
//             child->setText(0,pieces.at(i));
//             root->addChild(child);
//             root=child;
//         }
//    }
//}

