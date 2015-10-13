#ifndef TEST_LIB
#define TEST_LIB
#include <iostream>
#include <chrono>
#include <papi.h>
#include <linux/perf_event.h>
#include <linux/hw_breakpoint.h>
#include <asm/unistd.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <cstring>
namespace test {
  class clock {
    typedef std::chrono::high_resolution_clock hsc;
    typedef hsc::time_point tp;
  public:
    void start();
    void stop();
    long long count();
  private:
    tp s,e;
  };

  void clock::start() {
    s = hsc::now();
  }

  void clock::stop() {
    e = hsc::now();
  }

  long long clock::count() {
    using namespace std;
    using namespace std::chrono;
    return duration_cast<milliseconds>(e-s).count();
  }

  class PAPI {

  public:
    PAPI(int events[], long long values[], int size);
    void start();
    void stop();
  private:
    int *evts;
    long long *values;
    int size;
  };

  PAPI::PAPI(int events[], long long values[], int size) {
    PAPI_library_init(PAPI_VER_CURRENT);
    evts = events;
    this->values = values;
    this->size = size;
  }

  void PAPI::start() {
    PAPI_start_counters(evts, size);
  }

  void PAPI::stop() {
    PAPI_stop_counters(values, size);
  }

  class pagefaults {
  public:
    pagefaults();
    void start();
    void stop();
    long long count();
  private:
    struct perf_event_attr pe_attr_page_faults;
    int page_faults_fd;
    uint64_t page_faults_count;
  };

  pagefaults::pagefaults() {
    memset(&pe_attr_page_faults, 0, sizeof(pe_attr_page_faults));
    pe_attr_page_faults.size = sizeof(pe_attr_page_faults);
    pe_attr_page_faults.type = PERF_TYPE_SOFTWARE;
    pe_attr_page_faults.config = PERF_COUNT_SW_PAGE_FAULTS;
    pe_attr_page_faults.disabled = 1;
    pe_attr_page_faults.exclude_kernel = 1;
    page_faults_fd = perf_event_open(&pe_attr_page_faults, 0, -1, -1, 0);
    if (page_faults_fd ==  -1) {
      printf("perf_event_open failed for page faults %s\n", strerror(errno));
      return;
    }
  }

  void pagefaults::start() {
    ioctl(page_faults_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(page_faults_fd, PERF_EVENT_IOC_ENABLE, 0);
  }

  void pagefaults::stop() {
    ioctl(page_faults_fd, PERF_EVENT_IOC_DISABLE, 0);
    read(page_faults_fd, &page_faults_count, sizeof(page_faults_count));
  }

  uint64_t pagefaults::count() {
    return page_faults_count;
  }


};


#endif
