#ifndef _PROFILER_H_
#define _PROFILER_H_

#include <chrono>
#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <ratio>
#include <utility>

#ifdef WIN32
#include <Windows.h>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")
#else
#include <sys/resource.h>
#include <sys/time.h>
#endif

namespace TinySTL{
	namespace Profiler{

		class ProfilerInstance{
		public:
			typedef std::chrono::steady_clock SteadyClock;
			typedef SteadyClock::time_point TimePoint;
			typedef std::chrono::duration<double, std::ratio<1, 1>> DurationTime;//��λ��
			enum class MemoryUnit{KB_, MB_, GB_};
		private:
			#define KB / 1024
			#define MB KB / 1024
			#define GB MB / 1024
		private:
			static DurationTime duringTime;
			static TimePoint startTime;
			static TimePoint finishTime;
		public:
			static void start();//��ʼ��ʱ
			static void finish();//������ʱ
			static void dumpDuringTime(std::ostream& os = std::cout);//��ӡʱ��

			static double second();//����Ϊ��λ����ʱ��
			static double millisecond();//�Ժ���Ϊ��λ����ʱ��

			static size_t memory(MemoryUnit mu = MemoryUnit::KB_);//��ѯ��ǰ������ڴ�ʹ����
		};
	}
}

#endif