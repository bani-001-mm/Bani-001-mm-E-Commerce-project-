#ifndef WIN_SYNC_HPP
#define WIN_SYNC_HPP

#include <windows.h>

class Mutex {
public:
    Mutex() {
        InitializeCriticalSection(&cs_);
    }

    ~Mutex() {
        DeleteCriticalSection(&cs_);
    }

    void lock() {
        EnterCriticalSection(&cs_);
    }

    void unlock() {
        LeaveCriticalSection(&cs_);
    }

private:
    CRITICAL_SECTION cs_;
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
};

class LockGuard {
public:
    explicit LockGuard(Mutex& m) : m_(m) {
        m_.lock();
    }

    ~LockGuard() {
        m_.unlock();
    }

private:
    Mutex& m_;
    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;
};

#endif // WIN_SYNC_HPP
