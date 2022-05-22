//
// Created by Vittorio Cellucci on 2022-05-05.
//

#ifndef HELIUM4_RENDERTHREAD_H
#define HELIUM4_RENDERTHREAD_H
#include <functional>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <jni.h>
#include "EGL/egl.h"
#include "GLES2/gl2.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"

using CanvasEGLContext = struct
{
    EGLDisplay glDisplay = EGL_NO_DISPLAY;
    EGLContext glContext = EGL_NO_CONTEXT;
    EGLConfig glConfig = 0;
    sk_sp<GrDirectContext> skContext = nullptr;
};

class RenderThread {
public:
    using Operation = std::function<void(void)>;
    using WorkQueue = std::queue<Operation>;
public:
    RenderThread();
    ~RenderThread();

    void push(Operation&& op) {
        {
            std::unique_lock<std::mutex> l(lock);
            workQueue.push(std::move(op));
        }
        cv.notify_one();
    }

    void wait() {
         std::unique_lock<std::mutex> l(lock);
    }

    std::shared_ptr<CanvasEGLContext> ctx() {
        return context;
    }

protected:
    std::thread threader;
    std::mutex lock;
    std::condition_variable cv;
    WorkQueue  workQueue;
    bool quit = false;
    std::shared_ptr<CanvasEGLContext> context;
};


#endif //HELIUM4_RENDERTHREAD_H
