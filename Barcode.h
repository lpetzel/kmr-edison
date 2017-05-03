// This file is for the barcode detection.
// Currently I follow a very easy strategy:
// There is an endless loop, which always waits for a signal.
// Everytime, the signal is triggered, the machine tries to
// take a picture and send a protobuf message.
#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <condition_variable>

namespace protobuf_comm {
  class ProtobufStreamServer;
}


class Barcode {
  public:
    protobuf_comm::ProtobufStreamServer* server_;
    std::string machine_name_;
    cv::VideoCapture cap_;
    std::mutex lock_;
    std::condition_variable take_picture_;


    Barcode(protobuf_comm::ProtobufStreamServer* s);
    // run will never return. Start it in a new thread!
    // It will handle the barcode requests. Unless it is idle,
    // it holds the lock_.
    void run();
    // Set the machine name. The lock is aquired internally.
    void setMachineName(const std::string& s);
    // Signal the thread to take a picture. Does not block, usually.
    void takePicture();
};
