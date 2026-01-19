# Profiling

Charts were generated using (time_profiler_visualizer)[https://github.com/volatilflerovium/time_profiler_visualizer]

## Performance

Performace tests run under Debian 11 with AMD Ryzen7 processor.

For a file of 1GB
   - SHA1 previous version: ~1870ms
   - Linux command sha1sum: ~1471ms
   - SHA1 optimized: ~1340ms (under Linux with mmap) and ~1530ms (for Linux and Windows using std::fread)

On a 1 GB file (in seconds)

[![performance-1GB-file.png](https://i.postimg.cc/GpPT4tPZ/performance-1GB-file.png)](https://postimg.cc/7C6ZcxWV)

On incremental string (in microseconds)

[![performance.png](https://i.postimg.cc/J4dN8sK0/performance.png)](https://postimg.cc/YhYGfq5B)
