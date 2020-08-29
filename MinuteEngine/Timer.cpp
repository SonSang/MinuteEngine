/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#include "Timer.h"

namespace ME {
    void Timer::clearRecords() noexcept {
        record.clear();
    }
    void Timer::addRecord(double record) noexcept {
        this->record.push_back(record);
    }
    Timer::Record& Timer::getRecord() noexcept {
        return record;
    }
    const Timer::Record& Timer::getRecordC() const noexcept {
        return record;
    }
    void Timer::setBeg() noexcept {
        begTime = std::chrono::system_clock::now();
    }
    void Timer::setEnd() noexcept {
        endTime = std::chrono::system_clock::now();
    }
    double Timer::getElapsedTime() const noexcept {
        std::chrono::duration<double> sec = endTime - begTime;
        return sec.count();
    }
    void Timer::addElapsedTimeToRecord() noexcept {
        std::chrono::duration<double> sec = endTime - begTime;
        addRecord(sec.count());
    }
    Timer::TimePoint Timer::getTime() noexcept {
        return std::chrono::system_clock::now();
    }
}