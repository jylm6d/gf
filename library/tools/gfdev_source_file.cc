#include <cstdarg>
#include <cstdlib>
#include <cstdio>

namespace {

  class File {
  public:
    File(const char *filename, const char *mode)
    : m_stream(std::fopen(filename, mode))
    {

    }

    ~File() {
      if (m_stream) {
        std::fclose(m_stream);
      }
    }

    operator bool() const {
      return m_stream != nullptr;
    }

    int getc() const {
      return std::fgetc(m_stream);
    }

    int printf(const char *fmt, ...) {
      va_list args;
      va_start(args, fmt);
      int ret = std::vfprintf(m_stream, fmt, args);
      va_end(args);
      return ret;
    }

  private:
    std::FILE *m_stream;
  };

}


static void usage() {
  std::printf("Usage: gfdev_source_file <source> <destination> <name>\n");
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    usage();
    return EXIT_FAILURE;
  }

  File inFile(argv[1], "rb");

  if (!inFile) {
    usage();
    return EXIT_FAILURE;
  }

  File outFile(argv[2], "wb");

  if (!outFile) {
    usage();
    return EXIT_FAILURE;
  }

  outFile.printf("// DO NOT MODIFY!\n");
  outFile.printf("// This file has been generated by gfdev_source_file from '%s'\n\n", argv[1]);
  outFile.printf("#ifndef GF_GENERARED_%s\n#define GF_GENERARED_%s\n\n", argv[3], argv[3]);
  outFile.printf("static constexpr char %s[] = {", argv[3]);

  std::size_t count = 0;
  int c;

  while ((c = inFile.getc()) != EOF) {
    if (count % 12 == 0) {
      outFile.printf("\n   ");
    }

    outFile.printf("0x%.2X, ", c);

    count++;
  }

  outFile.printf("0x00\n};\n\n");
  outFile.printf("// size = %zu;\n", count);
  outFile.printf("#endif\n");

  return EXIT_SUCCESS;
}
