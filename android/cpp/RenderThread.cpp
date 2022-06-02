//
// Created by Vittorio Cellucci on 2022-05-05.
//

#include "RenderThread.h"

RenderThread::RenderThread() {
    context = std::make_shared<CanvasEGLContext>();
    threader = std::thread([this]{
        std::unique_lock<std::mutex> l(lock);
        do {
            cv.wait(l, [this]{
                return workQueue.size() || quit;
            });
            if(workQueue.size()) {
                auto op = std::move(workQueue.front());
                workQueue.pop();
                l.unlock();
                op();
                l.lock();
            }
        } while (!quit);

        // we quit, drain the queue
        while(!workQueue.empty()) {
            auto op = workQueue.front();
            workQueue.pop();
            op();
        }
    });
}

void RenderThread::stop() {
  {
    std::unique_lock<std::mutex> l(lock);
    quit = true;
  }
  push([]{});
//  if(!quit) {
//    quit = true;
//    cv.notify_all();
//    lock.unlock();
//
    if (threader.joinable()) {
      threader.join();
    }
//  }
}

RenderThread::~RenderThread() {
    stop();
}
