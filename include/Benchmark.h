#ifndef BENCHMARK_H_
#define BENCHMARK_H_
#include <chrono>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <type_traits>
#include <sstream>

/// @brief simple Benchmark class for timing things
enum class WriteMode {CSV,VERBOSE};

template<typename Clock=std::chrono::high_resolution_clock, typename Resolution=std::chrono::microseconds>
class Benchmark
{
public:

    Benchmark(size_t _reserve)
    {
       m_durations.reserve(_reserve);
    }
    Benchmark(size_t _reserve, const std::string &_fileName,WriteMode _mode=WriteMode::VERBOSE)
    {
      m_mode=_mode;
      if(m_mode == WriteMode::VERBOSE)
      {
        m_durations.reserve(_reserve);
      }
      m_filename=_fileName;
    }
    Benchmark(const Benchmark&)=delete;
    void startTimer()
    {
      m_lastTimePoint=Clock::now();
    }


    void writeVerbose()
    {
      if(m_filename.size() !=0)
      {
        std::ofstream fileOut;
        fileOut.open(m_filename.c_str());
        // check to see if we can open the file
        if (!fileOut.is_open())
        {
          std::cerr <<"Could not open File : "<<m_filename<<" for writing \n";
          exit(EXIT_FAILURE);
        }
        fileOut<<"# Benchmark Data \n";
        if  ( std::is_same<Resolution, std::chrono::microseconds>::value )
        {
          fileOut<<"# Resolution microseconds\n";
        }
        else if  ( std::is_same<Resolution, std::chrono::milliseconds>::value )
        {
          fileOut<<"# Resolution milliseconds\n";
        }
        else if  ( std::is_same<Resolution, std::chrono::nanoseconds>::value )
        {
          fileOut<<"# Resolution nanoseconds\n";
        }
        else if  ( std::is_same<Resolution, std::chrono::seconds>::value )
        {
          fileOut<<"# Resolution seconds\n";
        }
        for(auto d : m_durations)
          fileOut<<"Duration\t"<<std::chrono::duration_cast<Resolution>(d).count()<<'\n';

        fileOut<<"Min\t"<<min()<<'\n';
        fileOut<<"Max\t"<<max()<<'\n';
        fileOut<<"Average\t"<<average()<<'\n';
        fileOut<<"Mode\t"<<mode()<<'\n';
        fileOut<<"Mode\t"<<median()<<'\n';

      }
    }

    void writeCSV()
    {
      if(m_filename.size() !=0)
      {
        std::ofstream fileOut;
        fileOut.open(m_filename.c_str());
        // check to see if we can open the file
        if (!fileOut.is_open())
        {
          std::cerr <<"Could not open File : "<<m_filename<<" for writing \n";
          exit(EXIT_FAILURE);
        }

        // first write header
        size_t numEntries;
        std::string header;
        for(auto h : m_csv)
        {
          header+=h.first+',';
          numEntries=h.second.size();
        }
        header.pop_back();
        fileOut<<header;
        fileOut<<'\n';
        // now for the csv values

        for(size_t i=0; i<numEntries; ++i)
        {
          std::stringstream csv;
          for(auto h : m_csv)
          {
            csv<<h.second[i].count()<<',';
            //fileOut<<h.second[i].count()<<',';
          }
          // remove last ,
          csv.seekp(-1, std::ios_base::end);
          csv<<'\n';
          fileOut<<csv.str();
        }
      }
    }
    ~Benchmark()
    {
      if(m_mode == WriteMode::VERBOSE)
      {
       writeVerbose();
      }
      else
      {
        writeCSV();
      }
    }

    void addDuration()
    {
      auto now=Clock::now();
      auto duration=now-m_lastTimePoint;
      m_durations.push_back(duration);
      m_lastTimePoint=Clock::now();

      if (duration < m_min)
      {
          m_min = duration;
      }

      if (duration > m_max)
      {
        m_max = duration;
      }
    }

    void addCSV(const std::string &_col)
    {
      auto now=Clock::now();
      auto duration=now-m_lastTimePoint;
      m_csv[_col].push_back(duration);
      m_lastTimePoint=Clock::now();
    }




    // add a time duration
    void addDuration(typename Clock::duration now)
    {
      m_durations.push_back(now);
      if (now < m_min) 
      {
          m_min = now;
      }

      if (now > m_max) 
      {
        m_max = now;
      }
    }

    auto average()
    {
      typename Clock::duration result{0};
      for (auto times : m_durations) 
      {
          result +=times;
      }
      result /= m_durations.size();
      return std::chrono::duration_cast<Resolution>(result).count();
    }


    auto mode()
    {
      // Map of tick and count
      std::unordered_map<size_t,size_t> histogram;
      for(auto d : m_durations)
      {
        auto key=std::chrono::duration_cast<Resolution>(d).count();
        histogram[key]++;
      }
      // now find the max
      auto max=std::max_element(std::begin(histogram),std::end(histogram),
      [](const auto& p1, const auto& p2)
      {
        return p1.second < p2.second;
      });
      // this is the mode!
      return max->first;
    }

    auto max()
    {
        return std::chrono::duration_cast<Resolution>(m_max).count();
    }

    auto min()
    {
        return std::chrono::duration_cast<Resolution>(m_min).count();
    }

    auto median()
    {
        std::sort(std::begin(m_durations), std::end(m_durations));
        return std::chrono::duration_cast<Resolution>(m_durations[m_durations.size() / 2]).count();
    }

  private :
    std::vector<typename Clock::duration> m_durations;
    std::unordered_map<std::string,std::vector<typename Clock::duration>> m_csv;
    typename Clock::duration m_min{Clock::duration::max()};
    typename Clock::duration m_max{Clock::duration::min()};
    std::string m_filename;
    std::chrono::time_point<Clock> m_lastTimePoint;
    WriteMode m_mode=WriteMode::VERBOSE;


};

#endif
