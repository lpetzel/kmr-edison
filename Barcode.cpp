#include "Barcode.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <zbar.h>
#include <iostream>
#include <protobuf_comm/server.h>
#include <cassert>
#include <memory>
#include "protobuf/WorkpieceInfo.pb.h"


using namespace cv;
using namespace std;
using namespace zbar;

using namespace llsf_msgs; 
using namespace protobuf_comm;
using google::protobuf::Message;


Barcode::Barcode(ProtobufStreamServer* p): server_(p), cap_(0) {}

void Barcode::takePicture() {
  take_picture_.notify_all();
}

void Barcode::setMachineName (const string& s) {
  lock_guard<mutex> l(lock_);
  if (s != machine_name_) {
    if (s.empty()) {
      cerr << "Warning: won't set machine name to empty string!" << endl;
      return;
    } else if (machine_name_.empty()) {
      cout << "Setting machine name to " << machine_name_ << endl;
    } else {
      cout << "Warning: Changing the machine name from " << machine_name_
        << " to " << s <<"!" << endl;
    }
    machine_name_ = s;
  }
}

void Barcode::run() {
  unique_lock<mutex> l(lock_);
  ImageScanner scan;
  Mat frame;
  Mat grey;
  assert(cap_.isOpened());
  scan.set_config(ZBAR_UPCE, ZBAR_CFG_ENABLE, 1);  
  for (;;) {
    take_picture_.wait(l);
    int product=0;
    // Give it 5 trys
    for(int count=0; (count < 5) and (not product); ++count){
      bool bSuccess = cap_.read(frame); // read a new frame from video
      if (bSuccess) {
        cvtColor(frame,grey,CV_BGR2GRAY);
        int width = frame.cols;  
        int height = frame.rows;  
        unsigned char *raw = (uchar *)grey.data;  
        // wrap image data  
        Image image(width, height, "Y800", raw, width * height);  
        // scan the image for barcodes  
        int n = scan.scan(image);  
       	string id_str;
        for(Image::SymbolIterator symbol = image.symbol_begin();  symbol != image.symbol_end();  ++symbol) {  
          id_str=symbol->get_data();
          id_str.erase(id_str.end()-1);
          id_str.erase(id_str.begin());
          id_str.erase(id_str.begin());
          id_str.erase(id_str.begin());
          int newid = stoi(id_str) ;
          if(newid!=0)
          {
            if (product && (product != newid)) {
              cerr << "Warning: seeing multiple barcodes! Will take only "
                << product << " and will discard " << newid << "!" << endl;
            } else {
              product = newid;
            }
          }
        }
      } else {
        cerr << "Warning: Could not read frame! Is the video stream broken?"
          << endl;
      }
    }
    if (product) {
      // std::cout << "id "  << product <<  std::endl;
      // std::cout << "at_machine " << at_machine << std::endl;
      // std::cout << "visible " << visible << std::endl;
      shared_ptr<Workpiece> workpiece(new Workpiece());
      workpiece->set_id(product);
      workpiece->set_at_machine(machine_name_);
      workpiece->set_visible(true);
      server_->send_to_all(dynamic_pointer_cast<Message> (workpiece));
    } else {
      cerr << "No barcode found within 5 tries." << endl;
    }
  }
}
    


