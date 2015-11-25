#include <iostream>
#include <sstream>
#include "product.h"
#include "datastore.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "review.h"
#include "datafunction.h"
#include "login_window.h"
#include <QApplication>

using namespace std;

int main(int argc, char* argv[])
{

  // QApplication for QT GUI Interface
  QApplication app(argc, argv);
  
   if(argc < 2){
    cerr << "Please specify a database file" << endl;
  }

  LoginWindow loginWindow(argv[1]);

  loginWindow.show();

  return app.exec();
}

