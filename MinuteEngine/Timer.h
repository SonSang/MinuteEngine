/*
 *******************************************************************************************
 * Author	: Sang Hyun Son
 * Email	: shh1295@gmail.com
 * Github	: github.com/SonSang
 *******************************************************************************************
 */

#ifndef __ME_TIMER_H__
#define __ME_TIMER_H__

#ifdef _MSC_VER
#pragma once 
#endif

#include <string>
#include <vector>
#include <chrono>

namespace ME {
    // In seconds(sec).
    class Timer {
    public:
        using Record = std::vector<double>;
        using TimePoint = std::chrono::system_clock::time_point;
    private:
        std::string name;
        Record record;
        TimePoint begTime;
        TimePoint endTime;
    public:
        void clearRecords() noexcept;
        void addRecord(double record) noexcept;
        Record& getRecord() noexcept;
        const Record& getRecordC() const noexcept;

        void setBeg() noexcept;
        void setEnd() noexcept;
        double getElapsedTime() const noexcept; 
        void addElapsedTimeToRecord() noexcept;

        static TimePoint getTime() noexcept;

        //void save_time(const std::string& filename);
        //int get_size();
    };
}

#endif